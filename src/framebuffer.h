#ifndef __FRAMEBUFFER_H
#define __FRAMEBUFFER_H

static void framebuffer_fail(unsigned int num);
void framebuffer_init();
static void framebuffer_vert_scroll();
void console_write(char *text);
#endif
