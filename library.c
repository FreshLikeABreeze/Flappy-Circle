#include "graphics.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <math.h>
#include "iso_font.h"

int size;
int fb;
void *map;


color_t encode(int red, int green, int blue)
{
	/*
	* red (0-31)
	* green (0-63)
	* blue (0-31)
	*/
	color_t done = 0;
	
	int red_counter = 0;
	int green_counter = 0;
	int blue_counter = 0;
	
	while(red_counter < 5)
	{
		if(red % 2 == 1)
		{
			done = done | (1 << (15 - red_counter));
			
		}
		
		red /= 2;
		red_counter += 1;
	}
	
	while(green_counter < 5)
	{
		if(green % 2 == 1)
		{
			done = done | (1 << (10 - green_counter));
			
		}
		
		green /= 2;
		green_counter += 1;
	}
	
	while(blue_counter < 5)
	{
		if(blue % 2 == 1)
		{
			done = done | (1 << (4 - blue_counter));
			
		}
		
		blue /= 2;
		blue_counter += 1;
	}
	
	return done;
}

void init_graphics()
{
	
	//open frambuf
	fb = open("/dev/fb0", O_RDWR);
	
	
	//get screensize and bits per pixel
	ioctl(fb, FBIOGET_VSCREENINFO, &var);
	ioctl(fb, FBIOGET_FSCREENINFO, &fix);
	
	//total size of map file
	size = var.yres_virtual * fix.line_length;
	
	
	map = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);
	
	
	//get terminal settings
	ioctl(STDIN_FILENO, TCGETS, &original_tio);
	
	tio = original_tio;

	tio.c_lflag &= ~ICANON; //disable canonical
	tio.c_lflag &= ~ECHO; //disable echo
	//set new terminal settings
	ioctl(STDIN_FILENO, TCSETS, &tio);
}

void exit_graphics()
{
	close(fb);
	ioctl(STDIN_FILENO, TCSETS, &original_tio);
	munmap(map, size);
}

char getkey()
{
	fd_set rfds;
	
	FD_ZERO(&rfds);
	FD_SET(0, &rfds);
	
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	
	
	int select_return = select(1, &rfds, NULL, NULL, &tv );
	if(select_return > 0)
	{
		char key;
		read(STDIN_FILENO, &key, 1);
		return key;
	}
	//fallback key
	return '\0';
	
}

void sleep_ms(long ms)
{
	tim.tv_sec = ms / 1000;
	tim.tv_nsec = (ms % 1000) * 1000000;
	nanosleep(&tim, NULL);
}

void clear_screen()
{
	write(STDOUT_FILENO, "\033[2J", sizeof("\033[2J") - 1);
	
}

void draw_pixel(int x, int y, color_t color)
{
	int width = var.xres_virtual;
	int bytes_per_pix = var.bits_per_pixel / 8;
	size_t offset = (y * width + x) * bytes_per_pix;
	*((color_t*)((char*)map + offset)) = color;
}

void draw_rect(int x1, int y1, int width, int height, color_t c)
{
	int xright = x1 + width;
	int ydown = y1 + height;
	
	for (int ypoint = y1; ypoint < ydown; ypoint++)
	{
		for (int xpoint = x1; xpoint < xright; xpoint++)
		{
			draw_pixel(xpoint, ypoint, c);
		}
	}
	
}

void draw_text(int x, int y, const char *text, color_t c)
{
	while(*text != '\0')
	{
		draw_char(x,y,*text,c);
		x = x + 8;
		text++;
	}
}

void draw_char(int x, int y, const char *text, color_t c)
{
	//8 cols
	//16 rows
	int char_index = (int)text * 16;
	

	for(int rows = 0; rows < 16; rows++)
	{
		//int row_bits = iso_font[char_index + rows];
		
		for(int cols = 0; cols <8; cols++)
		{
			if(iso_font[(int)text * 16 + rows] & (1 << (7 - cols)))
			{
				draw_pixel(x + cols, y + rows, c);
			}
		}
	}
		
	
}

void draw_circle(int x, int y, int r, color_t c)
{
	//http://www.softwareandfinance.com/CSharp/Draw_Circle_Pixel.html
	
	double radius = r;
	for (double i = 0.0; i < 360.0; i += 0.1)
	{
		double angle = i * 3.1415 / 180.0;
		int circlex = (int)(x + radius * cos(angle));
		int cirlcey = (int)(y + radius * sin(angle));
		if (circlex >= 0 && circlex < var.xres_virtual && cirlcey >= 0 && cirlcey < var.yres_virtual)
		{
			draw_pixel(circlex,cirlcey,c);
		}
		
	}
	
	
	
}
