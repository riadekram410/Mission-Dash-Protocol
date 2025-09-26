#ifndef ENEMY_SYSTEM_HPP
#define ENEMY_SYSTEM_HPP

#include "Background.hpp"
#include <windows.h>
#include <cmath>
#include "HealthBar.hpp"

const int MAX_ENEMIES = 2;
const int MAX_ENEMY_BULLETS = 20;

struct Enemy {
	int x, y;
	int health;
	bool alive;
	char* image;
	DWORD lastShootTime;
	bool startedShooting;
	int frameIndex;
	DWORD lastAnimTime;
};

struct EnemyBullet {
	int x, y;
	bool active;
	int dx, dy;
};

// Boss Enemy System
const int BOSS_WALK_FRAMES = 10;
const int BOSS_ATTACK_FRAMES = 13;
const int MAX_BOSS_BULLETS = 30;

// Boss walking sprites
char* bossWalkLeftSprites[BOSS_WALK_FRAMES] = {
	(char*)"image/LeftWalk1.bmp", (char*)"image/LeftWalk2.bmp",
	(char*)"image/LeftWalk3.bmp", (char*)"image/LeftWalk4.bmp",
	(char*)"image/LeftWalk5.bmp", (char*)"image/LeftWalk6.bmp",
	(char*)"image/LeftWalk7.bmp", (char*)"image/LeftWalk8.bmp",
	(char*)"image/LeftWalk9.bmp", (char*)"image/LeftWalk10.bmp"
};

char* bossWalkRightSprites[BOSS_WALK_FRAMES] = {
	(char*)"image/RightWalk1.bmp", (char*)"image/RightWalk2.bmp",
	(char*)"image/RightWalk3.bmp", (char*)"image/RightWalk4.bmp",
	(char*)"image/RightWalk5.bmp", (char*)"image/RightWalk6.bmp",
	(char*)"image/RightWalk7.bmp", (char*)"image/RightWalk8.bmp",
	(char*)"image/RightWalk9.bmp", (char*)"image/RightWalk10.bmp"
};

// Boss attack sprites
char* bossAttackLeftSprites[BOSS_ATTACK_FRAMES] = {
	(char*)"image/HitLeft00.bmp", (char*)"image/HitLeft01.bmp",
	(char*)"image/HitLeft02.bmp", (char*)"image/HitLeft03.bmp",
	(char*)"image/HitLeft04.bmp", (char*)"image/HitLeft05.bmp",
	(char*)"image/HitLeft06.bmp", (char*)"image/HitLeft07.bmp",
	(char*)"image/HitLeft08.bmp", (char*)"image/HitLeft09.bmp",
	(char*)"image/HitLeft10.bmp", (char*)"image/HitLeft11.bmp",
	(char*)"image/HitLeft12.bmp"
};

char* bossAttackRightSprites[BOSS_ATTACK_FRAMES] = {
	(char*)"image/HitRight00.bmp", (char*)"image/HitRight01.bmp",
	(char*)"image/HitRight02.bmp", (char*)"image/HitRight03.bmp",
	(char*)"image/HitRight04.bmp", (char*)"image/HitRight05.bmp",
	(char*)"image/HitRight06.bmp", (char*)"image/HitRight07.bmp",
	(char*)"image/HitRight08.bmp", (char*)"image/HitRight09.bmp",
	(char*)"image/HitRight10.bmp", (char*)"image/HitRight11.bmp",
	(char*)"image/HitRight12.bmp"
};

enum BossState {
	BOSS_WALKING_LEFT,
	BOSS_WALKING_RIGHT,
	BOSS_ATTACKING_LEFT,
	BOSS_ATTACKING_RIGHT,
	BOSS_IDLE
};

struct Boss {
	int x, y;
	int health;
	bool alive;
	BossState state;
	int frameIndex;
	DWORD lastAnimTime;
	DWORD lastShootTime;
	DWORD lastStateChange;
	int moveDirection;
	bool isAttacking;
	DWORD attackStartTime;
};

struct BossBullet {
	int x, y;
	bool active;
	int dx, dy;
};

Boss boss;
BossBullet bossBullets[MAX_BOSS_BULLETS];

const int MAX_DRONES = 3;
const int DRONE_FRAMES = 4;
char* droneSprites[DRONE_FRAMES] = {
	(char*)"image/drone1.bmp",
	(char*)"image/drone2.bmp",
	(char*)"image/drone3.bmp",
	(char*)"image/drone4.bmp"
};

struct Drone {
	int x, y;
	bool active;
	int segmentIndex;
	DWORD lastShootTime;
	int frameIndex;
	DWORD lastAnimTime;
};

struct DroneBullet {
	int x, y;
	bool active;
	int dx, dy;
};

const int ENEMY_L2_FRAMES = 5;
char* enemy2Sprites[ENEMY_L2_FRAMES] = {
	(char*)"image/ENEMY1.bmp",
	(char*)"image/ENEMY2.bmp",
	(char*)"image/ENEMY3.bmp",
	(char*)"image/ENEMY4.bmp",
	(char*)"image/ENEMY5.bmp"
};

extern int charX, charY;
extern int characterHealth;
extern bool isDead;
extern int backgroundOfSetX;
extern int currentBgSegment;
extern int currentLevelType;

Enemy enemies[MAX_ENEMIES];
EnemyBullet enemyBullets[MAX_ENEMY_BULLETS];

Drone drones[MAX_DRONES];
DroneBullet droneBullets[MAX_DRONES * 5];

void initializeBoss() {
	boss.x = 850;
	boss.y = 55;
	boss.health = 500;
	boss.alive = true;
	boss.state = BOSS_WALKING_LEFT;
	boss.frameIndex = 0;
	boss.lastAnimTime = GetTickCount();
	boss.lastShootTime = GetTickCount();
	boss.lastStateChange = GetTickCount();
	boss.moveDirection = -1;
	boss.isAttacking = false;
	boss.attackStartTime = 0;

	for (int i = 0; i < MAX_BOSS_BULLETS; i++) {
		bossBullets[i].active = false;
	}
}

void updateBoss() {
	if (!boss.alive || currentBgSegment != 8 || currentLevelType != 1) return;

	DWORD currentTime = GetTickCount();

	// Animation update
	if (currentTime - boss.lastAnimTime > 120) {
		if (boss.state == BOSS_WALKING_LEFT || boss.state == BOSS_WALKING_RIGHT) {
			boss.frameIndex = (boss.frameIndex + 1) % BOSS_WALK_FRAMES;
		}
		else if (boss.state == BOSS_ATTACKING_LEFT || boss.state == BOSS_ATTACKING_RIGHT) {
			boss.frameIndex = (boss.frameIndex + 1) % BOSS_ATTACK_FRAMES;
		}
		boss.lastAnimTime = currentTime;
	}

	// State management
	if (!boss.isAttacking) {
		// Movement logic
		if (boss.state == BOSS_WALKING_LEFT || boss.state == BOSS_WALKING_RIGHT) {
			boss.x += boss.moveDirection * 2;

			if (boss.x <= 400) {
				boss.moveDirection = 1;
				boss.state = BOSS_WALKING_RIGHT;
			}
			else if (boss.x >= 800) {
				boss.moveDirection = -1;
				boss.state = BOSS_WALKING_LEFT;
			}
		}

		// Attack decision
		int distanceToPlayer = abs(boss.x - charX);
		if (distanceToPlayer < 300 && currentTime - boss.lastShootTime > 2000) {
			boss.isAttacking = true;
			boss.attackStartTime = currentTime;
			boss.frameIndex = 0;

			if (charX < boss.x) {
				boss.state = BOSS_ATTACKING_LEFT;
			}
			else {
				boss.state = BOSS_ATTACKING_RIGHT;
			}
		}
	}
	else {
		// Handle attack sequence
		if (currentTime - boss.attackStartTime > 1600) {
			// Create bullets during attack
			if (boss.frameIndex >= 6 && boss.frameIndex <= 10) {
				for (int i = 0; i < MAX_BOSS_BULLETS; i++) {
					if (!bossBullets[i].active) {
						bossBullets[i].x = boss.x + (boss.state == BOSS_ATTACKING_LEFT ? 0 : 80);
						bossBullets[i].y = boss.y + 40;

						int dx = (charX + 25) - bossBullets[i].x;
						int dy = (charY + 25) - bossBullets[i].y;
						float length = sqrt(dx*dx + dy*dy);
						if (length == 0) length = 1.0f;

						bossBullets[i].dx = (int)(8.0f * dx / length);
						bossBullets[i].dy = (int)(8.0f * dy / length);
						bossBullets[i].active = true;
						break;
					}
				}
			}

			if (boss.frameIndex >= BOSS_ATTACK_FRAMES - 1) {
				boss.isAttacking = false;
				boss.lastShootTime = currentTime;
				if (boss.state == BOSS_ATTACKING_LEFT) {
					boss.state = BOSS_WALKING_LEFT;
					boss.moveDirection = -1;
				}
				else {
					boss.state = BOSS_WALKING_RIGHT;
					boss.moveDirection = 1;
				}
				boss.frameIndex = 0;
			}
		}
	}
}

void drawBoss() {
	if (!boss.alive || currentBgSegment != 8 || currentLevelType != 1) return;

	int drawX = boss.x - backgroundOfSetX;
	int drawY = boss.y;

	// Draw boss sprite based on current state
	char* currentSprite = nullptr;
	switch (boss.state) {
	case BOSS_WALKING_LEFT:
		currentSprite = bossWalkLeftSprites[boss.frameIndex];
		break;
	case BOSS_WALKING_RIGHT:
		currentSprite = bossWalkRightSprites[boss.frameIndex];
		break;
	case BOSS_ATTACKING_LEFT:
		currentSprite = bossAttackLeftSprites[boss.frameIndex];
		break;
	case BOSS_ATTACKING_RIGHT:
		currentSprite = bossAttackRightSprites[boss.frameIndex];
		break;
	}

	if (currentSprite) {
		iShowBMP2(drawX, drawY, currentSprite, 0xFFFFFF);
	}

	// Draw boss health bar
	int barWidth = 80;
	int healthWidth = (boss.health * barWidth) / 500;
	int healthBarX = drawX + 10;
	int healthBarY = drawY + 120;

	iSetColor(22, 111, 168);
	iFilledRectangle(healthBarX, healthBarY, barWidth, 10);
	iSetColor(173, 216, 230);
	if (boss.health <= 300 && boss.health > 150)
		iSetColor(255, 255, 0);
	else if (boss.health <= 150)
		iSetColor(255, 80, 80);
	iFilledRectangle(healthBarX, healthBarY, healthWidth, 10);
	iSetColor(255, 255, 255);
	iRectangle(healthBarX, healthBarY, barWidth, 10);

	iText(healthBarX, healthBarY + 15, "BOSS", GLUT_BITMAP_HELVETICA_12);
}

void updateBossBullets() {
	if (currentBgSegment != 8 || currentLevelType != 1) return;

	for (int i = 0; i < MAX_BOSS_BULLETS; i++) {
		if (bossBullets[i].active) {
			bossBullets[i].x += bossBullets[i].dx;
			bossBullets[i].y += bossBullets[i].dy;

			if (bossBullets[i].x < -10 || bossBullets[i].x > 1010 ||
				bossBullets[i].y < -10 || bossBullets[i].y > 610) {
				bossBullets[i].active = false;
			}
		}
	}
}

void drawBossBullets() {
	if (currentBgSegment != 8 || currentLevelType != 1) return;

	iSetColor(255, 100, 0);
	for (int i = 0; i < MAX_BOSS_BULLETS; i++) {
		if (bossBullets[i].active) {
			iFilledCircle(bossBullets[i].x, bossBullets[i].y, 6);
		}
	}
	iSetColor(255, 255, 255);
}

void checkBossBulletHits() {
	if (currentBgSegment != 8 || currentLevelType != 1) return;

	const int charWidth = 50, charHeight = 50;
	for (int i = 0; i < MAX_BOSS_BULLETS; i++) {
		if (bossBullets[i].active) {
			if (bossBullets[i].x > charX && bossBullets[i].x < charX + charWidth &&
				bossBullets[i].y > charY && bossBullets[i].y < charY + charHeight) {
				characterHealth -= 20;
				if (characterHealth < 0) characterHealth = 0;
				bossBullets[i].active = false;
				if (characterHealth == 0) isDead = true;
			}
		}
	}
}

bool isBlockedByBoss(int proposedX, int proposedY) {
	if (currentBgSegment != 8 || currentLevelType != 1 || !boss.alive) return false;

	const int charWidth = 50, charHeight = 50;
	const int bossWidth = 100, bossHeight = 100;
	static DWORD lastBossHitTime = 0;
	DWORD currentTime = GetTickCount();

	int bx = boss.x - backgroundOfSetX;
	int by = boss.y;

	if (proposedX + charWidth > bx && proposedX < bx + bossWidth &&
		proposedY + charHeight > by && proposedY < by + bossHeight) {

		if (currentTime - lastBossHitTime > 1000) {
			characterHealth -= 40;
			if (characterHealth < 0) {
				characterHealth = 0;
				isDead = true;
			}

			extern int charX;
			charX -= 150;
			if (charX < 50) charX = 50;
			lastBossHitTime = currentTime;
		}
		return true;
	}
	return false;
}

void checkCharacterBulletHitsBoss(int bulletX[], int bulletY[], int bulletActive[], int MAX_BULLETS) {
	if (currentBgSegment != 8 || currentLevelType != 1 || !boss.alive) return;

	const int bulletR = 2;
	const int bossWidth = 100, bossHeight = 100;

	for (int i = 0; i < MAX_BULLETS; i++) {
		if (bulletActive[i]) {
			int bx = boss.x - backgroundOfSetX;
			int by = boss.y;

			if (bulletX[i] + bulletR > bx && bulletX[i] < bx + bossWidth &&
				bulletY[i] + bulletR > by && bulletY[i] < by + bossHeight) {
				boss.health -= 25;
				if (boss.health <= 0) {
					boss.alive = false;
					boss.health = 0;
				}
				bulletActive[i] = false;
			}
		}
	}
}

void spawnEnemiesForSegment(int segment) {
	for (int i = 0; i < MAX_ENEMIES; i++) {
		enemies[i].alive = false;
		enemies[i].health = 0;
		enemies[i].startedShooting = false;
		enemies[i].image = nullptr;
		enemies[i].frameIndex = 0;
		enemies[i].lastAnimTime = GetTickCount();
	}

	if (segment == 8 && currentLevelType == 1) {
		initializeBoss();
		return;
	}

	if (segment >= 9) return;
	BackgroundSegment* seg = getCurrentSegment();
	if (seg->enemyType1 == 1)
		enemies[0] = { seg->enemy1X, seg->enemy1Y, 100, true, seg->enemy1Img, GetTickCount(), false, 0, GetTickCount() };
	if (seg->enemyType2 == 1)
		enemies[1] = { seg->enemy2X, seg->enemy2Y, 100, true, seg->enemy2Img, GetTickCount(), false, 0, GetTickCount() };
}

void onSegmentChange(int segment) {
	spawnEnemiesForSegment(segment);
	for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
		enemyBullets[i].active = false;
	}
	if (currentLevelType >= 1) {
		for (int i = 0; i < MAX_DRONES; i++) {
			drones[i].active = false;
			drones[i].segmentIndex = -1;
			drones[i].frameIndex = 0;
			drones[i].lastAnimTime = GetTickCount();
		}
		if (segment < 8) {
			if (segment == 1 || segment == 4 || segment == 7) {
				drones[segment % 3] = { 1000 + 100, 400, true, segment, GetTickCount(), 0, GetTickCount() };
			}
		}
		for (int i = 0; i < MAX_DRONES * 5; i++) {
			droneBullets[i].active = false;
		}
	}
}

void drawEnemies() {
	if (currentBgSegment >= 9) return;

	if (currentBgSegment == 8 && currentLevelType == 1) {
		drawBoss();
		return;
	}

	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (enemies[i].alive) {
			int drawX = enemies[i].x - backgroundOfSetX;
			int drawY = enemies[i].y;

			if (currentLevelType == 1 && enemies[i].image && (strstr(enemies[i].image, "level2") || strstr(enemies[i].image, "ENEMY"))) {
				DWORD now = GetTickCount();
				if (now - enemies[i].lastAnimTime > 120) {
					enemies[i].frameIndex = (enemies[i].frameIndex + 1) % ENEMY_L2_FRAMES;
					enemies[i].lastAnimTime = now;
				}
				iShowBMP2(drawX, drawY, enemy2Sprites[enemies[i].frameIndex], 0xFFFFFF);
			}
			else if (enemies[i].image) {
				iShowBMP2(drawX, drawY, enemies[i].image, 0xFFFFFF);
			}

			// Enemy health bar
			int barWidth = 36;
			int healthWidth = (enemies[i].health * barWidth) / 100;
			int healthBarX = drawX + 45;
			int healthBarY = drawY + 80;
			iSetColor(22, 111, 168);
			iFilledRectangle(healthBarX, healthBarY, barWidth, 7);
			iSetColor(173, 216, 230);
			if (enemies[i].health <= 60 && enemies[i].health > 30)
				iSetColor(255, 255, 0);
			else if (enemies[i].health <= 30)
				iSetColor(255, 80, 80);
			iFilledRectangle(healthBarX, healthBarY, healthWidth, 7);
			iSetColor(255, 255, 255);
			iRectangle(healthBarX, healthBarY, barWidth, 7);
		}
	}
}

void updateDrones() {
	if (currentLevelType < 1 || currentBgSegment >= 9) return;

	for (int i = 0; i < MAX_DRONES; i++) {
		if (!drones[i].active) continue;
		drones[i].x -= 6;
		if (drones[i].x < -100) {
			drones[i].active = false;
			continue;
		}

		DWORD now = GetTickCount();
		if (now - drones[i].lastAnimTime > 80) {
			drones[i].frameIndex = (drones[i].frameIndex + 1) % DRONE_FRAMES;
			drones[i].lastAnimTime = now;
		}

		int playerMidX = charX + 25;
		if (abs(drones[i].x - playerMidX) < 120 && now - drones[i].lastShootTime > 2000) {
			int bulletsToShoot = 5 + (rand() % 2);
			int bulletsCreated = 0;

			for (int j = 0; j < MAX_DRONES * 5 && bulletsCreated < bulletsToShoot; j++) {
				if (!droneBullets[j].active) {
					int ex = drones[i].x;
					int ey = drones[i].y;
					int ddx = charX + 25 - ex;
					int ddy = charY + 25 - ey;
					float length = sqrt(ddx*ddx + ddy*ddy);
					if (length == 0) length = 1.0f;

					float spread = (bulletsCreated - 2.5f) * 0.3f;
					droneBullets[j].x = ex;
					droneBullets[j].y = ey;
					droneBullets[j].dx = int((10.0f * ddx / length) + spread);
					droneBullets[j].dy = int((10.0f * ddy / length) + spread);
					droneBullets[j].active = true;
					bulletsCreated++;
				}
			}

			if (bulletsCreated > 0) {
				drones[i].lastShootTime = now;
			}
		}
	}
}

void drawDrones() {
	if (currentLevelType < 1 || currentBgSegment >= 9) return;

	for (int i = 0; i < MAX_DRONES; i++) {
		if (drones[i].active) {
			int drawX = drones[i].x - backgroundOfSetX;
			int drawY = drones[i].y;
			iShowBMP2(drawX, drawY, droneSprites[drones[i].frameIndex], 0xFFFFFF);
		}
	}
}

void updateDroneBullets() {
	if (currentLevelType < 1 || currentBgSegment >= 9) return;

	for (int i = 0; i < MAX_DRONES * 5; i++) {
		if (droneBullets[i].active) {
			droneBullets[i].x += droneBullets[i].dx;
			droneBullets[i].y += droneBullets[i].dy;
			if (droneBullets[i].x < -10 || droneBullets[i].x > 1010 || droneBullets[i].y < -10 || droneBullets[i].y > 610)
				droneBullets[i].active = false;
		}
	}
}

void drawDroneBullets() {
	if (currentLevelType < 1 || currentBgSegment >= 9) return;

	iSetColor(0, 200, 255);
	for (int i = 0; i < MAX_DRONES * 5; i++) {
		if (droneBullets[i].active) {
			iFilledCircle(droneBullets[i].x, droneBullets[i].y, 4);
		}
	}
	iSetColor(255, 255, 255);
}

void checkDroneBulletHits() {
	if (currentLevelType < 1 || currentBgSegment >= 9) return;

	const int charWidth = 50, charHeight = 50;
	for (int i = 0; i < MAX_DRONES * 5; i++) {
		if (droneBullets[i].active) {
			if (droneBullets[i].x > charX && droneBullets[i].x < charX + charWidth &&
				droneBullets[i].y > charY && droneBullets[i].y < charY + charHeight) {
				characterHealth -= 12;
				if (characterHealth < 0) characterHealth = 0;
				droneBullets[i].active = false;
				if (characterHealth == 0) isDead = true;
			}
		}
	}
}

void updateEnemyShooting() {
	DWORD now = GetTickCount();
	if (currentBgSegment >= 9) return;

	if (currentBgSegment == 8 && currentLevelType == 1) return;

	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (!enemies[i].alive) continue;
		int dx = abs((enemies[i].x - backgroundOfSetX) - charX);
		if (!enemies[i].startedShooting && dx <= 250)
			enemies[i].startedShooting = true;

		if (enemies[i].startedShooting && now - enemies[i].lastShootTime > 1800) {
			int bulletsToShoot = 2 + (rand() % 2);
			int bulletsCreated = 0;

			for (int j = 0; j < MAX_ENEMY_BULLETS && bulletsCreated < bulletsToShoot; j++) {
				if (!enemyBullets[j].active) {
					int ex = enemies[i].x - backgroundOfSetX + 25;
					int ey = enemies[i].y + 25;
					int ddx = charX + 25 - ex;
					int ddy = charY + 25 - ey;
					float length = sqrt(ddx*ddx + ddy*ddy);
					if (length == 0) length = 1.0f;

					float spread = (bulletsCreated - 1.0f) * 0.2f;
					enemyBullets[j].x = ex;
					enemyBullets[j].y = ey;
					enemyBullets[j].dx = int((8.0f * ddx / length) + spread);
					enemyBullets[j].dy = int((8.0f * ddy / length) + spread);
					enemyBullets[j].active = true;
					bulletsCreated++;
				}
			}

			if (bulletsCreated > 0) {
				enemies[i].lastShootTime = now;
			}
		}
	}
}

void updateEnemyBullets() {
	for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
		if (enemyBullets[i].active) {
			enemyBullets[i].x += enemyBullets[i].dx;
			enemyBullets[i].y += enemyBullets[i].dy;
			if (enemyBullets[i].x < -10 || enemyBullets[i].x > 1010 || enemyBullets[i].y < -10 || enemyBullets[i].y > 610)
				enemyBullets[i].active = false;
		}
	}
}

void drawEnemyBullets() {
	if (currentBgSegment >= 9) return;

	iSetColor(255, 0, 0);
	for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
		if (enemyBullets[i].active) {
			iFilledCircle(enemyBullets[i].x, enemyBullets[i].y, 4);
		}
	}
	iSetColor(255, 255, 255);
}

void checkEnemyBulletHits() {
	if (currentBgSegment >= 9) return;

	const int charWidth = 50, charHeight = 50;
	for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
		if (enemyBullets[i].active) {
			if (enemyBullets[i].x > charX && enemyBullets[i].x < charX + charWidth &&
				enemyBullets[i].y > charY && enemyBullets[i].y < charY + charHeight) {
				characterHealth -= 10;
				if (characterHealth < 0) characterHealth = 0;
				enemyBullets[i].active = false;
				if (characterHealth == 0) isDead = true;
			}
		}
	}
}

bool isBlockedByEnemy(int proposedX, int proposedY) {
	if (currentBgSegment >= 9) return false;

	if (currentBgSegment == 8 && currentLevelType == 1) {
		return isBlockedByBoss(proposedX, proposedY);
	}

	const int charWidth = 50, charHeight = 50;
	const int enemyWidth = 50, enemyHeight = 50;
	static DWORD lastPushbackTime = 0;
	DWORD currentTime = GetTickCount();

	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (enemies[i].alive) {
			int ex = enemies[i].x - backgroundOfSetX;
			int ey = enemies[i].y;
			if (proposedX + charWidth > ex && proposedX < ex + enemyWidth &&
				proposedY + charHeight > ey && proposedY < ey + enemyHeight) {

				if (currentLevelType >= 1 && currentTime - lastPushbackTime > 1000) {
					characterHealth -= 30;
					if (characterHealth < 0) {
						characterHealth = 0;
						isDead = true;
					}
					extern int charX;
					charX -= 100;
					if (charX < 50) charX = 50;
					lastPushbackTime = currentTime;
				}
				return true;
			}
		}
	}
	return false;
}

void checkCharacterBulletHits(int bulletX[], int bulletY[], int bulletActive[], int MAX_BULLETS) {
	if (currentBgSegment >= 9) return;

	if (currentBgSegment == 8 && currentLevelType == 1) {
		checkCharacterBulletHitsBoss(bulletX, bulletY, bulletActive, MAX_BULLETS);
		return;
	}

	const int bulletR = 2;
	const int enemyWidth = 50, enemyHeight = 50;
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (bulletActive[i]) {
			for (int j = 0; j < MAX_ENEMIES; j++) {
				if (enemies[j].alive) {
					int ex = enemies[j].x - backgroundOfSetX;
					int ey = enemies[j].y;
					if (bulletX[i] + bulletR > ex && bulletX[i] < ex + enemyWidth &&
						bulletY[i] + bulletR > ey && bulletY[i] < ey + enemyHeight) {
						enemies[j].health -= 20;
						if (enemies[j].health <= 0) {
							enemies[j].alive = false;
						}
						bulletActive[i] = false;
					}
				}
			}
		}
	}
}

#endif