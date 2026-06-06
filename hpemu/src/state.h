#ifndef __STATE_H
#define __STATE_H

/* Full emulator save-state: serialize/restore CPU, RAM, bus config, ports,
 * timers, hardware and display state so a running game can be resumed exactly.
 * Returns 0 on success, negative on error. */
int state_save(const char *path);
int state_load(const char *path);

#endif
