#ifndef __FRAMEBUFFER_H
#define __FRAMEBUFFER_H

void framebuffer_fail(unsigned int num);
void framebuffer_init();
void framebuffer_vert_scroll();
void console_write(char *text);
#endif
