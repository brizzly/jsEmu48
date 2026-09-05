# MoHPC forum — follow-up reply (draft)

Quick update — thanks for the early feedback! I've polished things a fair bit since the first post: https://jmapp.com/hp48gx.php

**Controls aren't a guessing game anymore.** Each game now shows its own key legend under the screen (they all use different keys). MarioLandHP, for instance: M/N to move, Q to jump, P for fireballs. Every game also links to its hpcalc.org page.

**Rebuilt the emulator face from source** — dropped a leftover "Game" button, rounded the key corners, and added a subtle pixel-grid on the LCD so the 4-level grayscale reads like a real dot-matrix screen.

**More games boot straight into play** now (captured launched save-states, MarioHP included), and I added the little Mario Land Demo from '97. There's also a "load a file onto the stack" button if you want to push your own HP48 object (a level .var, a .lib…) into the running calc.

Source (fork of HP EMU): github.com/brizzly/jsEmu48

Still keen on feedback, especially the grayscale rendering and the mobile controls. If something won't run, tell me your browser/device.

Cheers,
Julien (sunhp)
