#include "clipboard.h"

/* Linux stub: headless-safe no-op backend (no X11/Wayland dependency).
 * A future xclip-style bridge can replace these without touching
 * the Alya API; the in-memory backend stays authoritative. */

int clipboard_native_available(void) {
    return 0;
}

int clipboard_native_seq(void) {
    return 0;
}

int clipboard_sys_has_text(void) {
    return 0;
}

const char *clipboard_sys_get_text(void) {
    return "";
}

int clipboard_sys_set_text(const char *utf8) {
    (void)utf8;
    return 0;
}

int clipboard_sys_clear(void) {
    return 0;
}
