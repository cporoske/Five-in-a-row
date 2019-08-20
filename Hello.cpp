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

//����������ϵ��
#define NONE1	0
#define NONE2	10
#define NONE3	3000
#define NONE4	10000//��ʤ
#define NONE5	100000//��ʤ

//���߶�������ϵ��
#define BOTH1	0
#define BOTH2	3
#define BOTH3	100
#define BOTH4	500
#define BOTH5	100000

//һ�߿�����ϵ��
#define SIDE1	0
#define SIDE2	5
#define SIDE3	500
#define SIDE4	1000//ʤ��
#define SIDE5	100000


struct{
	int type;
	int success;
}mSuccess;//�ɹ���Ϣ

enum{
	GAME_MENU,
	GAME_RUN
}g_GameDraw;//�������״̬

enum{
	GAME_MENU1,
    GAME_RUN1,
	GAME_SUCCESS
}g_GameState;//�߼�����

struct
{
	int x;
	int y;
	int w;
	int h;
	bool draw;
}rect[3];

int round = 1;//��ʼ���غ���
int mx,my;
int IsMousePaint = true;
int Chess[NUM][NUM]={0};
int person=1;
int Success;
int Back[NUM][NUM]={0};

void AIPlay(int *x,int *y);//��������
bool Judge(int x, int y);//�ж��Ƿ�ʤ��
void draw_Chess(int, int,int);//������
void drawAllChess(int x,int y);//������������
void drawBoard(int x, int y );//������
int GetType(int Num , int type);//��ȡ���ͷ���
int GetScore(int x, int y, int type);//

void OnPaint();
int WndProc(MOUSEMSG);

//void playChess(int,int,int);//��������

char *str[] = { "����Ϸ", "������Ϸ", "�˳�" };//��ʼ����ָʾ





int main(int argc, char* argv[])
{
	initgraph(640, 480);//��ʼ����

	MOUSEMSG m;		// ���������Ϣ
	g_GameDraw = GAME_MENU;
	g_GameState = GAME_MENU1;

	rect[0].draw = false;
	rect[1].draw = false;
	rect[2].draw = false;//����ɫ

	while(true)
	{
		// ��ȡһ�������Ϣ
		while(MouseHit())
		{
			m = GetMouseMsg();
			WndProc(m);
		}
	}

	// �ر�ͼ�δ���

	closegraph();

	return 0;
}


//��Ϣ�������
int WndProc(MOUSEMSG m)
{
	HWND hWnd = GetHWnd();
	switch(g_GameState)//�߼�����
	{
	case GAME_MENU1:
		switch(m.uMsg)
		{
		case WM_MOUSEMOVE:
			//��������ƶ�
			if( m.x >= rect[0].x && m.x<=rect[0].w && m.y>=rect[0].y && m.y<=rect[0].h)
			{
				//��Ӧ
				rect[0].draw = true;
			}
			else
				rect[0].draw = false;
			if( m.x >= rect[1].x && m.x<=rect[1].w && m.y>=rect[1].y && m.y<=rect[1].h)
			{
				//��Ӧ
				rect[1].draw = true;
			}
			else
				rect[1].draw = false;
			if( m.x >= rect[2].x && m.x<=rect[2].w && m.y>=rect[2].y && m.y<=rect[2].h)
			{
				//��Ӧ
				rect[2].draw = true;
			}
			else
				rect[2].draw = false;
			break;
		case WM_LBUTTONUP:
			//��갴�µ���
			if( m.x >= rect[0].x && m.x<=rect[0].w && m.y>=rect[0].y && m.y<=rect[0].h)
			{
				//��Ӧ
				g_GameDraw = GAME_RUN;//������Ϸ����Ļ���
				g_GameState = GAME_RUN1;//������Ϸ������߼�����
				rect[0].draw = false;
				rect[1].draw = false;
				rect[2].draw = false;//����ɫ

			}
			else if( m.x >= rect[1].x && m.x<=rect[1].w && m.y>=rect[1].y && m.y<=rect[1].h) 
			{
				rect[0].draw = false;
				rect[1].draw = false;
				rect[2].draw = false;//����ɫ
				
				FILE *fp;
				fp = fopen("recode.txt", "r");
				if( fp==NULL )
				{
					MessageBox(hWnd, "�浵�ļ�������", "��ʾ", MB_OK);
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
				exit(0);//�˳�
			}
			break;//ֻ��������ƶ���Ϣ����갴�µ�����Ϣ
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
			//����
			AIPlay(&xId, &yId);
			Chess[xId][yId] = 2;
			
			if( Judge(xId, yId) )//����ɹ�
			{
				g_GameState = GAME_SUCCESS;
			}
			person = 1;
			break;

		}
		switch(m.uMsg)
		{
		case WM_MOUSEMOVE:
			//��������ƶ�
			if(m.x >= 30&& m.x<=405&&m.y>=80&&m.y<=455)
			{
				mx=m.x;
				my=m.y;
				IsMousePaint=true;
			}

			else
			{
				IsMousePaint = false;
				//if() �������Ļ���
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
		case WM_LBUTTONUP://����
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
						//������
						if( Judge(xId, yId) )//����ɹ�
						{
							g_GameState = GAME_SUCCESS;
						}
						person = 2;
						//�ֵ�����
						round++;
					}
				}

				else
				{

					//if() �������Ļ���
					if(m.x>=rect[0].x&&m.x<=rect[0].w&&m.y>=rect[0].y&&m.y<=rect[0].h)
					{
		
						for(i=0;i<NUM;i++)
						{
							for(j=0;j<NUM;j++)
								Chess[i][j]=Back[i][j];
						}//��Ӧ����
						round--;
					}
					if(m.x>=rect[1].x&&m.x<=rect[1].w&&m.y>=rect[1].y&&m.y<=rect[1].h)
					{
						//
						FILE *fp;
						fp = fopen( "recode.txt", "w" );
						if( fp==NULL )
						{
							MessageBox(hWnd, "����ʧ��", "��ʾ��Ϣ", MB_OK );
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
						//���浱ǰ״̬
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

		if( IDYES == MessageBox(hWnd, (person==1)?"����ʤ��,�����Ƿ������Ϸ?":"����ʤ��,�����Ƿ������Ϸ?","ʤ��", MB_YESNO) )
		{
			//����
			//�������
			// g_GameState = GAME_RUN1
			int i,j;
			for(i=0;i<NUM;i++)
				for(j=0;j<NUM;j++)
					Chess[i][j] = 0;
			round = 1;
			g_GameState = GAME_RUN1;
		}

		else{
			//�˳�
			exit(0);
		}

		switch(m.uMsg)
		{
		case WM_MOUSEMOVE:
			//��������ƶ�
			
			break;
		case WM_LBUTTONDOWN:
			//
			g_GameDraw = GAME_RUN;
            //ÿ������һ������ж�ʤ��
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
	//�������

	
	OnPaint();

	return 0;
}


void OnPaint()
{
	//�������
	int w,h;
	int i;
	cleardevice();
	BeginBatchDraw();
	switch(g_GameDraw)
	{
	case GAME_MENU:
		setfillcolor(RGB(249,214,91));
		solidrectangle(0, 0, W,H);//��������
		
		settextstyle(60,0, "΢���ź�");
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
		solidrectangle(0, 0, W,H);//��������
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

	//��Ϸ�������̵ȵĻ���
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
    char *t[3]={"������","����","�˳�"};
	char temp[20];

    settextstyle(40,0, "����");
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

	settextstyle(30,0,"����");
	outtextxy(500,290,t[1]);
	outtextxy(500,350,t[2]);
	outtextxy(450,100,"��ǰ�غ���");
	sprintf( temp, "%-3d", round);
	outtextxy(515,150,temp);//�ȼ���Ϊ17���˻�����һ����һ���غϣ�
}


void draw_Chess(int x, int y, int type)
{
	setfillcolor((type==2)?BLACK:WHITE);//�ڰ����ӣ���������
	solidcircle(x, y, 10);
}


int GetType(int Num , int type)
{

/*****************************
	��ֵ����
  *****************************/


	if( type == 0 )
	{//˫������
		
		switch(Num)
		{
		case 1:
			return NONE1;//10��
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
	{//һ��������
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

	//���ĳһ��Ĺ�ֵ
	//���ڵ��� ����ֵ����������ֵ
	//���߼����������ܵ�ֵ�����
	
	int i,j;
	int p,q;
	int Num1,Num2;
	int Score = 0;
	char s[20];
	//��
	for(i=x-1,Num1=0; i>=0 && Chess[i][y] == type; i--,Num1++);
	for(j=x+1,Num2=0; j<NUM && Chess[j][y] == type; j++, Num2++);
	//
	//���˳�ѭ��
	//���������,һ����  �ҵ���һ���ĵ�
	//һ���ǵ���߽�
	Num1++;//���ϵ�ǰ��		
	if( i>=0 && j <NUM )//����ִ�б仯�Ժ��
	{
		
		sprintf(s, "%d,%d", i, j); 
		//������δ����߽�
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
		//������һ������߽�
		Score += GetType(Num1+Num2, 1);
	}

	//????????????????
	
	for(i=y-1,Num1=0; i>=0 && Chess[x][i] == type; i--,Num1++);
	for(j=y+1,Num2=0; j<NUM && Chess[x][j] == type; j++, Num2++);
	//
	//���˳�ѭ��
	//���������,һ����  �ҵ���һ���ĵ�
	//һ���ǵ���߽�
	Num1++;//���ϵ�ǰ��		
	if( i>=0 && j <NUM )//����ִ�б仯�Ժ��
	{
		//������δ����߽�
		if( Chess[x][i] == 0 && Chess[x][j] == 0 )
			Score += GetType(Num1+Num2, 0);
		else if( Chess[x][i] != type && Chess[x][j] != type)
			Score+=GetType(Num1+Num2, 2);
		else
			Score += GetType(Num1+Num2, 1);
	}

	else
	{
		//������һ������߽�
		Score += GetType(Num1+Num2, 1);	
	}


	//������Խ�������
	//�I
	for(i=x-1,j=y-1,Num1=0; i>=0 && j>=0 && Chess[i][j] == type; i--,j--,Num1++);
	//�K
	for(q=y+1,p=x+1,Num2=0; p<NUM && q<NUM && Chess[p][q] == type; p++,q++,Num2++);
	//
	//���˳�ѭ��
	//���������,һ����  �ҵ���һ���ĵ�
	//һ���ǵ���߽�
	Num1++;//���ϵ�ǰ��		
	if( i>=0 && j>=0 && p<NUM && q<NUM )//����ִ�б仯�Ժ��
	{
		//������δ����߽�
		if( Chess[i][j] == 0 && Chess[p][q] == 0 )
			Score += GetType(Num1+Num2, 0);
		else if( Chess[i][j] != type && Chess[p][q] != type)
			Score+=GetType(Num1+Num2, 2);
		else
			Score += GetType(Num1+Num2, 1);
	}

	else
	{
		//������һ������߽�
		Score += GetType(Num1+Num2, 1);	
	}

	//�����ҶԽ�������
	//�J
	for(i=x+1,j=y-1,Num1=0; i<NUM && j>=0 && Chess[i][j] == type; i++,j--,Num1++);
	//�L
	for(q=y+1,p=x-1,Num2=0; p>=0 && q<NUM && Chess[p][q] == type; p--,q++,Num2++);
	//
	//���˳�ѭ��
	//���������,һ����  �ҵ���һ���ĵ�
	//һ���ǵ���߽�
	Num1++;//���ϵ�ǰ��	
	
	if( i<NUM && j>=0 && p>=0 && q<NUM )//����ִ�б仯�Ժ��
	{
		//������δ����߽�
		if( Chess[i][j] == 0 && Chess[p][q] == 0 )
			Score += GetType(Num1+Num2, 0);
		else if( Chess[i][j] != type && Chess[p][q] != type)
			Score+=GetType(Num1+Num2, 2);
		else
			Score += GetType(Num1+Num2, 1);
	}

	else
	{
		//������һ������߽�
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
	//����
	int i,j;

	for(i=0;i<NUM;i++)
		for(j=0;j<NUM;j++)
		{
			//����ÿһ����
			//
			if( Chess[i][j] != 0 )
				continue;
			tmp1 = GetScore(i, j, person);//��øõ�
			//tmp1  ��� ��������ڵ��Ե��ӽ����� �ĸõ����ֵ
			tmp2 = GetScore(i, j, (person==2?1:2));//

			if( tmp1 > goalMax1 )
			{
				goalMax1 = tmp1;
				xMax = i;
				yMax = j;
			}//�õ�����

			if( tmp2 > goalMax2 )
			{
				goalMax2 = tmp2;
				xMax2 = i;
				yMax2 = j;
			}//�������������ӽǵ����ֵ
			
			//�������  �����ֵ����
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



bool Judge(int x, int y)//��(i,j)��ʼ���� 
{
	//��֪��ǰ�µ���
	//�˸�����?
	//��ʼ����
	int i,j;
	int Num;
	
	//����
	//��
	for(j=y,Num=0; j>=0 && Chess[x][j] == person; j--,Num++);
	for(j=y+1; j<NUM && Chess[x][j] == person; j++,Num++);
	if( Num >= 5 )
	{
	    return true;
	}
	//��
	for(i=x,Num=0; i>=0 && Chess[i][y] == person; i--,Num++);
	for(i=x+1; i<NUM && Chess[i][y] == person; i++,Num++);
	if( Num >= 5 )
	{
		return true;
	}
	//��

	//�L
	for(j=y,i=x,Num=0; i>=0 && j>=0 && Chess[i][j] == person; i--,j--,Num++);
	for(j=y+1,i=x+1; i<NUM && j<NUM && Chess[i][j] == person; j++,i++,Num++);
	if( Num >= 5 )
	{
		return true;
	}
	
	//�J
	for(j=y,i=x,Num=0; i<NUM && j>=0 && Chess[i][j] == person ; i++,j--,Num++);
	//�K
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
}//�趨���ӵĻ���

void drawAllChess(int x,int y)
{
	//������������ 0 ����
	//1
	//2
	int i,j;
	int x0,y0;
	for(i=0; i<NUM; i++)
		for(j=0;j<NUM; j++)
		{
			//���㵱ǰ����
			x0 = x+i*WIDTH;
			y0 = y+j*WIDTH;
			//�������õ����ǽ��������Ҳ���ǻ�������
			//�����ȱ���
			switch(Chess[i][j])
			{
			case 1:
				//��
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


