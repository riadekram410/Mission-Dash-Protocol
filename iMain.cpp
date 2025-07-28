#include "iGraphics.h"

void drawStartPage();

int showText = 1;

void blinkTimer() {
	showText = !showText; // Toggle visibility
}

int bgImage1,bgImage2;
int currentPage = 1; // To store the image reference

// Image IDs
int img[4];

// Image positions and sizes
int imgX[4] = { 420, 420, 420, 420 };
int imgY[4] = { 430, 310, 210, 110 };
int imgW = 200;
int imgH = 70;

// Hover effect (scale factor)
float hoverScale = 1.2;  // 20% bigger on hover
int hoveredImg = -1;

int startPage = 0;

void iDraw()
{
	iClear();
	if (currentPage == 1){
		iShowImage(0, 0, 1000, 600, bgImage1); // Draws the background image
		if (showText) { //Shows text
			iSetColor(255, 255, 255);
			iText(600, 50, "Please press ENTER to continue...", GLUT_BITMAP_HELVETICA_18);
		}
	}
	else{
		iShowImage(0, 0, 1000, 600, bgImage2);

		for (int i = 0; i < 4; i++) {
			int w = imgW, h = imgH;

			// Apply scaling only to the hovered image
			if (hoveredImg == i) {
				w = imgW * hoverScale;
				h = imgH * hoverScale;
				// Center the scaled image
				iShowImage(imgX[i] - (w - imgW) / 2, imgY[i] - (h - imgH) / 2, w, h, img[i]);
			}
			else {
				// Draw normal size
				iShowImage(imgX[i], imgY[i], imgW, imgH, img[i]);
			}
		}
		if (startPage == 1){ //Activates start button
			drawStartPage();
		}
		if (startPage == 2){ //Shos 2nd page of start button
			iSetColor(0, 0, 0);
			iFilledRectangle(0, 0, 1000, 600);
			iShowBMP2(0, 0, "Image//story2.bmp", 0);
		}
	}
}

void iMouseMove(int mx, int my)
{

}

void iPassiveMouseMove(int mx, int my)
{
	if (currentPage == 2) {
		hoveredImg = -1;  // Reset hover state

		// Check which image is hovered
		for (int i = 0; i < 4; i++) {
			if (mx >= imgX[i] && mx <= imgX[i] + imgW &&my >= imgY[i] && my <= imgY[i] + imgH) {
				hoveredImg = i;
				break;
			}
		}
	}
}

void iMouse(int button, int state, int mx, int my)
{
	
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (currentPage == 2 && (mx >= imgX[0] && mx <= imgX[0] + imgW && my >= imgY[0] && my <= imgY[0] + imgH)) { //Click on start button
			startPage = 1;
		}

	}
	
	
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		
	}
}

// Special Keys:
// GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6, GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12, 
// GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP, GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT

void fixedUpdate()
{
	if (isKeyPressed('w') || isSpecialKeyPressed(GLUT_KEY_UP))
	{
		
	}
	if (isKeyPressed('a') || isSpecialKeyPressed(GLUT_KEY_LEFT))
	{
		
	}
	if (isKeyPressed('s') || isSpecialKeyPressed(GLUT_KEY_DOWN))
	{
		
	}
	if (isKeyPressed('d') || isSpecialKeyPressed(GLUT_KEY_RIGHT))
	{
		startPage = 2;
	}

	if (isKeyPressed('\r')) {
		currentPage = 2;
	}
}

void drawStartPage(){ //Draws first page of start button
	iSetColor(0, 0, 0);
	iFilledRectangle(0, 0, 1000, 600);
	iShowBMP2(0, 0, "Image//story1.bmp",0);
}

int main()
{
	// Set up a timer that calls blinkTimer every 750ms
		iSetTimer(750, blinkTimer);
		iInitialize(1000, 600, "Mission Dash Protocol"); // Set window size and title
		bgImage1 = iLoadImage("Image/loading/home.bmp");
		bgImage2 = iLoadImage("Image/loading/menu.bmp");
		img[0] = iLoadImage("Image/menu/start.bmp");
		img[1] = iLoadImage("Image/menu/instructions.bmp");
		img[2] = iLoadImage("Image/menu/aboutus.bmp");
		img[3] = iLoadImage("Image/menu/exit.bmp");

		iStart(); 
		return 0;
}