#pragma once

#include <vector>
#include <cstdint>

extern "C" {
#include <pipewire/pipewire.h>
#include <spa/utils/hook.h>
}

#include "../helpers/Memory.hpp"

class CPipewireNode {
  public:
    CPipewireNode(uint32_t id, uint32_t permissions, const char* type, uint32_t version, const spa_dict* props);
    ~CPipewireNode();

    void              setVolume(float x);
    void              setMute(bool x);

    uint32_t          m_id = 0;
    std::string       m_name, m_mediaClass;
    pw_node*          m_proxy = nullptr;
    spa_hook          m_listener;
    size_t            m_channelCount = 0;

    float             m_volume = 0;
    bool              m_muted  = false;

    WP<CPipewireNode> m_self;
};