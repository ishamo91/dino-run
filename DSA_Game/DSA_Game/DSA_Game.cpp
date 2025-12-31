#include<iostream>
#include<conio.h>	
#include<Windows.h>
#include<ctime>
#include<vector>

using namespace std;

void setCursor(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

class Dino {
private:
	int y;
	bool isJumping;
	bool isBottom;
	int jumpHeight;
	int jumpCounter;

public:
	Dino(){
		y = 0;
		isJumping = false;
		isBottom = true;
		jumpHeight = 5;
		jumpCounter = 0;
	}

	void Jump() {
		if (isBottom) {
			isJumping = true;
			isBottom = false;
		}
	}

	void Update() {
		if (isJumping) {
			y++;
			jumpCounter++;
			if (jumpCounter >= jumpHeight) {
				isJumping = false;
			}
		}
		else if (!isBottom) {
			y--;
			jumpCounter--;
			if (jumpCounter <= 0) {
				isBottom = true;
				y = 0;
			}
		}
	}

	int getY()const { return y; }
};

class Obstacle {
private:
	int x;

public:
	Obstacle(int startX) : x(startX) {}

	void move() {
		x--;
	}

	int getX()const { return x; }
};

class Game {
private:
	Dino dino;
	vector<Obstacle> obstacles;
	int width;
	int score;
	bool gameOver;

public:
	Game() : width(50), score(0), gameOver(false){}

	void Draw() {
		setCursor(0, 0);

		cout << "Score: " << score << endl;
		for (int i = 0; i < 5; i++) cout << endl; //spacing

		int groundlevel = 10;

		char buffer[12][51];

		for (int r = 0; r < 12; r++) {
			for (int c = 0; c < 50; c++) {
				buffer[r][c] = ' ';
			}
			buffer[r][50] = '\0';
		}

		for (int c = 0; c < 50; c++)buffer[10][c] = '_';
	
		int dinoRow = 9 - dino.getY();
		buffer[dinoRow][5] = 'D';

		for (auto& obs : obstacles) {
			if (obs.getX() >= 0 && obs.getX() < 50) {
				buffer[9][obs.getX()] = 'X';
			}
		}

		for (int r = 0; r < 11; r++) {
			cout << buffer[r] << endl;
		}
		
		if (gameOver)cout << "GAME OVER!" << endl;
	}

	void input() {
		if (_kbhit()) {
			char ch = _getch();
			if (ch == ' ') {
				dino.Jump();
			}
		}
	}

	void logic() {
		dino.Update();

		if (rand() % 20 == 0 && (obstacles.empty() || obstacles.back().getX() < 30)) {
			obstacles.push_back(Obstacle(49));
		}

		for (int i = 0; i < obstacles.size(); i++) {
			obstacles[i].move();

			if (obstacles[i].getX() == 5 && dino.getY() == 0) {
				gameOver = true;
			}

			if (obstacles[i].getX() < 0) {
				obstacles.erase(obstacles.begin() + i);
				score++;
				i--;
			}
		}
	}

	void run() {
		while (!gameOver) {
			input();
			logic();
			Draw();
			Sleep(50);
		}
	}
};

int main() {
	srand(time(0));
	Game dinoRun;
	dinoRun.run();
	return 0;
}