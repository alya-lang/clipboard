#ifndef ALYA_Clipboard_H
#define ALYA_Clipboard_H

int alya_clipboard_add(int a, int b);
int clipboard_text_limit(void);
int clipboard_native_available(void);
int clipboard_native_seq(void);
int clipboard_sys_has_text(void);
const char *clipboard_sys_get_text(void);
int clipboard_sys_set_text(const char *utf8);
int clipboard_sys_clear(void);

#endif
