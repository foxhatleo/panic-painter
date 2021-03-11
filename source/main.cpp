#include "PPApp.h"

#define GAME_WIDTH 1024
#define GAME_HEIGHT 576

/**
 * Main entry point of program. Shouldn't really have to change anything here.
 */
int main(int argc, char *argv[]) {
    PanicPainterApp app;

    app.setName("Panic Painter");
    app.setOrganization("Dragonglass Studios");
    app.setSize(GAME_WIDTH, GAME_HEIGHT); // Only applies to desktop
    app.setFPS(60.0f);
    app.setHighDPI(true);

    // DO NOT EDIT BELOW
    if (!app.init()) {
        return 1;
    }
    app.onStartup();
    while (app.step());
    app.onShutdown();

    exit(0); // Necessary to quit on mobile devices
    return 0; // This line is never reached
}
