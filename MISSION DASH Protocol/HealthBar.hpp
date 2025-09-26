#ifndef HEALTHBAR_HPP
#define HEALTHBAR_HPP

extern int characterHealth;
extern int sprintTime;
extern int healthBarX, healthBarY, healthBarWidth, healthBarHeight;
extern int sprintBarX, sprintBarY, sprintBarWidth, sprintBarHeight;
extern bool isRunning;

const int MAX_CHARACTER_HEALTH = 500;

inline void drawHealthAndSprintBars() {
	// Health bar background
	iSetColor(30, 62, 107);
	iFilledRectangle(healthBarX, healthBarY, healthBarWidth, healthBarHeight);

	// Health bar fill
	int currentHealthWidth = (characterHealth * healthBarWidth) / MAX_CHARACTER_HEALTH;
	iSetColor(44, 210, 255);
	iFilledRectangle(healthBarX, healthBarY, currentHealthWidth, healthBarHeight);

	// Sprint bar background
	iSetColor(22, 122, 111);
	iFilledRectangle(sprintBarX, sprintBarY, sprintBarWidth, sprintBarHeight);

	// Sprint bar fill
	int currentSprintWidth = (sprintTime * sprintBarWidth) / 5;
	iSetColor(150, 255, 139);
	iFilledRectangle(sprintBarX, sprintBarY, currentSprintWidth, sprintBarHeight);

	// Labels
	iSetColor(255, 255, 255);
	iText(healthBarX, healthBarY + healthBarHeight + 4, "Health", GLUT_BITMAP_HELVETICA_12);
	iText(sprintBarX, sprintBarY + sprintBarHeight + 4, "Sprint", GLUT_BITMAP_HELVETICA_12);
}

inline void updateSprintTime() {
	static int sprintCooldown = 0;
	if (isRunning && sprintTime > 0) {
		if (sprintCooldown <= 0) {
			sprintTime -= 1;
			sprintCooldown = 10;
		}
		else {
			sprintCooldown--;
		}
	}
	else if (!isRunning && sprintTime < 5) {
		if (sprintCooldown <= 0) {
			sprintTime += 1;
			sprintCooldown = 20;
		}
		else {
			sprintCooldown--;
		}
	}
	if (sprintTime < 0) sprintTime = 0;
	if (sprintTime > 5) sprintTime = 5;
}
#endif