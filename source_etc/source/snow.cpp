/*---------------------------------------------------------------------------------

	by GugloPWN

---------------------------------------------------------------------------------*/

#include "snow.h"

void flake::spawnflake()
{
	x = (rand() % 4600 - 2300) * 10;
	y = 17300;
	spinspeed = rand() % 512 - 256;
	isLive = true;
	return;
}

void flake::spawnflakerand()
{
	x = (rand() % 4600 - 2300) * 10;
	y = (rand() % 3460 - 1730) * 10;;
	spinspeed = rand() % 512 - 256;
	isLive = true;
	return;
}

void flake::moveflake()
{
	if (!isLive) {y = 18000; return;}
	y -= 100;
	spin += spinspeed;
	if (spin > DEGREES_IN_CIRCLE) spin -= DEGREES_IN_CIRCLE;
	if (spin < 0) spin += DEGREES_IN_CIRCLE;
	if (y < -17300) {spawnflake();}
	return;
}

void flake::drawflake(int r,int g,int b)
{
	if (!isLive) return;
	glLoadIdentity();
	glTranslate3f32(x,y,-24576);
	glRotatef32i(spin,0,0,4096);
	glColor3b(r,g,b);
	glBegin(GL_QUADS);
		glTexCoord2f32(0,0);
		glVertex3v16(1400, 1400, 0);
		glTexCoord2f32(0,4096);
		glVertex3v16(1400, -1400, 0);
		glTexCoord2f32(4096,4096);
		glVertex3v16(-1400, -1400, 0);
		glTexCoord2f32(4096,0);
		glVertex3v16(-1400, 1400, 0);
	glEnd();
	return;
}

void flake::killflake()
{
	isLive = false;
}

bool flake::check_collision(int cx,int cy)
{
	if (!isLive) return false;
	if (sqrt(((int64)x-(int64)cx)*((int64)x-(int64)cx)+((int64)y-(int64)cy)*((int64)y-(int64)cy)) > 1536) return false;
	return true;
}




