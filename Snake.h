#pragma once

#define WALL "■"
#define FOOD "★"


#define INIT_X 20
#define INIT_Y 20

typedef struct SnakeNode
{
	int x;
	int y;
	struct SnakeNode* next;
}SnakeNode,*pSnakeNode;

enum  Direction
{
	UP = 1,
	DOWN,
	LEFT,
	RIGHT
};

enum GameStatus
{
	OK,
	NOMAL,
	KILL_WALl,
	KILL_SELF
};

typedef struct Snake
{
	pSnakeNode _pSnake;
	pSnakeNode _pFood;
	int _totalScore;
	int _AddScore;
	int _Sleeptime;
	enum Direction _Dir;
	enum GameStatus _Sta;

}Snake,*pSnake;


void GameStart(pSnake ps);
void WelcomeToGame();
void CreateFood(pSnake ps);
void CreateMap();					//设置地图
void InitSnake(pSnake ps);


void GameRun(pSnake ps);
void SnakeMove(pSnake ps);
void GameEnd(pSnake ps);
int KillByWall(pSnake ps);
int KillBySelf(pSnake ps);
