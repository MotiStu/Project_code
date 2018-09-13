#define _CRT_SECURE_NO_WARNINGS 1
#include "Snake.h"
#include <string.h>
#include <Windows.h>

void GameStart(pSnake ps)
{
	WelcomeToGame();            //欢迎界面
	CreateMap();					//设置地图
	InitSnake(ps);
	CreateFood(ps);
	ps->_Sleeptime = 200;
	ps->_totalScore = 0;
	ps->_AddScore = 10;
	ps->_Sta = OK;
	ps->_Dir = RIGHT;
}




int main()
{
	system("color 3F");
	Snake snake;
	srand((unsigned)time(NULL));
	
	GameStart(&snake);           //游戏开始
	GameRun(&snake);             //运行游戏
	GameEnd(&snake);             //游戏结束

	
	system("pause");
	return 0;
}

