#include "pw/PwState.hpp"
#include "ui/UI.hpp"

int main(int argc, char** argv, char** envp) {
    g_pipewire = makeUnique<CPipewireState>(argc, argv);
    g_ui       = makeUnique<CUI>();

    g_ui->run();

    return 0;
};