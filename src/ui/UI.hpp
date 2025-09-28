#pragma once

#include <hyprtoolkit/core/Backend.hpp>
#include <hyprtoolkit/window/Window.hpp>
#include <hyprtoolkit/element/Rectangle.hpp>
#include <hyprtoolkit/element/Text.hpp>
#include <hyprtoolkit/element/ColumnLayout.hpp>
#include <hyprtoolkit/element/RowLayout.hpp>
#include <hyprtoolkit/element/Null.hpp>
#include <hyprtoolkit/element/Button.hpp>

#include "../helpers/Memory.hpp"

class CNodeVolumeSlider;
class CPipewireNode;

class CUI {
  public:
    CUI();
    ~CUI() = default;

    void run();

    void updateNode(WP<CPipewireNode> node);
    void nodeRemoved(WP<CPipewireNode> node);

  private:
    void                                  changeTab(size_t idx);

    size_t                                m_tab = 1337;

    SP<Hyprtoolkit::CBackend>             m_backend;
    SP<Hyprtoolkit::IWindow>              m_window;
    SP<Hyprtoolkit::CRectangleElement>    m_background;
    SP<Hyprtoolkit::CColumnLayoutElement> m_layout;

    struct {
        SP<Hyprtoolkit::CRowLayoutElement> buttonLayout;
        SP<Hyprtoolkit::CNullElement>      tabContainer;

        SP<Hyprtoolkit::CButtonElement>    nodesButton;
        SP<Hyprtoolkit::CButtonElement>    inputsButton;

        struct {
            std::vector<SP<CNodeVolumeSlider>>    nodeSliders;
            SP<Hyprtoolkit::CColumnLayoutElement> nodesLayout;
        } nodesTab;

        struct {
            std::vector<SP<CNodeVolumeSlider>>    inputSliders;
            SP<Hyprtoolkit::CColumnLayoutElement> inputsLayout;
        } inputsTab;
    } m_tabs;

    friend class CNodeVolumeSlider;
};

inline UP<CUI> g_ui;
