// 控制台游戏.cpp : 定义控制台应用程序的入口点。
//

#include <vector>
#include <iostream>
#include <sstream>
#include <Windows.h>
#include "Game.h"

using namespace std;

Plane PLayerPlane(true);
Plane EnemyPlane[enemyPlaneCount];
GAME_STATUS gameStatus =GAME_STATUS_START;
int gameScore = 0;


char backBuffer[backBufferHeight][backBufferWidth] = {};


char ImagePlanePlayer[planeSizeY][planeSizeX] =
{
	{ '.',	'|',		'.' },
	{ '^',	'^',	'^' },
};

char ImagePlaneEnemy[planeSizeY][planeSizeX] =
{
	{ '#',	'#', 	'#' },
	{ ' ',	'~',	   ' ' },
};

char ImageBullet[bulletSizeY][bulletSizeX]=
{
	{'.'}
};

char ImageStringMadeByJige[string_MadeByJige_SizeY][string_MadeByJige_SizeX]=
{
	'M','a','d','e',' ','B','y',' ','J','I','G','E'
};

char ImageStringReady[string_Ready_SizeY][string_Ready_SizeX]=
{
	'R','e','a','d','y','.','.','.'
};

char ImageStringGO[string_Go_SizeY][string_Go_SizeX] =
{
	'G','O','!','!','!'
};

char ImageGameOverYourScore[string_GameOver_SizeY][string_GameOver_SizeX]
{
	{'G','a','m','e',' ','O','v','e','r','!','!','!'},
	{'R','e','p','l','a','y','-','S','P','A','C','E'},
	{'Y','o','u','r',' ','S','c','o','r','e',':',' '}
};


int main()
{
	system("MODE con: COLS=41 LINES=20");
	//initialize planes
	Init();

	while (gameStatus!=GAME_STATUS_EXIT)
	{

		switch (gameStatus)
		{
		case GAME_STATUS_START:
			Clear();
			UpdateGameStartAnimation();
			RenderToScreen();
			Sleep(150);
			break;

		case GAME_STATUS_PLAYING:
			//clear back buffer
			Clear();
			//deal with keyboard msg
			UpdateGameplay();

			//Render
			PLayerPlane.Render();
			for (int i = 0;i < enemyPlaneCount;i++)EnemyPlane[i].Render();
			DrawScore(0,1);

			//cout : present to scr
			RenderToScreen();

			//give some time for the image to stay on scr
			Sleep(150);
			break;


		case GAME_STATUS_GAMEOVER:
			Clear();
			DrawImage(&ImageGameOverYourScore[0][0],
				string_GameOver_SizeX, string_GameOver_SizeY,
				backBufferWidth / 2 - string_GameOver_SizeX / 2, backBufferHeight / 2  -1
				);
			DrawScore(backBufferWidth / 2 -  4, backBufferHeight / 2+3);
			//set player's position to  middle bottom
			PLayerPlane.SetPosition(backBufferWidth / 2, backBufferHeight - planeSizeY);
			PLayerPlane.Render();
			RenderToScreen();

			//press SPACE to restart
			if (KEYDOWN(VK_SPACE))
			{
				Init();
			}

			Sleep(150);
			break;

		}
	}

	return 0;
}


void Init()
{
	gameStatus=GAME_STATUS_START;
	gameScore = 0;

	PLayerPlane.SetPosition(backBufferWidth / 2, backBufferHeight - 3);
	PLayerPlane.SetImageMemAddr(&ImagePlanePlayer[0][0]);
	PLayerPlane.SetHP(3);

	for (int i = 0;i < 8;i++)
	{
		EnemyPlane[i].SetPosition(float(rand()%backBufferWidth), -2);
		EnemyPlane[i].SetImageMemAddr(&ImagePlaneEnemy[0][0]);
		EnemyPlane[i].SetHP(2);
	}

	//far away from battle field, they will come out after a while
	for (int i = 8;i < 16;i++)
	{
		EnemyPlane[i].SetPosition(float(rand() % backBufferWidth), -300);
		EnemyPlane[i].SetImageMemAddr(&ImagePlaneEnemy[0][0]);
		EnemyPlane[i].SetHP(4);
	}
}


void Clear()
{
	//clear console lines
	system("cls");

	//clear back buffer
	for (int j = 0;j < backBufferHeight;j++)
	{
		for (int i = 0;i < backBufferWidth;i++)
		{
			//clear all pixel
			backBuffer[j][i] = ' ';
		}
	}
}


void UpdateGameplay()
{

#pragma region KeyInput
	//deal with pressed keys and player
	if (KEYDOWN(VK_ESCAPE))
	{
		gameStatus = GAME_STATUS_EXIT;
	}

	if (KEYDOWN(VK_LEFT))
	{
		PLayerPlane.Move(-1.0f, 0);
	}
	if (KEYDOWN(VK_RIGHT))
	{
		PLayerPlane.Move(1.0f, 0);
	}
	if (KEYDOWN(VK_UP))
	{
		PLayerPlane.Move(0, -0.5f);
	}
	if (KEYDOWN(VK_DOWN))
	{
		PLayerPlane.Move(0, 0.5f);
	}

	if (KEYDOWN(VK_SPACE))
	{
		//traverse to see which bullet is available
		for (int i = 0;i < planeBulletCount;i++)
		{
			if (!PLayerPlane.GetBullet(i).enabled)
			{
				//generate new bullets!!!!
				PLayerPlane.SetBulletPos(i,PLayerPlane.GetPosX()+planeSizeX/2, PLayerPlane.GetPosY());
				PLayerPlane.SetBulletEnabled(true, i);
				break;
			}
		}
	}
#pragma endregion KeyInput


#pragma region UpdateBullet
	//update bullets of player / enemies
	for (int i = 0;i < enemyPlaneCount;i++)
	{
		EnemyPlane[i].UpdateBullets();
	}
	PLayerPlane.UpdateBullets();
#pragma endregion UpdateBullet


#pragma region UpdateEnemies
	//update enemies 
	for (int i = 0;i < enemyPlaneCount;i++)
	{
		EnemyPlane[i].Move(0, 0.5f);

		//if enemy hit player
		if (EnemyPlane[i].CollideWithPoint(PLayerPlane.GetPosX(), PLayerPlane.GetPosY()) ||
			EnemyPlane[i].CollideWithPoint(PLayerPlane.GetPosX() + planeSizeX, PLayerPlane.GetPosY()) ||
			EnemyPlane[i].CollideWithPoint(PLayerPlane.GetPosX() + planeSizeX / 2, PLayerPlane.GetPosY() + planeSizeY))
		{
			//GAME OVER!!!!!!!!!
			//RESET BULLETS!!!!
			for (int k = 0;k < planeBulletCount;k++)
			{
				PLayerPlane.SetBulletEnabled(false, k);
			}
			gameStatus = GAME_STATUS_GAMEOVER;
		}



		for (int j = 0;j < planeBulletCount;j++)
		{

			//if enemy was hit
			if ( EnemyPlane[i].CollideWithBullet(PLayerPlane.GetBullet(j)))
			{
				EnemyPlane[i].ReduceHP();

				//.............
				gameScore += (30 + rand() % 20);
			}

			//if enemy dies by being hit by bullets or 	 run out of windows ,re-init
			if ((EnemyPlane[i].GetHP()<=0) || (EnemyPlane[i].GetPosY() > backBufferHeight))
			{
				//set a randomized init position
				EnemyPlane[i].SetPosition(rand() % backBufferWidth, -rand() % 10);
				EnemyPlane[i].SetHP(3.0f);
			}

		}
	}
#pragma endregion UpdateEnemies


}


void UpdateGameStartAnimation()
{
	static int gameStartFrameCounter;

	++gameStartFrameCounter;

	//stage1 : draw "Made By JIGE" Title
	if (gameStartFrameCounter < 15)
	{
		DrawImage(&ImageStringMadeByJige[0][0],
			string_MadeByJige_SizeX, string_MadeByJige_SizeY,
			backBufferWidth/2 - string_MadeByJige_SizeX / 2, backBufferHeight / 2
			);
		return;
	}

	//stage2 : draw "Ready....."
	if (gameStartFrameCounter < 25)
	{
		Clear();
		DrawImage(&ImageStringReady[0][0],
			string_Ready_SizeX, string_Ready_SizeY,
			backBufferWidth/2 - string_Ready_SizeX / 2, backBufferHeight / 2
			);
		//set player's position to  middle bottom
		PLayerPlane.SetPosition(backBufferWidth / 2, backBufferHeight - planeSizeY);
		PLayerPlane.Render();
		return;
	}

	//stage 3: "Go!!"
	if (gameStartFrameCounter < 30)
	{
		DrawImage(&ImageStringGO[0][0],
			string_Go_SizeX, string_Go_SizeY,
			backBufferWidth/2 - string_Go_SizeX / 2, backBufferHeight / 2
			);
		//set player's position to  middle bottom
		PLayerPlane.SetPosition(backBufferWidth / 2, backBufferHeight - planeSizeY);
		PLayerPlane.Render();
		//re-init the timer, in case the player want to restart
		return;
	}

	gameStartFrameCounter = 0;
	//frame counter greater than 45 (frames)
	gameStatus = GAME_STATUS_PLAYING;

}


void DrawScore(int x,int y)
{
	stringstream tmpS;
	tmpS << gameScore;
	for (unsigned int i = 0;i < tmpS.str().size();i++)
	{
		backBuffer[y][x+i] = tmpS.str().at(i);
	}
}


void RenderToScreen()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	for (int j = 0;j < backBufferHeight;j++)
	{
		for (int i = 0;i < backBufferWidth;i++)
		{
			if (backBuffer[j][i] != ' ')
			{
				COORD consoleCursor;
				consoleCursor.X = i;
				consoleCursor.Y = j;
				SetConsoleCursorPosition(hOut, consoleCursor);
				//output each "pixel"
				cout << backBuffer[j][i];
			}
		}
		cout << endl;
	}
};


void DrawImage(char* image, int sizeX, int sizeY, int posX, int posY)
{
	//check if pointer is invalid (null)
	if (!image)return;

	//.......
	for (int j = 0;j < sizeY;j++)
	{
		for (int i = 0;i < sizeX;i++)
		{
			//a "pixel" must not go out of the picture
			if (valInRange(posX + i, 0, backBufferWidth - 1) && valInRange(posY + j, 0, backBufferHeight - 1))
			{
				//directly draw to back buffer
				backBuffer[posY + j][posX + i] =*(image+(j*sizeX+i));
			}
		}
	}
};


bool valInRange(int val, int leftBound, int rightBound)
{
	if(val>=leftBound && val <=rightBound)
	{
		return true;
	}
	else
	{
		return false;
	}
	return false;
}


float clamp(float val, float left, float right)
{
	//return (val >= left ? val : (val <= right ? val : right));
	if (val <= left)return left;
	if (val >= right)return right;
	return val;
}