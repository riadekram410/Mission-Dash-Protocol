#ifndef AUDIO_HPP
#define AUDIO_HPP
#include <windows.h>

extern bool isHomeMusicPlaying;
extern bool isPlayMusicStarted;

inline void playHomeMusic() {
	if (isHomeMusicPlaying) {
		mciSendString("close homeMusic", NULL, 0, NULL);
		mciSendString("open \"Audios//home.mp3\" alias homeMusic", NULL, 0, NULL);
		mciSendString("play homeMusic repeat", NULL, 0, NULL);
	}
}

inline void startPlayMusic() {
	if (!isPlayMusicStarted) {
		mciSendString("close playMusic", NULL, 0, NULL);
		mciSendString("open \"Audios//playSound.mp3\" alias playMusic", NULL, 0, NULL);
		mciSendString("play playMusic repeat", NULL, 0, NULL);
		isPlayMusicStarted = true;
	}
}

inline void stopHomeMusic() {
	if (isHomeMusicPlaying) {
		mciSendString("stop homeMusic", NULL, 0, NULL);
		mciSendString("close homeMusic", NULL, 0, NULL);
		isHomeMusicPlaying = false;
	}
}
#endif