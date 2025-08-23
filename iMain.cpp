#include "iGraphics.h"
#include "Audio.hpp"
#include "Home_menu_level.hpp"
#include "Level1.hpp"

// Blink text toggle
int showText = 1;

// Backgrounds
int bgImage1, bgImage2, bgPlayGround;
int currentPage = 1;  // 1 = home, 2 = menu, 3 = playground

// Menu button images
int img[4];
int imgX[4] = { 420, 420, 420, 420 };
int imgY[4] = { 430, 310, 210, 110 };
int imgW = 200, imgH = 70;

// Hover effect
float hoverScale = 1.2f;
int hoveredImg = -1;

// Page states
int startPage = 0;        // 1 = story1, 2 = story2
int currentState = 0;     // for progressing menu stories
int levelPage = 0;        // 1 = level select
int levelStoryPage = 0;   // 1 = level1 story1, 2 = level1 story2
int playGroundPage = 0;  // 1 = playground (after level story)

// Level button images
int levelBg;
int lvlImg[3];
int hoveredLevel = -1;
int lvlX[3] = { 380, 380, 380 };
int lvlY[3] = { 360, 250, 140 };
int lvlW = 240, lvlH = 80;

// Character Position and Animation Variables
int charX = 100, charY = 55;
bool isFiring = false;
int fireFrame = 0;
int bulletCount = 0;


int currentFrame = 0;
bool isMoving = false;
bool isLeftMoving = false;
bool isRunning = false;
bool isLeftWalking = false;
bool dPressed = false;
bool aPressed = false;
bool shiftPressed = false;
int  drawY = 0, jumpY = 0, jumpVelocity = 0;
bool isJumping = false;
bool wasRunningBeforeJump = false;
bool isHomeMusicPlaying = true;
bool isPlayMusicStarted = false;
void updateFrame();

int bgOffset = 0, bgCount = 0;// Tracks how much we've scrolled

void iDraw()
{
	iClear();

	homeMenuPage();
	showPlayLevel_1();
}



void iPassiveMouseMove(int mx, int my)
{
	hoverMenu(mx, my);
	hoverLevel(mx, my);
}

void iMouse(int button, int state, int mx, int my)
{
	showMenuLevel_1(button, state, mx, my);
}
void iMouseMove(int mx, int my)
{
	// optional: handle dragging; leave empty if not needed
}

void fixedUpdate()
{
	keyPressLevel_1();
}

void blinkTimer() { 
	showText = !showText; 
}

int main()
{
	iSetTimer(750, blinkTimer);
	iSetTimer(100, updateFrameLevel_1);
	iInitialize(1000, 600, "Mission Dash Protocol");
	// Backgrounds
	bgImage1 = iLoadImage("image/loading/home.bmp");
	bgImage2 = iLoadImage("image/loading/menu.bmp");
	bgPlayGround = iLoadImage("image/play.bmp");  // Added playground background

	// Menu buttons
	img[0] = iLoadImage("image/menu/start.bmp");
	img[1] = iLoadImage("image/menu/instructions.bmp");
	img[2] = iLoadImage("image/menu/aboutus.bmp");
	img[3] = iLoadImage("image/menu/exit.bmp");

	// Level Background
	levelBg = iLoadImage("image/level0.bmp");
	// Play home music until levelStoryPage reaches 2
	playHomeMusic();

	// Level buttons
	lvlImg[0] = iLoadImage("image/LEVELBUTTON/levelbutton1.bmp");
	lvlImg[1] = iLoadImage("image/LEVELBUTTON/levelbutton2.bmp");
	lvlImg[2] = iLoadImage("image/LEVELBUTTON/levelbutton3.bmp");
	for (int i = 0; i < MAX_BULLETS; i++){
		bulletActive[i] = false;
	}

	iStart();
	return 0;
}
