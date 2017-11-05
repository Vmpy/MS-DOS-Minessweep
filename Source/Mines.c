#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include <windows.h>
#include <time.h>

/*****************************控制台颜色函数宏定义*****************************/
#define RED SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED)
#define GREEN SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN)
#define BLUE SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE)
#define GREENPLUS SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN|FOREGROUND_INTENSITY)
#define REDPLUS SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_INTENSITY)
#define BLUEPLUS SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE|FOREGROUND_INTENSITY)
#define YELLO SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY)
#define INDIG SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY)
#define MagentaPlus SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_INTENSITY)
#define White SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY)
#define Other SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED&FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY)

#define MINE -1

/*Mine[MINE][MINE].Mark*/
#define Close 0
#define Open 1
#define Flags -1

/*For Display()*/
#define Block '~'
#define MineIcon '@'
#define FlagsIcon '^'

#define X 9
#define Y 9

struct mine
{
	int Num;
	int Mark;
}Mine[X][Y];

void Reset(void);
void Init(int,int);
void Play(void);
bool Not_stepping_on_the_mine(void);
int FindMineNum(int,int);
int CountFlagsNum(void); 
void Display(int,int);
void Open_large_area(int,int);
bool IfWin(void);
void End(bool);

int main(void)
{
	char Chioce;
	srand((unsigned)(time(NULL)));
	A:
	Play();
	getchar();
	printf("再来一次?(Yes[Y]/No[N]\n");
	Ch:
	Chioce = getch();
	switch(Chioce)
	{
		case 'Y':
		case 'y':
		{
			Reset();
			goto A;
		}
		case 'N':
		case 'n':
		{
			return;
		}
		default:
		{
			goto Ch;
		}
	}
	return 0;
}

void Reset(void)
{
	int x,y;
	for(x = 0;x < X;x++)
	{
		for(y = 0;y < Y;y++)
		{
			Mine[x][y].Num = 0;
		}
	}
}

void Init(int Posx,int Posy)
{
	int x,y,tmp;
	for(x = 0;x < X;x++)
	{
		for(y = 0;y < Y;y++)
		{
			Mine[x][y].Mark = Close;
			Mine[x][y].Num = 0;
		} 
	} 
	tmp = 0;
	while(tmp < 10)
	{
		Again:
		x = rand()%X;
		y = rand()%Y;
		if(x == Posx && y == Posy)
		{
			goto Again;
		} 
		if(Mine[x][y].Num != MINE)
		{
			Mine[x][y].Num = MINE;
		}
		else
		{
			goto Again; 
		} 
		tmp++;
	}
	for(x = 0;x < X;x++)
	{
		for(y = 0;y < Y;y++)
		{
			if(Mine[x][y].Num != MINE)
			{
				Mine[x][y].Num = FindMineNum(x,y);							//该元素周围存在多少个地雷. 
			}
		}
	}
}

int FindMineNum(int x,int y)
{
	int Count = 0;
	if(x-1 >= 0 && y-1 >= 0)
	{
		if(Mine[x-1][y-1].Num == MINE)
		{
			Count++; 
		}
	}
	if(x-1 >= 0)
	{
		if(Mine[x-1][y].Num == MINE)
		{
			Count++; 
		}
	}
	if(y-1 >= 0)
	{
		if(Mine[x][y-1].Num == MINE)
		{
			Count++; 
		}
	}
	if(x+1 < X && y-1 >= 0)
	{
		if(Mine[x+1][y-1].Num == MINE)
		{
			Count++; 
		}
	}
	if(x+1 < X)
	{
		if(Mine[x+1][y].Num == MINE)
		{
			Count++; 
		}
	}
	if(x-1 >= 0 && y+1 < Y)
	{
		if(Mine[x-1][y+1].Num == MINE)
		{
			Count++; 
		}
	}
	if(y+1 < Y)
	{
		if(Mine[x][y+1].Num == MINE)
		{
			Count++; 
		}
	}
	if(x+1 < X && y+1 < Y)
	{
		if(Mine[x+1][y+1].Num == MINE)
		{
			Count++; 
		}
	}
	return Count;
}

void Play(void)
{
	int Posx,Posy;
	char ch;
	bool bFirst = false;
	Posx = Posy = 0;
	bool bWin = false;
	
	while(Not_stepping_on_the_mine())
	{
		system("cls");
		Display(Posx,Posy);
		ch = getch();
		switch(ch)
		{
			case 'w':
			case 'W':
			{
				Posy--;
				Posy %= Y;
				break;
			}
			case 's':
			case 'S':
			{
				Posy++;
				Posy %= Y;
				break;
			}
			case 'a':
			case 'A':
			{
				Posx--;
				Posx %= X;
				break;
			}
			case 'd':
			case 'D':
			{
				Posx++;
				Posx %= X;
				break;
			}
			case 'o':
			case 'O':
			{
				/*当该区域处于封闭状态，才可以打开*/
				if(Mine[Posx][Posy].Mark == Close)
				{
					if(!bFirst)
					{
						Init(Posx,Posy);
						bFirst = true;
						Mine[Posx][Posy].Mark = Open;
						if(Mine[Posx][Posy].Num == 0)
						{
							Open_large_area(Posx,Posy);
						}
					}
					else
					{
						Mine[Posx][Posy].Mark = Open;
						if(Mine[Posx][Posy].Num == 0)
						{
							Open_large_area(Posx,Posy);
						}
					}
				}
				break;
			}
			case 'f':
			case 'F':
			{
				if(CountFlagsNum() < 10 && Mine[Posx][Posy].Mark != Flags)
				{
					if(Mine[Posx][Posy].Mark == Flags)
					{
						Mine[Posx][Posy].Mark = Close;
					}
					else if(Mine[Posx][Posy].Mark == Close)
					{
						Mine[Posx][Posy].Mark = Flags;
					}
				}
				break;
			}
			default:
			{
				break;
			}
		}
		if(IfWin())
		{
			bWin = true;
			break;
		}
	}
	
	system("cls");
	Display(Posx,Posy);
	
	End(bWin);
	return;
}

int CountFlagsNum(void)
{
	int Count = 0;
	int x,y;
	for(x = 0;x < X;x++)
	{
		for(y = 0;y < Y;y++)
		{
			if(Mine[x][y].Mark == Flags)
			{
				Count++;
			}
		}
	}
	return Count;
}

/**
*递归打开空白区域. 
*/
void Open_large_area(int x,int y)
{
	Mine[x][y].Mark = Open;
	if(Mine[x+1][y].Num == 0 && x+1 < X && Mine[x+1][y].Mark != Open)	/*所操作区域不超过Mine最大值*/
	{
		Mine[x+1][y].Mark = Open;
		Open_large_area(x+1,y);
	}
	if(Mine[x-1][y].Num == 0 && x-1 >= 0 && Mine[x-1][y].Mark != Open)	/*所操作区域不小于Mine最小值*/ 
	{
		Mine[x-1][y].Mark = Open;
		Open_large_area(x-1,y);
	}
	if(Mine[x][y+1].Num == 0 && y+1 < Y && Mine[x][y+1].Mark != Open)
	{
		Mine[x][y+1].Mark = Open;
		Open_large_area(x,y+1);
	}
	if(Mine[x][y-1].Num == 0 && y-1 >= 0 && Mine[x][y-1].Mark != Open)
	{
		Mine[x][y-1].Mark = Open;
		Open_large_area(x,y+1);
	}
}

void Display(int Posx,int Posy)
{
	int x,y;
	for(y = 0;y < Y;y++)
	{
		for(x = 0;x < X;x++)
		{
			printf(" ");
			if(x == Posx && y == Posy)
			{
				YELLO;
			}
			switch(Mine[x][y].Mark)
			{
				case Open:
				{
					if(Mine[x][y].Num == MINE)
					{
						printf("%c",MineIcon);
					}
					else
					{ 
						printf("%d",Mine[x][y].Num);
					}
					break;
				}
				case Close:
				{
					printf("%c",Block);
					break;
				}
				case Flags:
				{
					printf("%c",FlagsIcon);
					break;
				}
			}
			White;
			printf(" ");
		}
		printf("\n");
	}
	printf("\n");
	printf("扫雷剩余:%d",(10 - CountFlagsNum()));
}

bool Not_stepping_on_the_mine(void)
{
	int x,y;
	for(x = 0;x < X;x++)
	{
		for(y = 0;y < Y;y++)
		{
			if(Mine[x][y].Mark == Open && Mine[x][y].Num == MINE)
			{
				return 0;
			}
		}
	}
	return 1;
}

bool IfWin(void)
{
	int x,y;
	if(CountFlagsNum() == 10)
	{
		for(x = 0;x < X;x++)
		{
			for(y = 0;y < Y;y++)
			{
				if(Mine[x][y].Mark == Flags && Mine[x][y].Num != MINE)
				{
					return false; 
				}
			}
		}
	}
	else
	{
		return false;
	}
	return true;
} 

void End(bool Rel)
{
	if(Rel)
	{
		printf("\n\n恭喜您赢了!用时:%.2lf(s)\n\n",(double)(clock()/1000.0));
	}
	else
	{
		printf("\n\n抱歉您触雷了!用时:%.2lf(s)\n\n",(double)(clock()/1000.0));
	}
}
