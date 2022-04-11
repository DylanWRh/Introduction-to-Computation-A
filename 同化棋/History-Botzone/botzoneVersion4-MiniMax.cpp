#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;
void init();void chessboard_init();
int judge();
int can_move(int,int);
int can_move_to(int,int);
void move_chess(int,int,int,int);
int move_judge(int,int,int,int);
void reverse_chess(int,int,int);
void do_reverse(int,int,int);
void ai_move(int);
void board_to_chess();
int in_chessboard(int,int);
int chessboard[8][8]={0};
int cur_color=1;
struct Chess{int x=0,y=0;}chess[3][50];

struct Case{int x1=0,y1=0,x2=0,y2=0;double evaluation=-1000;};//用于返回决策结果到ai_move()中
Case minimax(int);
double human_play(int,int);
double ai_play(int,int);
double evaluate();
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
int judge(){
    int result=0;
    if(chess[1][0].x==0)result=2;
    else if(chess[2][0].x==0)result=1;
    else {
        for(int i=1;i<=7;i++){
            for(int j=1;j<=7;j++){
                if(
                    chessboard[i][j]==cur_color
                    && can_move(i,j)
                )return 0;
            }
        }
        if(chess[1][0].x>chess[2][0].x)result=1;
        else if(chess[2][0].x>chess[1][0].x)result=2;
        else result=3;
    }
    return result;
}
void move_chess(int x1,int y1,int x2,int y2){
    int judge_tmp=move_judge(x1,y1,x2,y2);
    if(judge_tmp!=0) {
        chessboard[x2][y2]=cur_color;
        reverse_chess(x2,y2,cur_color);
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
void reverse_chess(int x,int y,int color){
    for(int i=-1;i<=1;i++){
        for(int j=-1;j<=1;j++){
            if(i || j)do_reverse(x+i,y+j,color);
        }
    }
    return;
}
void do_reverse(int x,int y,int color){
    if(
        in_chessboard(x,y)
        && chessboard[x][y]==3-color
    )chessboard[x][y]=color;
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
int in_chessboard(int x,int y){
    if(x>=1 && x<=7 && y>=1 && y<=7)return true;
    return false;
}
void ai_move(int color){
    int x1,y1,x2,y2;
    Case next_step=minimax(3);
    x1=next_step.x1;y1=next_step.y1;
    x2=next_step.x2;y2=next_step.y2;
    cout << x1-1 << " " << y1-1 << " " << x2-1 << " " << y2-1; 
    return;
}
Case minimax(int minimax_depth){
    int depth=1;
    //寻找最优步
    Case next_step;
    Case step_choice[1000];
    int x1,y1,x2,y2;
    double evaluation=0;
    for(int i0=1;i0<=chess[cur_color][0].x;i0++){
        x1=chess[cur_color][i0].x;y1=chess[cur_color][i0].y;
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
                    reverse_chess(x2,y2,cur_color);
                    if(max(abs(p),abs(q))==2)chessboard[x1][y1]=0;
                    board_to_chess();
                    //估值
                    if(evaluate()>=next_step.evaluation){
                        evaluation=human_play(depth,minimax_depth);
                        if(evaluation>=step_choice[0].evaluation){
                            if(evaluation>step_choice[0].evaluation){
                                step_choice[0].evaluation=evaluation;
                                step_choice[0].x1=0;
                            }
                            step_choice[++step_choice[0].x1].evaluation=evaluation;
                            step_choice[step_choice[0].x1].x1=x1;
                            step_choice[step_choice[0].x1].x2=x2;
                            step_choice[step_choice[0].x1].y1=y1;
                            step_choice[step_choice[0].x1].y2=y2;
                        }
                    }
                    //分析完毕，恢复棋盘
                    for(int i=1;i<=7;i++){
                        for(int j=1;j<=7;j++)chessboard[i][j]=chessboard_tmp[i][j];
                    }
                    board_to_chess();
                }
            }
        }
    }
    srand(time(0)*int(evaluation*100));
    next_step=step_choice[rand()%step_choice[0].x1+1];
    return next_step;
}
double ai_play(int depth,int minimax_depth){
    depth++;
    if(depth>minimax_depth || judge())return evaluate();
    //寻找最优步
    int x1,y1,x2,y2;
    double evaluation=0;
    double max_evaluation=-1000;
    for(int i0=1;i0<=chess[cur_color][0].x;i0++){
        x1=chess[cur_color][i0].x;y1=chess[cur_color][i0].y;
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
                    reverse_chess(x2,y2,cur_color);
                    if(max(abs(p),abs(q))==2)chessboard[x1][y1]=0;
                    board_to_chess();
                    //估值
                    if(evaluate()>=max_evaluation){
                        evaluation=human_play(depth,minimax_depth);
                        if(evaluation>max_evaluation){
                            max_evaluation=evaluation;
                        }
                    }
                    //分析完毕，恢复棋盘
                    for(int i=1;i<=7;i++){
                        for(int j=1;j<=7;j++)chessboard[i][j]=chessboard_tmp[i][j];
                    }
                    board_to_chess();
                }
            }
        }
    }
    return max_evaluation;
}
double human_play(int depth,int minimax_depth){
    depth++;
    if(depth>minimax_depth || judge())return evaluate();
    //寻找最优步
    int x1,y1,x2,y2;
    double evaluation=0;
    double min_evaluation=1000;
    for(int i0=1;i0<=chess[3-cur_color][0].x;i0++){
        x1=chess[3-cur_color][i0].x;y1=chess[3-cur_color][i0].y;
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
                    chessboard[x2][y2]=3-cur_color;
                    reverse_chess(x2,y2,3-cur_color);
                    if(max(abs(p),abs(q))==2)chessboard[x1][y1]=0;
                    board_to_chess();
                    //估值
                    if(evaluate()<=min_evaluation){
                        evaluation=ai_play(depth,minimax_depth);
                        if(evaluation<min_evaluation){
                            min_evaluation=evaluation;
                        }
                    }
                    //分析完毕，恢复棋盘
                    for(int i=1;i<=7;i++){
                        for(int j=1;j<=7;j++)chessboard[i][j]=chessboard_tmp[i][j];
                    }
                    board_to_chess();
                }
            }
        }
    }
    return min_evaluation;
}
double evaluate(){
    if(judge()==cur_color)return 1;
    else if(judge()==3-cur_color)return -1;
    else return (double)chess[cur_color][0].x/(double)(chess[1][0].x+chess[2][0].x);
    return 0;
}