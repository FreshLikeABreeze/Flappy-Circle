#include <termios.h>
#include <sys/time.h>
#include <linux/fb.h>

typedef unsigned short color_t;
struct fb_fix_screeninfo fix; //bit depth
struct fb_var_screeninfo var; //virtual resolution
struct termios original_tio;
struct termios tio;
struct timeval tv;
struct timespec tim;


void init_graphics();
void exit_graphics();
char getkey();
void sleep_ms(long ms);
void clear_screen();
void draw_pixel(int x, int y, color_t color);
void draw_rect(int x1, int y1, int width, int height, color_t c);
void draw_text(int x, int y, const char *text, color_t c);
void draw_circle(int x, int y, int r, color_t c);
void draw_char(int x, int y, const char *text, color_t c);
color_t encode(int red, int green, int blue);
