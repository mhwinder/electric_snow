/*---------------------------------------------------------------------------------

	by GugloPWN

---------------------------------------------------------------------------------*/

#ifndef DOT_H
#define DOT_H

#include <nds.h>


class dot {

	public:

	void setdot(int inx, int iny);
	void drawdot(int drawwire, int r, int g, int b);
	void movedot(int inx, int iny);
	
	private:
	
	//The dot elements
	int x[10];
	int y[10];
	int i;
};





#endif

