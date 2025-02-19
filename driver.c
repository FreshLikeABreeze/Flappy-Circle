#include "graphics.h"
#include "library.c"

int main() {
	clear_screen();
	init_graphics();

	char key = getkey();
	
	color_t green = encode(0,63,0);
	
	int midx = 640/2;
	int midy = 479/2;
	
	//create fist circle
	draw_circle(midx, midy, 15, encode(31,0,0));
	
	int x = midx;
	int y = midy;
	
	//game rectangle
	
	//draw_rect(620,340,encode(31,0,0));
	
	int allRight = 620;
	int allDown = 479;
	
	int exit = 0;
	int rec1 = allRight;
	int rec2 = -11;
	int rec3 = -11;
	while(exit != 1)
	{
		switch(key)
		{
			case 'q':
				exit = 1;
				break;
			case 'w':
				y = y - 15;
				break;
			case 'a':
				//x = x - 5;
				break;
			case 's':
				y = y + 15;
				break;
			case 'd':
				//x = x + 5;
				break;
			/*case 'p':
				draw_pixel(30,50,green);
			*/	break;	
			case 'r':
				
				break;
			case 't':
				
				break;
			/*case 'c':
				draw_circle(100, 200, 15, encode(31,0,0));
				break;
			*/
		}
		
		clear_screen();
		draw_circle(x, y, 15, encode(31,0,0));
		
		//inting
		if(rec1 >= allRight / 3 - 5 && rec1 <= allRight / 3 + 5 )
		{
			rec2 = allRight;
		}
		if(rec2 >= allRight / 3 - 5 && rec2 <= allRight / 3 + 5 )
		{
			rec3 = allRight;
		}
		
		//resetting
		if(rec1 >= -10 && rec1 <= 0)
		{
			rec1 = allRight;
		}
		if(rec2 >= -10 && rec2 <= 0)
		{
			rec2 = allRight;
		}
		if(rec3 >= -10 && rec3 <= 0)
		{
			rec3 = allRight;
		}
		
		//subbing
		if(rec1 !=0)
		{
			rec1 -= 10;
		}
		if(rec2 !=0)
		{
			rec2 -= 10;
		}
		if(rec3 !=0)
		{
			rec3 -= 10;
		}
		
		
		draw_rect(rec1,0,10,340,green);
		if(rec2 > 0)
		{
			draw_rect(rec2,479 - 340 ,10,340,green);
		}
		if(rec3 > 0)
		{
			draw_rect(rec3,0,10,340,green);
		}
		
		
		
		//draw_rect(rec1,0,10,340,green);
		
		if(x + 15 >= rec1 && y - 15 < 340 && x - 15 < (rec1 + 10))
		{
			
			exit = 1;
		}
		if(x + 15 >= rec2 && y + 15 > (479 - 340) && x - 15 < (rec2 + 10))
		{
			
			exit = 1;
		}
		if(x + 15 >= rec3 && y - 15 < 340 && x - 15 < (rec3 + 10))
		{
			
			exit = 1;
		}
		
		
		key = getkey();
	}
	draw_text(midx, midy, "l0l!", encode(23,54,16));
	exit_graphics();
	
	

    return 0;
}
