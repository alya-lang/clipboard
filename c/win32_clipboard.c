#include "clipboard.h"
#include <windows.h>
#include <string.h>
#include <stdlib.h>

#define CLIP_TEXT_CAP 65535

static char g_clip_text[CLIP_TEXT_CAP + 1];

int clipboard_native_available(void) {
    return 1;
}

int clipboard_native_seq(void) {
    return (int)GetClipboardSequenceNumber();
}

int clipboard_sys_has_text(void) {
    return IsClipboardFormatAvailable(CF_UNICODETEXT) ? 1 : 0;
}

const char *clipboard_sys_get_text(void) {
    g_clip_text[0] = '\0';
    if (!IsClipboardFormatAvailable(CF_UNICODETEXT)) {
        return g_clip_text;
    }
    if (!OpenClipboard(NULL)) {
        return g_clip_text;
    }
    HANDLE h = GetClipboardData(CF_UNICODETEXT);
    if (h == NULL) {
        CloseClipboard();
        return g_clip_text;
    }
    const wchar_t *wsrc = (const wchar_t *)GlobalLock(h);
    if (wsrc == NULL) {
        CloseClipboard();
        return g_clip_text;
    }
    int needed = WideCharToMultiByte(CP_UTF8, 0, wsrc, -1, NULL, 0, NULL, NULL);
    if (needed > 1) {
        int cap = needed < CLIP_TEXT_CAP ? needed : CLIP_TEXT_CAP;
        /* Convert directly when it fits, else convert in two steps. */
        if (needed <= CLIP_TEXT_CAP) {
            WideCharToMultiByte(CP_UTF8, 0, wsrc, -1, g_clip_text, cap + 1, NULL, NULL);
        } else {
            char *tmp = (char *)malloc((size_t)needed);
            if (tmp != NULL) {
                WideCharToMultiByte(CP_UTF8, 0, wsrc, -1, tmp, needed, NULL, NULL);
                memcpy(g_clip_text, tmp, CLIP_TEXT_CAP);
                g_clip_text[CLIP_TEXT_CAP] = '\0';
                free(tmp);
            }
        }
    }
    GlobalUnlock(h);
    CloseClipboard();
    return g_clip_text;
}

int clipboard_sys_set_text(const char *utf8) {
    if (utf8 == NULL) {
        return 0;
    }
    int wlen = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
    if (wlen < 1) {
        return 0;
    }
    if (!OpenClipboard(NULL)) {
        return 0;
    }
    EmptyClipboard();
    HGLOBAL h = GlobalAlloc(GMEM_MOVEABLE, (SIZE_T)wlen * sizeof(wchar_t));
    if (h == NULL) {
        CloseClipboard();
        return 0;
    }
    wchar_t *dst = (wchar_t *)GlobalLock(h);
    if (dst == NULL) {
        GlobalFree(h);
        CloseClipboard();
        return 0;
    }
    MultiByteToWideChar(CP_UTF8, 0, utf8, -1, dst, wlen);
    GlobalUnlock(h);
    /* The system owns the handle after a successful call. */
    if (SetClipboardData(CF_UNICODETEXT, h) == NULL) {
        GlobalFree(h);
        CloseClipboard();
        return 0;
    }
    CloseClipboard();
    return 1;
}

int clipboard_sys_clear(void) {
    if (!OpenClipboard(NULL)) {
        return 0;
    }
    EmptyClipboard();
    CloseClipboard();
    return 1;
}
