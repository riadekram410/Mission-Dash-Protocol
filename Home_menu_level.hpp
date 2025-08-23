#ifndef HOME_MENU_LEVEL_HPP
#define HOME_MENU_LEVEL_HPP
#include "Audio.hpp"

extern int charX, charY;
extern bool isPlayMusicStarted;
extern int showText;
// Backgrounds
extern int bgImage1, bgImage2, bgPlayGround;
extern int currentPage;  // 1 = home, 2 = menu, 3 = playground

// Menu button images
extern int img[4];
extern int imgX[4];
extern int imgY[4];
extern int imgW, imgH;

// Hover effect
extern float hoverScale;
extern int hoveredImg;

// Page states
extern int startPage;        // 1 = story1, 2 = story2
extern int currentState;     // for progressing menu stories
extern int levelPage;        // 1 = level select
extern int levelStoryPage;   // 1 = level1 story1, 2 = level1 story2
extern int playGroundPage;  // 1 = playground (after level story)

// Level button images
extern int levelBg;
extern int lvlImg[3];
extern int hoveredLevel;
extern int lvlX[3];
extern int lvlY[3];
extern int lvlW, lvlH;

extern int bgOffset, bgCount;// Tracks how much we've scrolled
const int bgWidth = 1000;

char* leftWalkSprites[7] = {
	"image/reverse1.bmp", "image/reverse2.bmp", "image/reverse3.bmp",
	"image/reverse4.bmp", "image/reverse5.bmp", "image/reverse6.bmp", "image/reverse7.bmp"
};
char* idleSprites[7] = {
	"image/still1.bmp", "image/still2.bmp", "image/still3.bmp",
	"image/still4.bmp", "image/still5.bmp", "image/still6.bmp", "image/still7.bmp"
};
char* runSprites[8] = {
	"image/run1.bmp", "image/run2.bmp", "image/run3.bmp", "image/run4.bmp",
	"image/run5.bmp", "image/run6.bmp", "image/run7.bmp", "image/run8.bmp"
};
char* walkSprites[7] = {
	"image/walk1.bmp", "image/walk2.bmp", "image/walk3.bmp",
	"image/walk4.bmp", "image/walk5.bmp", "image/walk4.bmp", "image/walk4.bmp"
};
char* jumpSprite = "image\\run2.bmp";
char* enemySprite = "image\\idleEnemy.bmp";
char* fireSprites[4] = {
	"image/shoot1.bmp",
	"image/shoot2.bmp",
	"image/shoot3.bmp",
	"image/shoot4.bmp"
};

extern bool isFiring;
extern int fireFrame;

//bullet forward
extern const int MAX_BULLETS;
extern int bulletX[], bulletY[];
extern int bulletActive[];
extern int bulletCount;

extern int currentFrame;
extern bool isMoving;
extern bool isLeftMoving;
extern bool isRunning;
extern bool isLeftWalking;
extern bool dPressed;
extern bool aPressed;
extern bool shiftPressed;
extern int  drawY, jumpY, jumpVelocity;
extern bool isJumping;
extern bool wasRunningBeforeJump;

void homeMenuPage(){
	// Home page
	if (currentPage == 1) {
		iShowImage(0, 0, 1000, 600, bgImage1);
		if (showText) {
			iSetColor(255, 255, 255);
			iText(600, 50, "Please press ENTER to continue...", GLUT_BITMAP_HELVETICA_18);
		}
	}
	// Menu page
	else if (currentPage == 2 && !levelPage && !levelStoryPage && !playGroundPage) {
		iShowImage(0, 0, 1000, 600, bgImage2);
		for (int i = 0; i < 4; i++) {
			int w = imgW, h = imgH;
			if (hoveredImg == i) {
				w = imgW * hoverScale;
				h = imgH * hoverScale;
				iShowImage(imgX[i] - (w - imgW) / 2, imgY[i] - (h - imgH) / 2, w, h, img[i]);
			}
			else {
				iShowImage(imgX[i], imgY[i], imgW, imgH, img[i]);
			}
		}

		if (startPage == 1) {
			iSetColor(0, 0, 0);
			iFilledRectangle(0, 0, 1000, 600);
			iShowBMP2(0, 0, "image/story1.bmp", 0);
		}
		else if (startPage == 2) {
			iSetColor(0, 0, 0);
			iFilledRectangle(0, 0, 1000, 600);
			iShowBMP2(0, 0, "image/story2.bmp", 0);
		}
	}
	// Level select page
	if (levelPage == 1) {
		iShowImage(0, 0, 1000, 600, levelBg);

		for (int i = 0; i < 3; i++) {
			int w = lvlW, h = lvlH;
			if (hoveredLevel == i) {
				w = lvlW * hoverScale;
				h = lvlH * hoverScale;
			}
			int dx = lvlX[i] - (w - lvlW) / 2;
			int dy = lvlY[i] - (h - lvlH) / 2;
			iShowImage(dx, dy, w, h, lvlImg[i]);
			if (i > 0) { // Locked text for lvl 2 & 3
				iSetColor(255, 0, 0);
				iText(dx + 80, dy + h / 2 - 5, "Locked", GLUT_BITMAP_HELVETICA_12);
			}
		}
	}

	// Level 1 story pages
	if (levelStoryPage == 1) {
		iSetColor(0, 0, 0);
		iFilledRectangle(0, 0, 1000, 600);
		iShowBMP2(0, 0, "image/levelStory1.bmp", 0);
	}
	else if (levelStoryPage == 2) {
		iSetColor(0, 0, 0);
		iFilledRectangle(0, 0, 1000, 600);
		iShowBMP2(0, 0, "image/levelStory2.bmp", 0);
	}
	// Start the playGround.mp3 after levelStoryPage 2
	if (levelStoryPage == 2 && !isPlayMusicStarted) {
		stopHomeMusic();  // Stop home music
		startPlayMusic();  // Start playSound.mp3
	}
}

void hoverMenu(int mx, int my){
	if (currentPage == 2 && !levelPage && !levelStoryPage && !playGroundPage) {
		hoveredImg = -1;
		for (int i = 0; i < 4; i++) {
			if (mx >= imgX[i] && mx <= imgX[i] + imgW &&
				my >= imgY[i] && my <= imgY[i] + imgH) {
				hoveredImg = i;
				break;
			}
		}
	}
}
void hoverLevel(int mx, int my){
	if (levelPage == 1) {
		hoveredLevel = -1;
		for (int i = 0; i < 3; i++) {
			if (mx >= lvlX[i] && mx <= lvlX[i] + lvlW &&
				my >= lvlY[i] && my <= lvlY[i] + lvlH) {
				hoveredLevel = i;
				break;
			}
		}
	}
}
void showMenuLevel_1(int button, int state, int mx, int my){
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		// Start button in menu
		if (currentPage == 2 && !levelPage && !levelStoryPage &&
			mx >= imgX[0] && mx <= imgX[0] + imgW &&
			my >= imgY[0] && my <= imgY[0] + imgH) {
			startPage = 1;
		}

		// Level select
		if (levelPage == 1) {
			// Level 1
			if (mx >= lvlX[0] && mx <= lvlX[0] + lvlW &&
				my >= lvlY[0] && my <= lvlY[0] + lvlH) {
				levelPage = 0;
				levelStoryPage = 1; // Show first story image of level 1
			}
			// Level 2 & 3 do nothing for now
		}
	}
}
void showPlayLevel_1(){
	// Playground Page (Game starts here)
	if (playGroundPage == 1) {
		iShowImage(-bgOffset, 0, bgWidth, 600, bgPlayGround);  // Left background
		iShowImage(-bgOffset + bgWidth, 0, bgWidth, 600, bgPlayGround);  // Right background
		if (bgCount == 2){
			iShowBMP2(-bgOffset + bgWidth, charY + 5, enemySprite, 0xFFFFFF);
		}
		drawY = charY + jumpY;

		// Handle character movement and display animations
		if (isMoving) {
			if (isRunning) {
				iShowBMP2(charX, charY, runSprites[currentFrame], 0xFFFFFF);
			}
			else {
				iShowBMP2(charX, charY, walkSprites[currentFrame], 0xFFFFFF);
			}
		}
		else if (isJumping){
			iShowBMP2(charX, drawY, jumpSprite, 0xFFFFFF);
		}
		else if (isLeftMoving && isLeftWalking){
			iShowBMP2(charX, charY, leftWalkSprites[currentFrame], 0xFFFFFF);
		}
		else if (isFiring) {
			iShowBMP2(charX, charY, fireSprites[fireFrame], 0xFFFFFF);
		}
		else {
			iShowBMP2(charX, charY, idleSprites[currentFrame], 0xFFFFFF);  // Idle position
		}
		for (int i = 0; i < MAX_BULLETS; i++) {
			if (bulletActive[i]) {
				iSetColor(255, 255, 0);  // yellow bullet
				iFilledCircle(bulletX[i] + 5, bulletY[i] + 2, 2);
			}
		}
	}
}

#endif