#pragma once

#include <cstdint>
#include <string>

#include "../helpers/Memory.hpp"

class IPwNode {
  public:
    virtual ~IPwNode() = default;

    virtual void setVolume(float x) = 0;
    virtual void setMute(bool x)    = 0;

    uint32_t     m_id = 0;
    std::string  m_name, m_mediaClass;

    float        m_volume = 0;
    bool         m_muted  = false;
    bool         m_isApp  = false;

    WP<IPwNode>  m_self;
};
