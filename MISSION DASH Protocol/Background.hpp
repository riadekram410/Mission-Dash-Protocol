#ifndef BACKGROUND_SYSTEM_HPP
#define BACKGROUND_SYSTEM_HPP

const int MAX_BG_SEGMENTS = 10;
extern int currentBgSegment;
extern int backgroundOfSetX, bgCount;
extern const int bgWidth;
extern const int screenWidth;

// Background segments with traps and enemies
struct BackgroundSegment {
	int trapType1;   int trap1X, trap1Y;
	int trapType2;   int trap2X, trap2Y;
	int enemyType1;  int enemy1X, enemy1Y; char* enemy1Img;
	int enemyType2;  int enemy2X, enemy2Y; char* enemy2Img;
};

BackgroundSegment level1Segments[MAX_BG_SEGMENTS] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, nullptr, 0, 0, 0, nullptr },
	{ 2, 250, 55, 0, 0, 0, 0, 0, 0, nullptr, 0, 0, 0, nullptr },
	{ 1, 350, 55, 0, 0, 0, 0, 0, 0, nullptr, 0, 0, 0, nullptr },
	{ 2, 200, 55, 1, 400, 55, 0, 0, 0, nullptr, 0, 0, 0, nullptr },
	{ 0, 0, 0, 0, 0, 0, 1, 300, 55, (char*)"image/idleEnemy.bmp", 0, 0, 0, nullptr },
	{ 2, 250, 55, 0, 0, 0, 1, 500, 55, (char*)"image/idleEnemy.bmp", 0, 0, 0, nullptr },
	{ 1, 250, 55, 0, 0, 0, 1, 500, 55, (char*)"image/idleEnemy.bmp", 0, 0, 0, nullptr },
	{ 2, 200, 55, 1, 400, 55, 0, 0, 0, nullptr, 0, 0, 0, nullptr },
	{ 0, 0, 0, 0, 0, 0, 1, 250, 55, (char*)"image/idleEnemy.bmp", 1, 500, 55, (char*)"image/idleEnemy.bmp" },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, nullptr, 0, 0, 0, nullptr }
};

BackgroundSegment level2Segments[MAX_BG_SEGMENTS] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, nullptr, 0, 0, 0, nullptr },
	{ 2, 250, 55, 0, 0, 0, 0, 0, 0, nullptr, 0, 0, 0, nullptr },
	{ 0, 0, 0, 2, 350, 55, 0, 0, 0, nullptr, 0, 0, 0, nullptr },
	{ 2, 200, 55, 0, 0, 0, 1, 400, 55, (char*)"image/level2_enemy.bmp", 0, 0, 0, nullptr },
	{ 0, 0, 0, 0, 0, 0, 1, 300, 55, (char*)"image/level2_enemy.bmp", 0, 0, 0, nullptr },
	{ 2, 250, 55, 0, 0, 0, 1, 500, 55, (char*)"image/level2_enemy.bmp", 0, 0, 0, nullptr },
	{ 0, 0, 0, 0, 0, 0, 1, 500, 55, (char*)"image/level2_enemy.bmp", 0, 0, 0, nullptr },
	{ 2, 200, 55, 0, 0, 0, 0, 0, 0, nullptr, 0, 0, 0, nullptr },
	{ 0, 0, 0, 0, 0, 0, 1, 250, 55, (char*)"image/level2_enemy.bmp", 1, 500, 55, (char*)"image/level2_enemy.bmp" },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, nullptr, 0, 0, 0, nullptr }
};

enum LevelType {
	LEVEL_1 = 0,
	LEVEL_2 = 1
};

extern int currentLevelType;

inline BackgroundSegment* getCurrentSegment() {
	if (currentBgSegment >= 0 && currentBgSegment < MAX_BG_SEGMENTS) {
		if (currentLevelType == LEVEL_1)
			return &level1Segments[currentBgSegment];
		else if (currentLevelType == LEVEL_2)
			return &level2Segments[currentBgSegment];
	}
	return &level1Segments[0];
}

inline void updateBackgroundScrolling() {
	extern bool dPressed, shiftPressed;
	extern int charX, sprintTime;
	extern void activateElectricTrapsForSegment(int);
	extern void onSegmentChange(int);

	if (dPressed) {
		int moveSpeed = (shiftPressed && sprintTime > 0) ? 8 : 4;
		charX += moveSpeed;
		if (charX > screenWidth / 2) {
			int scrollSpeed = (shiftPressed && sprintTime > 0) ? 32 : 16;
			backgroundOfSetX += scrollSpeed;
			charX = screenWidth / 2;
			if (backgroundOfSetX >= bgWidth) {
				backgroundOfSetX = 0;
				int previousSegment = currentBgSegment;
				if (currentBgSegment < MAX_BG_SEGMENTS - 1)
					currentBgSegment++;
				activateElectricTrapsForSegment(currentBgSegment);
				onSegmentChange(currentBgSegment);
				charX = 100;
			}
		}
	}
}
#endif