#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <cmath>
#include <fstream>
#include <string>



using namespace std;

const int width = 20;
const int height = 20;

const int speed = 14;

int box[width][height];
int paint[2 * width][2 * height];

const int blank = 0;
const char s_blank = ' ';

const int border = 1;
const char s_border = '+';

const int apple = 9;
const char s_apple = 'o';

int resolutionMultiplier = 1;


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

void gotoxy(int x, int y) {
	cursorPosition.X = x;
	cursorPosition.Y = y;
	SetConsoleCursorPosition(console, cursorPosition);
}

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

	if (!(snakeHead.dirX == 0 && snakeHead.dirY == 0) || (appleCounter == ((width - 2) * (height - 2)) - 5))
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
		else if (key == 'm')
		{
			if (resolutionMultiplier == 1)
			{
				resolutionMultiplier = 2;
				system("MODE 84,43");
			}
			else
			{
				system("cls");
				resolutionMultiplier = 1;
				system("MODE 40, 23");

			}
		}
		else if (key == 'p')
		{
			key = 0;

			if (resolutionMultiplier == 1)
				gotoxy(width / 2 + 4, height / 2 + 1);
			else
				gotoxy(width + 14, height + 1);


			cout <<"-- PAUSED --";

			while (true)
			{
				key = _getch();

				if (key == 'p')
				{
					system("cls");
					break;
				}
				else if (key == 'm')
				{
					
				}
			}
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

	for (int i = 0; i < height* resolutionMultiplier; i+= resolutionMultiplier)
	{
		for (int j = 0; j < width* resolutionMultiplier; j+= resolutionMultiplier)
		{
			char pic;

			switch (box[j/ resolutionMultiplier][i/ resolutionMultiplier])
			{
			case 0:
				pic = s_blank;
				break;
			case 1:
				pic = s_border;
				break;
			case 20:
				pic = snakeHead.sprite;
				break;
			case 9:
				pic = s_apple;
				break;
			default:
				pic = snakeHead.sprite;
			}

			paint[j][i] = pic;
			if (resolutionMultiplier == 2)
			{
				paint[j + 1][i] = pic;
				paint[j][i + 1] = pic;
				paint[j + 1][i + 1] = pic;
			}

		}
	}

	string output = "";

	if (resolutionMultiplier == 2)
	cout << "                    ";

	cout << "      --------- score: " << appleCounter << " --------" << endl;


	for (int i = 0; i < height * resolutionMultiplier; i++)
	{
		for (int j = 0; j < width * resolutionMultiplier; j++)
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
	system("MODE 140,30");
	srand(time(0));

	        cout << "\n" << endl;
	        cout << "            the   \n" << endl;
 
	/*1*/	cout << "         ####     ##     ##      ##        ##     ##     ######     " << endl;
	/*2*/	cout << "        ######    ###    ##     ####       ##    ##.   ####....#    " << endl;
	/*3*/	cout << "       ###...#   ####   ##.     ####       ##   ##..  ##........    " << endl;
	/*4*/	cout << "       ##.....   ##.#   ##.    ##..##     ##.  ##..   ##...    .    " << endl;
	/*5*/	cout << "       ##.   .   ##.#   ##     #...##     ##. ##..    ###           " << endl;
	/*6*/	cout << "       .##       #. ##  #.    ##.  ##     #####..     .######       " << endl;
	/*7*/	cout << "       ..##     ##. ## ##.    #######    ######.      .###...       " << endl;
	/*8*/	cout << "        ..#     ##  .# ##    ##....##    ##..##      ###.....       " << endl;
	/*9*/	cout << "         .##    ##  .# ##    ##....##    ##...##     ##...          " << endl;
	/*10*/	cout << "    ##   ###    #.   ###.   ##.    ##   ##.   ##    ###.            " << endl;
	/*11*/	cout << "    .######.   ##.   .##.   ##.    ##   ##.   .##   #####    #      " << endl;
	/*12*/	cout << "    ..####..   ##    .##   ##      ##   ##    .##   ..#######.      " << endl;
	/*13*/	cout << "     ......    ..     ..   ..      ..   ..     ..   ..........      " << endl;
	/*14*/	cout << "      ....     ..     ..   ..      ..   ..     ..     ......        " << endl;
	        cout << "    \n                                    -- by ege --              " << endl;
			cout << "    \n\n                  press 'spacebar' to start                 " << endl;

	while (1)
	{

		if (_kbhit())
		{
			char x = _getch();

			if(x == 32)
			break;
		}



	}


	system("MODE 40, 23");

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

		if (appleCounter == ((width - 2) * (height - 2)) - 5)
			gameWon = true;

		system("cls");

		fstream myFile;

		myFile.open("data.txt", ios::in);

		int lastScore = 0;

		if (myFile.is_open())
		{
			string line;

			getline(myFile, line);

			try
			{
				lastScore = stoi(line);
			}
			catch (exception e)
			{
				lastScore = 0;
			}
			
			myFile.close();
		}

		if (appleCounter > lastScore)
		{
			myFile.open("data.txt", ios::out);

			if (myFile.is_open())
			{
				myFile << appleCounter;
				myFile.close();
			}

			cout << "NEW HIGH SCORE!!" << endl;
		}
		else
		{
			cout << "Best Score: " << lastScore << endl;
		}




		if (!gameWon)
		{
			cout << "Your Score is: " << appleCounter << endl;
		}
		else
		{
			cout << "Congratulations! You've Beaten the Game!" << endl;
		}



		cout << "\n Press 'r' to Retry, 'q' to Quit " << endl;

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


