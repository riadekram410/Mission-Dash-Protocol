#ifndef SCOREBOARD_HPP
#define SCOREBOARD_HPP

#include <stdio.h>

// Simple variables to store best times
int level1BestTime = 999; // 999 means no record
int level2BestTime = 999;

extern int scoreboardPage;

// Save scores to a simple text file
void saveScores() {
	FILE* file;
	errno_t err = fopen_s(&file, "scores.txt", "w");
	if (err == 0 && file != NULL) {
		fprintf(file, "%d\n", level1BestTime);
		fprintf(file, "%d\n", level2BestTime);
		fclose(file);
	}
}

// Load scores from text file
void loadScores() {
	FILE* file;
	errno_t err = fopen_s(&file, "scores.txt", "r");
	if (err == 0 && file != NULL) {
		fscanf_s(file, "%d", &level1BestTime);
		fscanf_s(file, "%d", &level2BestTime);
		fclose(file);
	}
}

// Add a new time record
void addTimeRecord(int levelType, int minutes, int seconds) {
	int totalSeconds = minutes * 60 + seconds;

	if (levelType == 0) { // Level 1
		if (totalSeconds < level1BestTime) {
			level1BestTime = totalSeconds;
			saveScores(); // Save immediately when we get a new record
		}
	}
	else { // Level 2
		if (totalSeconds < level2BestTime) {
			level2BestTime = totalSeconds;
			saveScores(); // Save immediately when we get a new record
		}
	}
}

// Convert seconds back to minutes and seconds
void getTimeString(int totalSeconds, char* timeString) {
	if (totalSeconds >= 999) {
		strcpy_s(timeString, 20, "No record yet");
	}
	else {
		int minutes = totalSeconds / 60;
		int seconds = totalSeconds % 60;
		sprintf_s(timeString, 20, "%02d:%02d", minutes, seconds);
	}
}

// Draw the scoreboard
void drawScoreboard() {
	if (scoreboardPage != 1) return;

	// Dark blue background
	iSetColor(0, 0, 80);
	iFilledRectangle(0, 0, 1000, 600);

	// Title
	iSetColor(255, 255, 0);
	iText(400, 500, "BEST TIMES", GLUT_BITMAP_TIMES_ROMAN_24);

	// Level 1 section
	iSetColor(255, 255, 255);
	iText(300, 400, "LEVEL 1:", GLUT_BITMAP_HELVETICA_18);

	char level1TimeText[20];
	getTimeString(level1BestTime, level1TimeText);
	if (level1BestTime < 999) {
		iSetColor(0, 255, 0); // Green for records
	}
	else {
		iSetColor(150, 150, 150); // Gray for no record
	}
	iText(300, 370, level1TimeText, GLUT_BITMAP_HELVETICA_12);

	// Level 2 section
	iSetColor(255, 255, 255);
	iText(300, 300, "LEVEL 2:", GLUT_BITMAP_HELVETICA_18);

	char level2TimeText[20];
	getTimeString(level2BestTime, level2TimeText);
	if (level2BestTime < 999) {
		iSetColor(0, 255, 0); // Green for records
	}
	else {
		iSetColor(150, 150, 150); // Gray for no record
	}
	iText(300, 270, level2TimeText, GLUT_BITMAP_HELVETICA_12);

	// Show if you beat your record
	iSetColor(255, 215, 0);
	iText(300, 200, "Complete levels faster to beat your record!", GLUT_BITMAP_HELVETICA_10);

	// Back button
	iSetColor(120, 120, 120);
	iFilledRectangle(50, 50, 100, 40);
	iSetColor(255, 255, 255);
	iRectangle(50, 50, 100, 40);
	iText(75, 62, "< Back", GLUT_BITMAP_HELVETICA_18);
}

// Initialize - load scores when game starts
void initializeScoreboard() {
	loadScores();
}

#endif