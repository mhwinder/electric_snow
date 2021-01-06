/*---------------------------------------------------------------------------------

	by GugloPWN

---------------------------------------------------------------------------------*/

#ifndef SNOW_H
#define SNOW_H

#include <nds.h>
#include <stdio.h>
#include <string.h>
#include <math.h>



class flake {

	public:

	void spawnflake();
	void spawnflakerand();
	void moveflake();
	void drawflake(int r,int g,int b);
	void killflake();
	bool check_collision(int cx,int cy);
	
	//Setters and getters for the various block elements
	inline int getx() {return x;}
	inline int gety() {return y;}
	inline bool getisLive() {return isLive;}
	
	private:
	
	//The flake elements
	int x;
	int y;
	int spin;
	int spinspeed;
	bool isLive;
	
};


#endif


