#include "iGraphics.h"
#include "Audio.hpp"
#include "Home_menu_level.hpp"
#include "Level1.hpp"
#include "Level2.hpp"
#include "Traps.hpp"
#include "HealthBar.hpp"
#include "Background.hpp"
#include "EnemySystem.hpp"
#include "PauseMenu.hpp"
#include  "Scoreboard.hpp" // Use the easy file-based scoreboard

int showText = 1;
int bgImage1, bgImage2, bgPlayGround;
int stage1, stage2;
const int screenWidth = 1000;
const int screenHeight = 600;
const int bgWidth = 1000 * 2;

int currentPage = 1;

int img[4];
int imgX[4] = { 420, 420, 420, 420 };
int imgY[4] = { 430, 310, 210, 110 };
int imgW = 200, imgH = 70;

float hoverScale = 1.2f;
int hoveredImg = -1;

int startPage = 0;
int currentState = 0;
int levelPage = 0;
int levelStoryPage = 0;
int playGroundPage = 0;

// About us and instructions page variables
int aboutUsPage = 0;
int aboutUsImg;
int instructionsPage = 0;
int instructionsImg;

// Scoreboard variables
int scoreboardPage = 0;
int scoreboardButtonImg;

int levelBg;
int lvlImg[3];
int hoveredLevel = -1;
int lvlX[3] = { 380, 380, 380 };
int lvlY[3] = { 360, 250, 140 };
int lvlW = 240, lvlH = 80;

// Scoreboard button position (below level buttons)
int scoreboardBtnX = 380;
int scoreboardBtnY = 50;
int scoreboardBtnW = 240;
int scoreboardBtnH = 60;
int hoveredScoreboardBtn = 0;

int charX = 100, charY = 55;
int characterHealth = 500;
int sprintTime = 5;
bool isFiring = false;
int fireFrame = 0;
int bulletCount = 0;

int healthBarX = 10, healthBarY = 565, healthBarWidth = 150, healthBarHeight = 14;
int sprintBarX = 10, sprintBarY = 545, sprintBarWidth = 140, sprintBarHeight = 10;

int trapX = 500, trapY = 100;
int trapFrameIndex = 0;
DWORD lastTrapChangeTime = 0;
const int trapAnimationSpeed = 100;

int deadFrameIndex = 0;
bool isDead = false;

const int MAX_BULLETS = 20;
int bulletX[MAX_BULLETS], bulletY[MAX_BULLETS];
int bulletActive[MAX_BULLETS];

int currentFrame = 0;
bool isMoving = false;
bool isLeftMoving = false;
bool isRunning = false;
bool isLeftWalking = false;
bool dPressed = false;
bool aPressed = false;
bool shiftPressed = false;
int drawY = 0, jumpY = 0, jumpVelocity = 0;
bool isJumping = false;
bool wasRunningBeforeJump = false;
bool isHomeMusicPlaying = true;
bool isPlayMusicStarted = false;

int backgroundOfSetX = 0, bgCount = 0;
int currentBgSegment = 0;
int currentLevelType = 0;

bool isPaused = false;
bool soundOn = true;
int hoveredPauseBtn = -1;

// Timer system variables
DWORD levelStartTime = 0;
int levelTimeElapsed = 0;
bool timeExceeded = false;
bool levelCompleted = false;

void iDraw()
{
	iClear();

	// Home or Menu pages
	if (!playGroundPage) {
		homeMenuPage();

		// Draw scoreboard if active
		if (scoreboardPage == 1) {
			drawScoreboard();
		}

		return;
	}

	// Gameplay page timer check
	if (playGroundPage == 1 && !isDead && currentBgSegment < 9 && !timeExceeded && !levelCompleted) {
		DWORD currentTime = GetTickCount();
		levelTimeElapsed = (currentTime - levelStartTime) / 1000;

		if (levelTimeElapsed >= 210) {
			timeExceeded = true;
			isDead = true;
		}
	}

	// Check level completion
	extern Boss boss;
	if ((currentBgSegment >= 9 || (currentBgSegment == 8 && currentLevelType == 1 && !boss.alive)) && !levelCompleted && !timeExceeded) {
		levelCompleted = true;

		// Add time record when level is completed
		int minutes = levelTimeElapsed / 60;
		int seconds = levelTimeElapsed % 60;
		addTimeRecord(currentLevelType, minutes, seconds);
	}

	// Check collisions before drawing
	if (!isDead && currentBgSegment < 9) {
		checkHpItemCollisions();
		checkSprintItemCollisions();
	}

	// Draw main game
	showPlayLevel_1();

	// Draw timer during gameplay
	if (playGroundPage == 1 && currentBgSegment < 9 && !isDead) {
		iSetColor(255, 255, 255);
		char timerText[20];
		int minutes = levelTimeElapsed / 60;
		int seconds = levelTimeElapsed % 60;
		sprintf_s(timerText, "Time -> %02d:%02d", minutes, seconds);
		iText(850, 570, timerText, GLUT_BITMAP_HELVETICA_18);
	}

	// Draw health and sprint bars
	if (!isDead && currentBgSegment < 9) {
		drawHealthAndSprintBars();
		handleTrapCollision(charX, charY);
	}

	// Game Over screen
	if (isDead && currentBgSegment < 9) {
		drawHealthAndSprintBars();
		showDeathAnimation();
		iSetColor(255, 0, 0);
		iText(400, 300, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);
		iSetColor(255, 255, 255);
		iText(350, 250, "Press 'R' to restart", GLUT_BITMAP_HELVETICA_18);
		if (timeExceeded) {
			iText(380, 200, "Time exceeded", GLUT_BITMAP_HELVETICA_18);
		}
	}

	// Level completion screen
	if ((currentBgSegment >= 9 || (currentBgSegment == 8 && currentLevelType == 1 && !boss.alive)) && !timeExceeded) {
		iSetColor(0, 0, 0);
		iFilledRectangle(0, 0, 1000, 600);
		iSetColor(0, 215, 90);
		if (currentLevelType == LEVEL_1)
			iText(330, 300, "Congratulations! Level-1 completed!", GLUT_BITMAP_TIMES_ROMAN_24);
		else if (currentLevelType == LEVEL_2) {
			if (currentBgSegment == 8 && !boss.alive) {
				iText(350, 300, "Boss Defeated! Level-2 completed!", GLUT_BITMAP_TIMES_ROMAN_24);
			}
			else {
				iText(330, 300, "Congratulations! Level-2 completed!", GLUT_BITMAP_TIMES_ROMAN_24);
			}
		}
		iSetColor(255, 255, 255);
		iText(270, 250, "Press 'R' to reset and '->' to proceed to next level", GLUT_BITMAP_HELVETICA_18);
		char timeText[50];
		int minutes = levelTimeElapsed / 60;
		int seconds = levelTimeElapsed % 60;
		sprintf_s(timeText, "Time -> %02d:%02d", minutes, seconds);
		iText(390, 200, timeText, GLUT_BITMAP_HELVETICA_18);

		// Show message about scoreboard
		iSetColor(255, 215, 0);
		iText(350, 170, "Check scoreboard for your best time!", GLUT_BITMAP_HELVETICA_12);
	}

	// Pause button
	iSetColor(255, 255, 255);
	iFilledRectangle(screenWidth - 60, 20, 40, 40);
	iSetColor(36, 50, 90);
	iRectangle(screenWidth - 60, 20, 40, 40);
	iText(screenWidth - 52, 33, "||", GLUT_BITMAP_TIMES_ROMAN_24);

	// Pause overlay
	if (isPaused) {
		iSetColor(0, 0, 80);
		iFilledRectangle(0, 0, screenWidth, screenHeight);
		drawPauseMenu();
	}
}

void iMouseMove(int mx, int my) {}

void iPassiveMouseMove(int mx, int my)
{
	if (isPaused) {
		hoverPauseMenu(mx, my);
		return;
	}
	hoverMenu(mx, my);
	hoverLevel(mx, my);

	// Handle scoreboard button hover on level page
	if (levelPage == 1) {
		hoveredScoreboardBtn = 0;
		if (mx >= scoreboardBtnX && mx <= scoreboardBtnX + scoreboardBtnW &&
			my >= scoreboardBtnY && my <= scoreboardBtnY + scoreboardBtnH) {
			hoveredScoreboardBtn = 1;
		}
	}
}

void iMouse(int button, int state, int mx, int my)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (isPaused) {
			handlePauseMenuClick(mx, my);
			return;
		}
		if (playGroundPage == 1 &&
			mx >= screenWidth - 60 && mx <= screenWidth - 20 &&
			my >= 20 && my <= 60) {
			isPaused = true;
			return;
		}

		// Check scoreboard button click on level page
		if (levelPage == 1 &&
			mx >= scoreboardBtnX && mx <= scoreboardBtnX + scoreboardBtnW &&
			my >= scoreboardBtnY && my <= scoreboardBtnY + scoreboardBtnH) {
			scoreboardPage = 1;
			return;
		}

		// Check scoreboard back button click
		if (scoreboardPage == 1 &&
			mx >= 50 && mx <= 150 && my >= 50 && my <= 90) {
			scoreboardPage = 0;
			return;
		}

		showMenuLevel_1(button, state, mx, my);
	}
}

void fixedUpdate()
{
	if (isPaused) return;
	if (currentLevelType == 0)
		keyPressLevel_1();
	else
		keyPressLevel_2();

	if (playGroundPage == 1 && currentBgSegment < 9) {
		updateSprintTime();
		updateTrapAnimation();
		updateEnemyShooting();
		updateEnemyBullets();
		updateDrones();
		updateDroneBullets();
		checkEnemyBulletHits();
		checkDroneBulletHits();
		checkCharacterBulletHits(bulletX, bulletY, bulletActive, MAX_BULLETS);

		if (currentBgSegment == 8 && currentLevelType == 1) {
			updateBoss();
			updateBossBullets();
			checkBossBulletHits();
		}
	}
	if (isKeyPressed('r') || isKeyPressed('R')) {
		resetGameState();
	}
	extern Boss boss;
	if (playGroundPage == 1 && (currentBgSegment >= 9 || (currentBgSegment == 8 && currentLevelType == 1 && !boss.alive)) && isSpecialKeyPressed(GLUT_KEY_RIGHT)) {
		resetGameState();
		playGroundPage = 0;
		currentPage = 2;
		levelPage = 1;
	}
}

void blinkTimer() {
	showText = !showText;
}

int main()
{
	iSetTimer(750, blinkTimer);
	iSetTimer(100, updateFrameLevel_1);
	iSetTimer(100, updateFrameLevel_2);
	iSetTimer(50, fixedUpdate);

	iInitialize(screenWidth, screenHeight, "Mission Dash Protocol");

	bgImage1 = iLoadImage("image/loading/home.bmp");
	bgImage2 = iLoadImage("image/loading/menu.bmp");
	bgPlayGround = iLoadImage("image/play.bmp");
	stage1 = iLoadImage("image/play.bmp");
	stage2 = iLoadImage("image/play2.bmp");

	img[0] = iLoadImage("image/menu/start.bmp");
	img[1] = iLoadImage("image/menu/instructions.bmp");
	img[2] = iLoadImage("image/menu/aboutus.bmp");
	img[3] = iLoadImage("image/menu/exit.bmp");

	// Load about us and instructions images
	aboutUsImg = iLoadImage("image/AboutUs.bmp");
	instructionsImg = iLoadImage("image/Instructions.bmp");

	levelBg = iLoadImage("image/level0.bmp");
	lvlImg[0] = iLoadImage("image/LEVELBUTTON/levelbutton1.bmp");
	lvlImg[1] = iLoadImage("image/LEVELBUTTON/levelbutton2.bmp");

	// Load scoreboard button image
	scoreboardButtonImg = iLoadImage("image/scoreboard.bmp");

	for (int i = 0; i < MAX_BULLETS; i++) {
		bulletActive[i] = false;
	}

	// Initialize scoreboard (loads scores from file)
	initializeScoreboard();

	initializeTraps();
	activateElectricTrapsForSegment(0);
	extern void onMenuSegmentChange(int);
	onMenuSegmentChange(0);

	playHomeMusic();

	iStart();
	return 0;
}