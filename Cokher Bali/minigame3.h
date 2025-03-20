#include "iGraphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 1520
#define SCREEN_HEIGHT 792
#define GRID_SIZE 3  
#define TILE_SIZE 150



int isPuzzleSolved();

typedef struct {
	int x, y;
	int correctX, correctY;
	int id;
} PuzzlePiece;

PuzzlePiece pieces[GRID_SIZE * GRID_SIZE];
int selectedPiece = -1;
int timeLeft = 120;
int gameOver = 0;

void updateTimer() {
	if (gameOver) {
		if (timeLeft > 0) {
			timeLeft--;


			if (isPuzzleSolved()) {
				gameOver = 1;
				printf("YOU WON!\n");
				iPauseTimer(0);
				return;
			}
		}

		if (timeLeft == 0) {
			gameOver = 1;
			printf("GAME OVER!\n");
			iPauseTimer(0);
		}
	}

}

void printPiecePositions() {
	for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++) {
		printf("Piece %d: (%d, %d)\n", pieces[i].id, pieces[i].x, pieces[i].y);
	}
}

void swapPieces(int i, int j) {
	PuzzlePiece temp = pieces[i];
	pieces[i] = pieces[j];
	pieces[j] = temp;
	printf("Swapped %d and %d\n", pieces[i].id, pieces[j].id);
}

void shufflePieces() {
	srand(time(0));
	for (int i = GRID_SIZE * GRID_SIZE - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		swapPieces(i, j);
	}

	for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++) {
		pieces[i].x = 400 + (i % GRID_SIZE) * TILE_SIZE;
		pieces[i].y = 200 + (i / GRID_SIZE) * TILE_SIZE;
	}

	printf("Shuffled Order: ");
	for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++) {
		printf("%d ", pieces[i].id);
	}
	printf("\n");
	printPiecePositions();
}

void drawPuzzle() {
	for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++) {
		iSetColor(255, 255, 255);
		iRectangle(pieces[i].x, pieces[i].y, TILE_SIZE, TILE_SIZE);

		char label[5];
		sprintf_s(label, sizeof(label), "%d", pieces[i].id);
		iText(pieces[i].x + 60, pieces[i].y + 60, label, GLUT_BITMAP_HELVETICA_18);
	}
}

void drawMinigame3() {

	if (gameOver) {
		if (isPuzzleSolved()) {
			iSetColor(0, 255, 0);
			iText(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2, "YOU WIN!", GLUT_BITMAP_HELVETICA_18);
		}
		else {
			iSetColor(255, 0, 0);
			iText(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2, "GAME OVER!", GLUT_BITMAP_HELVETICA_18);
		}
		return;
	}

	iSetColor(255, 255, 0);
	iText(20, SCREEN_HEIGHT - 30, "Reconstruct the Star Map!", GLUT_BITMAP_HELVETICA_18);
	drawPuzzle();

	char timerText[20];
	sprintf_s(timerText, sizeof(timerText), "Fuel Left: %d", timeLeft);
	iText(SCREEN_WIDTH - 200, SCREEN_HEIGHT - 30, timerText, GLUT_BITMAP_HELVETICA_18);
}

void iMouseMove_minigame3Control(int mx, int my) {
	if (selectedPiece != -1) {
		pieces[selectedPiece].x = mx - TILE_SIZE / 2;
		pieces[selectedPiece].y = my - TILE_SIZE / 2;
	}
}


void iMouse_minigame3Control(int button, int state, int mx, int my) {
	if (gameOver) {
		if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
			printf("Resetting puzzle...\n");
			shufflePieces();
			timeLeft = 120;
			gameOver = 0;
			iResumeTimer(0);
		}
		return;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++) {
			if (mx >= pieces[i].x && mx <= pieces[i].x + TILE_SIZE &&
				my >= pieces[i].y && my <= pieces[i].y + TILE_SIZE) {
				selectedPiece = i;
				break;
			}
		}
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		if (selectedPiece != -1) {
			int gridX = (pieces[selectedPiece].x - 400 + TILE_SIZE / 2) / TILE_SIZE;
			int gridY = (pieces[selectedPiece].y - 200 + TILE_SIZE / 2) / TILE_SIZE;

			if (gridX < 0) gridX = 0;
			if (gridX >= GRID_SIZE) gridX = GRID_SIZE - 1;
			if (gridY < 0) gridY = 0;
			if (gridY >= GRID_SIZE) gridY = GRID_SIZE - 1;

			pieces[selectedPiece].x = 400 + gridX * TILE_SIZE;
			pieces[selectedPiece].y = 200 + gridY * TILE_SIZE;

			selectedPiece = -1;
			printPiecePositions();

			if (isPuzzleSolved()) {
				gameOver = 1;
				printf("YOU WON!\n");
				iPauseTimer(0);
			}
		}
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		printf("Right-click detected! Resetting puzzle...\n");
		shufflePieces();
		timeLeft = 120;
		gameOver = 0;
		iResumeTimer(0);
	}
}

int isPuzzleSolved() {

	int grid[GRID_SIZE][GRID_SIZE];


	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			grid[i][j] = -1;
		}
	}

	for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++) {
		int gridX = (pieces[i].x - 400) / TILE_SIZE;
		int gridY = (pieces[i].y - 200) / TILE_SIZE;

		if (gridX >= 0 && gridX < GRID_SIZE && gridY >= 0 && gridY < GRID_SIZE) {
			grid[gridY][gridX] = pieces[i].id;
		}
		else {
			return 0;
		}
	}


	int expectedID = 1;
	for (int y = GRID_SIZE - 1; y >= 0; y--) {
		for (int x = 0; x < GRID_SIZE; x++) {
			if (grid[y][x] != expectedID) {
				return 0;
			}
			expectedID++;
		}
	}

	return 1;
}
