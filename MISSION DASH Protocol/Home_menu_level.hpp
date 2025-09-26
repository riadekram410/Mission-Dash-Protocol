#ifndef HOME_MENU_LEVEL_HPP
#define HOME_MENU_LEVEL_HPP
#include "Audio.hpp"
#include "Background.hpp"
#include "EnemySystem.hpp"

extern int charX, charY;
extern bool isPlayMusicStarted;
extern int showText;
extern bool isDead;
extern int currentBgSegment;
extern int backgroundOfSetX;
extern int stage1;
extern int stage2;
extern const int screenWidth, screenHeight, bgWidth;
extern int currentLevelType;

void displayElectricTraps();
void displaySpikeTraps();
void activateElectricTrapsForSegment(int segment);

extern int bgImage1, bgImage2, bgPlayGround;
extern int currentPage;
extern int img[4];
extern int imgX[4];
extern int imgY[4];
extern int imgW, imgH;
extern float hoverScale;
extern int hoveredImg;
extern int startPage;
extern int currentState;
extern int levelPage;
extern int levelStoryPage;
extern int playGroundPage;
extern int levelBg;
extern int lvlImg[3];
extern int hoveredLevel;
extern int lvlX[3];
extern int lvlY[3];
extern int lvlW, lvlH;
extern int bgCount;

// About us page variable
extern int aboutUsPage;
extern int aboutUsImg;
extern int instructionsPage;
extern int instructionsImg;

// Scoreboard variables
extern int scoreboardPage;
extern int scoreboardButtonImg;
extern int scoreboardBtnX, scoreboardBtnY, scoreboardBtnW, scoreboardBtnH;
extern int hoveredScoreboardBtn;

// Character animation sprites
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
char* jumpSprite = "image/run2.bmp";
char* enemySprite = "image/idleEnemy.bmp";
char* fireSprites[4] = {
	"image/shoot1.bmp",
	"image/shoot2.bmp",
	"image/shoot3.bmp",
	"image/shoot4.bmp"
};
char* spikeTrapSprite = "image/traps/spike1.bmp";

extern bool isFiring;
extern int fireFrame;
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
extern int drawY, jumpY, jumpVelocity;
extern bool isJumping;
extern bool wasRunningBeforeJump;
extern int sprintTime;

// Timer system variables
extern DWORD levelStartTime;
extern int levelTimeElapsed;
extern bool timeExceeded;
extern bool levelCompleted;

// Health and Sprint items
struct HpItem {
	int x, y;
	bool active;
};
const int MAX_HP_ITEMS = 2;
HpItem hpItems[MAX_HP_ITEMS];

struct SprintItem {
	int x, y;
	bool active;
};
const int MAX_SPRINT_ITEMS = 2;
SprintItem sprintItems[MAX_SPRINT_ITEMS];

// HP items only available in Level 2
void spawnMenuHpItemsForSegment(int segment) {
	for (int i = 0; i < MAX_HP_ITEMS; i++) {
		hpItems[i].active = false;
	}
	if (segment >= 9 || currentLevelType != 1) return;

	if (segment == 8) return;

	int y = 75;
	int itemCount = 0;
	if (segment == 1 || segment == 4 || segment == 7) itemCount = 1;
	if (itemCount == 0) return;
	srand(segment * 123 + 99);
	for (int i = 0; i < itemCount; i++) {
		hpItems[i].x = 200 + rand() % 700;
		hpItems[i].y = y;
		hpItems[i].active = true;
	}
}

// Sprint tokens only available in Level 2
void spawnMenuSprintItemsForSegment(int segment) {
	for (int i = 0; i < MAX_SPRINT_ITEMS; i++) {
		sprintItems[i].active = false;
	}
	if (currentLevelType != 1) return;

	if (segment == 8) return;

	if (segment == 2 || segment == 5) {
		sprintItems[0].x = 300 + (segment % 3) * 100;
		sprintItems[0].y = 85;
		sprintItems[0].active = true;
	}
}

void onMenuSegmentChange(int segment) {
	onSegmentChange(segment);
	spawnMenuHpItemsForSegment(segment);
	spawnMenuSprintItemsForSegment(segment);
}

void drawHpItems() {
	if (currentBgSegment >= 9 || currentLevelType != 1) return;

	if (currentBgSegment == 8) return;

	for (int i = 0; i < MAX_HP_ITEMS; i++) {
		if (hpItems[i].active) {
			int drawX = hpItems[i].x - backgroundOfSetX;
			int drawY = hpItems[i].y;
			iSetColor(255, 0, 0);
			iFilledCircle(drawX + 18, drawY + 18, 18);
			iSetColor(255, 255, 255);
			iFilledRectangle(drawX + 12, drawY + 16, 12, 4);
			iFilledRectangle(drawX + 16, drawY + 12, 4, 12);
		}
	}
}

void drawSprintItems() {
	if (currentLevelType != 1) return;

	if (currentBgSegment == 8) return;

	for (int i = 0; i < MAX_SPRINT_ITEMS; i++) {
		if (sprintItems[i].active) {
			int drawX = sprintItems[i].x - backgroundOfSetX;
			int drawY = sprintItems[i].y;
			iSetColor(150, 255, 139);
			iFilledCircle(drawX + 18, drawY + 18, 18);
			iSetColor(0, 150, 0);
			iFilledCircle(drawX + 18, drawY + 18, 14);
			iSetColor(255, 255, 255);
			iFilledRectangle(drawX + 10, drawY + 15, 16, 6);
			iSetColor(0, 100, 0);
			iText(drawX + 13, drawY + 16, "S", GLUT_BITMAP_HELVETICA_12);
		}
	}
}

void checkHpItemCollisions() {
	if (currentBgSegment == 8) return;

	const int charWidth = 50, charHeight = 50, itemW = 36, itemH = 36;
	for (int i = 0; i < MAX_HP_ITEMS; i++) {
		if (hpItems[i].active) {
			int hx = hpItems[i].x - backgroundOfSetX;
			int hy = hpItems[i].y;
			if (charX + charWidth > hx && charX < hx + itemW &&
				charY + charHeight > hy && charY < hy + itemH) {
				hpItems[i].active = false;
				extern int characterHealth;
				characterHealth += 50;
				extern const int MAX_CHARACTER_HEALTH;
				if (characterHealth > MAX_CHARACTER_HEALTH) characterHealth = MAX_CHARACTER_HEALTH;
			}
		}
	}
}

void checkSprintItemCollisions() {
	if (currentBgSegment == 8) return;

	const int charW = 50, charH = 50, itemW = 36, itemH = 36;
	for (int i = 0; i < MAX_SPRINT_ITEMS; i++) {
		if (sprintItems[i].active) {
			int sx = sprintItems[i].x - backgroundOfSetX;
			int sy = sprintItems[i].y;
			if (charX + charW > sx && charX < sx + itemW &&
				charY + charH > sy && charY < sy + itemH) {
				sprintItems[i].active = false;
				extern int sprintTime;
				sprintTime = 5;
			}
		}
	}
}

void drawBackButton(int x, int y) {
	// Back button styling
	iSetColor(120, 120, 120);
	iFilledRectangle(x, y, 100, 40);
	iSetColor(255, 255, 255);
	iRectangle(x, y, 100, 40);
	iText(x + 25, y + 12, "< Back", GLUT_BITMAP_HELVETICA_18);
}

void homeMenuPage() {
	if (currentPage == 1) {
		iShowImage(0, 0, 1000, 600, bgImage1);
		if (showText) {
			iSetColor(255, 255, 255);
			iText(600, 50, "Please press ENTER to continue...", GLUT_BITMAP_HELVETICA_18);
		}
	}
	else if (currentPage == 2 && !levelPage && !levelStoryPage && !playGroundPage && !aboutUsPage && !instructionsPage) {
		// Main menu
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
	// Level selection page
	if (levelPage == 1) {
		iShowImage(0, 0, 1000, 600, levelBg);

		// Draw level buttons
		for (int i = 0; i < 2; i++) {
			int w = lvlW, h = lvlH;
			if (hoveredLevel == i) {
				w = lvlW * hoverScale;
				h = lvlH * hoverScale;
			}
			int dx = lvlX[i] - (w - lvlW) / 2;
			int dy = lvlY[i] - (h - lvlH) / 2;
			iShowImage(dx, dy, w, h, lvlImg[i]);
		}

		// Draw scoreboard button
		int w = scoreboardBtnW, h = scoreboardBtnH;
		if (hoveredScoreboardBtn == 1) {
			w = scoreboardBtnW * hoverScale;
			h = scoreboardBtnH * hoverScale;
		}
		int dx = scoreboardBtnX - (w - scoreboardBtnW) / 2;
		int dy = scoreboardBtnY - (h - scoreboardBtnH) / 2;
		iShowImage(dx, dy, w, h, scoreboardButtonImg);

		// Back button for level selection
		drawBackButton(50, 50);
	}
	// About us page
	if (aboutUsPage == 1) {
		iShowImage(0, 0, 1000, 600, aboutUsImg);
		// Back button for about us page
		drawBackButton(50, 50);
	}
	// Instructions page
	if (instructionsPage == 1) {
		iShowImage(0, 0, 1000, 600, instructionsImg);
		// Back button for instructions page
		drawBackButton(50, 50);
	}
	if (levelStoryPage == 1) {
		iSetColor(0, 0, 0);
		iFilledRectangle(0, 0, 1000, 600);
		if (currentLevelType == 1)
			iShowBMP2(0, 0, "image/level2_story1.bmp", 0);
		else
			iShowBMP2(0, 0, "image/levelStory1.bmp", 0);
	}
	else if (levelStoryPage == 2) {
		iSetColor(0, 0, 0);
		iFilledRectangle(0, 0, 1000, 600);
		if (currentLevelType == 1)
			iShowBMP2(0, 0, "image/level2_story2.bmp", 0);
		else
			iShowBMP2(0, 0, "image/levelStory2.bmp", 0);
	}
	if (levelStoryPage == 2 && !isPlayMusicStarted) {
		stopHomeMusic();
		startPlayMusic();
		extern DWORD levelStartTime;
		extern bool timeExceeded, levelCompleted;
		levelStartTime = GetTickCount();
		timeExceeded = false;
		levelCompleted = false;
	}
}

void hoverMenu(int mx, int my) {
	if (currentPage == 2 && !levelPage && !levelStoryPage && !playGroundPage && !aboutUsPage && !instructionsPage) {
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

void hoverLevel(int mx, int my) {
	if (levelPage == 1) {
		hoveredLevel = -1;
		for (int i = 0; i < 2; i++) {
			if (mx >= lvlX[i] && mx <= lvlX[i] + lvlW &&
				my >= lvlY[i] && my <= lvlY[i] + lvlH) {
				hoveredLevel = i;
				break;
			}
		}
	}
}

void showMenuLevel_1(int button, int state, int mx, int my) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		// Check back button clicks first
		if ((levelPage == 1 || aboutUsPage == 1 || instructionsPage == 1) &&
			mx >= 50 && mx <= 150 && my >= 50 && my <= 90) {
			// Back to main menu
			levelPage = 0;
			aboutUsPage = 0;
			instructionsPage = 0;
			currentPage = 2;
			return;
		}

		// Main menu button clicks
		if (currentPage == 2 && !levelPage && !levelStoryPage && !aboutUsPage && !instructionsPage &&
			mx >= imgX[0] && mx <= imgX[0] + imgW &&
			my >= imgY[0] && my <= imgY[0] + imgH) {
			startPage = 1;
		}
		// Level selection button
		else if (currentPage == 2 && !levelPage && !levelStoryPage && !aboutUsPage && !instructionsPage &&
			mx >= imgX[0] && mx <= imgX[0] + imgW &&
			my >= imgY[0] && my <= imgY[0] + imgH) {
			levelPage = 1;
		}
		// Instructions button
		else if (currentPage == 2 && !levelPage && !levelStoryPage && !aboutUsPage && !instructionsPage &&
			mx >= imgX[1] && mx <= imgX[1] + imgW &&
			my >= imgY[1] && my <= imgY[1] + imgH) {
			instructionsPage = 1;
		}
		// About us button
		else if (currentPage == 2 && !levelPage && !levelStoryPage && !aboutUsPage && !instructionsPage &&
			mx >= imgX[2] && mx <= imgX[2] + imgW &&
			my >= imgY[2] && my <= imgY[2] + imgH) {
			aboutUsPage = 1;
		}
		// Exit button
		else if (currentPage == 2 && !levelPage && !levelStoryPage && !aboutUsPage && !instructionsPage &&
			mx >= imgX[3] && mx <= imgX[3] + imgW &&
			my >= imgY[3] && my <= imgY[3] + imgH) {
			exit(0);
		}

		// Level selection clicks
		if (levelPage == 1) {
			for (int i = 0; i < 2; i++) {
				if (mx >= lvlX[i] && mx <= lvlX[i] + lvlW &&
					my >= lvlY[i] && my <= lvlY[i] + lvlH) {
					levelPage = 0;
					currentLevelType = i;
					levelStoryPage = 1;
				}
			}
		}
	}
}

void showPlayLevel_1() {
	if (playGroundPage == 1 && currentBgSegment < 9) {
		for (int i = 0; i <= bgWidth / screenWidth; i++) {
			double drawX = i * screenWidth - backgroundOfSetX;
			if (drawX + screenWidth >= 0 && drawX <= screenWidth) {
				if (currentLevelType == LEVEL_1)
					iShowImage(drawX, 0, screenWidth, screenHeight, stage1);
				else if (currentLevelType == LEVEL_2)
					iShowImage(drawX, 0, screenWidth, screenHeight, stage2);
			}
		}
		if (currentLevelType == LEVEL_1) {
			displayElectricTraps();
			displaySpikeTraps();
		}
		else {
			displaySpikeTraps();
		}
		drawEnemies();
		drawEnemyBullets();
		drawDrones();
		drawDroneBullets();

		if (currentBgSegment == 8 && currentLevelType == 1) {
			drawBossBullets();
		}

		drawHpItems();
		drawSprintItems();
		for (int i = 0; i < MAX_BULLETS; i++) {
			if (bulletActive[i]) {
				iSetColor(255, 255, 0);
				iFilledCircle(bulletX[i] + 5, bulletY[i] + 2, 2);
			}
		}
		if (!isDead) {
			drawY = charY + jumpY;

			if (isMoving && dPressed) {
				if (isRunning && sprintTime > 0) {
					iShowBMP2(charX, charY, runSprites[currentFrame], 0xFFFFFF);
				}
				else {
					iShowBMP2(charX, charY, walkSprites[currentFrame], 0xFFFFFF);
				}
			}
			else if (isJumping) {
				iShowBMP2(charX, drawY, jumpSprite, 0xFFFFFF);
			}
			else if (isLeftMoving && isLeftWalking) {
				iShowBMP2(charX, charY, leftWalkSprites[currentFrame], 0xFFFFFF);
			}
			else if (isFiring) {
				iShowBMP2(charX, charY, fireSprites[fireFrame], 0xFFFFFF);
			}
			else {
				iShowBMP2(charX, charY, idleSprites[currentFrame], 0xFFFFFF);
			}
		}
	}

	extern Boss boss;
	if ((currentBgSegment >= 9 || (currentBgSegment == 8 && currentLevelType == 1 && !boss.alive)) && !timeExceeded) {
		// Level completion handled in main
	}
}

#endif