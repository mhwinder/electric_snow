/*---------------------------------------------------------------------------------

	by GugloPWN

---------------------------------------------------------------------------------*/

#include "drawstuff.h"


void drawbackpane(int r, int g, int b)
{
	glColor3b(r,g,b);
	glLoadIdentity();
	glTranslate3f32(0,0,-28276);
	glBegin(GL_QUADS);
		glTexCoord2f32(0,0);
		glVertex3v16(26400, 19700, 0);
		glTexCoord2f32(0,4096);
		glVertex3v16(26400, -19800, 0);
		glTexCoord2f32(4096,4096);
		glVertex3v16(-26400, -19800, 0);
		glTexCoord2f32(4096,0);
		glVertex3v16(-26400, 19700, 0);
	glEnd();
	return;
}

void drawfrontflash(int r, int g, int b, int flashtimer)
{
	glColor3b(r,g,b);
	glPolyFmt(POLY_ALPHA(flashtimer) | POLY_CULL_NONE | POLY_ID(31));
	glLoadIdentity();
	glTranslate3f32(0,0,-20276);
	glBegin(GL_QUADS);
		glTexCoord2f32(4096,0);
		glVertex3v16(18900, 14200, 0);
		glTexCoord2f32(4096,4096);
		glVertex3v16(18900, -14200, 0);
		glTexCoord2f32(0,4096);
		glVertex3v16(-18900, -14200, 0);
		glTexCoord2f32(0,0);
		glVertex3v16(-18900, 14200, 0);
	glEnd();
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(0));
	return;
}

void drawgift(int x, int y, int animation, int r, int g, int b)
{
	glColor3b(r,g,b);
	glLoadIdentity();
	glTranslate3f32(x,y,-24576);
	glRotatef32i(animation*DEGREES_IN_CIRCLE/360,4096,4096,1024);
	glBegin(GL_QUADS);
		glTexCoord2f32(0,0);		//Front
		glVertex3v16(2048, 2048, 2048);
		glTexCoord2f32(0,4096);
		glVertex3v16(2048, -2048, 2048);
		glTexCoord2f32(4096,4096);
		glVertex3v16(-2048, -2048, 2048);
		glTexCoord2f32(4096,0);
		glVertex3v16(-2048, 2048, 2048);

		glTexCoord2f32(0,0);		//Back
		glVertex3v16(2048, 2048, -2048);
		glTexCoord2f32(0,4096);
		glVertex3v16(2048, -2048, -2048);
		glTexCoord2f32(4096,4096);
		glVertex3v16(-2048, -2048, -2048);
		glTexCoord2f32(4096,0);
		glVertex3v16(-2048, 2048, -2048);

		glTexCoord2f32(0,0);		//Top
		glVertex3v16(2048, 2048, 2048);
		glTexCoord2f32(0,4096);
		glVertex3v16(2048, 2048, -2048);
		glTexCoord2f32(4096,4096);
		glVertex3v16(-2048, 2048, -2048);
		glTexCoord2f32(4096,0);
		glVertex3v16(-2048, 2048, 2048);

		glTexCoord2f32(0,0);		//Bottom
		glVertex3v16(2048, -2048, 2048);
		glTexCoord2f32(0,4096);
		glVertex3v16(2048, -2048, -2048);
		glTexCoord2f32(4096,4096);
		glVertex3v16(-2048, -2048, -2048);
		glTexCoord2f32(4096,0);
		glVertex3v16(-2048, -2048, 2048);

		glTexCoord2f32(0,0);		//left
		glVertex3v16(2048, 2048, 2048);
		glTexCoord2f32(0,4096);
		glVertex3v16(2048, 2048, -2048);
		glTexCoord2f32(4096,4096);
		glVertex3v16(2048, -2048, -2048);
		glTexCoord2f32(4096,0);
		glVertex3v16(2048, -2048, 2048);

		glTexCoord2f32(0,0);		//Right
		glVertex3v16(-2048, 2048, 2048);
		glTexCoord2f32(0,4096);
		glVertex3v16(-2048, 2048, -2048);
		glTexCoord2f32(4096,4096);
		glVertex3v16(-2048, -2048, -2048);
		glTexCoord2f32(4096,0);
		glVertex3v16(-2048, -2048, 2048);
	glEnd();
}




