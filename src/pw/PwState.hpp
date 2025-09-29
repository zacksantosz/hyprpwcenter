#pragma once

#include "PwNode.hpp"
#include "IPwNode.hpp"

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

class CPipewireState {
  public:
    CPipewireState(int argc, char** argv);
    ~CPipewireState();

    int  getFd();
    void dispatch();

    void setVolume(uint32_t id, float x);
    void setMuted(uint32_t id, bool x);

    void onGlobal(uint32_t id, uint32_t permissions, const char* type, uint32_t version, const spa_dict* props);
    void onGlobalRemoved(uint32_t id);

  private:
    struct {
        pw_main_loop*            loop;
        pw_context*              context;
        pw_core*                 core;
        pw_registry*             registry;
        spa_hook                 registry_listener;

        std::vector<SP<IPwNode>> nodes;
    } m_pwState;

    friend class CPipewireNode;
    friend class CPipewireClient;
    friend class CPipewireDevice;
};

inline UP<CPipewireState> g_pipewire;