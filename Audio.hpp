#ifndef AUDIO_HPP
#define AUDIO_HPP
#include<windows.h> //For mciSendString

extern bool isHomeMusicPlaying;
extern bool isPlayMusicStarted;

void playHomeMusic() {
	if (isHomeMusicPlaying) {
		mciSendString("open \"Audios//home.mp3\" alias homeMusic", NULL, 0, NULL);
		mciSendString("play homeMusic repeat", NULL, 0, NULL);  // Loop home.mp3
	}
}
//function to start the playSound.mp3
void startPlayMusic() {
	if (!isPlayMusicStarted) {
		mciSendString("open \"Audios//playSound.mp3\" alias playMusic", NULL, 0, NULL);
		mciSendString("play playMusic repeat", NULL, 0, NULL);  // Play the music continuously
		isPlayMusicStarted = true;
	}
}
// Function to stop the home music
void stopHomeMusic() {
	if (isHomeMusicPlaying) {
		mciSendString("stop homeMusic", NULL, 0, NULL);  // Stop home music
		mciSendString("close homeMusic", NULL, 0, NULL);  // Close the file to free memory
		isHomeMusicPlaying = false;
	}
}

#endif 