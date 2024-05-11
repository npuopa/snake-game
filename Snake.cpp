#include <iostream>
#include <windows.h>
#include <vector>
#include <deque>
#include <random>
#include <thread>
#include <chrono>

using namespace std;

const int w = 80, h = 20;
vector<vector<bool> > snake(h, vector<bool>(w, false));
enum Directions { STOP, LEFT, RIGHT, UP, DOWN };
Directions currDir;
bool GAME;
int iHead, jHead, iFruit, jFruit, score;
deque <pair<int, int> > Tail;
random_device rd;
uniform_int_distribution<int> irand(0, h - 1);
uniform_int_distribution<int> jrand(0, w - 1);

void ClearScreen() {
	COORD cursorPosition;
	cursorPosition.X = 0;
	cursorPosition.Y = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}

void Draw() {
	ClearScreen();
	for (int i = 0; i < w + 2; ++i) {
		cout << '#';
	}
	cout << '\n';
	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			if (j == 0) {
				cout << '#';
			}
			if(iHead == i && jHead == j) {
				cout << '0';
			} else if (snake[i][j] == true) {
				cout << 'o';
			} else if (iFruit == i && jFruit == j) {
				cout << '@';
			} else {
				cout << ' ';
			}
		}
		cout << "#\n";
	}
	for (int i = 0; i < w + 2; ++i) {
		cout << '#';
	}
	cout << "\n\nSCORE: " << score<<'\n';
}

void Input() {
	if (GetAsyncKeyState(0x57) && currDir != DOWN) {
		currDir = UP;
	} else if (GetAsyncKeyState(0x53) && currDir != UP) {
		currDir = DOWN;
	} else if (GetAsyncKeyState(0x44) && currDir != LEFT) {
		currDir = RIGHT;
	} else if (GetAsyncKeyState(0x41) && currDir != RIGHT) {
		currDir = LEFT;
	} else if (GetAsyncKeyState(0x58)) {
		currDir = STOP;
		GAME = false;
	}
}

void Logic() {
	if (currDir == STOP) return;
	Tail.push_front({ iHead, jHead });
	switch (currDir) {
		case LEFT:
			jHead--;
			break;
		case RIGHT:
			jHead++;
			break;
		case UP:
			iHead--;
			break;
		case DOWN:
			iHead++;
			break;
	}
	if(iHead == h || iHead < 0 || jHead == w || jHead < 0 || snake[iHead][jHead]) {
		GAME = false;
		return;
	}
	snake[iHead][jHead] = true;
	if(iHead == iFruit && jHead == jFruit) {
		iFruit = irand(rd);
		jFruit = jrand(rd);
		score++;
	} else {
		snake[Tail.back().first][Tail.back().second] = false;
		Tail.pop_back();
	}
}

int main() {
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode = 0;
	GetConsoleMode(hStdin, &mode);
	SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(out, &cursorInfo);
	cout << "\n\t~Snake game~\n";
	cout << "\nControls:\n\t\"W\" - Move UP\n\t\"A\" - Move LEFT\n\t\"S\" - Move DOWN\n\t\"D\" - Move RIGHT\n\t\"X\" - END GAME\n";
	cout << "\n(Press \"Enter\" to START the game)\n";
	cin.ignore();
	cin.get();
	srand(time(NULL));
	iHead = h / 2;
	jHead = w / 2;
	snake[iHead][jHead] = true;
	currDir = STOP;
	GAME = true;
	iFruit = irand(rd);
	jFruit = jrand(rd);
	score = 0;
	while (GAME) {
		Draw();
		Input();
		Logic();
		this_thread::sleep_for(chrono::milliseconds(50));
	}
	system("cls");
	cout << "\n\tGAME OVER, YOUR SCORE: " << score << '\n';
	return 0;
}