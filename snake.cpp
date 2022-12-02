#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <cmath>



using namespace std;

const int width = 20;
const int height = 20;

const int speed = 20;

int box[width][height];
int paint[width][height];

const int blank = 0;
const char s_blank = ' ';

const int border = 1;
const char s_border = '+';

const int apple = 9;
const char s_apple = 'o';


int appleCounter = 0;

bool gameOver = false;

bool gameWon = false;


HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD cursorPosition;

struct SnakeHead
{
	int posX = round(width / 2);
	int posY = round(height / 2);
	int layer = 20;
	char sprite = '#';

	int dirX = 0;
	int dirY = 0;
};



SnakeHead snakeHead;


void ResetScreen()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			box[j][i] = blank;
		}
	}


}

void InitializeSnakeHead(bool* appleChecker)
{

	int tempDirX = snakeHead.dirX;
	int tempDirY = snakeHead.dirY;
	if (snakeHead.posX + snakeHead.dirX == 0)
		tempDirX = width - 3;
	else if (snakeHead.posX + snakeHead.dirX == width - 1)
		tempDirX = -(width - 3);


	if (snakeHead.posY + snakeHead.dirY == 0)
		tempDirY = height - 3;
	else if (snakeHead.posY + snakeHead.dirY == height - 1)
		tempDirY = -(height - 3);

	snakeHead.posX += tempDirX;
	snakeHead.posY += tempDirY;

	if (!(snakeHead.dirX == 0 && snakeHead.dirY == 0))
	{
		if (box[snakeHead.posX][snakeHead.posY] > 20)
			gameOver = true;
	}

	if (box[snakeHead.posX][snakeHead.posY] == apple)
	{
		*appleChecker = false;
		appleCounter++;
	}

	box[snakeHead.posX][snakeHead.posY] = snakeHead.layer;





}




void InitializeBorder()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (j == 0 || j == width - 1 || i == 0 || i == height - 1)
				box[j][i] = border;

		}
	}
}

void ClearScreen()
{
	cursorPosition;	cursorPosition.X = 0;	cursorPosition.Y = 0;	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}

void GetInput()
{
	if (_kbhit())
	{
		char key = _getch();

		if (((key == 'd') || (key == 'D')) && snakeHead.dirX != -1)
		{
			snakeHead.dirX = 1;
			snakeHead.dirY = 0;
		}
		else if (((key == 'w') || (key == 'W')) && snakeHead.dirY != 1)
		{
			snakeHead.dirX = 0;
			snakeHead.dirY = -1;
		}
		else if (((key == 's') || (key == 'S')) && snakeHead.dirY != -1)
		{
			snakeHead.dirX = 0;
			snakeHead.dirY = 1;
		}
		else if (((key == 'a') || (key == 'A')) && snakeHead.dirX != 1)
		{
			snakeHead.dirX = -1;
			snakeHead.dirY = 0;
		}



	}
}

void SpawnApple()
{
	while (1)
	{
		int _x = 1 + rand() % (width - 2);
		int _y = 1 + rand() % (height - 2);

		if (box[_x][_y] > 15)
			continue;


		box[_x][_y] = apple;

		break;
	}




}



void PlusTool()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (box[j][i] > 19)
				box[j][i] += 1;
		}
	}
}

void OutOfSnakeLengthCheck()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (box[j][i] == snakeHead.layer + appleCounter + 4)
				box[j][i] = blank;

		}
	}
}



int k = 0;

void Update()
{
	bool appleExists = true;

	GetInput();

	PlusTool();

	InitializeSnakeHead(&appleExists);

	OutOfSnakeLengthCheck();


	if (!appleExists)
	{
		SpawnApple();
	}


}

void SetScreenClean()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			paint[j][i] = blank;
		}
	}
}


void Draw()
{

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			switch (box[j][i])
			{
			case 0:
				paint[j][i] = s_blank;
				break;
			case 1:
				paint[j][i] = s_border;
				break;
			case 20:
				paint[j][i] = snakeHead.sprite;
				break;
			case 9:
				paint[j][i] = s_apple;
				break;
			default:
				paint[j][i] = snakeHead.sprite;
			}

		}
	}

	string output = "";

	cout << " score: " << appleCounter << endl;


	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			output += paint[j][i];
			output += " ";
		}
		output += '\n';



	}

	cout << output << endl;


}


int main()
{
	srand(time(0));

	while (1)
	{

		appleCounter = 0;
		gameOver = false;
		gameWon = false;

		system("cls");

		ResetScreen();

		InitializeBorder();

		SetScreenClean();

		SpawnApple();



		snakeHead.posX = round(width / 2);
		snakeHead.posY = round(height / 2);
		snakeHead.dirX = 0;
		snakeHead.dirY = 0;



		while (1)
		{
			Update();

			Draw();

			if (gameOver)
				break;

			Sleep(1000 / speed);

			ClearScreen();

		}

		if (appleCounter == ((width - 2) * (height - 2)) - 4)
			gameWon = true;

		system("cls");

		if (!gameWon)
		{
			cout << " your score is: " << appleCounter << endl;
		}
		else
		{
			cout << "congratulations! you've beaten the game!" << endl;
		}

		cout << " press 'r' to retry, 'q' to quit " << endl;

		bool goRestart = false;

		while (1)
		{
			if (_kbhit())
			{
				char key = _getch();

				if ((key == 'q') || (key == 'Q'))
					break;
				else if ((key == 'r') || (key == 'R'))
				{
					goRestart = true;
					break;
				}

				Sleep(1000 / speed);
			}

		}

		if (!goRestart)
		{
			break;
		}

	}


	return 0;
}


