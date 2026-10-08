#include "clipboard.h"

/* macOS stub: headless-safe no-op backend.
 * A full NSPasteboard bridge needs an Objective-C translation unit;
 * until then the in-memory Alya backend is authoritative. */

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
