#include "clipboard.h"

/* Portable fallback core: pure helpers with no OS dependencies.
 * OS clipboard primitives live in the platform files
 * (win32_clipboard.c / cocoa_clipboard.c / linux_clipboard.c). */

int alya_clipboard_add(int a, int b) {
    return a + b;
}

int clipboard_text_limit(void) {
    return 65535;
}
