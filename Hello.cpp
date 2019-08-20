// Hello.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>


#define W 640
#define H 640
#define WIDTH 25
#define NUM	15

//两边无棋子系列
#define NONE1	0
#define NONE2	10
#define NONE3	3000
#define NONE4	10000//必胜
#define NONE5	100000//必胜

//两边都有棋子系列
#define BOTH1	0
#define BOTH2	3
#define BOTH3	100
#define BOTH4	500
#define BOTH5	100000

//一边可以下系列
#define SIDE1	0
#define SIDE2	5
#define SIDE3	500
#define SIDE4	1000//胜利
#define SIDE5	100000


struct{
	int type;
	int success;
}mSuccess;//成功信息

enum{
	GAME_MENU,
	GAME_RUN
}g_GameDraw;//界面绘制状态

enum{
	GAME_MENU1,
    GAME_RUN1,
	GAME_SUCCESS
}g_GameState;//逻辑处理

struct
{
	int x;
	int y;
	int w;
	int h;
	bool draw;
}rect[3];

int round = 1;//初始化回合数
int mx,my;
int IsMousePaint = true;
int Chess[NUM][NUM]={0};
int person=1;
int Success;
int Back[NUM][NUM]={0};

void AIPlay(int *x,int *y);//电脑下棋
bool Judge(int x, int y);//判断是否胜利
void draw_Chess(int, int,int);//画棋子
void drawAllChess(int x,int y);//绘制所有棋子
void drawBoard(int x, int y );//画棋盘
int GetType(int Num , int type);//获取类型分数
int GetScore(int x, int y, int type);//

void OnPaint();
int WndProc(MOUSEMSG);

//void playChess(int,int,int);//函数声明

char *str[] = { "新游戏", "继续游戏", "退出" };//初始界面指示





int main(int argc, char* argv[])
{
	initgraph(640, 480);//初始界面

	MOUSEMSG m;		// 定义鼠标消息
	g_GameDraw = GAME_MENU;
	g_GameState = GAME_MENU1;

	rect[0].draw = false;
	rect[1].draw = false;
	rect[2].draw = false;//键变色

	while(true)
	{
		// 获取一条鼠标消息
		while(MouseHit())
		{
			m = GetMouseMsg();
			WndProc(m);
		}
	}

	// 关闭图形窗口

	closegraph();

	return 0;
}


//消息处理程序
int WndProc(MOUSEMSG m)
{
	HWND hWnd = GetHWnd();
	switch(g_GameState)//逻辑处理
	{
	case GAME_MENU1:
		switch(m.uMsg)
		{
		case WM_MOUSEMOVE:
			//处理鼠标移动
			if( m.x >= rect[0].x && m.x<=rect[0].w && m.y>=rect[0].y && m.y<=rect[0].h)
			{
				//响应
				rect[0].draw = true;
			}
			else
				rect[0].draw = false;
			if( m.x >= rect[1].x && m.x<=rect[1].w && m.y>=rect[1].y && m.y<=rect[1].h)
			{
				//响应
				rect[1].draw = true;
			}
			else
				rect[1].draw = false;
			if( m.x >= rect[2].x && m.x<=rect[2].w && m.y>=rect[2].y && m.y<=rect[2].h)
			{
				//响应
				rect[2].draw = true;
			}
			else
				rect[2].draw = false;
			break;
		case WM_LBUTTONUP:
			//鼠标按下弹起
			if( m.x >= rect[0].x && m.x<=rect[0].w && m.y>=rect[0].y && m.y<=rect[0].h)
			{
				//响应
				g_GameDraw = GAME_RUN;//进入游戏界面的绘制
				g_GameState = GAME_RUN1;//进入游戏界面的逻辑处理
				rect[0].draw = false;
				rect[1].draw = false;
				rect[2].draw = false;//键变色

			}
			else if( m.x >= rect[1].x && m.x<=rect[1].w && m.y>=rect[1].y && m.y<=rect[1].h) 
			{
				rect[0].draw = false;
				rect[1].draw = false;
				rect[2].draw = false;//键变色
				
				FILE *fp;
				fp = fopen("recode.txt", "r");
				if( fp==NULL )
				{
					MessageBox(hWnd, "存档文件不存在", "提示", MB_OK);
				}
				int i,j;
				for(i=0;i<NUM;i++)
					for(j=0;j<NUM;j++)
						fscanf(fp, "%d\t", &Chess[i][j] );
				fscanf(fp, " %d", &round);

				fclose(fp);
				g_GameDraw = GAME_RUN;
				g_GameState = GAME_RUN1;

			}
			else if( m.x >= rect[2].x && m.x<=rect[2].w && m.y>=rect[2].y && m.y<=rect[2].h) 
			{
				closegraph();
				exit(0);//退出
			}
			break;//只处理鼠标移动信息和鼠标按下弹起信息
		case WM_RBUTTONDOWN:
			break;
		case WM_RBUTTONUP:
			//
			break;
		default:
			break;
		}
	break;

	case GAME_RUN1:
		int xId,yId;
		if( person == 2 )
		{
			//电脑
			AIPlay(&xId, &yId);
			Chess[xId][yId] = 2;
			
			if( Judge(xId, yId) )//如果成功
			{
				g_GameState = GAME_SUCCESS;
			}
			person = 1;
			break;

		}
		switch(m.uMsg)
		{
		case WM_MOUSEMOVE:
			//处理鼠标移动
			if(m.x >= 30&& m.x<=405&&m.y>=80&&m.y<=455)
			{
				mx=m.x;
				my=m.y;
				IsMousePaint=true;
			}

			else
			{
				IsMousePaint = false;
				//if() 两个键的绘制
				if(m.x>=rect[0].x&&m.x<=rect[0].w&&m.y>=rect[0].y&&m.y<=rect[0].h)
					rect[0].draw=true;
				else
					rect[0].draw=false;
				if(m.x>=rect[1].x&&m.x<=rect[1].w&&m.y>=rect[1].y&&m.y<=rect[1].h)
					rect[1].draw=true;
				else
					rect[1].draw=false;

			}

			break;
		case WM_LBUTTONUP://下棋
			int i,j;
			if( person == 1)
			{
				if(m.x >= 30&& m.x<=405&&m.y>=80&&m.y<=455)
				{
					if((m.x-30.0)/WIDTH-int((m.x-30.0)/WIDTH)>=0.5)
						xId=(m.x-30)/WIDTH+1;
					else
						xId=(m.x-30)/WIDTH;
					if((m.y-80.0)/WIDTH-int((m.y-80.0)/WIDTH)>=0.5)
						yId=(m.y-80)/WIDTH+1;
					else
						yId=(m.y-80)/WIDTH;
                

					int i,j;
					for(i=0;i<NUM;i++)
					{
						for(j=0;j<NUM;j++)
							Back[i][j]=Chess[i][j];
					}
					
					if( !Chess[xId][yId] )
					{
						Chess[xId][yId] = 1;
						//人下棋
						if( Judge(xId, yId) )//如果成功
						{
							g_GameState = GAME_SUCCESS;
						}
						person = 2;
						//轮到电脑
						round++;
					}
				}

				else
				{

					//if() 两个键的绘制
					if(m.x>=rect[0].x&&m.x<=rect[0].w&&m.y>=rect[0].y&&m.y<=rect[0].h)
					{
		
						for(i=0;i<NUM;i++)
						{
							for(j=0;j<NUM;j++)
								Chess[i][j]=Back[i][j];
						}//响应悔棋
						round--;
					}
					if(m.x>=rect[1].x&&m.x<=rect[1].w&&m.y>=rect[1].y&&m.y<=rect[1].h)
					{
						//
						FILE *fp;
						fp = fopen( "recode.txt", "w" );
						if( fp==NULL )
						{
							MessageBox(hWnd, "保存失败", "提示信息", MB_OK );
						}
						
						for(i=0;i<NUM;i++)
						{
							for(j=0;j<NUM;j++)
							{
								fprintf(fp, "%d\t", Chess[i][j] );
							}

							fprintf(fp,"\n");
						}

						fprintf(fp, "%d", round);

						fclose(fp);
						//保存当前状态
						exit(0);
					}
				}
			}

			break;
		case WM_RBUTTONDOWN:
			break;
		case WM_RBUTTONUP:
			//
			break;
		default:
			break;
		}
		break;
	case GAME_SUCCESS:

		if( IDYES == MessageBox(hWnd, (person==1)?"黑棋胜利,请问是否继续游戏?":"白棋胜利,请问是否继续游戏?","胜利", MB_YESNO) )
		{
			//继续
			//清空棋盘
			// g_GameState = GAME_RUN1
			int i,j;
			for(i=0;i<NUM;i++)
				for(j=0;j<NUM;j++)
					Chess[i][j] = 0;
			round = 1;
			g_GameState = GAME_RUN1;
		}

		else{
			//退出
			exit(0);
		}

		switch(m.uMsg)
		{
		case WM_MOUSEMOVE:
			//处理鼠标移动
			
			break;
		case WM_LBUTTONDOWN:
			//
			g_GameDraw = GAME_RUN;
            //每次下了一颗棋后判断胜负
			break;
		case WM_LBUTTONUP:
			break;
		case WM_RBUTTONDOWN:
			break;
		case WM_RBUTTONUP:
			//
			break;
		default:
			break;
		}
		break;
	}
	//界面绘制

	
	OnPaint();

	return 0;
}


void OnPaint()
{
	//界面绘制
	int w,h;
	int i;
	cleardevice();
	BeginBatchDraw();
	switch(g_GameDraw)
	{
	case GAME_MENU:
		setfillcolor(RGB(249,214,91));
		solidrectangle(0, 0, W,H);//整个界面
		
		settextstyle(60,0, "微软雅黑");
		for( i=0; i<3; i++)
		{
			setbkmode(TRANSPARENT);
			setfillcolor(RGB(180,139,54));
			w = textwidth(str[i]);
			h = textheight(str[i]);
			if( rect[i].draw )
				setfillcolor(RGB(188,163,86));
			fillrectangle((W-w)/2 - 5, 50 + i*150, (W-w) / 2 + w + 5, 50+ h + 5+i*150);
			outtextxy( (W-w)/2, 50 + i*150 + 2, str[i] );

			rect[i].x = (W-w)/2 - 5;
			rect[i].y = 50 + i*150;
			rect[i].w = (W-w) / 2 + w + 5;
			rect[i].h = 50+ h + 5+i*150;
		}

		break;
	case GAME_RUN:
		int i,j;
		setfillcolor(RGB(249,214,91));
		solidrectangle(0, 0, W,H);//整个界面
		drawBoard(30,80);
			
		for(i=0;i<NUM;i++)
		{
			for(j=0;j<NUM;j++)
			{
				if( Chess[i][j] )
					draw_Chess(30+WIDTH*i,80+WIDTH*j,Chess[i][j]);
			}
		}

		if(IsMousePaint)
			draw_Chess(mx,my,1);

	//游戏界面棋盘等的绘制
		break;
	}
	FlushBatchDraw();
}


void drawBoard(int x, int y )
{
	int i;
	setlinecolor(BLACK);
	for(i=0; i<NUM+1; i++)
		line(x, y+i*WIDTH,x + NUM * WIDTH,  y+i*WIDTH);//STARTX STARTY
	for(i=0;i<NUM+1; i++)
		line(x+i*WIDTH, y, x+i*WIDTH, y+NUM*WIDTH);
    char *t[3]={"五子棋","悔棋","退出"};
	char temp[20];

    settextstyle(40,0, "楷体");
    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    outtextxy(160, 20, t[0]);
    setfillcolor(RGB(200,151,68));
	solidrectangle(500,140,550,190);
    
	if( rect[0].draw )
		setfillcolor(RGB(188,163,86));
	else
		setfillcolor(RGB(200,151,68));
	solidrectangle(470,280,590,330);
	if( rect[1].draw )
		setfillcolor(RGB(188,163,86));
	else
		setfillcolor(RGB(200,151,68));
	solidrectangle(470,340,590,390);

    rect[0].x=470;rect[0].y=280;rect[0].w=590;rect[0].h=330;
	rect[1].x=470;rect[1].y=340;rect[1].w=590;rect[1].h=390;

	settextstyle(30,0,"黑体");
	outtextxy(500,290,t[1]);
	outtextxy(500,350,t[2]);
	outtextxy(450,100,"当前回合数");
	sprintf( temp, "%-3d", round);
	outtextxy(515,150,temp);//先假设为17（人机各下一次算一个回合）
}


void draw_Chess(int x, int y, int type)
{
	setfillcolor((type==2)?BLACK:WHITE);//黑白棋子，两种类型
	solidcircle(x, y, 10);
}


int GetType(int Num , int type)
{

/*****************************
	估值函数
  *****************************/


	if( type == 0 )
	{//双无棋子
		
		switch(Num)
		{
		case 1:
			return NONE1;//10分
			break;
		case 2:
			return NONE2;
			break;
		case 3:
			return NONE3;
			break;
		case 4:
			return NONE4;
			break;
		case 5:
			return NONE5;
			break;
		}

	}


	else if(type==1)
	{//一边有棋子
		switch(Num)
		{
		case 1:
			return SIDE1;
			break;
		case 2:
			return SIDE2;
			break;
		case 3:
			return SIDE3;
			break;
		case 4:
			return SIDE4;
			break;
		case 5:
			return SIDE5;
			break;
		}

	}

	else
	{
		//type == 2
		switch(Num)
		{
		case 1:
			return BOTH1;
			break;
		case 2:
			return BOTH2;
			break;
		case 3:
			return BOTH3;
			break;
		case 4:
			return BOTH4;
			break;
		case 5:
			return BOTH5;
			break;
		}

	}

}

int GetScore(int x, int y, int type)
{

	//获得某一点的估值
	//对于电脑 用正值，对于人正值
	//二者加起来就是总的值最后呢
	
	int i,j;
	int p,q;
	int Num1,Num2;
	int Score = 0;
	char s[20];
	//←
	for(i=x-1,Num1=0; i>=0 && Chess[i][y] == type; i--,Num1++);
	for(j=x+1,Num2=0; j<NUM && Chess[j][y] == type; j++, Num2++);
	//
	//当退出循环
	//有两种情况,一种是  找到不一样的点
	//一种是到达边界
	Num1++;//加上当前点		
	if( i>=0 && j <NUM )//这是执行变化以后的
	{
		
		sprintf(s, "%d,%d", i, j); 
		//两个都未到达边界
		if( Chess[i][y] == 0 && Chess[j][y] == 0 )
			Score += GetType(Num1+Num2, 0);
		else if( Chess[i][y] != type && Chess[j][y] != type)
			Score+=GetType(Num1+Num2, 2);
		else
			Score += GetType(Num1+Num2, 1);
		outtextxy(0, 0, s);
	}

	else
	{
		//其中有一个到达边界
		Score += GetType(Num1+Num2, 1);
	}

	//????????????????
	
	for(i=y-1,Num1=0; i>=0 && Chess[x][i] == type; i--,Num1++);
	for(j=y+1,Num2=0; j<NUM && Chess[x][j] == type; j++, Num2++);
	//
	//当退出循环
	//有两种情况,一种是  找到不一样的点
	//一种是到达边界
	Num1++;//加上当前点		
	if( i>=0 && j <NUM )//这是执行变化以后的
	{
		//两个都未到达边界
		if( Chess[x][i] == 0 && Chess[x][j] == 0 )
			Score += GetType(Num1+Num2, 0);
		else if( Chess[x][i] != type && Chess[x][j] != type)
			Score+=GetType(Num1+Num2, 2);
		else
			Score += GetType(Num1+Num2, 1);
	}

	else
	{
		//其中有一个到达边界
		Score += GetType(Num1+Num2, 1);	
	}


	//进行左对角线搜索
	//↖
	for(i=x-1,j=y-1,Num1=0; i>=0 && j>=0 && Chess[i][j] == type; i--,j--,Num1++);
	//↘
	for(q=y+1,p=x+1,Num2=0; p<NUM && q<NUM && Chess[p][q] == type; p++,q++,Num2++);
	//
	//当退出循环
	//有两种情况,一种是  找到不一样的点
	//一种是到达边界
	Num1++;//加上当前点		
	if( i>=0 && j>=0 && p<NUM && q<NUM )//这是执行变化以后的
	{
		//两个都未到达边界
		if( Chess[i][j] == 0 && Chess[p][q] == 0 )
			Score += GetType(Num1+Num2, 0);
		else if( Chess[i][j] != type && Chess[p][q] != type)
			Score+=GetType(Num1+Num2, 2);
		else
			Score += GetType(Num1+Num2, 1);
	}

	else
	{
		//其中有一个到达边界
		Score += GetType(Num1+Num2, 1);	
	}

	//进行右对角线搜索
	//↗
	for(i=x+1,j=y-1,Num1=0; i<NUM && j>=0 && Chess[i][j] == type; i++,j--,Num1++);
	//↙
	for(q=y+1,p=x-1,Num2=0; p>=0 && q<NUM && Chess[p][q] == type; p--,q++,Num2++);
	//
	//当退出循环
	//有两种情况,一种是  找到不一样的点
	//一种是到达边界
	Num1++;//加上当前点	
	
	if( i<NUM && j>=0 && p>=0 && q<NUM )//这是执行变化以后的
	{
		//两个都未到达边界
		if( Chess[i][j] == 0 && Chess[p][q] == 0 )
			Score += GetType(Num1+Num2, 0);
		else if( Chess[i][j] != type && Chess[p][q] != type)
			Score+=GetType(Num1+Num2, 2);
		else
			Score += GetType(Num1+Num2, 1);
	}

	else
	{
		//其中有一个到达边界
		Score += GetType(Num1+Num2, 1);	
	}

	return Score;
}
void AIPlay(int *x,int *y)
{


	//person = 2
	int xMax,yMax;
	int xMax2, yMax2;
	int goalMax1 = 0;
	int goalMax2 = 0;
	int tmp1,tmp2;
	//电脑
	int i,j;

	for(i=0;i<NUM;i++)
		for(j=0;j<NUM;j++)
		{
			//遍历每一个点
			//
			if( Chess[i][j] != 0 )
				continue;
			tmp1 = GetScore(i, j, person);//获得该点
			//tmp1  获得 的是相对于电脑的视角来看 的该点分数值
			tmp2 = GetScore(i, j, (person==2?1:2));//

			if( tmp1 > goalMax1 )
			{
				goalMax1 = tmp1;
				xMax = i;
				yMax = j;
			}//得到最大点

			if( tmp2 > goalMax2 )
			{
				goalMax2 = tmp2;
				xMax2 = i;
				yMax2 = j;
			}//获得相对于人类视角的最大值
			
			//怎样设计  这个估值函数
			//F(X,Y) = F1(x,y) + F2(x,y)
		}
	
	if( goalMax1 > 100000 )
	{
		*x = xMax;
		*y = yMax;
	}
	else{
		*x = (goalMax1 > goalMax2)?xMax:xMax2;
		*y = (goalMax1 > goalMax2)?yMax:yMax2;
	}
}



bool Judge(int x, int y)//从(i,j)开始搜索 
{
	//已知当前下的人
	//八个方向?
	//开始搜索
	int i,j;
	int Num;
	
	//向上
	//下
	for(j=y,Num=0; j>=0 && Chess[x][j] == person; j--,Num++);
	for(j=y+1; j<NUM && Chess[x][j] == person; j++,Num++);
	if( Num >= 5 )
	{
	    return true;
	}
	//←
	for(i=x,Num=0; i>=0 && Chess[i][y] == person; i--,Num++);
	for(i=x+1; i<NUM && Chess[i][y] == person; i++,Num++);
	if( Num >= 5 )
	{
		return true;
	}
	//右

	//↙
	for(j=y,i=x,Num=0; i>=0 && j>=0 && Chess[i][j] == person; i--,j--,Num++);
	for(j=y+1,i=x+1; i<NUM && j<NUM && Chess[i][j] == person; j++,i++,Num++);
	if( Num >= 5 )
	{
		return true;
	}
	
	//↗
	for(j=y,i=x,Num=0; i<NUM && j>=0 && Chess[i][j] == person ; i++,j--,Num++);
	//↘
	for(j=y+1,i=x-1; i>=0 && j<NUM && Chess[i][j] == person ; i--,j++,Num++);
	if( Num >= 5 )
	{
		return true;
	}

	return false;
}


void drawChess(int x, int y,int type)
{
	setfillcolor((type==2)?BLACK:WHITE);
	solidcircle(x, y, 10);
}//设定棋子的画法

void drawAllChess(int x,int y)
{
	//棋子类型两种 0 不画
	//1
	//2
	int i,j;
	int x0,y0;
	for(i=0; i<NUM; i++)
		for(j=0;j<NUM; j++)
		{
			//计算当前坐标
			x0 = x+i*WIDTH;
			y0 = y+j*WIDTH;
			//这个计算得到的是交叉点坐标也就是绘制坐标
			//行优先遍历
			switch(Chess[i][j])
			{
			case 1:
				//白
				drawChess(x0, y0, 1);
				break;
			case 2:
				drawChess(x0, y0, 2);
				break;
			default:
				break;
			}
		}



}


