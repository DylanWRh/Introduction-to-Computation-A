#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;
void init();void chessboard_init();
int judge();
int can_move(int,int);
int can_move_to(int,int);
void move_chess(int,int,int,int);
int move_judge(int,int,int,int);
void reverse_chess(int,int);
void do_reverse(int,int);
void ai_move(int);
void board_to_chess();
int in_chessboard(int,int);
int chessboard[8][8]={0};
int cur_color=1;
struct Chess{int x=0,y=0;}chess[3][50];
struct Case{int x1=0,y1=0,x2=0,y2=0,factors_mixed=-100;};
Case get_factors_mixed(int);
int can_absorb(int,int);
double aver_absorbed(int);
int main(){
    istream::sync_with_stdio(false);
	init();chessboard_init();
	int n;cin >> n;
	int bot_color=2;
	for(int i=1;i<=2*n-1;i++){
		int x1,y1,x2,y2;
		cin >> x1 >> y1 >> x2 >> y2;
		if(x1==-1)bot_color=1;
        else{
            move_chess(x1+1,y1+1,x2+1,y2+1);
        }
	}
    ai_move(bot_color);
    return 0;
}
void init(){
    cur_color=1;
    for(int i=0;i<=7;i++){
        for(int j=0;j<=7;j++)chessboard[i][j]=0;
    }
    board_to_chess();
    return;
}
void chessboard_init(){
    for(int i=1;i<=7;i++){
        for(int j=1;j<=7;j++)chessboard[i][j]=0;
    }
    chessboard[1][1]=1;chessboard[7][7]=1;
    chessboard[1][7]=2;chessboard[7][1]=2;
    chess[1][0].x=2;
    chess[1][1].x=1;chess[1][1].y=1;
    chess[1][2].x=7;chess[1][2].y=7;
    chess[2][0].x=2;
    chess[2][1].x=1;chess[2][1].y=7;
    chess[2][2].x=7;chess[2][2].y=1;
    cur_color=1;
    return;
}
int can_move(int x,int y){
    for(int i=-2;i<=2;i++){
        for(int j=-2;j<=2;j++){
            if(
                (i || j)
                && can_move_to(x+i,y+j)
            )return 1;
        }
    }
    return 0;
}
int can_move_to(int x,int y){
    if(
        in_chessboard(x,y)
        && chessboard[x][y]==0
    )return 1;
    return 0;
}
void move_chess(int x1,int y1,int x2,int y2){
    int judge_tmp=move_judge(x1,y1,x2,y2);
    if(judge_tmp!=0) {
        chessboard[x2][y2]=cur_color;
        reverse_chess(x2,y2);
        if(judge_tmp==2)chessboard[x1][y1]=0;
        board_to_chess();
        cur_color=3-cur_color;
    }
    return;
}
int move_judge(int x1,int y1,int x2,int y2){
    int result=0;
    if(in_chessboard(x1,y1) && in_chessboard(x2,y2)){
        result=max(abs(x2-x1),abs(y2-y1));
        if(result>=3)return 0;
        if(chessboard[x2][y2])return 0;
        if(!chessboard[x1][y1])return 0;
    }
    return result;
}
void reverse_chess(int x,int y){
    for(int i=-1;i<=1;i++){
        for(int j=-1;j<=1;j++){
            if(i || j)do_reverse(x+i,y+j);
        }
    }
    return;
}
void do_reverse(int x,int y){
    if(
        in_chessboard(x,y)
        && chessboard[x][y]==3-cur_color
    )chessboard[x][y]=cur_color;
    return;
}
void ai_move(int color){
    int x1,y1,x2,y2;
    Case next_step=get_factors_mixed(color);
    x1=next_step.x1;y1=next_step.y1;
    x2=next_step.x2;y2=next_step.y2;
    cout << x1-1 << " " << y1-1 << " " << x2-1 << " " << y2-1; 
    return;
}
void board_to_chess(){
    chess[1][0].x=0;chess[2][0].x=0;
    for(int i=1;i<=7;i++){
        for(int j=1;j<=7;j++){
            if(chessboard[i][j]==1){
                chess[1][++chess[1][0].x].x=i;
                chess[1][chess[1][0].x].y=j;
            }
            else if(chessboard[i][j]==2){
                chess[2][++chess[2][0].x].x=i;
                chess[2][chess[2][0].x].y=j;
            }
        }
    }
}
int can_absorb(int x2,int y2){
    int result=0;
    for(int i=-1;i<=1;i++){
        for(int j=-1;j<=1;j++){
            if(
                (i || j)
                && in_chessboard(x2+i,y2+j)
                && chessboard[x2+i][y2+j]==3-cur_color
            )result++;
        }
    }
    return result;
}
int in_chessboard(int x,int y){
    if(x>=1 && x<=7 && y>=1 && y<=7)return true;
    return false;
}
Case get_factors_mixed(int color){
    //color表示当前执子方，用以判断考虑综合因素时应减去(人类执子)还是加上(电脑执子)相应值
    
    //寻找最优步
    Case next_step;
    int x1,y1,x2,y2;
    double factors_mixed=0;
    for(int i=1;i<=chess[color][0].x;i++){
        x1=chess[color][i].x;y1=chess[color][i].y;
        for(int p=-2;p<=2;p++){
            for(int q=-2;q<=2;q++){
                x2=x1+p;y2=y1+q;
                if(can_move_to(x2,y2)){
                    //留存副本，分析完成后恢复棋盘
                    int chessboard_tmp[8][8]={0};
                    for(int i=1;i<=7;i++){
                        for(int j=1;j<=7;j++)chessboard_tmp[i][j]=chessboard[i][j];
                    }
                    //走子，实现下一步判断
                    chessboard[x2][y2]=cur_color;
                    reverse_chess(x2,y2);
                    if(max(abs(p),abs(q))==2)chessboard[x1][y1]=0;
                    //走完之后，贪心算法判断相差数
                    board_to_chess();
                    factors_mixed=2*(chess[color][0].x-chess[3-color][1].x);
                    //再判断走完之后自身可能被同化情况，即对方棋子能落子处同化量的平均值
                    //同时返回了对方下一步最多同化的子数
                    //这在一定程度上实现了最大矩形类似物的判断
                    factors_mixed-=aver_absorbed(color);
                    //分析完毕，恢复棋盘
                    for(int i=1;i<=7;i++){
                        for(int j=1;j<=7;j++)chessboard[i][j]=chessboard_tmp[i][j];
                    }
                    board_to_chess();
                    if(factors_mixed>=next_step.factors_mixed){
                        next_step.factors_mixed=factors_mixed;
                        next_step.x1=x1;next_step.y1=y1;
                        next_step.x2=x2;next_step.y2=y2;
                    }
                }
            }
        }
    }
    return next_step;
}
double aver_absorbed(int color){
    int cnt=0,max_absorbed=0,max_cnt=0;
    double result=0;
    for(int i=1;i<=7;i++){
        for(int j=1;j<=7;j++){
            if(chessboard[i][j]==0){
                for(int p=-2;p<=2;p++){
                    for(int q=-2;q<=2;q++){
                        if((p || q) && chessboard[p][q]==3-color){
                            cnt++;
                            int result_tmp=result;
                            for(int P=-1;P<=1;P++){
                                for(int Q=-1;Q<=1;Q++){
                                    if((P || Q) && chessboard[P][Q]==color)result++;
                                }
                            }
                            if(result-result_tmp>max_absorbed){
                                max_absorbed=result-result_tmp;
                                max_cnt=1;
                            }
                            else if(result-result_tmp==max_absorbed)max_cnt++;
                            goto FIND_NEXT_EMPTY;
                        }
                    }
                }
            }
            FIND_NEXT_EMPTY:;
        }
    }
    return (result+max_absorbed*max_cnt)/cnt;
}