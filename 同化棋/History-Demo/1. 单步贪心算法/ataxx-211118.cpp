#include <iostream>
#include <cstring>
#include <fstream>
#include <algorithm>
using namespace std;

//以下函数用于实现基本的游戏选项、数据存储、走子和胜负判断
void welcome();//初始选择界面，同时根据用户选择新游戏或读档初始化棋盘
void init();//在welcome()中调用，初始化所有变量
void chessboard_init(int,int);//在mode_choose()中被调用，初始化棋盘和参数
void print_chessboard();//在move_chess()中被调用，清空当前屏幕并打印棋盘
void mode_choose(int);//在welcome()中被调用，选择单双人、先后手，参数表示新开或读档，同时新建文件以供复盘
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
void board_to_chess();//在move_chess()和chessboard_init()中被调用，通过棋盘情况给chess[]赋值
void save_progress();//在move_chess()中被调用，保存每一步，从而支持复盘
void re_progress();//复盘函数，在mode_choose()中被调用
int in_chessboard(int,int);//不知道写了多少代码才发现需要这样一个函数，真是罪过
//以上函数用于实现基本的游戏选项、走子和胜负判断

int init_or_gote=0;//在mode_choose()被赋值（新开游戏）或在init()中被赋值（读取存档）
//用于判断单人游戏时玩家执何方，在print_chessboard()中作为信息打印
int mode_choice=0;//在mode_choose()中被赋值，用于判断单人游玩还是双人游玩
//同时将运用于save()、init()、chessboard_init()中，在print_chessboard()中应用以判断应读入/输出哪一类存档
int welcome_choose=0;//在welcome()中被调用，用于判断是新开游戏还是读取存档
int save_flag=0;//判断是否保存并退出，在save()中赋值，在judge()中使用
int give_in_flag=0;//判断是否保存并退出，在give_in()中赋值，在judge()中使用
int chessboard[8][8]={0};//棋盘，0表示空，1表示●，2表示○
int cur_color=1;//当前行动方，1表示●，2表示○，用cur_color=3-cur_color交换执手
struct Chess{int x=0,y=0;}chess[3][50];//Chess[][0].x表示该种棋子个数
//chess[1][1]-chess[1][chessnum[1]]表示所有的●，chess[2][1]-chess[2][chessnum[2]]表示所有的○
string chessman[3]={" ","●","○"};

//以下是在ai_move()中调用的核心函数
struct Case{int x1=0,y1=0,x2=0,y2=0,factors_mixed=-1000;};//用于返回决策结果到ai_move()中
Case get_factors_mixed(int);//对当前棋局状况，判断多种因素的影响并计算权重，求factors_mixed时被调用
int can_absorb(int,int);//判断走出一步之后能同化多少棋子
int max_matrix(int);//走一步之后棋盘中最大的矩形
//以上是在ai_move()中调用的核心函数

int main(){
    WELCOME:
    welcome();
    goto WELCOME;
    return 0;
}
void welcome(){
    system("cls");
    init();
    cout << "输入相应数字以实现相应功能：\n"
         << "1-开始新游戏\n"
         << "2-读取存档\n"
         << "3-退出游戏\n";
    cin >> welcome_choose;
    if(welcome_choose==3)exit(0);
    else mode_choose(welcome_choose);
    return;
}
void init(){
    init_or_gote=0;mode_choice=0;welcome_choose=0;
    save_flag=0;give_in_flag=0;cur_color=1;
    for(int i=0;i<=7;i++){
        for(int j=0;j<=7;j++)chessboard[i][j]=0;
    }
    board_to_chess();
    return;
}
void chessboard_init(int new_or_old,int sin_or_dou){
    //确定棋盘上某位置的棋子、某棋子在棋盘何处，统计棋子个数
    //确定当前执子方和单人模式下玩家选择的执子方
    if(new_or_old==1){
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
    }
    else {
        string name;
        if(mode_choice==1)name="single.txt";
        else name="double.txt";
        ifstream fin;
        fin.open(name);
        //读入cur_color和init_or_gote
        fin >> cur_color >> init_or_gote;
        //读入chessboard[][]
        for(int i=1;i<=7;i++){
            for(int j=1;j<=7;j++)fin >> chessboard[i][j];
        }
        //根据chessboard[][]获得chess[][]
        board_to_chess();

        fin.close();
    }
}
void print_chessboard(){
    system("cls");
    cout << "\t当前执手方：" << chessman[cur_color];
    if(mode_choice==1){
        if(cur_color==init_or_gote)cout << "(HUMAN)";
        else cout << "(AI)";
    }
    cout << "\t\t●数量：" << chess[1][0].x
         << "\t○数量：" << chess[2][0].x << "\n\n";
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
void mode_choose(int new_or_old){
    cout << "请选择游戏模式\n"
         << "1-单人游戏\n"
         << "2-双人游戏\n";
    cin >> mode_choice;
    //调用初始化模块
    chessboard_init(new_or_old,mode_choice);

    //当新开单人游戏，选择先后手
    if(mode_choice==1 && new_or_old==1){
        cout << "请选择游戏模式\n"
             << "1-先手\n"
             << "2-后手\n";
        cin >> init_or_gote;
    }
    //建立复盘文件
    ofstream fout("save.txt",ios::out | ios::trunc);
    fout << mode_choice << ' ' << init_or_gote << endl;
    save_progress();
    fout.close();
    //开始游戏
    print_chessboard();
    if(mode_choice==2) double_playing();
    else single_playing(init_or_gote);

    //观看复盘
    cout << "是否查看复盘\n"
         << "1. 查看复盘\n"
         << "2. 重新游戏\n";
    int watch_re_progress;cin >> watch_re_progress;
    if(watch_re_progress==1)re_progress();
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
    //判断是否保存并退出
    if(save_flag){
        save_flag=0;return 1;
    }
    //判断是否投降
    if(give_in_flag){
        give_in_flag=1;return 1;
    }
    //判断游戏能否继续进行
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
        //对新的情况，统计棋子
        board_to_chess();
        //交换执手
        cur_color=3-cur_color;
        save_progress();
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
    if(in_chessboard(x1,y1) && in_chessboard(x2,y2)){
        result=max(abs(x2-x1),abs(y2-y1));
        //落子位置是否在24个格子中
        if(result>=3)return 0;
        //判断落子位置有无棋子
        if(chessboard[x2][y2])return 0;
        //判断起手位置棋子是否正确
        if(chessboard[x1][y1]!=cur_color)return 0;
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
void save(){
    string name;
    if(mode_choice==1)name="single.txt";
    else name="double.txt";
    ofstream fout(name,ios::out | ios::trunc);
    //第1行保存cur_color
    //第2行保存单人游戏时玩家选择的执方
    //第3至9行每行7个数，保存chessboard[][]
    fout << cur_color << endl;
    fout << init_or_gote << endl;
    for(int i=1;i<=7;i++){
        for(int j=1;j<=7;j++){
            fout << chessboard[i][j] << ' ';
        }
        fout << "\n";
    }
    fout.close();
    save_flag=1;
    return;
}
void give_in(){
    cout << chessman[cur_color] << "方认输，"
         << chessman[3-cur_color] << "方获胜\n";
    give_in_flag=1;
    system("pause");
    return;
}
void ai_move(int color){
    //color若为1，则执●，若为2，则执○
    int x1,y1,x2,y2;
    //核心算法部分：寻找合适的x1,y1,x2,y2
    /*
    **1. 行动后能同化的棋子的数目，用can_absorb(x2,y2)函数判断，越多越好
    **2. 走出一格还是两格，用move_judge(x1,y1,x2,y2)函数判断，越少越好
    合并1. 和2. 走出后相差的最大值
    **3. 走出一步之后，对手下一步所能同化的棋子数目，越少越好
    **4. 走出一步之后，对手下一步走到同化棋子最多处的格数，越多越好
    **5. 假如我有一大坨聚集起来，对方更不容易同化我，所以要尽可能团成团，尽可能尝试构造更大的矩形
    */
    Case next_step=get_factors_mixed(cur_color);
    x1=next_step.x1;y1=next_step.y1;
    x2=next_step.x2;y2=next_step.y2;
    //移动棋子
    move_chess(x1,y1,x2,y2);
    cout << "电脑将棋子从(" << x1 << "," << y1 << ")"
         << "移动到(" << x2 << "," << y2 << ").\n";
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
void save_progress(){
    ofstream fout("save.txt",ios::out | ios::app);  
    for(int i=1;i<=7;i++){
        fout << endl;
        for(int j=1;j<=7;j++)fout << chessboard[i][j] << ' ';
    }
    fout.close();
    return;
}
void re_progress(){
    ifstream fin;
    fin.open("save.txt");
    fin >> mode_choice >> init_or_gote;
    if(welcome_choose==1)cur_color=1;
    else {
        string name;
        if(mode_choice==1)name="single.txt";
        else name="double.txt";
        ifstream fin;
        fin.open(name);
        fin >> cur_color;
        fin.close();
    }
    while(fin >> chessboard[1][1]){
        for(int i=1;i<=7;i++){
            for(int j=1;j<=7;j++){
                if((i+j)!=2)fin >> chessboard[i][j];
            }
        }
        print_chessboard();
        cur_color=3-cur_color;
        system("pause");
    }
    fin.close();
    cout << "复盘结束" << endl;
    system("pause");
    return;
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
    for(int i0=1;i0<=chess[color][0].x;i0++){
        x1=chess[color][i0].x;y1=chess[color][i0].y;
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
                    factors_mixed=(chess[color][0].x-chess[3-color][0].x)-move_judge(x1,y1,x2,y2)*3;
                    //判断走完子后棋盘上最大的矩形
                    factors_mixed+=max_matrix(color);
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
int max_matrix(int color){
    int max_m=0,min_m=49;
    for(int i_begin=1;i_begin<=7;i_begin++){
        for(int i_end=1;i_end<=7;i_end++){
            for(int j_begin=1;j_begin<=7;j_begin++){
                for(int j_end=1;j_end<=7;j_end++){
                    int flag=0;
                    for(int i=i_begin;i<=i_end;i++){
                        for(int j=j_begin;j<=j_end;j++){
                            if(chessboard[i][j]!=color){
                                flag=1;
                                break;
                            }
                        }
                        if(flag)break;
                    }
                    if(flag)break;
                    else max_m=max(max_m,(i_end-i_begin+1)*(j_end-j_begin+1));
                }
            }
        }
    }
    return max_m;
}