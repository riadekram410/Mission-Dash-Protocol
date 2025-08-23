#ifndef LEVEL1_HPP
#define LEVEL1_HPP

unsigned int last, current;
// Character Position and Animation Variables
extern int charX, charY;

// Page states
extern int startPage;        // 1 = story1, 2 = story2
extern int levelPage;        // 1 = level select
extern int levelStoryPage;   // 1 = level1 story1, 2 = level1 story2
extern int playGroundPage;  // 1 = playground (after level story)
extern int currentPage;  // 1 = home, 2 = menu, 3 = playground

extern bool isFiring;
extern int fireFrame;

//bullet forward
const int MAX_BULLETS = 20;
int bulletX[MAX_BULLETS], bulletY[MAX_BULLETS];
int bulletActive[MAX_BULLETS];
extern int bulletCount;


extern int currentFrame;
extern bool isMoving;
extern bool isLeftMoving;
extern bool isRunning;
extern bool isLeftWalking;
extern bool dPressed;
extern bool aPressed;
extern bool shiftPressed;
extern int  drawY, jumpY, jumpVelocity;
extern bool isJumping;
extern bool wasRunningBeforeJump;

extern int bgOffset, bgCount;// Tracks how much we've scrolled
extern const int bgWidth;

void handlePageTransitions() {
	current = clock();
	double duration = (current - last) / CLOCKS_PER_SEC;
	last = current;
	if (duration > 0.75) {
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

void keyPressLevel_1(){
	// Handle Enter key press for progression from home page to menu
	if (isKeyPressed('\r') && currentPage == 1) {  // '\r' is the Enter key
		currentPage = 2;
	}
	// Handle Right Arrow for progression between stories and level select
	if (isSpecialKeyPressed(GLUT_KEY_RIGHT)) {
		handlePageTransitions();
	}
	// Handling character movement keys
	if (isKeyPressed('d') || isKeyPressed('D')) {
		dPressed = true;
		isMoving = true;
	}
	else {
		dPressed = false;
		isMoving = false;
		isRunning = false;
		shiftPressed = false;
	}
	if (isKeyPressed('a') || isKeyPressed('A')) {
		aPressed = true;
		isLeftMoving = true;
		isLeftWalking = true;
	}
	else {
		aPressed = false;
		isLeftMoving = false;
		isLeftWalking = false;
	}

	if (isKeyPressed('f') || isKeyPressed('F')) {
		if (!isJumping) {
			isJumping = true;
			jumpVelocity = 60;
		}
	}


	if (isKeyPressed(' ')) {
		shiftPressed = true;
		isRunning = true;
	}
	else{
		isRunning = false;
		shiftPressed = false;
	}
	if (isKeyPressed('s') || isKeyPressed('S')){

		isFiring = true;
	}
	else{

		isFiring = false;
		fireFrame = 0;// key release

	}
	if (isKeyPressed('s') || isKeyPressed('S')) {
		isFiring = true;

		// Create bullet if none active at current index
		if (!bulletActive[bulletCount]) {
			bulletX[bulletCount] = charX + 60;  // from in front of character
			bulletY[bulletCount] = charY + 40;  // adjust to gun height
			bulletActive[bulletCount] = true;

			bulletCount++;
			if (bulletCount >= MAX_BULLETS) bulletCount = 0;  // loop back
		}
	}
	else {
		isFiring = false;
		fireFrame = 0;
	}

}

//Updating Frames of Level 1
void updateFrameLevel_1() {
	currentFrame++;
	if (isMoving) {
		if (isRunning && currentFrame >= 8)
			currentFrame = 0;
		else if (!isRunning && currentFrame >= 7)
			currentFrame = 0;
	}
	else if (currentFrame >= 7) {
		currentFrame = 0;
	}
	else if (isLeftWalking && isLeftMoving && currentFrame >= 7){
		currentFrame = 0;
	}
	if (dPressed) {
		if (shiftPressed)
			charX += 20;
		else
			charX += 10;
		if (charX > 400){
			bgOffset += (shiftPressed ? 80 : 40); // Scroll background with same speed as character
			charX = 400; // Boundary
			if (bgOffset >= bgWidth) {
				bgOffset = 0;
				bgCount = 2;
			}
		}
	}
	if (aPressed) {
		charX -= 10;
		if (charX < 100){
			charX = 100; // Boundary
		}
	}
	if (isJumping) {
		charX += 15;
		jumpY += jumpVelocity;
		jumpVelocity -= 15;
		if (jumpY <= 0) {
			if (charX > 400){
				bgOffset += (shiftPressed ? 80 : 40); // Scroll background with same speed as character
				charX = 400;
				if (bgOffset >= bgWidth) {
					bgOffset = 0;
					bgCount = 2;
				}
			}
			else{
				charX = charX;
			}
			jumpY = 0;
			isJumping = false;
		}
	}
	if (isFiring){
		fireFrame++;
		if (fireFrame >= 4)
			fireFrame = 0;
	}
	if (isKeyPressed('s') || isKeyPressed('S')) {
		isFiring = true;

		// Create bullet if none active at current index
		if (!bulletActive[bulletCount]) {
			bulletX[bulletCount] = charX + 60;  // from in front of character
			bulletY[bulletCount] = charY + 40;  // adjust to gun height
			bulletActive[bulletCount] = true;

			bulletCount++;
			if (bulletCount >= MAX_BULLETS) bulletCount = 0;  // loop back
		}
	}
	else {
		isFiring = false;
		fireFrame = 0;
	}

	for (int i = 0; i < MAX_BULLETS; i++) {
		if (bulletActive[i]) {
			bulletX[i] += 25; // bullet speed
			if (bulletX[i] > 1000) {  // off screen
				bulletActive[i] = false;
			}
		}
	}
}

#endif // !LEVEL1_HPP
