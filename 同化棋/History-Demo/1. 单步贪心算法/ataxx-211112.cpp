#include <iostream>
#include <cstring>
using namespace std;

//以下函数用于实现基本的游戏选项、数据存储、走子和胜负判断
void welcome();//初始选择界面，同时根据用户选择新游戏或读档初始化棋盘
void init(int);//在welcome()中被调用，初始化其他参数
void chessboard_init(int);//在welcome()中被调用，初始化棋盘
void print_chessboard();//在move_chess()中被调用，清空当前屏幕并打印棋盘
void mode_choose();//在welcome()中被调用，选择单双人、先后手
void single_playing(int);//在mode_choose()中被调用，进行单人游戏整数值表示先手或后手
void double_playing();//在mode_choose()中被调用，进行双人游戏
int judge();//判断胜负，每次落子之后执行
int can_move(int,int);//在judge()中被调用，判断是否有棋子能动
int can_move_to(int,int);//在can_move()中被调用
void input_move();//在single_playing()和double_playing()中读入用户操作，移动棋子、认输、存盘退出
void move_chess(int,int,int,int);//移动棋子，翻转棋子，交换行动方，打印棋盘
int move_judge(int,int,int,int);//在move_chess()中被调用，判断移动是否合理
void reverse_chess(int,int);//在move_chess()中被调用，翻转棋子
void do_reverse(int,int);//在reverse_chess()中被调用，实现棋子翻转的判定和执行
void save();//在input_move()中被调用，存盘退出
void give_in();//在input_move()中被调用，投降认输
void ai_move(int);//在single_playing()中被调用，AI走子
//以上函数用于实现基本的游戏选项、走子和胜负判断

//下面的函数生成随机数，仅做ai_move()测试使用
int get_random_number(int);
#include <cstdlib>
#include <ctime>
int get_random_number(int x){
    srand((unsigned)time(NULL));
    return rand()%x+1;
}
//上面的函数生成随机数，仅做ai_move()测试使用

int chessboard[8][8]={0};//棋盘，0表示空，1表示●，2表示○
int cur_color=1;//当前行动方，1表示●，2表示○，用cur_color=3-cur_color交换执手
int chess_num[3]={0};//●○棋子的个数，chess_num[1]为●数，chess_num[2]为○数
struct Chess{int x=0,y=0;}chess[3][50];//Chess[][0].x表示该种棋子个数
//chess[1][1]-chess[1][chessnum[1]]表示所有的●，chess[2][1]-chess[2][chessnum[2]]表示所有的○
string chessman[3]={" ","●","○"};

int main(){
    WELCOME:
    welcome();
    goto WELCOME;
    return 0;
}
void welcome(){
    system("cls");
    cout << "输入相应数字以实现相应功能：\n"
         << "1-开始新游戏\n"
         << "2-读取存档\n"
         << "3-退出游戏\n";
    int welcome_choose;cin >> welcome_choose;
    if(welcome_choose==3)exit(0);
    else {
        chessboard_init(welcome_choose);
        init(welcome_choose);
        mode_choose();
    }
    return;
}
void init(int choose){
    if(choose==1){
        cur_color=1;
    }
    //else 读入文件，需要cur_color的相关信息
    return;
}
void chessboard_init(int choose){
    //确定棋盘上某位置的棋子、某棋子在棋盘何处，统计棋子个数
    if(choose==1){
        for(int i=1;i<=7;i++){
            for(int j=1;j<=7;j++)chessboard[i][j]=0;
        }
        chessboard[1][1]=1;chessboard[7][7]=1;
        chessboard[1][7]=2;chessboard[7][1]=2;
        chess_num[1]=2;chess_num[2]=2;
        chess[1][0].x=2;
        chess[1][1].x=1;chess[1][1].y=1;
        chess[1][2].x=7;chess[1][2].y=7;
        chess[2][0].x=2;
        chess[2][1].x=1;chess[2][1].y=7;
        chess[2][2].x=7;chess[2][2].y=1;
    }
    //else 读入文件，需要chessboard，chess，chess_num的相关信息
}
void print_chessboard(){
    system("cls");
    cout << "\t当前执手方：" << chessman[cur_color]
         << "\t\t●方子数：" << chess_num[1]
         << "\t○方子数：" << chess_num[2] << "\n\n";
    cout << "\t    1       2       3       4       5       6       7\n\n";
    cout << "\t┏━━━━━━━┳━━━━━━━┳━━━━━━━┳━━━━━━━┳━━━━━━━┳━━━━━━━┳━━━━━━━┓\n";
    for(int i=1;i<=7;i++){
        cout << "\t┃\t┃\t┃\t┃\t┃\t┃\t┃\t┃\n";
        cout << "  " << i << "\t┃   ";
        for(int j=1;j<=7;j++){
            cout << chessman[chessboard[i][j]] << "\t┃   ";
        }
        cout << "\n";
        cout << "\t┃\t┃\t┃\t┃\t┃\t┃\t┃\t┃\n";

        if(i==7){
            cout << "\t┗━━━━━━━┻━━━━━━━┻━━━━━━━┻━━━━━━━┻━━━━━━━┻━━━━━━━┻━━━━━━━┛\n";
        }
        else {
            cout << "\t┣━━━━━━━╋━━━━━━━╋━━━━━━━╋━━━━━━━╋━━━━━━━╋━━━━━━━╋━━━━━━━┫\n";
        }
    }

    cout << "输入规则：\n"
         << "1. 输入用空格分隔的四个1-7之间的数\n"
         << "    前两个数x1,y1表示要移动的棋子位置在自上向下第x1行,自左向右第y1列\n"
         << "    后两个数x2,y2表示想要移动到的位置在自上向下第x2行,自左向右第y2列\n"
         << "2. 输入0，保存游戏并退出\n"
         << "3. 输入8，认输\n";
    return;
}
void mode_choose(){
    cout << "请选择游戏模式\n"
         << "1-单人游戏\n"
         << "2-双人游戏\n";
    int mode_choice;cin >> mode_choice;
    if(mode_choice==2){
        print_chessboard();
        double_playing();
    }
    else {
        cout << "请选择游戏模式\n"
             << "1-先手\n"
             << "2-后手\n";
        int init_or_gote;cin >> init_or_gote;
        print_chessboard();
        single_playing(init_or_gote);
    }
    return;
}
void single_playing(int init_or_gote){
    while(true){
        if(cur_color==init_or_gote){
            input_move();
            if(judge())break;
        }
        else {
            system("pause");//这里为了让玩家看清楚
            ai_move(3-init_or_gote);
            if(judge())break;
        }
    }
    return;
}
void double_playing(){
    while(true){
        input_move();
        if(judge())break;
    }
    return;
}
int judge(){
    int result=0;
    if(chess_num[1]==0)result=2;
    else if(chess_num[2]==0)result=1;
    else {
        for(int i=1;i<=7;i++){
            for(int j=1;j<=7;j++){
                if(
                    chessboard[i][j]==cur_color
                    && can_move(i,j)
                )return 0;
            }
        }
        if(chess_num[1]>chess_num[2])result=1;
        else if(chess_num[2]>chess_num[1])result=2;
        else result=3;
    }
    switch(result){
        case 0:break;
        case 1:
        case 2:{
            cout << chessman[result] << "方获胜\n";
            system("pause");
            break;
        }
        case 3:{
            cout << "平局\n";
            system("pause");
            break;
        }
    }
    return result;
}
int can_move(int x,int y){
    if(can_move_to(x-1,y-1))return 1;
    if(can_move_to(x-1,y))return 1;
    if(can_move_to(x-1,y+1))return 1;
    if(can_move_to(x,y-1))return 1;
    if(can_move_to(x,y+1))return 1;
    if(can_move_to(x+1,y-1))return 1;
    if(can_move_to(x+1,y))return 1;
    if(can_move_to(x+1,y+1))return 1;
    return 0;
}
int can_move_to(int x,int y){
    if(
        x>=1 && x<=7
        && y>=1 && y<=7
        && chessboard[x][y]==0
    )return 1;
    return 0;
}
void input_move(){
    /*先读入第一个数据
    **若为1-7，说明移动棋盘，继续读入，并尝试移动棋子
    **若为0，说明存盘退出
    **若为8，说明认输*/
    int x1;cin >> x1;
    if(x1>=1 && x1<=7){
        int y1,x2,y2;cin >> y1 >> x2 >> y2;
        move_chess(x1,y1,x2,y2);
    }
    else if(x1==0)save();
    else if(x1==8)give_in();
    else{
        cin >> x1 >> x1 >> x1;
        print_chessboard();
        cout << "输入有误，请重新输入\n";
    }
    return;
}
void move_chess(int x1,int y1,int x2,int y2){
    int judge_tmp=move_judge(x1,y1,x2,y2);
    if(judge_tmp!=0) {
        chessboard[x2][y2]=cur_color;
        reverse_chess(x2,y2);
        if(judge_tmp==2)chessboard[x1][y1]=0;
        else chess_num[cur_color]++;
        //对新的情况，统计棋子数目
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
        //交换执手
        cur_color=3-cur_color;
    }
    print_chessboard();
    if(judge_tmp==0)cout << "输入有误，请重新输入\n";
    return;
}
int move_judge(int x1,int y1,int x2,int y2){
    /*若移动不合理，返回0
    **若移动到临近一格，返回1
    **若移动到临近两格，返回2*/
    int result=0;
    //判断是否超出棋盘
    if(
        y1>=1 && y1<=7
        && x2>=1 && x2<=7
        && y2>=1 && y2<=7
    ){
        result=max(abs(x2-x1),abs(y2-y1));
        //落子位置是否在24个格子中
        if(result>=3)return 0;
        //判断落子位置有无棋子
        if(chessboard[x2][y2])return 0;
        //判断起手位置有无棋子
        if(!chessboard[x1][y1])return 0;
    }
    return result;
}
void reverse_chess(int x,int y){
    do_reverse(x-1,y-1);
    do_reverse(x-1,y);
    do_reverse(x-1,y+1);
    do_reverse(x,y-1);
    do_reverse(x,y+1);
    do_reverse(x+1,y-1);
    do_reverse(x+1,y);
    do_reverse(x+1,y+1);
    return;
}
void do_reverse(int x,int y){
    if(
        x>=1 && x<=7
        && y>=1 && y<=7
        && chessboard[x][y]==3-cur_color
    ){
        chessboard[x][y]=cur_color;
        chess_num[cur_color]++;
        chess_num[3-cur_color]--;
    }
    return;
}
void save(){
    return;
}
void give_in(){
    cout << chessman[cur_color] << "方认输，"
         << chessman[3-cur_color] << "方获胜\n";
    system("pause");
    welcome();
    return;
}
void ai_move(int color){
    //color若为1，则执●，若为2，则执○
    int x1,y1,x2,y2;
    //核心算法部分：寻找合适的x1,y1,x2,y2

    //以下是采用随机数的算法，旨在验证程序可以正常运行
    int a=get_random_number(chess_num[color]);int a_tmp=a;
    FIND_APPROPRIATE_MOVES:
    x1=chess[color][a].x;y1=chess[color][a].y;
    int random_step=get_random_number(24);
    int random_step_tmp=random_step;
    while(true){
        int i=random_step%5-2,j=random_step/5-2;
        x2=x1+i;
        y2=y1+j;
        if(move_judge(x1,y1,x2,y2)){
            move_chess(x1,y1,x2,y2);
            cout << "电脑将棋子从(" << x1 << "," << y1 << ")"
                 << "移动到(" << x2 << "," << y2 << ").\n";
            return;
        }
        random_step=random_step%24+1;
        if(random_step==random_step_tmp)return;
    }
    a=a%chess_num[color]+1;
    if(a==a_tmp)return;
    else goto FIND_APPROPRIATE_MOVES;
    //以上是采用随机数的算法，旨在验证程序可以正常运行

    //move_chess(x1,y1,x2,y2);
    return;
}