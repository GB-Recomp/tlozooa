/* Standalone launcher for tlozooa (The Legend of Zelda: Oracle of Ages).
 * Patterned after GB-Recomp/gbcamera/launcher_main.cpp. */
#define SDL_MAIN_HANDLED
extern "C" {
#include "tlozooa.h"
}

#include <SDL.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "platform_sdl.h"

typedef int (*GBLauncherMainFn)(int argc, char* argv[]);

typedef struct {
    const char* id;
    const char* title;
    const char* rom_path;
    GBLauncherMainFn main_fn;
} GBLauncherGame;

static int launch_tlozooa(int argc, char* argv[]) {
    return tlozooa_main(argc, argv);
}

static GBLauncherGame g_games[] = {
    {"tlozooa", "The Legend of Zelda: Oracle of Ages", "roms/tlozooa.gbc", launch_tlozooa},
};

static const char* g_launcher_name = "tlozooa";
static const size_t g_game_count = sizeof(g_games) / sizeof(g_games[0]);

static void print_games(void) {
    fprintf(stderr, "Available games in %s:\n", g_launcher_name);
    for (size_t i = 0; i < g_game_count; i++) {
        fprintf(stderr, "  %zu. %s [%s]\n", i + 1, g_games[i].title, g_games[i].id);
    }
}

static bool game_assets_available(const char* id) {
    if (!id || !*id) return false;
    char path[512];
    struct stat st;
    snprintf(path, sizeof(path), "assets/%s", id);
    if (stat(path, &st) == 0 && (st.st_mode & S_IFDIR)) return true;
    static const char* extensions[] = {".gb", ".gbc", ".sgb"};
    for (size_t i = 0; i < sizeof(extensions) / sizeof(extensions[0]); i++) {
        snprintf(path, sizeof(path), "roms/%s%s", id, extensions[i]);
        if (stat(path, &st) == 0) return true;
    }
    return false;
}

int main(int argc, char* argv[]) {
    if (!game_assets_available("tlozooa")) {
        fprintf(stderr,
                "[LAUNCH] Drop your Oracle of Ages ROM at roms/tlozooa.gbc and re-run.\n");
        print_games();
        return 1;
    }
    gb_platform_set_launcher_return_enabled(false);
    for (;;) {
        fprintf(stderr, "[LAUNCH] Starting %s [%s]\n", g_games[0].title, g_games[0].id);
        int rc = g_games[0].main_fn(argc, argv);
        if (gb_platform_consume_restart_requested()) continue;
        return rc;
    }
}
