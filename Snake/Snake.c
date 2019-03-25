#define _CRT_SECURE_NO_WARNINGS 1
#include "Snake.h"
#include <stdio.h>
#include <Windows.h>
#include <time.h> 
#include <stdlib.h>

void SetPos(int x, int y)
{
	COORD pos = { 0 };
	HANDLE hOutput = NULL;
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_ERROR_HANDLE);
	SetConsoleCursorPosition(hOutput, pos);
}


void WelcomeToGame()
{
	system("mode con cols=100 lines=35");
	SetPos(35, 17);
	printf("Welcome To The Game!!!");
	SetPos(65, 32);
	system("pause");
	system("cls");
	SetPos(35, 16);
	printf("↑↓ ← →  控制方向");
	SetPos(35, 18);
	printf("F1加速，F2减速，加速获得分更高");
	SetPos(65, 32);
	system("pause");
	system("cls");

}

void CreateMap()
{
	int i = 0;

	//up
	for (i = 0; i <= 58; i += 2)            //因为     strlen("■") == 2
	{
		SetPos(i, 0);
		printf(WALL);
	}

	//down
	for (i = 0; i <= 58; i += 2)
	{
		SetPos(i, 26);
		printf(WALL);
	}

	//left
	for (i = 0; i <= 26; i++)
	{
		SetPos(0, i);
		printf(WALL);
	}

	//right
	for (i = 0; i <= 26; i++)
	{
		SetPos(58, i);
		printf(WALL);
	}
}



SnakeNode* BuyNode()
{
	pSnakeNode NewNode = (pSnakeNode)malloc(sizeof(SnakeNode));
	if (NewNode == NULL)
	{
		return 0;
	}
	NewNode->x = 0;
	NewNode->y = 0;
	NewNode->next = NULL;
	return NewNode;
}
void InitSnake(pSnake ps)
{
	pSnakeNode first = (pSnakeNode)malloc(sizeof(SnakeNode));
	pSnakeNode cur = NULL;
	first->x = INIT_X;
	first->y = INIT_Y;
	first->next = NULL;
	for (int i = 0; i < 4; i++)
	{
		cur = BuyNode();
		cur->x = first->x + 2;
		cur->y = first->y;

		cur->next = first;        //头插
		first = cur;
	}

	while (cur)
	{
		SetPos(cur->x, cur->y);
		printf(FOOD);
		cur = cur->next;
	}
	
	ps->_pSnake = first;
}



void CreateFood(pSnake ps)
{
	pSnakeNode food = (pSnakeNode)malloc(sizeof(SnakeNode));
	pSnakeNode cur = NULL;
	
	cur = ps->_pSnake;
	food->y = rand() % 25+1;
	do
	{
		food->x = rand() % 55 + 2;
	} while (food->x % 2 != 0);

	//该循环首先会执行一次do{}之内的语句，然后在while()内检查条件是否为真，
	//如果条件为真的话，就会重复do...while这个循环,直至while()为假。


	while (cur!=NULL)            //保证food不在蛇身上
	{
		if (cur->x == food->x && cur->y == food->y)
		{
			CreateFood(ps);
			return;
		}		
		cur = cur->next;
	}

	SetPos(food->x, food->y);
	printf(FOOD);

	ps->_pFood = food;
	
}


void Pause()          //暂停
{
	while (1)
	{
		Sleep(200);
		if (GetAsyncKeyState(VK_SPACE))//当获取空格键，暂停
		{
			break;
		}
	}
}
void HighSpeed(pSnake ps)
{
	if (ps->_Sleeptime >40)
	{
		ps->_Sleeptime -= 20;
		ps->_AddScore += 2;
	}
}
void SlowSpeed(pSnake ps)
{
	if (ps->_Sleeptime < 400&&ps->_AddScore>2)
	{
		ps->_Sleeptime += 20;
		ps->_AddScore -= 2;
	}
}
void GameRun(pSnake ps)
{
	do
	{
		if (GetAsyncKeyState(VK_UP) && ps->_Dir != DOWN)
		{
			ps->_Dir = UP;
		}
		else if (GetAsyncKeyState(VK_DOWN)&&ps->_Dir!=UP)
		{
			ps->_Dir = DOWN;
		}
		else if (GetAsyncKeyState(VK_LEFT) && ps->_Dir != RIGHT)
		{
			ps->_Dir = LEFT;
		}
		else if (GetAsyncKeyState(VK_RIGHT) && ps->_Dir != LEFT)
		{
			ps->_Dir = RIGHT;
		}
		else if (GetAsyncKeyState(VK_SPACE))
		{
			Pause();
		}
		else if (GetAsyncKeyState(VK_ESCAPE))
		{
			ps->_Sta = NOMAL;
			break;
		}
		else if (GetAsyncKeyState(VK_F1))
		{
			HighSpeed(ps);
		}
		else if (GetAsyncKeyState(VK_F2))
		{
			SlowSpeed(ps);
		}
		Sleep(ps->_Sleeptime);
		SnakeMove(ps);

	} while (ps->_Sta == OK);
	
}



int NextHasFood(pSnakeNode next, pSnakeNode food)
{
	return ((next->x == food->x) && (next->y == food->y));
}

void EatFood(pSnake ps, pSnakeNode pn)
{
	pn->next = ps->_pSnake;
	ps->_pSnake = pn;

	pSnakeNode cur = ps->_pSnake;
	while (cur)
	{
		SetPos(cur->x, cur->y);
		printf(FOOD);
		cur = cur->next;
	}

	ps->_totalScore = ps->_totalScore + ps->_AddScore;
	CreateFood(ps);
}

void NoFood(pSnake ps, pSnakeNode pn)
{
	pn->next = ps->_pSnake;
	ps->_pSnake = pn;

	pSnakeNode cur = ps->_pSnake;

	while (cur->next->next)
	{
		SetPos(cur->x, cur->y);
		printf(FOOD);
		cur = cur->next;
	}

	SetPos(cur->x, cur->y);
	printf(FOOD);

	SetPos(cur->next->x, cur->next->y);
	printf(" ");

	free(cur->next);
	cur->next = NULL;

}
void SnakeMove(pSnake ps)
{
	SetPos(70, 10);
	printf("AddScore:%d ", ps->_AddScore);	
	SetPos(70, 12);
	printf("TotalScore:%d ", ps->_totalScore);
	SetPos(65, 16);
	printf("↑ ↓ ←  →  控制方向");

	pSnakeNode NextNode = BuyNode();
	switch (ps->_Dir)
	{
	case UP:
	{
			   NextNode->x = ps->_pSnake->x;
			   NextNode->y = ps->_pSnake->y-1;
			   if (NextHasFood(NextNode,ps->_pFood))
			   {
				   EatFood(ps,NextNode);
			   }
			   else
			   {
				   NoFood(ps,NextNode);
			   }
			  

	}
		break;
	case DOWN:
	{
				 NextNode->x = ps->_pSnake->x;
				 NextNode->y = ps->_pSnake->y + 1;
				 if (NextHasFood(NextNode, ps->_pFood))
				 {
					 EatFood(ps, NextNode);
				 }
				 else
				 {
					 NoFood(ps, NextNode);
				 }
	}
		break;
	case LEFT:
	{
				 NextNode->x = ps->_pSnake->x-2;
				 NextNode->y = ps->_pSnake->y;
				 if (NextHasFood(NextNode, ps->_pFood))
				 {
					 EatFood(ps, NextNode);
				 }
				 else
				 {
					 NoFood(ps, NextNode);
				 }
	}
		break;
	case RIGHT:
	{
				  NextNode->x = ps->_pSnake->x+2;
				  NextNode->y = ps->_pSnake->y;
				  if (NextHasFood(NextNode, ps->_pFood))
				  {
					  EatFood(ps, NextNode);
				  }
				  else
				  {
					  NoFood(ps, NextNode);
				  }
	}
		break;

	}
	if (KillBySelf(ps) == 1||KillByWall(ps) == 1)             //走后判断  die
	{
		GameEnd(ps);	
	}
}

int KillByWall(pSnake ps)
{
	if (ps->_pSnake->x == 0 ||
		ps->_pSnake->y == 0 ||
		ps->_pSnake->x == 58 ||
		ps->_pSnake->y == 26)
	{
		ps->_Sta = KILL_WALl;
		return 1;
	}
	return 0;
}
int KillBySelf(pSnake ps)
{
	pSnakeNode cur = ps->_pSnake->next;
	while (cur)
	{
		if (ps->_pSnake->x == cur->x && ps->_pSnake->y == cur->y)
		{
			ps->_Sta = KILL_SELF;
			return 1;
		}
		cur = cur->next;
	}
	return 0;
}



void GameEnd(pSnake ps)
{
	pSnakeNode cur = ps->_pSnake;

	SetPos(50, 32);
	switch (ps->_Sta)
	{
	case NOMAL:
	{
				  system("cls");
				  SetPos(40, 16);
				  printf("游戏结束！！！");
				  SetPos(38, 19);
	}
		break;
	case KILL_SELF:
		printf("撞上自己   GAME OVER  ");	
		break;
	case KILL_WALl:
		printf("撞墙   GAME OVER  ");		
		break;
	}

	while (cur)
	{
		pSnakeNode del = cur;
		ps->_pSnake = cur->next;
		cur = cur->next;
		free(del);
	}
}
