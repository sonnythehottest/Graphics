#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <time.h>
//#include <unistd.h>

#define	VIDEO_INT			0x10
#define SET_MODE			0x00
#define VGA_256_COLOR_MODE	0x13
#define TEXT_MODE			0x03

#define	SCREEN_WIDTH		320
#define SCREEN_HEIGHT		200
#define NUM_COLORS			256

#define sgn(x) ((x<0)?-1:((x>0)?1:0))

typedef unsigned char byte;
typedef unsigned short word;

byte *VGA=(byte *)0xA0000000L;
word *my_clock=(word *)0x0000046C;

void set_mode(byte mode){
	union REGS regs;
	
	regs.h.ah = SET_MODE;
	regs.h.al = mode;
	int86(VIDEO_INT, &regs, &regs);
}

void plot_pixel(int x, int y, byte color){
	VGA[(y<<8)+(y<<6)+x] = color;
}

void line_bresenham(int x1, int y1, int x2, int y2, byte color){
	int i,dx,dy,sdx,sdy,dxabs,dyabs,x,y,px,py;
	
	dx=x2-x1;
	dy=y2-y1;
	dxabs=abs(dx);
	dyabs=abs(dy);
	sdx=sgn(dx);
	sdy=sgn(dy);
	x=dyabs>>1;
	y=dxabs>>1;
	px=x1;
	py=y1;
	
	VGA[(py<<8)+(py<<6)+px]=color;
	
	if(dxabs>=dyabs){
		for(i=0;i<dxabs;i++){
			y+=dyabs;
			if(y>=dxabs){
				y-=dxabs;
				py+=sdy;
			}
			px+=sdx;
			plot_pixel(px,py,color);		
		}
	} else {
		for(i=0;i<dyabs;i++){
			x+=dxabs;
			if(x>=dyabs){
				x-=dyabs;
				px+=sdx;
			}
			py+=sdy;
			plot_pixel(px,py,color);
		}
	}
}

void main(){
	int color;
	
	srand(*my_clock);
	set_mode(VGA_256_COLOR_MODE);
	
	color=rand()%NUM_COLORS;
	
//	membuat view (kotak)
	line_bresenham(100,50,170,50,color);
	line_bresenham(170,50,170,120,color);
	line_bresenham(170,120,100,120,color);
	line_bresenham(100,120,100,50,color);
	
//	garis pemotong
	line_bresenham(20,10,215,150,color);
	
//	garis tak memotong
	line_bresenham(200,15,200,170,color);
	
	sleep(10);
	
	set_mode(TEXT_MODE);
	
	return;
}