/*---------------------------------------------------------------------------------

	by GugloPWN based on NeHe 4 by Dovoto

---------------------------------------------------------------------------------*/

// include your ndslib
#include <nds.h>
#include <stdio.h>
#include <fat.h>
#include <sys/stat.h>
#include <filesystem.h>
#include <maxmod9.h>
#include "snow.h"
#include "drawdot.h"
#include "drawstuff.h"

//Audio Business
#include "soundbank.h"
#include "soundbank_bin.h"

//3d Textures
#include "flake1_bin.h"
#include "flake2_bin.h"
#include "flake3_bin.h"
#include "static1_bin.h"
#include "static2_bin.h"
#include "static3_bin.h"
#include "static4_bin.h"
#include "bad2_bin.h"
#include "gift1_bin.h"
#include "circle_bin.h"
#include "credits_bin.h"
#include "backpane_bin.h"
#include "snow_pal_bin.h"
#include "backpane_pal_bin.h"
#include "gift1_pal_bin.h"
#include "static_pal_bin.h"
#include "credits_pal_bin.h"

//2d images
#include "bg.h"

//texture variables
int notex;
int flake1;
int flake2;
int flake3;
int static1;
int static2;
int static3;
int static4;
int bad2;
int gift1;
int circle;
int credits;
int backpane;
int snow_pal;
int backpane_pal;
int gift1_pal;
int static_pal;
int credits_pal;

//SFX
mm_sound_effect ting1fx;
mm_sound_effect ting2fx;
mm_sound_effect ting3fx;
mm_sound_effect ting4fx;
mm_sound_effect wah1fx;
mm_sound_effect wah2fx;
mm_sound_effect wah3fx;
mm_sound_effect wah4fx;
mm_sound_effect psssfx;

//Functions
void seedrandom();
void input();
void loadtex();
void subload();

void audioload();
void audiorun();

void demosnow();
void nukesnow();
void drawsnow(int style, int r, int g, int b);
void spawngift();
bool giftcollision();
void rungame();
void startgame();
void endgame();
void drawgame();

int x = 0;
int y = 0;
int flakenum = 0;
int animation = 0;
dot *player;
flake *snow[200];
touchPosition touch;

bool wastouch = false;
bool isGame = false;
bool soundisgo = true;
int hp;
int bombs;
int milestone = 0;
int killcount = 0;
u32 score;
u32 hiscore = 35000;
int giftx;
int gifty;
int flashtimer = 0;
FILE* scorefile;


int main() {	

	// Setup the Main screen for 3D 
	videoSetMode(MODE_0_3D);
	videoSetModeSub(MODE_3_2D);

	//Set Video RAM
	vramSetBankA(VRAM_A_TEXTURE);
	vramSetBankB(VRAM_B_TEXTURE);
	vramSetBankC(VRAM_C_SUB_BG);
	vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE);

	
	// initialize the geometry engine
	glInit();
	
	// enable antialiasing
	//glEnable(GL_ANTIALIAS);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	//Enable Console
	PrintConsole bottomScreen;
	consoleInit(&bottomScreen, 2,BgType_Text4bpp, BgSize_T_256x256, 0, 1, false, true);
	bgSetPriority(bottomScreen.bgId,0);
	printf("\n console init");
	
	// setup the rear plane
	glClearColor(0,0,0,31); // BG must be opaque for AA to work
	glClearPolyID(63); // BG must have a unique polygon ID for AA to work
	glClearDepth(0x7FFF);

	// Set our viewport to be the same size as the screen
	glViewport(0,0,255,191);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 256.0 / 192.0, 0.1, 100);


	//ds specific, several attributes can be set here	
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(0));
	
	// Set the current matrix to be the model matrix
	glMatrixMode(GL_MODELVIEW);

	seedrandom();

	//Init Filesystem
	printf("\nInitialize FAT... ");
	if (fatInitDefault()) {printf(" success!");};

	mkdir("fat:/data",0777);
	
	scorefile = fopen("fat:/data/electricsnow.txt","rw+");
	if(scorefile != NULL) fscanf(scorefile,"%10d",&hiscore);
	fclose(scorefile);
	

	int i;
	for(i=0;i<200;i++){snow[i]= new flake();}
	player = new dot();
	player->setdot(-23000,-23000);
	

	demosnow();
	spawngift();

	loadtex();
	
	
	//initialize the console on the subscreen
	subload();
	
	//lcdSwap();
	
	if (soundisgo) audioload();
	

	while (1) 
	{
		//Clean up the Console
		consoleClear();
		scanKeys();
		touchRead(&touch);

		//printf("\nx: %d y: %d anim: %d flakes: %d", x,y,animation, flakenum);
		
		if(isGame) rungame();
		else
		{
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n     Touch and hold to play!");
			printf("\n     Score: %d",score);
			printf("\n     High Score: %d",hiscore);
			
			if ((keysDown() & KEY_TOUCH)) {startgame();}
			else 
			{
				drawsnow(0,255,255,255); 
				glBindTexture(0,credits);
				glColorTable(GL_RGB256, credits_pal);
				drawfrontflash(255,255,255,31);
			}
		}

		// flush to screen	
		glFlush(0);
		
		// wait for the screen to refresh
		swiWaitForVBlank();

		animation++;
		if (animation > 360) animation = 1;
	}
	
	return 0;
}


void input()
{
	/*
	if ((keysDown() & KEY_A)) 
	{
		snow[flakenum]->spawnflake(); 
		flakenum++;
		if (flakenum > 200) flakenum = 0;
	}
	*/

	if ((keysHeld() & KEY_X)) flashtimer = 31;

	return;
}

void rungame()
{
	//------ The Whole Game Loop ------
	if (!((keysHeld() & KEY_TOUCH))) 
	{
		killcount+=3;
		flashtimer = killcount;
		if (killcount > 30) {endgame(); return;};
	}

	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	printf("\n     Score: %d",score);
	printf("\n     High Score: %d",hiscore);
	printf("\n     Bombs: %d",bombs);
	printf("\n     HP: %d",hp);

	if ((keysHeld() & KEY_TOUCH))
	{
		x = (int)(touch.px*180.3921-23000);
		y = (int)(-touch.py*180.208+17300);
		killcount = 0;
	}

	int i;
	if ((keysDown() & (KEY_L | KEY_R | KEY_A | KEY_B | KEY_X | KEY_Y | KEY_LEFT | KEY_DOWN | KEY_UP | KEY_RIGHT))) nukesnow();
	player->movedot(x,y);

	giftcollision();
	
	for(i=0;i<200;i++)
	{
		if(snow[i]->check_collision(x,y)) 
		{
			hp--; 
			snow[i]->killflake();
			if(soundisgo) 
			{
				if (milestone == 0 ) mmEffectEx(&wah1fx);
				else if (milestone == 1) mmEffectEx(&wah2fx);
				else if (milestone == 2) mmEffectEx(&wah3fx);
				else if (milestone == 3) mmEffectEx(&wah4fx);
			}
		}
	}

	if (hp < 1) {endgame(); return;}

	if (animation == 360)
	{
		snow[flakenum]->spawnflake(); 
		flakenum++;
		if (flakenum > 199) flakenum = 0;
	}

	if((animation%3) == 0) score++;

	drawgame();
	
	return;
}

//-------------------- BORING STUFF AHEAD -------------------------

void drawgame()
{
	//Draw The scene!
	if (score < 2500) //Peace
	{
		glBindTexture(0,circle);
		glColorTable(GL_RGB256, snow_pal);
		player->drawdot(0,255,255,255);

		drawsnow(0,255,255,255);

		glBindTexture(0,backpane);
		glColorTable(GL_RGB256, backpane_pal);
		drawbackpane((255),(255),(255));

		if(flashtimer>0)
		{
			glBindTexture(0,notex);
			glColorTable(GL_RGB256, snow_pal);
			drawfrontflash(255,255,255,flashtimer);
			flashtimer--;
		}
	

		glBindTexture(0,gift1);
		glColorTable(GL_RGB256, gift1_pal);
		drawgift(giftx,gifty,animation,255,255,255);

	}
	else if (score < 13200) //Darkness Falls
	{
		glBindTexture(0,circle);
		glColorTable(GL_RGB256, snow_pal);
		player->drawdot(0,255,255,255);

		drawsnow(0,255,255,255);

		glBindTexture(0,backpane);
		glColorTable(GL_RGB256, backpane_pal);
		drawbackpane((255-(score-2500)/42),(255-(score-2500)/42),(255-(score-2500)/42));

		if (soundisgo) mmSetModuleVolume( 1024-(score-2500)/11 );  //MUSIC

		if(flashtimer>0)
		{
			glBindTexture(0,notex);
			glColorTable(GL_RGB256, snow_pal);
			drawfrontflash(255,255,255,flashtimer);
			flashtimer--;
		}
	

		glBindTexture(0,gift1);
		glColorTable(GL_RGB256, gift1_pal);
		drawgift(giftx,gifty,animation,255,255,255);

	}
	else if (score < 16000) //Silent Night
	{
		glBindTexture(0,circle);
		glColorTable(GL_RGB256, snow_pal);
		player->drawdot(0,255,255,255);

		drawsnow(0,255,255,255);

		if (soundisgo) mmSetModuleVolume( 0 );  //MUSIC

		if(flashtimer>0)
		{
			glBindTexture(0,notex);
			glColorTable(GL_RGB256, snow_pal);
			drawfrontflash(255,255,255,flashtimer);
			flashtimer--;
		}
	

		glBindTexture(0,gift1);
		glColorTable(GL_RGB256, gift1_pal);
		drawgift(giftx,gifty,animation,255,255,255);

	}
	else if (score < 25000) //Fire
	{
		if (milestone == 0)
		{
			if (soundisgo) mmStart( MOD_0110101001, MM_PLAY_LOOP );
			if (soundisgo) mmSetModuleVolume( 1024 );  //MUSIC
			flashtimer=31;
			milestone++;
		}
		int glow = 0;
		if (animation <180) glow = animation;
		else glow = 360-animation;

		glBindTexture(0,circle);
		glColorTable(GL_RGB256, snow_pal);
		player->drawdot(0,0,glow+75,0);

		drawsnow(0,255,glow,0);

		if(flashtimer>0)
		{
			glBindTexture(0,notex);
			glColorTable(GL_RGB256, snow_pal);
			drawfrontflash(255,0,0,flashtimer);
			flashtimer--;
		}
	

		glBindTexture(0,gift1);
		glColorTable(GL_RGB256, gift1_pal);
		drawgift(giftx,gifty,animation,255,glow,0);
	}
	else if (score<35000) //Static
	{
		if (milestone == 1)
		{
			flashtimer=31;
			milestone++;
		}

		int glow = 0;
		if (animation <180) glow = animation;
		else glow = 360-animation;
		

		drawsnow(1,255,255,255);

		player->drawdot(0,0,0,glow+75);

		if(flashtimer>0)
		{
			drawfrontflash(255,255,255,flashtimer);
			flashtimer--;
		}

		drawgift(giftx,gifty,animation,0,glow+65,0);
	}
	else //Wireframe
	{

		if (milestone == 2)
		{
			if (soundisgo) mmStart( MOD_OCH_VEM_AER_DU, MM_PLAY_LOOP );
			if (soundisgo) mmSetModuleVolume( 1024 );  //MUSIC
			flashtimer=31;
			milestone++;
		}

		int glow = 0;
		if (animation <180) glow = animation;
		else glow = 360-animation;

		glBindTexture(0,notex);
		player->drawdot(1,glow+75,0,0);

		drawsnow(2,255,255,255);
	
		glColorTable(GL_RGB256, static_pal);
		if ((animation%8) < 2) {glBindTexture(0,static1);}
		else if ((animation%8) < 4) {glBindTexture(0,static2);}
		else if ((animation%8) < 6) {glBindTexture(0,static3);}
		else {glBindTexture(0,static4);}

		if(flashtimer>0)
		{
			drawfrontflash(255,255,255,flashtimer);
			flashtimer--;
		}
		
	
		drawbackpane(0,0,glow/3+30);

		glPolyFmt(POLY_ALPHA(0) | POLY_CULL_NONE);
		glBindTexture(0,notex);
		drawgift(giftx,gifty,animation,0,glow+75,0);
		glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);

		
	}
	return;
}


void startgame()
{
	
	score = 0;
	hp = 25; 
	bombs = 4;
	nukesnow();
	spawngift();
	isGame = true;
	wastouch = true;
	milestone = 0;

	x = (int)(touch.px*180.3921-23000);
	y = (int)(-touch.py*180.208+17300);
	player->setdot(x,y);
	drawgame();
	glFlush(0);
	swiWaitForVBlank();
	lcdSwap();
	return;
}

void endgame()
{
	
	demosnow();
	isGame = false;

	//Check for highscores here
	if (score > hiscore) 
	{
		scorefile = fopen ("fat:/data/electricsnow.txt","w+");
		fprintf(scorefile,"%10d",score);
		fclose(scorefile);
		hiscore = score;
	}

	glFlush(0);
	swiWaitForVBlank();
	lcdSwap();
	if (soundisgo) mmStart( MOD_2_SILVER_BALLS, MM_PLAY_LOOP );
	if (soundisgo) mmSetModuleVolume( 1024 );  //MUSIC
	return;
}

void demosnow()
{
	nukesnow();
	
	int n;
	for(n=0;n<50;n++)
	{
		snow[flakenum]->spawnflakerand(); 
		flakenum++;
		if (flakenum > 199) flakenum = 0;
	}
	return;
}

void audioload()
{
	mmInitDefaultMem((mm_addr)soundbank_bin);
	
	// load the module
	mmLoad( MOD_2_SILVER_BALLS );
	mmLoad( MOD_0110101001 );
	mmLoad( MOD_OCH_VEM_AER_DU );


	// load sound effects
	mmLoadEffect( SFX_PSSS );
	mmLoadEffect( SFX_TING1 );
	mmLoadEffect( SFX_TING2 );
	mmLoadEffect( SFX_TING3 );
	mmLoadEffect( SFX_TING4 );
	mmLoadEffect( SFX_WAH1 );
	mmLoadEffect( SFX_WAH2 );
	mmLoadEffect( SFX_WAH3 );
	mmLoadEffect( SFX_WAH4 );

	// Start playing module
	mmStart( MOD_2_SILVER_BALLS, MM_PLAY_LOOP );

	ting1fx = {
		{ SFX_TING1 } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		0,		// panning
	};

	ting2fx = {
		{ SFX_TING2 } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		0,		// panning
	};

	ting3fx = {
		{ SFX_TING3 } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		0,		// panning
	};

	ting4fx = {
		{ SFX_TING4 } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		0,		// panning
	};

	wah1fx = {
		{ SFX_WAH1 } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		0,		// panning
	};

	wah2fx = {
		{ SFX_WAH2 } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		0,		// panning
	};

	wah3fx = {
		{ SFX_WAH3 } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		0,		// panning
	};

	wah4fx = {
		{ SFX_WAH4 } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		0,		// panning
	};

	psssfx = {
		{ SFX_PSSS } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		0,	// panning
	};
}

void nukesnow()
{
	if(bombs < 1) return;
	int n;
	for(n=0;n<200;n++)
	{
		snow[n]->killflake();
	}
	bombs--;
	flashtimer = 31;
	if(soundisgo) mmEffectEx(&psssfx);
	return;
}

void drawsnow(int style, int r, int g, int b)
{
	int i;
	for(i=0;i<200;i++){snow[i]->moveflake();}

	glColorTable(GL_RGB256, snow_pal);
	
	if(style == 0)
	{
		for(i=0;i<200;i+=3){glBindTexture(0,flake1); snow[i]->drawflake(r,g,b);}
		for(i=1;i<200;i+=3){glBindTexture(0,flake2); snow[i]->drawflake(r,g,b);}
		for(i=2;i<200;i+=3){glBindTexture(0,flake3); snow[i]->drawflake(r,g,b);}
	}
	else if(style == 1)
	{
		glColorTable(GL_RGB256, static_pal);
		if ((animation%8) < 2) {glBindTexture(0,static1);}
		else if ((animation%8) < 4) {glBindTexture(0,static2);}
		else if ((animation%8) < 6) {glBindTexture(0,static3);}
		else {glBindTexture(0,static4);}

		for(i=0;i<200;i++){snow[i]->drawflake(r,g,b);}
	}
	else if(style == 2)
	{
		glPolyFmt(POLY_ALPHA(0) | POLY_CULL_NONE);
		glBindTexture(0,notex);
		for(i=0;i<200;i++){snow[i]->drawflake(r,g,b);}
		glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);
	}
	else if(style == 3)
	{
		glBindTexture(0,12);
		for(i=0;i<200;i++){snow[i]->drawflake(r,g,b);}
	}
	return;
}

void spawngift()
{
	gifty = (rand() % 1730) * -10;
	giftx = (rand() % 2300) * 10;
	if(x>0) giftx = (-giftx);
	return;
}

bool giftcollision()
{
	if (sqrt(((int64)x-(int64)giftx)*((int64)x-(int64)giftx)+((int64)y-(int64)gifty)*((int64)y-(int64)gifty)) > 3072) return false;
	spawngift();
	score += 200;

	if(soundisgo) 
	{
		if (milestone == 0 ) mmEffectEx(&ting1fx);
		else if (milestone == 1) mmEffectEx(&ting2fx);
		else if (milestone == 2) mmEffectEx(&ting3fx);
		else if (milestone == 3) mmEffectEx(&ting4fx);
	}

	snow[flakenum]->spawnflake(); 
	flakenum++;
	if (flakenum > 199) flakenum = 0;
	
	return true;
}


void loadtex()
{

	glGenTextures(1,&flake1);
	glBindTexture(0,flake1);
	glTexImage2D(0,0,GL_RGB16,TEXTURE_SIZE_32,TEXTURE_SIZE_32,0,TEXGEN_TEXCOORD|(1<<29),(u8*)flake1_bin);
	
	glGenTextures(1,&flake2);
	glBindTexture(0,flake2);
	glTexImage2D(0,0,GL_RGB16,TEXTURE_SIZE_32,TEXTURE_SIZE_32,0,TEXGEN_TEXCOORD|(1<<29),(u8*)flake2_bin);

	glGenTextures(1,&flake3);
	glBindTexture(0,flake3);
	glTexImage2D(0,0,GL_RGB16,TEXTURE_SIZE_32,TEXTURE_SIZE_32,0,TEXGEN_TEXCOORD|(1<<29),(u8*)flake3_bin);

	glGenTextures(1,&static1);
	glBindTexture(0,static1);
	glTexImage2D(0,0,GL_RGB16,TEXTURE_SIZE_32,TEXTURE_SIZE_32,0,TEXGEN_TEXCOORD,(u8*)static1_bin);

	glGenTextures(1,&static2);
	glBindTexture(0,static2);
	glTexImage2D(0,0,GL_RGB16,TEXTURE_SIZE_32,TEXTURE_SIZE_32,0,TEXGEN_TEXCOORD,(u8*)static2_bin);

	glGenTextures(1,&static3);
	glBindTexture(0,static3);
	glTexImage2D(0,0,GL_RGB16,TEXTURE_SIZE_32,TEXTURE_SIZE_32,0,TEXGEN_TEXCOORD,(u8*)static3_bin);

	glGenTextures(1,&static4);
	glBindTexture(0,static4);
	glTexImage2D(0,0,GL_RGB16,TEXTURE_SIZE_32,TEXTURE_SIZE_32,0,TEXGEN_TEXCOORD,(u8*)static4_bin);

	glGenTextures(1,&bad2);
	glBindTexture(0,bad2);
	glTexImage2D(0,0,GL_RGB16,TEXTURE_SIZE_32,TEXTURE_SIZE_32,0,TEXGEN_TEXCOORD|(1<<29),(u8*)bad2_bin);

	glGenTextures(1,&circle);
	glBindTexture(0,circle);
	glTexImage2D(0,0,GL_RGB16,TEXTURE_SIZE_32,TEXTURE_SIZE_32,0,TEXGEN_TEXCOORD|(1<<29),(u8*)circle_bin);

	glGenTextures(1,&gift1);
	glBindTexture(0,gift1);
	glTexImage2D(0,0,GL_RGB16,TEXTURE_SIZE_32,TEXTURE_SIZE_32,0,TEXGEN_TEXCOORD,(u8*)gift1_bin);

	glGenTextures(1,&backpane);
	glBindTexture(0,backpane);
	glTexImage2D(0,0,GL_RGB256,TEXTURE_SIZE_256,TEXTURE_SIZE_256,0,TEXGEN_TEXCOORD,(u8*)backpane_bin);

	glGenTextures(1,&credits);
	glBindTexture(0,credits);
	glTexImage2D(0,0,GL_RGB16,TEXTURE_SIZE_256,TEXTURE_SIZE_128,0,TEXGEN_TEXCOORD|(1<<29),(u8*)credits_bin);

	snow_pal = gluTexLoadPal((u16*)snow_pal_bin,256,GL_RGB256);
	backpane_pal = gluTexLoadPal((u16*)backpane_pal_bin,256,GL_RGB256);
	gift1_pal = gluTexLoadPal((u16*)gift1_pal_bin,256,GL_RGB256);
	static_pal = gluTexLoadPal((u16*)static_pal_bin,256,GL_RGB256);
	credits_pal = gluTexLoadPal((u16*)credits_pal_bin,256,GL_RGB256);

	//No texture
	glGenTextures(1,&notex);
	glBindTexture(0,notex);

}

void subload()
{
	int bg3 = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 4,0);

	dmaCopy(bgBitmap, bgGetGfxPtr(bg3), sizeof(bgBitmap));
	dmaCopy(bgPal, BG_PALETTE_SUB, sizeof(bgPal));
	BG_PALETTE_SUB[255] = RGB15(31,31,31);
}

void seedrandom()
{
	int hours, seconds, minutes; 
	time_t unixTime = time(NULL);
	struct tm* timeStruct = gmtime((const time_t *)&unixTime);
		
	hours = timeStruct->tm_hour;
	minutes = timeStruct->tm_min;
	seconds = timeStruct->tm_sec;

	int random_seed = hours+seconds+minutes;
	srand(random_seed);
}

