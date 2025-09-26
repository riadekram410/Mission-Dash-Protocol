#ifndef LEVEL1_HPP
#define LEVEL1_HPP
#include "Traps.hpp"
#include "EnemySystem.hpp"
#include <ctime>

unsigned int last, current;
extern int charX, charY;
extern int startPage, levelPage, levelStoryPage, playGroundPage, currentPage;
extern bool isFiring;
extern int fireFrame;

extern const int MAX_BULLETS;
extern int bulletX[], bulletY[];
extern int bulletActive[];
extern int bulletCount;

extern int currentFrame;
extern bool isMoving, isLeftMoving, isRunning, isLeftWalking;
extern bool dPressed, aPressed, shiftPressed;
extern int drawY, jumpY, jumpVelocity;
extern bool isJumping, wasRunningBeforeJump;
extern int backgroundOfSetX, bgCount;
extern const int bgWidth;
extern bool isDead;
extern int sprintTime;

void handlePageTransitions() {
	current = clock();
	double duration = (double)(current - last) / CLOCKS_PER_SEC;
	if (duration > 0.75) {
		last = current;
		if (startPage == 1) {
			startPage = 2;
		}
		else if (startPage == 2) {
			startPage = 0;
			levelPage = 1;
		}
		else if (levelStoryPage == 1) {
			levelStoryPage = 2;
		}
		else if (levelStoryPage == 2) {
			levelStoryPage = 0;
			playGroundPage = 1;
		}
	}
}

void createBullet() {
	static DWORD lastBulletTime = 0;
	DWORD currentTime = GetTickCount();
	if (currentTime - lastBulletTime < 100) return;

	for (int i = 0; i < MAX_BULLETS; i++) {
		if (!bulletActive[i]) {
			bulletX[i] = charX + 60;
			bulletY[i] = charY + 40;
			bulletActive[i] = true;
			lastBulletTime = currentTime;
			break;
		}
	}
}

extern bool isBlockedByEnemy(int, int);

void keyPressLevel_1() {
	if (isKeyPressed('\r') && currentPage == 1) {
		currentPage = 2;
	}
	if (isSpecialKeyPressed(GLUT_KEY_RIGHT)) {
		handlePageTransitions();
	}
	if (isDead || (playGroundPage == 1 && currentBgSegment >= 9)) {
		dPressed = aPressed = shiftPressed = isMoving = isLeftMoving = isLeftWalking = isRunning = false;
		isFiring = false;
		fireFrame = 0;
		return;
	}

	dPressed = isKeyPressed('d') || isKeyPressed('D');
	aPressed = isKeyPressed('a') || isKeyPressed('A');
	shiftPressed = isKeyPressed(' ');

	isRunning = dPressed && shiftPressed && sprintTime > 0;
	isMoving = dPressed;
	isLeftMoving = aPressed;
	isLeftWalking = aPressed;

	if ((isKeyPressed('f') || isKeyPressed('F')) && !isJumping) {
		isJumping = true;
		jumpVelocity = 45;
	}

	if (isKeyPressed('s') || isKeyPressed('S')) {
		isFiring = true;
		createBullet();
	}
	else {
		isFiring = false;
		fireFrame = 0;
	}
}

void updateFrameLevel_1() {
	currentFrame++;
	if (isMoving) {
		if (isRunning && currentFrame >= 8) currentFrame = 0;
		else if (!isRunning && currentFrame >= 7) currentFrame = 0;
	}
	else if (currentFrame >= 7) {
		currentFrame = 0;
	}

	if (!isDead && (playGroundPage == 1 && currentBgSegment < 9)) {
		if (isJumping) {
			int jumpForward = 12;
			int nextX = charX + jumpForward;
			if (!isBlockedByEnemy(nextX, charY)) {
				charX = nextX;
			}
			jumpY += jumpVelocity;
			jumpVelocity -= 10;
			if (jumpY <= 0) {
				updateBackgroundScrolling();
				jumpY = 0;
				isJumping = false;
			}
		}
		else {
			int walkSpeed = 4;
			int runSpeed = 8;
			int walkScroll = 8;
			int runScroll = 16;

			if (dPressed) {
				int moveSpeed = (isRunning && sprintTime > 0) ? runSpeed : walkSpeed;
				int nextCharX = charX + moveSpeed;
				if (!isBlockedByEnemy(nextCharX, charY)) {
					charX = nextCharX;
				}
				if (charX > 450) {
					int scrollSpeed = (isRunning && sprintTime > 0) ? runScroll : walkScroll;
					backgroundOfSetX += scrollSpeed;
					charX = 450;
					if (backgroundOfSetX >= bgWidth) {
						backgroundOfSetX = 0;
						int previousSegment = currentBgSegment;
						if (currentBgSegment < 9) {
							currentBgSegment++;
						}
						if (currentBgSegment != previousSegment) {
							activateElectricTrapsForSegment(currentBgSegment);
							extern void onMenuSegmentChange(int);
							onMenuSegmentChange(currentBgSegment);
							charX = 100;
						}
					}
				}
			}
			if (aPressed) {
				int nextCharX = charX - walkSpeed;
				if (nextCharX < 100) nextCharX = 100;
				if (!isBlockedByEnemy(nextCharX, charY)) {
					charX = nextCharX;
				}
			}
		}
	}

	// Update bullets
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (bulletActive[i]) {
			bulletX[i] += 10;
			if (bulletX[i] > 1000) {
				bulletActive[i] = false;
			}
		}
	}

	if (isFiring) {
		fireFrame++;
		if (fireFrame >= 4) fireFrame = 0;
	}
}
#endif