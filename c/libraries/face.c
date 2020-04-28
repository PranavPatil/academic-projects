#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
void draw_face(int x,int y,int r)
{
	circle(x,y,r);
	if (r<8)
		return;
	draw_face(x-r/2,y-r/3,r/3);
	line(x,y+r/2,x,y-r/2);
	draw_face(x+r/2,y-r/3,r/3);
	arc(x,y,180,0,0.8*r);
}
int main(void)
{
   /* request auto detection */
   int gdriver = DETECT, gmode, errorcode;
   int midx, midy;
   int radius = 230;
   /* initialize graphics and local variables */
   initgraph(&gdriver, &gmode, "");
   /* read result of initialization */
   errorcode = graphresult();
   if (errorcode != grOk)  /* an error occurred */
   {
      printf("Graphics error: %s\n", grapherrormsg(errorcode));
      printf("Press any key to halt:");
      getch();
      exit(1); /* terminate with an error code */
   }
   midx = getmaxx() / 2;
   midy = getmaxy() / 2;
   setcolor(7);
   setbkcolor(1);
   /* draw the circle */
   draw_face(midx, midy, radius);
   /* clean up */
   getch();
   closegraph();
   return 0;
}
