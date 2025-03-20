#define SCREEN_WIDTH 1520
#define SCREEN_HEIGHT 792
#define GRID_SIZE 3  
#define TILE_SIZE 150
#define buttonMusic mciSendString("open \"buttonSound.wav\" type waveaudio alias buttonSound", NULL, 0, NULL); 
#define sadSound mciSendString("open \"gameOver.wav\" type waveaudio alias sadsound", NULL, 0, NULL); 
#define bgMusic mciSendString("open \"gameMusic.wav\" type mpegvideo alias bgmusic", NULL, 0, NULL);

#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
void drawTextBox();
void newHighscore();

int dy = 1;
int score = 0;
int hp = 10;
char scoreText[100];

int t; // temp score for s
char n[30];//temp name for s
int lives = 3;
void iDrawScoreAndLives() {
	char scoreText[20], livesText[20];
	sprintf_s(scoreText, "Score: %d", score);
	sprintf_s(livesText, "Lives: %d", lives);
	iText(650, 550, scoreText);
	iText(650, 520, livesText);
}


void showScore(){

	iSetColor(255, 255, 255);


	sprintf_s(scoreText, sizeof(scoreText), "Score: %d", score);
	iText(20, SCREEN_HEIGHT - 50, scoreText, GLUT_BITMAP_TIMES_ROMAN_24);


	if (score >= 0 && score<70) dy = 1;
	if (score >= 70 && score<140) dy = 2;
	if (score >= 140 && score<180) dy = 3;

}

//--------------------- highscore --------------------------
int len = 0;
char str1[100];
bool newscore = true;

struct hscore{
	char name[30];
	int score;
}high_score[5];


//in order to take input after a player finishes the game or dies
void drawTextBox()
{
	iSetColor(255, 255, 255);
	iText(700, 200, scoreText, GLUT_BITMAP_TIMES_ROMAN_24);
	iText(300, 220, "Enter name:", GLUT_BITMAP_TIMES_ROMAN_24);
	iSetColor(150, 140, 150);
	iRectangle(300, 190, 180, 25);
	iSetColor(255, 255, 255);
	iText(320, 195, str1, GLUT_BITMAP_TIMES_ROMAN_24);
}
//will be added after a player dies or finishes the game
void newHighscore(){
	FILE *fp;
	fopen_s(&fp, "Score.txt", "r");
	for (int i = 0; i < 5; i++){
		fscanf_s(fp, "%s %d", high_score[i].name, (unsigned)_countof(high_score[i].name), &high_score[i].score);
	}
	//might need to add a bool
	if (newscore) {
		printf("%d\n", score);
		for (int i = 0; i < 5; i++) {
			if (high_score[i].score < score) {
				high_score[4].score = score;
				//***
				strcpy_s(high_score[4].name, sizeof(high_score[4].name), str1);

				for (int j = 0; j < 5; j++) { // Fix the condition here
					for (int k = 5; k > j; k--) {
						if (high_score[k].score > high_score[k - 1].score) {
							// Sorting logic
							int t = high_score[k - 1].score;
							high_score[k - 1].score = high_score[k].score;
							high_score[k].score = t;

							char n[50]; // Declare n here

							//***
							strcpy_s(n, sizeof(n), high_score[k - 1].name);
							n[sizeof(n)-1] = '\0';  // Ensure null-termination
							strcpy_s(high_score[k - 1].name, sizeof(high_score[k - 1].name), high_score[k].name);
							strcpy_s(high_score[k].name, sizeof(high_score[k].name), n);
						}
					}
				}

				// Write the sorted scores to the file
				FILE* fp;
				fopen_s(&fp, "Score.txt", "w");
				for (int i = 0; i < 5; i++) {
					fprintf(fp, "%s %d\n", high_score[i].name, high_score[i].score);
				}
				fclose(fp);

				newscore = false; // Reset newscore
				break; // Exit the loop since sorting is done
			}
		}
	}
}