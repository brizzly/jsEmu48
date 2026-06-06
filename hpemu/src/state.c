/*
 * Full emulator save-state for hpemu / jsEmu48.
 *
 * Serializes all mutable emulator state (CPU, RAM, bus configuration, ports,
 * timers, hardware registers and display registers) to a single file, and
 * restores it so a running game resumes exactly where it was saved. Used to
 * ship "game already launched" snapshots that load in one click.
 */

#include <stdio.h>
#include "types.h"
#include "cpu.h"
#include "bus.h"
#include "ports.h"
#include "timers.h"
#include "hdw.h"
#include "display.h"
#include "emulator.h"
#include "state.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define STATE_MAGIC   0x54535048u	/* "HPST" */
#define STATE_VERSION 1u

int state_save(const char *path)
{
    FILE *f = fopen(path, "wb");
    if (!f) return -1;

    unsigned int magic = STATE_MAGIC, version = STATE_VERSION;
    fwrite(&magic, sizeof(magic), 1, f);
    fwrite(&version, sizeof(version), 1, f);

    cpu_state_save(f);
    bus_state_save(f);		/* RAM + bus config */
    ports_state_save(f);
    timers_state_save(f);
    hdw_state_save(f);
    display_state_save(f);

    fclose(f);
    return 0;
}

int state_load(const char *path)
{
    FILE *f = fopen(path, "rb");
    if (!f) return -1;

    unsigned int magic = 0, version = 0;
    if (fread(&magic, sizeof(magic), 1, f) != 1 ||
        fread(&version, sizeof(version), 1, f) != 1 ||
        magic != STATE_MAGIC || version != STATE_VERSION) {
        fclose(f);
        return -2;
    }

    cpu_state_load(f);
    bus_state_load(f);		/* RAM + bus config, then rebuilds the map */
    ports_state_load(f);
    timers_state_load(f);
    hdw_state_load(f);
    display_state_load(f);

    fclose(f);
    emulator_state_resync();	/* reschedule periodic events to cpu.cycles */
    return 0;
}

#ifdef __EMSCRIPTEN__
/* Called from JavaScript (the "Save State" button) to dump the live state to a
 * fixed path in the virtual filesystem, which JS then downloads. */
EMSCRIPTEN_KEEPALIVE
int hp_save_state(void)
{
    return state_save("/save.state");
}
#endif
