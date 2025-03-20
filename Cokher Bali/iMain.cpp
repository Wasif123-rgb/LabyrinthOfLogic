#include "minigame3.h"
#include "Score.h"
#include <mmsystem.h> 
#pragma comment(lib, "winmm.lib")

#include<iostream>
using namespace std;




//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::Variables:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
int window_width = 1520, height = 792;
bool musicOn = false;
int minigame = 0;
///Variables for menu
int hoverStartGame = 0, hoverCredits = 0, hoverAbout = 0, hoverScore = 0, hoverExit = 0;
int currentPage = 1, menu = 1, start = 1, credits = 0, creditsPage = 0, aboutPage = 0, scorePage = 0;
int highScore = 0;
///Variables for car 
int carX = 90, carY = 90, bg_x = 0, carImg, jungleImg;
bool showCar = true;
int puzzleSolved;

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::Variables for puzzle::::::::::::::::::::::::::::::::::::::::::::::::::::::::

int x = 650, y = 300, posX = 650, posY = 300, dx, dx2 = 0;
int recshowdet = 6;
int width = 10, width1 = 10, width2 = 20;
bool det = false, det1 = false, det2 = true, ldet = false;
int i = 0, image[7];
int lvlone[20];
int cdx, cdy;
int ldx, ldy;
int lrecdec = 0;
int z = 1, z1 = 1, z2 = 1, z3 = 1;

int shipAnim[20];
int shipindex = 0;
int obstacleTimer = 0;
bool showObstacle = false;
bool showTextBox = false;
bool puzzleActive = false;
int obstacleImage;
bool is_obstacle = false;
// Background rendering variables
int totalImageCount = 10; // Number of background images
int increment = 15;      // Increased speed of background movement (faster)

char bgp[10][20] = { "s1.jpg", "s2.jpg", "s3.jpg", "s4.jpg", "s5.jpg", "s6.jpg", "s5.jpg", "s4.jpg", "s3.jpg", "s2.jpg" }; // Background image paths
int imgPosition[10];    // Positions of background images
int bgy = 0;             // Background y-position
int bgpi = 0;            // Background image index

int levelcounter;

int healthBarX = 50, healthBarY = 950;
int obstacleHitCount = 0;
int timer = 0; // Timer for 1 minute
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::Background with spaceship:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void initializeImagePosition() {
	for (int i = 0; i < totalImageCount; i++) {
		if (!is_obstacle)
			imgPosition[i] = i * 792; // Distribute images vertically
		else
			break;
	}
}

// Function to move the background upwards
void moveBackground() {
	for (int i = 0; i < totalImageCount; i++) {
		if (!is_obstacle){
			imgPosition[i] -= increment; // Move images upwards (faster)
			if (imgPosition[i] < -792) { // Reset position if image moves out of the screen
				imgPosition[i] = 792;
			}
		}
		else
			break;
	}
}

void renderBackground() {
	for (int i = 0; i < totalImageCount; i++) {
		iShowImage(0, imgPosition[i], 1520, 792, iLoadImage(bgp[i])); // Draw each background image
	}
}

void bt(){
	is_obstacle = true;
}

void shipCode(){

	renderBackground();

	iShowImage(680, 370, 150, 200, shipAnim[shipindex]);

	if (is_obstacle){
		int  obstacleImage = iLoadImage("obr.png");
		iShowImage(610, 550, 300, 250, obstacleImage);

		iSetColor(255, 182, 193); // White background for the text box
		iFilledRectangle(550, 220, 430, 40); // Draw the text box
		iFilledRectangle(550, 170, 430, 50);

		iSetColor(0, 0, 0); // Black text
		iText(610, 190, "<<Press spacebar to continue>>", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(560, 230, "<<To remove obstacle, solve the puzzle>>", GLUT_BITMAP_TIMES_ROMAN_24);
	}
}

//::::::::::::::::::::::::::::::::::::::::::::Methods for scores, lives and updating timer:::::::::::::::::::::::::::::::::::::::::::::::::::::::


//--------------------- highscore --------------------------
//gameOver=0;

//in order to take input after a player finishes the game or dies

//will be added after a player dies or finishes the game
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::Others::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


// Draw health bar with hearts
void drawHealthBar() {
	//for (int i = 0; i < lives; i++) {
	int heart = iLoadImage("heart.jpg"); // Assuming you have an image named Heart.png
	//iShowImage(healthBarX + i * 40, healthBarY, 30, 30, heart);
	//}
	if (lives == 3 || lives == 2 || lives == 1)
	{
		iShowImage(healthBarX, healthBarY, 30, 30, heart);
	}
	if (lives == 2 || lives == 1)
	{
		iShowImage(healthBarX + 1 * 40, healthBarY, 30, 30, heart);
	}
	if (lives == 1)
	{
		iShowImage(healthBarX + 2 * 40, healthBarY, 30, 30, heart);
	}
}

// Update timer


// Handle collision with obstacle
/*void handleObstacleHit() {
if (showObstacle) { // If the obstacle is visible and the spaceship hits it
// Check if spaceship collides with obstacle
// Add your collision detection logic here

if (collisionDetected) { // Assuming a function for detecting collision
obstacleHitCount++;
lives--;  // Lose a life on each collision
if (lives <= 0) {
gameOver = true;
}
}
}
}*/

// Draw game over screen
void drawGameOver() {
	if (gameOver) {
		iSetColor(255, 0, 0);
		iText(650, 400, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(650, 210, "Press spacebar to continue", GLUT_BITMAP_TIMES_ROMAN_24);
	}
}



//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::Code for first puzzle::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void  levelZero(){

	int background = iLoadImage("bbg.jpg");
	iShowImage(0, 0, 1520, 792, background);

	if (minigame == 0)
	{
		image[0] = iLoadImage("LevelOne//GridFit.png");
		image[1] = iLoadImage("LevelOne//ObstacleFit.png");
		image[2] = iLoadImage("LevelOne//demo.png");
		image[3] = iLoadImage("LevelOne//GridStartEndFit.png");

		iShowImage(70, 90, 270, 270, image[2]);
		iShowImage(430, 65, 650, 650, image[3]);

	}
	if (ldx <= -153){
		ldet = true;
		lrecdec = 1;
	}
	if (dx >= 170){
		det = true;
		recshowdet = 7;
	}

	if (dy >= 310){
		det1 = true;
		recshowdet = 3;
		det2 = false;
	}

	if (dx2 >= 170){
		det2 = true;
		puzzleSolved++;

		minigame++;
	}
	if (lrecdec == 0 || lrecdec == 1){
		iSetColor(255, 255, 255);
		iFilledRectangle(posX, posY, width + ldx, 20);
	}

	if (lrecdec == 1){
		iSetColor(255, 255, 255);
		iFilledRectangle(505, posY, 20, 20 + ldy);
	}

	if (recshowdet == 6 || recshowdet == 7 || recshowdet == 3){
		iSetColor(255, 255, 255);
		iFilledRectangle(posX, posY, width + dx, 20);
	}

	if (recshowdet == 7 || recshowdet == 3){
		iSetColor(255, 255, 255);
		iFilledRectangle(820, posY, 20, 20 + dy);
	}

	if (recshowdet == 3){
		iSetColor(255, 255, 255);
		iFilledRectangle(820, 630, width2 + dx2, 20);
	}
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::Second puzzle:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

struct enemy{
	int posx;
	int posy;
	int width;
	int height;
	enemy(int posx, int posy, int width, int height){
		this->posx = posx;
		this->posy = posy;
		this->width = width;
		this->height = height;

	}
	void enemyimage(){

		iShowImage(posx, posy, width, height, lvlone[0]);
	}
}e1(434, 70, 160, 160), e2(594, 390, 160, 160), e3(918, 229, 160, 160), e4(761, 397, 160, 160);

struct circle{
	int posx;
	int posy;
	int width;
	int height;
	circle(int posx, int posy, int width, int height){
		this->posx = posx;
		this->posy = posy;
		this->width = width;
		this->height = height;
	}

	void circleshowImage(){
		iSetColor(255, 255, 255);
		iFilledCircle(c1.posx, c1.posy, 50, 100);
	}
	void pseudoImage(){
		iSetColor(255, 0, 0);
		iFilledRectangle(posx, posy, width, height);
	}

}c1(400, 300, 50, 50), c1rec(350, 250, 100, 100);


bool collider(circle obj1, enemy obj2){
	if (obj1.posx < (obj2.posx + obj2.width) && (obj1.posx + obj1.width) > obj2.posx &&  obj1.posy < (obj2.posy + obj2.height) && (obj1.posy + obj1.height) > obj2.posy) {
		return true;
	}
	else {
		return false;
	}
}

void enemymove(enemy& obj, int max, int min, int &dir, bool move) {
	if (move){
		obj.posx = obj.posx + (10 * dir);

		if (obj.posx >= max) {
			dir = -1;
		}
		if (obj.posx <= min) {
			dir = 1;
		}
	}

	else if (!move){
		obj.posy = obj.posy + (10 * dir);

		if (obj.posy >= max) {
			dir = -1;
		}
		if (obj.posy <= min) {
			dir = 1;
		}
	}
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::Code for second puzzle::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void levelOne(){
	lvlone[0] = iLoadImage("LevelOne//Bombb.png");
	lvlone[1] = iLoadImage("LevelOne//block.png");
	lvlone[2] = iLoadImage("LevelOne//GridFit.png");

	int background = iLoadImage("bbg.jpg");
	iShowImage(0, 0, 1520, 792, background);
	iShowImage(430, 65, 650, 650, lvlone[2]);
	e1.enemyimage();
	e2.enemyimage();
	e3.enemyimage();
	e4.enemyimage();
	//c1rec.pseudoImage();
	c1.circleshowImage();
	if (collider(c1rec, e1) || collider(c1rec, e2) || collider(c1rec, e3) || collider(c1rec, e4)){
		sadSound // Opens the button sound file 
			mciSendString("play sadsound from 0", NULL, 0, NULL); // Plays once
		c1.posx = 650;
		c1.posy = 300;
		c1rec.posx = 600;
		c1rec.posy = 250;
		obstacleHitCount++;
		lives--;  // Lose a life on each collision
		if (lives <= 0) {
			gameOver = 1;
		}
	}
	else if (c1.posx >= 970 && c1.posx <= 1020 && c1.posy >= 605 && c1.posy <= 655)
	{
		minigame = 2;
	}
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::Third Puzzle::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
int foundObject = 0, showX = 0, showMan = 1;

struct man{
	int posx;
	int posy;
	int width;
	int height;
	man(int posx, int posy, int width, int height){
		this->posx = posx;
		this->posy = posy;
		this->width = width;
		this->height = height;
	}

	void manShowImage(){
		int man = iLoadImage("ass man.png");
		iShowImage(m1.posx, m1.posy, 145, 145, man);
		//iSetColor(255,255,255);
		//iFilledCircle(posx,posy,50,100);
	}
	void pseudoImage(){
		iSetColor(112, 146, 190);
		iFilledRectangle(posx, posy, width, height);
	}

}m1(600, 240, 160, 160), m1rec(550, 140, 145, 145);

int posObject = 0;
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::Code for third puzzle::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/*void levelTwo(){
int background = iLoadImage("bbg.jpg");
iShowImage(0, 0, 1520, 792, background);

lvlone[2] = iLoadImage("LevelOne//GridFit.png");
iShowImage(430, 65, 650, 650, lvlone[2]);

int man = iLoadImage("astronomer.jpg");
iShowImage(430, 65, 40, 60, man);

}*/
void  levelTwo(){

	int background = iLoadImage("bbg.jpg");
	iShowImage(0, 0, 1520, 792, background);

	//int man = iLoadImage("astronomer.jpg");
	//iShowImage(350, 250, 120, 120, man);

	int life = iLoadImage("gameLife.png");
	iShowImage(40, 700, 100, 100, life);

	int halfLife = iLoadImage("half life.png");
	iShowImage(80, 700, 100, 100, halfLife);

	//if(minigame==2)
	{
		image[0] = iLoadImage("LevelOne//GridFit.png");
		image[1] = iLoadImage("LevelOne//ObstacleFit.png");
		image[2] = iLoadImage("LevelOne//gridFit.png");
		image[3] = iLoadImage("LevelOne//GridStartEndFit.png");

		iShowImage(430, 65, 650, 650, image[0]);
		//m1.pseudoImage();
		//if(showMan && !showX && !foundObject)
		if (showMan)
			m1.manShowImage();
	}
	if (lives == 0)
		gameOver = 1;
}

void fuel()
{
	int fuel = iLoadImage("fuel pic.png");
	iShowImage(760, 390, 160, 160, fuel);
}

void xMark()
{
	int xMark = iLoadImage("x mark.png");
	iShowImage(m1.posx, m1.posy, 150, 150, xMark);
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::Last puzzle:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::Code for menu and pages:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void showMenu(){
	int background = iLoadImage("bg.jpg");
	iShowImage(0, 0, 1520, 792, background);

	int start = iLoadImage("Menu//start.png");
	iShowImage(400, 450, 200, 60, start);

	int credits = iLoadImage("Menu//credits.png");
	iShowImage(400, 360, 200, 60, credits);

	int about = iLoadImage("Menu//about.png");
	iShowImage(400, 270, 200, 60, about);

	int scores = iLoadImage("Menu//score.png");
	iShowImage(400, 180, 200, 60, scores);

	int exit = iLoadImage("Menu//exit.png");
	iShowImage(400, 90, 200, 60, exit);

	// Hover images

	if (hoverStartGame) {
		int hStart = iLoadImage("Menu//hstart.png");
		iShowImage(400, 450, 200, 60, hStart);
	}
	if (hoverCredits) {
		int hCredits = iLoadImage("Menu//hcredits.png");
		iShowImage(400, 360, 200, 60, hCredits);
	}
	if (hoverAbout) {
		int hAbout = iLoadImage("Menu//habout.png");
		iShowImage(400, 270, 200, 60, hAbout);
	}
	if (hoverScore) {
		int hScore = iLoadImage("Menu//hscore.png");
		iShowImage(400, 180, 200, 60, hScore);
	}
	if (hoverExit) {
		int hExit = iLoadImage("Menu//hexit.png");
		iShowImage(400, 90, 200, 60, hExit);
	}
}
void showCredits(){
	int creditsPage = iLoadImage("Menu//creditsPg.jpg");
	iShowImage(0, 0, 1520, 792, creditsPage);
	int back = iLoadImage("back.png");
	iShowImage(1150, 50, 200, 60, back);
}
void showAboutPage(){
	int aboutPg = iLoadImage("Menu//aboutPg.jpg");
	iShowImage(0, 0, 1520, 792, aboutPg);
	int back = iLoadImage("back.png");
	iShowImage(1150, 50, 200, 60, back);
}
void showScorePage() {
	int background = iLoadImage("Menu//Wallpapers.jpeg");
	iShowImage(0, 0, 1520, 792, background);

	drawTextBox();
	newHighscore();
	/*
	// Display the high score
	iSetColor(255, 255, 255);
	iText(650, 350, "High Score:", GLUT_BITMAP_TIMES_ROMAN_24);
	char scoreText[50];
	sprintf_s(scoreText, sizeof(scoreText), " %d", highScore);
	iText(770, 350, scoreText, GLUT_BITMAP_TIMES_ROMAN_24);
	*/
	int back = iLoadImage("back.png");
	iShowImage(1150, 50, 200, 60, back);
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::iDraw:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void iDraw()
{
	iClear();
	if (!gameOver) {
		// Draw game components
		drawHealthBar();
		updateTimer();
		//handleObstacleHit();
	}

	if (currentPage){
		if (menu){
			showMenu();
		}
		else if (start){
			if (showCar)
				shipCode();

			else if (!showCar && minigame == 0){

				levelZero();
				drawHealthBar();

				if (puzzleSolved == 1){
					score += 40;
					if (score > highScore)
						highScore = score;
					showCar = true;
					is_obstacle = false;
					minigame = 1;
					puzzleSolved = 0;
				}
				else if (lives == 0)
				{
					drawGameOver();
				}
			}

			else if (!showCar && minigame == 1){
				iSetColor(255, 255, 255);
				iFilledCircle(650, 300, 50, 50);
				levelOne();
				drawHealthBar();
				if (puzzleSolved == 1){
					score += 40;
					if (score > highScore)
						highScore = score;
					showCar = true;
					is_obstacle = false;
					minigame = 2;
				}
			}

			else if (!showCar && minigame == 2){
				levelTwo();

				if (foundObject == 1)
				{
					for (int i = 0; i<100; i++){
						showMan = 0;
						showX = 0;
						fuel();
					}

				}
				else if (foundObject == 0)
				{
					for (int i = 0; i<100; i++){
						showMan = 0;
						showX = 1;
						xMark();
					}
				}
				foundObject = 2;
				showX = 0;
				showMan = 1;
				drawHealthBar();

				if (puzzleSolved == 1){
					score += 40;
					if (score > highScore)
						highScore = score;
					showCar = true;
					is_obstacle = false;
					minigame = 3;
				}
			}
			else if (!showCar && minigame == 3){
				drawMinigame3();
			}
		}
		else if (credits){
			showCredits();
		}
		else if (aboutPage){
			showAboutPage();
		}
		else if (scorePage){
			showScorePage();
		}
	}
	if (m1.posx >= 760 && m1.posx <= 920 && m1.posy >= 390 && m1.posy <= 550 && minigame == 2){
		posObject = 1;
	}
	//cout<<minigame<<endl;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::Mouse movements (hover on buttons and menu + pages):::::::::::::::::::::::::::::::::::::::::::::::::::::::
void iPassiveMouseMove(int mx, int my)
{
	hoverStartGame = (mx >= 400 && mx <= 600) && (my >= 450 && my <= 510);
	hoverCredits = (mx >= 400 && mx <= 600) && (my >= 360 && my <= 420);
	hoverAbout = (mx >= 400 && mx <= 600) && (my >= 270 && my <= 330);
	hoverScore = (mx >= 400 && mx <= 600) && (my >= 180 && my <= 240);
	hoverExit = (mx >= 400 && mx <= 600) && (my >= 90 && my <= 150);
}

void iMouseMove(int mx, int my)
{
	cout << "mx:" << mx << endl;
	cout << "my:" << my << endl;
	cout << "ldy:" << ldy << endl;
	if (gameOver && minigame == 3){
		iMouseMove_minigame3Control(mx, my);
	}
}

void iMouse(int button, int state, int mx, int my)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		printf("%d %d\n", mx, my);
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && (mx >= 400 && mx <= 600) && (my >= 450 && my <= 510))
	{
		//PlaySound("buttonSound.wav", NULL, SND_ASYNC);
		buttonMusic // Opens the button sound file 
			mciSendString("play buttonSound from 0", NULL, 0, NULL); // Plays once

		start = 1;
		menu = 0; credits = 0; aboutPage = 0; scorePage = 0;
	}

	else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && (mx >= 400 && mx <= 600) && (my >= 360 && my <= 420)){
		//PlaySound("buttonSound.wav", NULL, SND_ASYNC);
		buttonMusic // Opens the button sound file 
			mciSendString("play buttonSound from 0", NULL, 0, NULL); // Plays once

		credits = 1;
		menu = 0; start = 0; aboutPage = 0; scorePage = 0;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && (mx >= 400 && mx <= 600) && (my >= 270 && my <= 330)){
		//PlaySound("buttonSound.wav", NULL, SND_ASYNC);
		buttonMusic // Opens the button sound file 
			mciSendString("play buttonSound from 0", NULL, 0, NULL);
		aboutPage = 1;
		menu = 0; start = 0; credits = 0; scorePage = 0;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && (mx >= 400 && mx <= 600) && (my >= 180 && my <= 240)){
		//PlaySound("buttonSound.wav", NULL, SND_ASYNC);
		buttonMusic // Opens the button sound file 
			mciSendString("play buttonSound from 0", NULL, 0, NULL);
		scorePage = 1;
		menu = 0; start = 0; credits = 0; aboutPage = 0;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && (mx >= 1150 && mx <= 1350) && (my >= 50 && my <= 110) && menu == 0 && start == 0){
		//PlaySound("buttonSound.wav", NULL, SND_ASYNC);
		buttonMusic // Opens the button sound file 
			mciSendString("play buttonSound from 0", NULL, 0, NULL);
		menu = 1;
		start = 0; credits = 0; aboutPage = 0; scorePage = 0;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && (mx >= 400 && mx <= 600) && (my >= 90 && my <= 150)){
		exit(0);
	}

	if (minigame == 3 && gameOver)
	{
		iMouse_minigame3Control(button, state, mx, my);
	}
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::Keyboard movements (for puzzle):::::::::::::::::::::::::::::::::::::::::::::::::::::::

/*function iKeyboard() is called whenever the user hits a key in keyboard. key- holds the ASCII value of the key pressed.*/
void iKeyboard(unsigned char key)
{
	if (key == 13 && gameOver) { // Press Enter to reset game after game over
		// Reset values and start a new game
		lives = 3;
		gameOver = 0;
		timer = 0;
		obstacleHitCount = 0;
	}
	if (key == 32)
	{
		bg_x -= 40;
		if (bg_x <= -1520) {
			bg_x = 0;
		}
	}

	if (key == ' '){
		if (is_obstacle){
			showCar = false;

		}
	}
	if (key == 32 && minigame == 2)
	{
		if (posObject)
		{
			//showObject
			foundObject = 1;
			puzzleSolved = 1;
		}
		else
		{
			foundObject = 0;
			puzzleSolved = 0;
			lives--;
		}
	}
}

/*function iSpecialKeyboard() is called whenver user hits special keys like-function keys, home, end, pg up, pg down, arraows etc.
you have to use appropriate constants to detect them. A list is:GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,GLUT_KEY_F7,
GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12, GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,GLUT_KEY_PAGE DOWN,
GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT*/

void iSpecialKeyboard(unsigned char key)
{
	if (minigame == 2){
		if (key == GLUT_KEY_LEFT)
		{
			m1.posx -= 160;

		}
		if (key == GLUT_KEY_RIGHT)
		{
			m1.posx += 160;

		}
		if (key == GLUT_KEY_UP)
		{
			m1.posy += 160;
		}
		if (key == GLUT_KEY_DOWN)
		{
			m1.posy -= 160;
		}
	}
	if (key == GLUT_KEY_RIGHT)
	{
		if (ldx >= 0){
			if (!det){
				dx += 20;
			}
			if (!det2){
				dx2 += 20;
			}
		}

		c1.posx += 20;
		c1rec.posx += 20;

	}

	if (key == GLUT_KEY_LEFT)
	{
		if (minigame == 1){
			c1.posx -= 20;
			c1rec.posx -= 20;
		}
	}

	if (key == GLUT_KEY_UP)
	{
		if (ldx >= 0){
			if (!det1){
				dy += 20;
			}
		}

		ldy += 20;
		if (minigame == 1){
			c1.posy += 20;
			c1rec.posy += 20;
		}
	}
	if (key == GLUT_KEY_DOWN)
	{
		c1.posy -= 20;
		c1rec.posy -= 20;
	}
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::move obstacles:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void move(){

	enemymove(e1, 900, 434, z, true);
	enemymove(e2, 900, 434, z1, true);
	enemymove(e3, 400, 100, z2, false);
	enemymove(e4, 600, 100, z3, false);
}
void shipAnimImage() {
	shipAnim[0] = iLoadImage("Move1.png");
	shipAnim[1] = iLoadImage("Move2.png");
	shipAnim[2] = iLoadImage("Move3.png");
	shipAnim[3] = iLoadImage("Move4.png");
	shipAnim[4] = iLoadImage("Move5.png");
}

void shipmove(){
	shipindex++;
	if (shipindex >= 5){
		shipindex = 0;
	}
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::Main function::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

int main()
{	//drawHealthBar();
	if (musicOn){
		//PlaySound("gameMusic.wav", NULL, SND_LOOP | SND_ASYNC);
		bgMusic // Opens the background music file 
			mciSendString("play bgmusic repeat", NULL, 0, NULL);
	}
	initializeImagePosition();
	moveBackground();
	renderBackground();

	iSetTimer(50, move);
	iSetTimer(50, shipmove);
	iSetTimer(1, moveBackground);
	iSetTimer(100, bt);

	if (minigame == 3)
	{
		int xOffset = 400, yOffset = 200;
		int index = 0;

		for (int i = 0; i < GRID_SIZE; i++) {
			for (int j = 0; j < GRID_SIZE; j++) {
				pieces[index].x = xOffset + j * TILE_SIZE;
				pieces[index].y = yOffset + i * TILE_SIZE;
				pieces[index].correctX = pieces[index].x;
				pieces[index].correctY = pieces[index].y;
				pieces[index].id = index + 1;
				index++;
			}
		}

		printPiecePositions();
		shufflePieces();

		iSetTimer(1000, updateTimer);
	}
	iInitialize(window_width, height, "Labyrinth of Logic");

	shipAnimImage();

	iStart();

	return 0;
}
