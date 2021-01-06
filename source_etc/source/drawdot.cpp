/*---------------------------------------------------------------------------------

	by GugloPWN

---------------------------------------------------------------------------------*/

#include "drawdot.h"
#include <stdio.h>

void dot::setdot(int inx, int iny)
{
	int j;
	i = 0;
	for(j=0;j<10;j++)
	{
		x[j] = inx;
		y[j] = iny;
	}
	
	return;
}

void dot::drawdot(int drawwire, int r, int g, int b)
{
	int j,k=i;
	glColor3b(r,g,b);
	for(j=0;j<10;j++)
	{
		if (drawwire == 0) glPolyFmt(POLY_ALPHA(31-3*j) | POLY_CULL_NONE | POLY_ID(2));
		else glPolyFmt(POLY_ALPHA(0) | POLY_CULL_NONE | POLY_ID(2));
		glLoadIdentity();
		glTranslate3f32(x[k],y[k],-24576);
		glBegin(GL_QUADS);
			glTexCoord2f32(0,0);
			glVertex3v16(1024-(50*j), 1024-(50*j), 0);
			glTexCoord2f32(0,4096);
			glVertex3v16(1024-(50*j), -1024+(50*j), 0);
			glTexCoord2f32(4096,4096);
			glVertex3v16(-1024+(50*j), -1024+(50*j), 0);
			glTexCoord2f32(4096,0);
			glVertex3v16(-1024+(50*j), 1024-(50*j), 0);
		glEnd();
		k--;
		if(k<0) k=9;
	}
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(0));
	return;
}

void dot::movedot(int inx, int iny)
{
	//printf("\nI: %d",i);
	i++;
	if(i>9) i=0;
	x[i] = inx;
	y[i] = iny;
	return;
}



