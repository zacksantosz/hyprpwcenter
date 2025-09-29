#include "PwState.hpp"
#include "../helpers/Log.hpp"
#include "../ui/UI.hpp"

extern "C" {
#include <pipewire/pipewire.h>
#include <spa/utils/defs.h>
#include <spa/utils/hook.h>
#include <spa/utils/result.h>
#include <spa/param/param.h>
#include <spa/param/props.h>
#include <spa/pod/iter.h>
#include <spa/pod/parser.h>
#include <spa/pod/pod.h>
}

static void onGlobal(void* userdata, uint32_t id, uint32_t permissions, const char* type, uint32_t version, const spa_dict* props) {
    g_pipewire->onGlobal(id, permissions, type, version, props);
}

static void onGlobalRemoved(void* userdata, uint32_t id) {
    g_pipewire->onGlobalRemoved(id);
}

static const pw_registry_events REGISTRY_EVENTS = {
    .version       = PW_VERSION_REGISTRY_EVENTS,
    .global        = ::onGlobal,
    .global_remove = ::onGlobalRemoved,
};

CPipewireState::CPipewireState(int argc, char** argv) {
    pw_init(&argc, &argv);

    m_pwState.loop    = pw_main_loop_new(nullptr);
    m_pwState.context = pw_context_new(pw_main_loop_get_loop(m_pwState.loop), nullptr, 0);
    m_pwState.core    = pw_context_connect(m_pwState.context, nullptr, 0);
    if (!m_pwState.core) {
        Debug::log(CRIT, "Couldn't connect to pw");
        exit(1);
    }

    m_pwState.registry = pw_core_get_registry(m_pwState.core, PW_VERSION_REGISTRY, 0);
    spa_zero(m_pwState.registry_listener);
    pw_registry_add_listener(m_pwState.registry, &m_pwState.registry_listener, &REGISTRY_EVENTS, nullptr);
}

CPipewireState::~CPipewireState() {
    m_pwState.nodes.clear();

    if (m_pwState.registry)
        pw_proxy_destroy(reinterpret_cast<pw_proxy*>(m_pwState.registry));
    if (m_pwState.core)
        pw_core_disconnect(m_pwState.core);
    if (m_pwState.context)
        pw_context_destroy(m_pwState.context);
    if (m_pwState.loop)
        pw_main_loop_destroy(m_pwState.loop);
}

int CPipewireState::getFd() {
    return pw_loop_get_fd(pw_main_loop_get_loop(m_pwState.loop));
}

void CPipewireState::dispatch() {
    while (pw_loop_iterate(pw_main_loop_get_loop(m_pwState.loop), 0) != 0) {
        ;
    }
}

void CPipewireState::onGlobal(uint32_t id, uint32_t permissions, const char* type, uint32_t version, const spa_dict* props) {
    const auto SV = std::string_view{type};
    if (SV == PW_TYPE_INTERFACE_Node) {
        auto x    = m_pwState.nodes.emplace_back(makeShared<CPipewireNode>(id, permissions, type, version, props));
        x->m_self = x;
    }
}

void CPipewireState::onGlobalRemoved(uint32_t id) {
    std::erase_if(m_pwState.nodes, [id](const auto& n) { return n->m_id == id; });
}

void CPipewireState::setVolume(uint32_t id, float x) {
    for (const auto& n : m_pwState.nodes) {
        if (n->m_id != id)
            continue;

        n->setVolume(x);
    }
}

void CPipewireState::setMuted(uint32_t id, bool x) {
    for (const auto& n : m_pwState.nodes) {
        if (n->m_id != id)
            continue;

        n->setMute(x);
    }
}
