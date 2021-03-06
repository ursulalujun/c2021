#include "Chess.h"
#include "max-min.h"

int board[L+2][L+2];//表示棋盘的二维数组

int AI_regretx, AI_regrety, man_regretx, man_regrety;//记录人和AI上一轮下棋的位置，便于悔棋

int AI_x=0, AI_y=0;//记录AI本轮下棋的位置
int man_x=0, man_y=0;//man_y=i,man_x=j
int man2_x=0, man2_y=0;
//board[i][j]与position之间的关系gotoxy(4 * man_x, 2 * (man_y-1) + 1)
int dir[][2] = { {-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1} };//表示八个方向的常量数组

int flag = 0;//判断输赢
int rank = 2;

//初始化表示棋盘的数组
void board_init()
{
	for (int i = 0;i < L+2;i++)
		for (int j = 0;j < L+2;j++)//解决边界问题，加一个边界，边界=3
			//1-15是棋盘
		{
			if (i != 0 && i != L+1 && j != 0 && j != L+1)
				board[i][j] = 0;
			else
				board[i][j] = 3;
		}
}
//实现人控制的光标移动和下棋
void man_move()//人类玩家移动光标，为了在人人对战时实现可以由两位玩家操控，这里用指针传递位置坐标实现多态
{
loop1:location(man_x, man_y,white);
	gotoxy(4 * (man_x) , 2 * (man_y - 1) + 1);
	char key='y';
	while(key!=' ')
	{
	    key=_getch();
		if (AI_x != 0 && AI_y != 0)//玩家先手，第一次不用清除
			clearlocation(AI_x, AI_y);
		clearlocation(man_x, man_y);
		switch(key)
		{
    		case 72:
	    		man_y--;
	    		if(man_y<=1)
	    		    man_y=1;
    			break;
    		case 80:
    			man_y++;
	    		if(man_y>=15)
	    		    man_y=15;
    			break;
    		case 75:
	    		man_x--;
	    		if(man_x<=1)
	    		    man_x=1;
	    		break;
    		case 77:
	    		man_x++;
	    		if(man_x>=15)
	    		    man_x=15;
    			break;
    		case 'B':
    		case 'b':
    		{ 
				int message=MessageBox(NULL,TEXT("是否确定悔棋？"),TEXT("友情提示"),MB_OKCANCEL);
		        if(IDCANCEL==message)
					break;
			    if(IDOK==message)
			    {
			    	Regret();
    				break;
    			}
    		}
    	}
		location(man_x, man_y,white);
		gotoxy(4 * (man_x), 2 * (man_y-1) + 1);
    }
	if (board[man_y][man_x])
	{
		gotoxy(70, 22);
		BackGround(4, 0);
		printf("这里已经被下过了");
		goto loop1;
	}
	else 
	{
		board[man_y][man_x] = white;
		man_regretx = man_x;
		man_regrety = man_y;
		printf("●");
	}
}
//实现AI控制的光标移动和下棋
void machine_move()//打印AI的棋子的函数，机器用红子，玩家用白子
{
	//IDDFS();
	minMax_AB(rank, red, NGIF, PTIF, board);
	clearlocation(man_x, man_y);
	board[AI_y][AI_x] = red;
	location(AI_x, AI_y, red);
	gotoxy(4*AI_x, 2*(AI_y-1)+1);//坐标和数组的顺序是反的
	AI_regretx = AI_x;//记录电脑上一落子的位置 ,方便悔棋 
	AI_regrety = AI_y;
	printf("●");
}
//每下一步棋之后，判断输赢的函数
int judge_winner(int x, int y, int me, int now_board[L+2][L+2])//判断输赢，要向两个方向搜索
{
	int n1 = 0, n2 = 0;
	int flag = 0;
	for (int i = 0;i < 4;i++)//i表示八个方向
	{
		for (int j = 0;j <= 5;j++)//j表示沿这个方向走了几步
		{
			int dx = x + j * dir[i][0];
			int dy = y + j * dir[i][1];
			if (now_board[dy][dx] == me)//AI_x,AI_y都是坐标上的x，y，表示数组要反过来
				n1++;
			else 
				break;
		}
		for (int j = 0;j <= 5;j++)//j表示沿这个方向走了几步
		{
			int dx = x + j * dir[7 - i][0];
			int dy = y + j * dir[7 - i][1];
			if (now_board[dy][dx] == me)
				n2++;
			else 
				break;
		}
		if (n1 + n2 >= 6)
		{
			flag = me;
			return flag;
		}
		n1 = 0;
		n2 = 0;
	}
	return flag;
}
//实现人机对战功能的函数
void man_machine()//人机对战模式 
{
	system("color F2");
	loop6:system("cls");
	char key;
	int control;
	gotoxy(2, 3);
	printf("1.玩 家 先 手");
	
	gotoxy(2, 5);
	printf("2.电 脑 先 手");
	
	gotoxy(2, 7);
	printf("玩家为白子，电脑为红子(输入相应序号选择)");
	key=_getch();
	system("cls");
	gotoxy(2, 5);
	printf("请输入难度等级(输入偶数,例:2,4,6,难度等级越大,程序运行越慢):");
	rank = _getch() - '0';
	system("cls");
	control = 1;
	if(key!='1'&& key!='2')
	{
		printf("请重新输入");
		goto loop6;
	}
	system("color 0F");
	gotoxy(70,5);
	printf("   人 机 对 战    ");
	man_y=7;
	man_x=8;
	chess_board();
	board_init();
	chess_menu();
	if (key == '2')
	{
		AI_x = 7;
		AI_y = 8;
		AI_regretx = AI_x;//记录电脑上一落子的位置 ,方便悔棋 
		AI_regrety = AI_y;
		board[AI_y][AI_x] = red;
		location(AI_x, AI_y,red);
		gotoxy(4 * AI_x, 2 * (AI_y - 1) + 1);//机器第一次下在中间位	
		printf("●");
	}
	while(flag==0)
	{
		if(control==1)
		{
			gotoxy(70,22);
			BackGround(6,0);
			printf("   玩 家 执 手    "); 
			man_move();
			flag=judge_winner(man_x,man_y,white,board);
		}
		else
		{
			gotoxy(70,22);
			BackGround(6,0);
			printf("   电 脑 执 手    "); 
			machine_move();
			flag=judge_winner(AI_x,AI_y,red,board);
		}
		control=-control;
	}
	gotoxy(8,18);
	if(flag==white)
	{
		BackGround(7,0);
		MessageBox(NULL,TEXT("太厉害了，您战胜了电脑!"),TEXT("五子棋游戏"),MB_OK);
	}
	if(flag==red)
	{
		MessageBox(NULL,TEXT("游戏结束，您输给了电脑"),TEXT("五子棋游戏"),MB_OK);
	}
}
//实现人与人对战功能的函数
void man_man() 
{
	char key;
	int control=1;
	system("cls");
	gotoxy(70, 5);
	printf("   人 人 对 战    ");
	man_y = 7;
	man_x = 8;
	man2_y = 7;
	man2_x = 8;
	chess_board();
	board_init();
	chess_menu();
	while (flag == 0)
	{
		if (control == 1)
		{
			gotoxy(70, 22);
			BackGround(6, 0);
			printf("   玩 家1 执 手    ");
			man_move();
			flag = judge_winner(man_x, man_y, white, board);
		}
		else
		{
			gotoxy(70, 22);
			BackGround(6, 0);
			printf("   玩 家2 执 手    ");
			man_move();
			flag = judge_winner(man2_x, man2_y, red, board);
		}
		control = -control;
	}
	if (flag == 1)
	{
		BackGround(7, 0);
		MessageBox(NULL, TEXT("游戏结束，红子胜利"), "五子棋游戏", MB_OK);
	}
	if (flag == 2)
	{
		MessageBox(NULL, TEXT("游戏结束，白子胜利"), "五子棋游戏", MB_OK);
	}
}
//实现悔棋功能的函数
void Regret()//悔棋函数 
{
	gotoxy(4 * man_regretx , 2 * (man_regrety - 1) + 1);
	BackGround(0,0);
 	printf("  ");
	board[man_regrety][man_regretx]=0;
	gotoxy(4 * AI_regretx , 2 * (AI_regrety - 1) + 1);
	BackGround(0,0);
 	printf("  ");
	board[AI_regrety][AI_regretx]=0;
} 

int main()
{
	system("title 五子棋");
	system("mode con cols=92 lines=33");
	system("color F2");
	char choose, temp;
loop:welcome();
	choose = _getch();
	switch (choose)
	{
	case '1':
		man_machine();
		break;
	case '2':
		man_man();
		break;
	case '3':
		temp = Gametips();
		if (temp == 'E' || temp == 'e')
			goto loop;
		break;
	case '4':
	{
		int message = MessageBox(NULL, TEXT("是否退出？"),TEXT( "友情提示"), MB_OKCANCEL);
		if (IDCANCEL == message)
			goto loop;
		if (IDOK == message)
		{
			break;
		}
	}
	}
}
