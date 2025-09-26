#ifndef PAUSE_MENU_HPP
#define PAUSE_MENU_HPP

extern bool isPaused;
extern bool soundOn;
extern int playGroundPage, currentPage, levelPage;

const int pauseMenuWidth = 360;
const int pauseMenuHeight = 320;
const int pauseMenuX = 320;
const int pauseMenuY = 140;
const int pauseBtnWidth = 280;
const int pauseBtnHeight = 55;

enum PauseOption {
	PAUSE_NONE = -1,
	PAUSE_RESUME = 0,
	PAUSE_RESTART = 1,
	PAUSE_SOUND = 2,
	PAUSE_EXIT = 3
};

extern int hoveredPauseBtn;

void drawPauseMenu() {
	// Menu background
	iSetColor(36, 50, 90);
	iFilledRectangle(pauseMenuX, pauseMenuY, pauseMenuWidth, pauseMenuHeight);
	iSetColor(255, 255, 255);
	iRectangle(pauseMenuX, pauseMenuY, pauseMenuWidth, pauseMenuHeight);

	char* btnLabels[4] = {
		"Resume",
		"Restart",
		"",
		"Exit to Level Select"
	};
	btnLabels[2] = soundOn ? "Sound: On" : "Sound: Off";

	for (int i = 0; i < 4; i++) {
		int bx = pauseMenuX + 40;
		int by = pauseMenuY + pauseMenuHeight - (i + 1) * (pauseBtnHeight + 15);

		if (hoveredPauseBtn == i) {
			iSetColor(100, 180, 220);
			iFilledRectangle(bx, by, pauseBtnWidth, pauseBtnHeight);
			iSetColor(255, 255, 255);
		}
		else {
			iSetColor(80, 110, 160);
			iFilledRectangle(bx, by, pauseBtnWidth, pauseBtnHeight);
			iSetColor(255, 255, 255);
		}
		iRectangle(bx, by, pauseBtnWidth, pauseBtnHeight);
		iText(bx + 30, by + 18, btnLabels[i], GLUT_BITMAP_HELVETICA_18);
	}
	iSetColor(255, 255, 255);
	iText(pauseMenuX + pauseMenuWidth / 2 - 50, pauseMenuY + pauseMenuHeight - 35 + 50, "Paused", GLUT_BITMAP_TIMES_ROMAN_24);
}

void hoverPauseMenu(int mx, int my) {
	hoveredPauseBtn = -1;
	for (int i = 0; i < 4; i++) {
		int bx = pauseMenuX + 40;
		int by = pauseMenuY + pauseMenuHeight - (i + 1) * (pauseBtnHeight + 15);
		if (mx >= bx && mx <= bx + pauseBtnWidth &&
			my >= by && my <= by + pauseBtnHeight) {
			hoveredPauseBtn = i;
			break;
		}
	}
}

void handlePauseMenuClick(int mx, int my) {
	hoverPauseMenu(mx, my);
	switch (hoveredPauseBtn) {
	case PAUSE_RESUME:
		isPaused = false;
		break;
	case PAUSE_RESTART:
		extern void resetGameState();
		resetGameState();
		isPaused = false;
		break;
	case PAUSE_SOUND:
		soundOn = !soundOn;
		if (soundOn) {
			extern void playHomeMusic();
			playHomeMusic();
		}
		else {
			extern void stopHomeMusic();
			stopHomeMusic();
		}
		break;
	case PAUSE_EXIT:
		isPaused = false;
		playGroundPage = 0;
		currentPage = 2;
		levelPage = 1;
		break;
	}
}

#endif