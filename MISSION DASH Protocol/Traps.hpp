#ifndef TRAPS_HPP
#define TRAPS_HPP
#include "HealthBar.hpp"
#include "Background.hpp"
#include <windows.h>
#include <cstdlib>
#include <ctime>

extern int characterHealth;
extern int charX, charY;
extern bool isDead;
extern int deadFrameIndex;
extern int currentBgSegment;
extern int backgroundOfSetX;
extern int currentLevelType;

static int electricTrapFrames[23];
static bool electricTrapsLoaded = false;
const int MAX_ELECTRIC_TRAPS = 2;

struct ElectricTrap {
	int x, y;
	int frameIndex;
	bool active;
	DWORD lastFrameTime;
	bool hasHitPlayer;
};

static ElectricTrap electricTraps[MAX_ELECTRIC_TRAPS];
extern char* spikeTrapSprite;
static int deadSprites[4] = { -1, -1, -1, -1 };
static bool deadSpritesLoaded = false;
static bool trapInitialized = false;

void initializeTraps() {
	if (trapInitialized) return;

	if (!electricTrapsLoaded) {
		for (int i = 0; i < 23; i++) {
			char filename[50];
			sprintf_s(filename, "image/traps/trap%d.bmp", i + 1);
			electricTrapFrames[i] = iLoadImage(filename);
		}
		electricTrapsLoaded = true;
	}

	for (int i = 0; i < MAX_ELECTRIC_TRAPS; i++) {
		electricTraps[i].active = false;
		electricTraps[i].frameIndex = 0;
		electricTraps[i].lastFrameTime = GetTickCount();
		electricTraps[i].hasHitPlayer = false;
		electricTraps[i].x = 0;
		electricTraps[i].y = 55;
	}

	if (!deadSpritesLoaded) {
		deadSprites[0] = iLoadImage("image/dead1.bmp");
		deadSprites[1] = iLoadImage("image/dead2.bmp");
		deadSprites[2] = iLoadImage("image/dead3.bmp");
		deadSprites[3] = iLoadImage("image/dead4.bmp");
		deadSpritesLoaded = true;
	}
	trapInitialized = true;
}

void activateElectricTrapsForSegment(int segment) {
	for (int i = 0; i < MAX_ELECTRIC_TRAPS; i++) {
		electricTraps[i].active = false;
		electricTraps[i].hasHitPlayer = false;
	}
	if (currentLevelType != 0) return;

	BackgroundSegment* seg = getCurrentSegment();
	if (seg->trapType1 == 1) {
		electricTraps[0].active = true;
		electricTraps[0].x = seg->trap1X;
		electricTraps[0].y = seg->trap1Y;
		electricTraps[0].frameIndex = 0;
	}
	if (seg->trapType2 == 1) {
		electricTraps[1].active = true;
		electricTraps[1].x = seg->trap2X;
		electricTraps[1].y = seg->trap2Y;
		electricTraps[1].frameIndex = 0;
	}
}

void updateElectricTrapAnimations() {
	DWORD currentTime = GetTickCount();
	for (int i = 0; i < MAX_ELECTRIC_TRAPS; i++) {
		if (electricTraps[i].active) {
			if (currentTime - electricTraps[i].lastFrameTime >= 80) {
				electricTraps[i].frameIndex = (electricTraps[i].frameIndex + 1) % 23;
				electricTraps[i].lastFrameTime = currentTime;
			}
		}
	}
}

void displayElectricTraps() {
	if (currentBgSegment >= 9) return;
	if (currentLevelType != 0) return;

	for (int i = 0; i < MAX_ELECTRIC_TRAPS; i++) {
		if (electricTraps[i].active && electricTrapsLoaded) {
			int adjustedX = electricTraps[i].x - backgroundOfSetX;
			if (adjustedX > -100 && adjustedX < 1100) {
				int frameIndex = electricTraps[i].frameIndex;
				if (frameIndex >= 0 && frameIndex < 23 && electricTrapFrames[frameIndex] != -1) {
					char filename[50];
					sprintf_s(filename, "image/traps/trap%d.bmp", frameIndex + 1);
					iShowBMP2(adjustedX, electricTraps[i].y, filename, 0xFFFFFF);
				}
			}
		}
	}
}

bool checkElectricTrapCollision(int charX, int charY) {
	if (currentBgSegment >= 9) return false;
	if (currentLevelType != 0) return false;

	const int charWidth = 50, charHeight = 50;
	const int trapWidth = 100, trapHeight = 50;

	for (int i = 0; i < MAX_ELECTRIC_TRAPS; i++) {
		if (electricTraps[i].active && !electricTraps[i].hasHitPlayer) {
			int adjustedTrapX = electricTraps[i].x - backgroundOfSetX;
			int adjustedTrapY = electricTraps[i].y;
			if (charX + charWidth > adjustedTrapX &&
				charX < adjustedTrapX + trapWidth &&
				charY + charHeight > adjustedTrapY &&
				charY < adjustedTrapY + trapHeight &&
				(charY <= adjustedTrapY + trapHeight - 15)) {
				electricTraps[i].hasHitPlayer = true;
				return true;
			}
		}
	}
	return false;
}

void displaySpikeTraps() {
	if (currentBgSegment >= 9) return;
	BackgroundSegment* seg = getCurrentSegment();

	if (seg->trapType1 == 2) {
		int adjustedX = seg->trap1X - backgroundOfSetX;
		int adjustedY = seg->trap1Y;
		iShowBMP2(adjustedX, adjustedY, spikeTrapSprite, 0xFFFFFF);
	}
	if (seg->trapType2 == 2) {
		int adjustedX = seg->trap2X - backgroundOfSetX;
		int adjustedY = seg->trap2Y;
		iShowBMP2(adjustedX, adjustedY, spikeTrapSprite, 0xFFFFFF);
	}
}

bool checkSpikeTrapCollision(int charX, int charY) {
	if (currentBgSegment >= 9) return false;
	BackgroundSegment* seg = getCurrentSegment();
	const int trapWidth = 100, trapHeight = 50;
	const int charWidth = 50, charHeight = 50;

	if (seg->trapType1 == 2) {
		int adjustedTrapX = seg->trap1X - backgroundOfSetX;
		int adjustedTrapY = seg->trap1Y;
		if (charX + charWidth > adjustedTrapX &&
			charX < adjustedTrapX + trapWidth &&
			charY + charHeight > adjustedTrapY &&
			charY < adjustedTrapY + trapHeight &&
			(charY <= adjustedTrapY + trapHeight - 15)) {
			return true;
		}
	}
	if (seg->trapType2 == 2) {
		int adjustedTrapX = seg->trap2X - backgroundOfSetX;
		int adjustedTrapY = seg->trap2Y;
		if (charX + charWidth > adjustedTrapX &&
			charX < adjustedTrapX + trapWidth &&
			charY + charHeight > adjustedTrapY &&
			charY < adjustedTrapY + trapHeight &&
			(charY <= adjustedTrapY + trapHeight - 15)) {
			return true;
		}
	}
	return false;
}

void handleTrapCollision(int charX, int charY) {
	static DWORD lastElectricHit = 0;
	static DWORD lastSpikeHit = 0;
	DWORD currentTime = GetTickCount();

	if (checkElectricTrapCollision(charX, charY)) {
		if (currentTime - lastElectricHit > 1000) {
			characterHealth -= 10;
			lastElectricHit = currentTime;
			if (characterHealth <= 0) {
				characterHealth = 0;
				isDead = true;
				deadFrameIndex = 0;
			}
		}
	}
	if (checkSpikeTrapCollision(charX, charY)) {
		if (currentTime - lastSpikeHit > 1000) {
			characterHealth -= 10;
			lastSpikeHit = currentTime;
			if (characterHealth <= 0) {
				characterHealth = 0;
				isDead = true;
				deadFrameIndex = 0;
			}
		}
	}
}

void showDeathAnimation() {
	static DWORD lastDeathFrameTime = 0;
	DWORD currentTime = GetTickCount();

	if (currentTime - lastDeathFrameTime > 300) {
		deadFrameIndex = (deadFrameIndex + 1) % 4;
		lastDeathFrameTime = currentTime;
	}

	if (!deadSpritesLoaded || deadSprites[deadFrameIndex] == -1) {
		iSetColor(255, 0, 0);
		iText(charX + 25, charY + 25, "DEAD", GLUT_BITMAP_HELVETICA_18);
		return;
	}
	iShowImage(charX, charY, 50, 50, deadSprites[deadFrameIndex]);
}

void updateTrapAnimation() {
	DWORD currentTime = GetTickCount();
	static DWORD lastTrapChangeTime = 0;
	static const int trapAnimationSpeed = 100;

	if (currentTime - lastTrapChangeTime >= trapAnimationSpeed) {
		for (int i = 0; i < MAX_ELECTRIC_TRAPS; i++)
		if (electricTraps[i].active)
			electricTraps[i].frameIndex = (electricTraps[i].frameIndex + 1) % 23;
		lastTrapChangeTime = currentTime;
	}
	updateElectricTrapAnimations();
}

void resetGameState() {
	characterHealth = 500;
	isDead = false;
	deadFrameIndex = 0;

	for (int i = 0; i < MAX_ELECTRIC_TRAPS; i++) {
		electricTraps[i].hasHitPlayer = false;
		electricTraps[i].active = false;
	}

	extern int backgroundOfSetX, bgCount, currentBgSegment;
	backgroundOfSetX = 0;
	bgCount = 0;
	currentBgSegment = 0;

	extern int charX, charY, jumpY, jumpVelocity;
	extern bool isJumping;
	charX = 100;
	charY = 55;
	jumpY = 0;
	jumpVelocity = 0;
	isJumping = false;

	extern int sprintTime;
	sprintTime = 5;

	// Reset timer variables
	extern DWORD levelStartTime;
	extern int levelTimeElapsed;
	extern bool timeExceeded, levelCompleted;
	levelStartTime = GetTickCount();
	levelTimeElapsed = 0;
	timeExceeded = false;
	levelCompleted = false;

	// Reset boss if it exists
	extern struct Boss boss;
	if (currentLevelType == 1) {
		boss.alive = false;
		boss.health = 0;
		for (int i = 0; i < MAX_BOSS_BULLETS; i++) {
			extern struct BossBullet bossBullets[];
			bossBullets[i].active = false;
		}
	}

	activateElectricTrapsForSegment(0);
	extern void onMenuSegmentChange(int);
	onMenuSegmentChange(0);

	extern int bulletActive[], bulletX[], bulletY[];
	extern const int MAX_BULLETS;
	for (int i = 0; i < MAX_BULLETS; i++) {
		bulletActive[i] = 0;
		bulletX[i] = 0;
		bulletY[i] = 0;
	}
}
#endif