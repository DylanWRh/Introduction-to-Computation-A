#include <iostream>
#include <cstring>
#include <fstream>
#include <ctime>
#include <Windows.h>
//图像绘制头文件
#include "graphics.h"
#include "conio.h"
//打开网页头文件
#include <tchar.h>
#include <assert.h>
#include <shellapi.h>

using namespace std;

//以下函数用于实现基本的游戏选项、数据存储、走子和胜负判断
void welcome();//初始选择界面，同时根据用户选择新游戏或读档初始化棋盘
void init();//在welcome()中调用，初始化所有变量
void chessboard_init(int, int);//在mode_choose()中被调用，初始化棋盘和参数
void print_chessboard_and_play(int, int, int);//在move_chess()中被调用，清空当前屏幕并打印棋盘
void retract_a_move(int);//悔棋
void mode_choose(int);//在welcome()中被调用，选择单双人、先后手，参数表示新开或读档，同时新建文件以供复盘
int judge();//判断胜负，每次落子之后执行
void get_help_of_game();//获取规则和演示等游戏帮助
int can_move(int, int);//在judge()中被调用，判断是否有棋子能动
int can_move_to(int, int);//在can_move()中被调用
void move_chess(int, int, int, int);//移动棋子，翻转棋子，交换行动方，打印棋盘
int move_judge(int, int, int, int);//在move_chess()中被调用，判断移动是否合理
void reverse_chess(int, int, int);//在move_chess()中被调用，翻转棋子
void do_reverse(int, int, int);//在reverse_chess()中被调用，实现棋子翻转的判定和执行
void save();//在input_move()中被调用，存盘退出
void watch_re_progress();//查看复盘
void board_to_chess();//在move_chess()和chessboard_init()中被调用，通过棋盘情况给chess[]赋值
void save_progress(int, int, int, int);//在move_chess()中被调用，保存每一步，从而支持复盘
int in_chessboard(int, int);//不知道写了多少代码才发现需要这样一个函数，真是罪过
//以上函数用于实现基本的游戏选项、走子和胜负判断

//以下变量作用于游戏结构
int give_in_flag = 0;//判断玩家是否认输
int difficulty = -1;//在mode_choose()被赋值，游戏难度
int init_or_gote = 0;//在mode_choose()被赋值（新开游戏）或在init()中被赋值（读取存档）
//用于判断单人游戏时玩家执何方，在print_chessboard()中作为信息打印
int mode_choice = 0;//在mode_choose()中被赋值，用于判断单人游玩还是双人游玩
//同时将运用于save()、init()、chessboard_init()中，在print_chessboard()中应用以判断应读入/输出哪一类存档
int welcome_choose = 0;//在welcome()中被调用，用于判断是新开游戏还是读取存档
int cur_round = 0;//记录游戏回合
//以上变量作用于游戏结构

//以下变量作用于游戏过程
int chessboard_tmp[8][8] = { 0 };
//用于复盘或悔棋
//在chessboard_tmp[0][1-3]中分别存储着cur_color、●个数、○个数
//在chessboard_tmp[0][0]中存储本步是否轮到AI
//在chessboard_tmp[0][4-7]中存储x1,y1,x2,y2
int chessboard[8][8] = { 0 };//棋盘，0表示空，1表示●，2表示○
int cur_color = 1;//当前行动方，1表示●，2表示○，用cur_color=3-cur_color交换执手
struct Chess { int x = 0, y = 0; }chess[3][50];//Chess[][0].x表示该种棋子个数
//chess[1][1]-chess[1][chessnum[1]]表示所有的●，chess[2][1]-chess[2][chessnum[2]]表示所有的○
//以上变量作用于游戏过程

//以下是在电脑走子过程中调用的核心函数
struct Case { int x1 = 0, y1 = 0, x2 = 0, y2 = 0; double evaluation = -1000; };//用于返回决策结果到ai_move()中
Case minimax(int);
Case step_choice[1000];//minimax()函数中存储所有可能性的数组

Case random_pick_a_step();
double human_play(int, int);
double ai_play(int, int);
double evaluate();
//以上是在电脑走子过程中调用的核心函数

int main() {
WELCOME:
    welcome();
    goto WELCOME;
    return 0;
}
void welcome() {
    //初始化
    init();

    //界面
    initgraph(700, 450);
    setbkcolor(WHITE);
    cleardevice();
    setlinecolor(BLACK);
    setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 5);

    setfillcolor(BLACK);
    fillrectangle(50, 175, 300, 275);
    fillrectangle(50, 325, 300, 425);
    fillrectangle(400, 175, 650, 275);
    fillrectangle(400, 325, 650, 425);

    LOGFONT f;
    gettextstyle(&f);						// 获取当前字体设置
    f.lfHeight = 48;						// 设置字体高度为 48
    _tcscpy_s(f.lfFaceName, _T("黑体"));	// 设置字体为“黑体”(高版本 VC 推荐使用 _tcscpy_s 函数)
    f.lfQuality = ANTIALIASED_QUALITY;		// 设置输出效果为抗锯齿  
    settextstyle(&f);						// 设置字体样式
    setbkmode(TRANSPARENT);
    settextcolor(WHITE);

    outtextxy(75, 200, _T("新开游戏"));
    outtextxy(75, 350, _T("查看帮助"));
    outtextxy(425, 200, _T("读取存档"));
    outtextxy(425, 350, _T("退出游戏"));

    settextcolor(BLACK);
    gettextstyle(&f); f.lfHeight = 100; f.lfWeight = 100;
    f.lfWidth = 60; f.lfUnderline = 1; settextstyle(&f);
    outtextxy(200, 5, _T("ATAXX"));

    gettextstyle(&f); f.lfHeight = 30; 
    f.lfWidth = 15; f.lfUnderline = 0; settextstyle(&f);
    outtextxy(400, 120, _T("——by WRH"));

    MOUSEMSG m;
    while (1) {
        m = GetMouseMsg();
        if (m.uMsg == WM_LBUTTONDOWN) {
            if (m.y >= 175 && m.y <= 275) {
                if (m.x >= 50 && m.x <= 300) {
                    welcome_choose = 1; break;
                }
                else if (m.x >= 400 && m.x <= 650) {
                    welcome_choose = 2; break;
                }
            }
            else if (m.y >= 325 && m.y <= 425) {
                if (m.x >= 50 && m.x <= 300) {
                    welcome_choose = 3; break;
                }
                else if (m.x >= 400 && m.x <= 650) {
                    welcome_choose = 4; break;
                }
            }
        }
    }
    

    if (welcome_choose == 4) {
        closegraph();//关闭窗口
        exit(0);
    }
    else if (welcome_choose == 3) {
        get_help_of_game();//规则帮助
        closegraph();
        welcome_choose = 0;
    }
    else {
        mode_choose(welcome_choose);
        closegraph();//关闭窗口
    }
    return;
}
void get_help_of_game() {
    initgraph(400, 450);
    setbkcolor(WHITE);
    cleardevice();
    setlinecolor(BLACK);
    setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 5);

    setfillcolor(BLACK);
    fillrectangle(100, 25, 300, 125);
    fillrectangle(100, 175, 300, 275);
    fillrectangle(100, 325, 300, 425);

    LOGFONT f;
    gettextstyle(&f);						// 获取当前字体设置
    f.lfHeight = 48;						// 设置字体高度为 48
    _tcscpy_s(f.lfFaceName, _T("黑体"));	// 设置字体为“黑体”(高版本 VC 推荐使用 _tcscpy_s 函数)
    f.lfQuality = ANTIALIASED_QUALITY;		// 设置输出效果为抗锯齿  
    settextstyle(&f);						// 设置字体样式
    setbkmode(TRANSPARENT);
    settextcolor(WHITE);
    outtextxy(105, 50, _T("查看规则"));
    outtextxy(105, 200, _T("观看演示"));
    outtextxy(105, 350, _T("返回上层"));

    int help_choice = 0;
    MOUSEMSG m;
    while (1) {
        m = GetMouseMsg();
        if (m.uMsg == WM_LBUTTONDOWN) {
            if (m.x >= 100 && m.x <= 300) {
                if (m.y >= 25 && m.y <= 125) {
                    help_choice = 1; break;
                }
                if (m.y >= 175 && m.y <= 275) {
                    help_choice = 2; break;
                }
                if (m.y >= 325 && m.y <= 425) {
                    help_choice = 3; break;
                }
            }
        }
    }
    closegraph();//关闭窗口
    
    //返回上层
    if (help_choice == 3) {
        help_choice = 0;
        return;
    }

    //打开网页
    const TCHAR szOperation[] = _T("open");
    const TCHAR szAddress_help[] = _T("https://wiki.botzone.org.cn/index.php?title=Ataxx#");
    const TCHAR szAddress_video[] = _T("https://www.botzone.org.cn/match/580ca98df1001dd47a928bdc");
    if (help_choice == 1) {
        int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR lpCmd, int nShow);
        {
            HINSTANCE hRslt = ShellExecute(NULL, szOperation, szAddress_help, NULL, NULL, SW_SHOWNORMAL);
            assert(hRslt > (HINSTANCE)HINSTANCE_ERROR);
        }
    }
    else if (help_choice == 2) {
        int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR lpCmd, int nShow);
        {
            HINSTANCE hRslt = ShellExecute(NULL, szOperation, szAddress_video, NULL, NULL, SW_SHOWNORMAL);
            assert(hRslt > (HINSTANCE)HINSTANCE_ERROR);
        }
    }
    return;
}
void init() {
    init_or_gote = 0; mode_choice = 0; welcome_choose = 0; give_in_flag = 0;
    cur_color = 1; cur_round = 1; difficulty = -1;
    for (int i = 0; i <= 7; i++) {
        for (int j = 0; j <= 7; j++)chessboard[i][j] = 0;
    }
    board_to_chess();
    return;
}
void chessboard_init(int new_or_old, int sin_or_dou) {
    //复盘棋盘的初始化
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            chessboard_tmp[i][j] = 0;
        }
    }
    //确定棋盘上某位置的棋子、某棋子在棋盘何处，统计棋子个数
    //确定当前执子方和单人模式下玩家选择的执子方
    if (new_or_old == 1) {
        for (int i = 1; i <= 7; i++) {
            for (int j = 1; j <= 7; j++)chessboard[i][j] = 0;
        }
        chessboard[1][1] = 1; chessboard[7][7] = 1;
        chessboard[1][7] = 2; chessboard[7][1] = 2;
        chess[1][0].x = 2;
        chess[1][1].x = 1; chess[1][1].y = 1;
        chess[1][2].x = 7; chess[1][2].y = 7;
        chess[2][0].x = 2;
        chess[2][1].x = 1; chess[2][1].y = 7;
        chess[2][2].x = 7; chess[2][2].y = 1;
        cur_color = 1;
    }
    else {
        string name;
        if (mode_choice == 1)name = "single.txt";
        else name = "double.txt";
        ifstream fin;
        fin.open(name);
        //读入cur_color、init_or_gote和difficulty
        fin >> cur_color >> init_or_gote >> difficulty;
        //读入chessboard[][]
        for (int i = 1; i <= 7; i++) {
            for (int j = 1; j <= 7; j++)fin >> chessboard[i][j];
        }
        //根据chessboard[][]获得chess[][]
        board_to_chess();

        fin.close();
    }
}
void print_chessboard_and_play(int single_or_double, int init_or_gote, int difficulty) {
    initgraph(1300, 800);

    LOGFONT f;
    gettextstyle(&f);						// 获取当前字体设置
    f.lfHeight = 48;						// 设置字体高度为 48
    _tcscpy_s(f.lfFaceName, _T("黑体"));	// 设置字体为“黑体”(高版本 VC 推荐使用 _tcscpy_s 函数)
    f.lfQuality = ANTIALIASED_QUALITY;		// 设置输出效果为抗锯齿  
    settextstyle(&f);						// 设置字体样式
    setbkmode(TRANSPARENT);
    settextcolor(BLACK);

    while (true) {
        //界面
        setbkcolor(WHITE);
        cleardevice();

        setlinecolor(BLACK);
        setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 5);
        for (int i = 1; i <= 7; i++) {
            for (int j = 1; j <= 7; j++) {
                rectangle(100 * i - 50, 100 * j - 50, 100 * i + 50, 100 * j + 50);
            }
        }

        setfillcolor(BLACK);
        for (int i = 1; i <= 7; i++) {
            for (int j = 1; j <= 7; j++) {
                if (chessboard[i][j] == 1)fillcircle(i * 100, j * 100, 30);
                else if (chessboard[i][j] == 2)circle(i * 100, j * 100, 30);
            }
        }

        gettextstyle(&f); f.lfHeight = 48; settextcolor(BLACK); settextstyle(&f);

        outtextxy(800, 80, _T("第")); outtextxy(950, 80, _T("回合"));
        TCHAR s_round[10]; _stprintf_s(s_round, _T("%d"), cur_round); outtextxy(860, 80, s_round);


        outtextxy(800, 230, _T("当前行动方"));
        if (cur_color == 1)fillcircle(1100, 250, 30);
        else if (cur_color == 2)circle(1100, 250, 30);
        if (single_or_double == 1 && init_or_gote != cur_color) {
            outtextxy(1150, 230, _T("(AI)"));
        }

        outtextxy(800, 390, _T("棋子数目"));
        gettextstyle(&f); f.lfHeight = 60; settextstyle(&f);
        fillcircle(1100, 370, 30); circle(1100, 470, 30);
        TCHAR s1[5]; _stprintf_s(s1, _T("%d"), chess[1][0].x); outtextxy(1150, 340, s1);
        TCHAR s2[5]; _stprintf_s(s2, _T("%d"), chess[2][0].x); outtextxy(1150, 440, s2);

        setfillcolor(BLACK);
        fillrectangle(820, 520, 990, 620);//悔棋
        fillrectangle(1050, 520, 1220, 620);//认输
        fillrectangle(820, 650, 990, 750);//存档
        fillrectangle(1050, 650, 1220, 750);//退出
        gettextstyle(&f); f.lfHeight = 60; settextcolor(WHITE); settextstyle(&f);
        outtextxy(845, 535, _T("悔棋"));
        outtextxy(1080, 535, _T("认输"));
        outtextxy(845, 665, _T("存档"));
        outtextxy(1080, 665, _T("退出"));

        //操作
        if (single_or_double == 2 || (single_or_double == 1 && init_or_gote == cur_color)) {
            int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
            MOUSEMSG m;
            while (1) {
            CHOOSE_FIRST:
                m = GetMouseMsg();
                if (m.uMsg == WM_LBUTTONDOWN) {
                    x1 = m.x; y1 = m.y;
                    //走子
                    if (x1 >= 50 && x1 <= 750 && y1 >= 50 && y1 <= 750) {
                        x1 = x1 + 50; y1 = y1 + 50;
                        x1 = x1 / 100; y1 = y1 / 100;
                        if (chessboard[x1][y1] != cur_color)goto CHOOSE_FIRST;

                        //已选中棋子高亮显示
                        setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 5);
                        setlinecolor(RED);
                        rectangle(x1 * 100 - 40, y1 * 100 - 40, x1 * 100 + 40, y1 * 100 + 40);
                        for (int i = -2; i <= 2; i++) {
                            for (int j = -2; j <= 2; j++) {
                                if (can_move_to(x1 + i, y1 + j)) {
                                    if (max(abs(i), abs(j)) == 2)setlinecolor(GREEN);
                                    else if (max(abs(i), abs(j)) == 1)setlinecolor(YELLOW);
                                    rectangle((x1 + i) * 100 - 40, (y1 + j) * 100 - 40, (x1 + i) * 100 + 40, (y1 + j) * 100 + 40);
                                }
                            }
                        }

                        while (1) {
                            m = GetMouseMsg();
                            if (m.uMsg == WM_LBUTTONDOWN) {
                                x2 = m.x + 50; y2 = m.y + 50;
                                if (x2 >= 100 && x2 <= 800 && y2 >= 100 && y2 <= 800) {
                                    x2 = x2 / 100; y2 = y2 / 100;
                                    //不合法，取消高亮
                                    if (chessboard[x2][y2] != 0) {
                                        setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 5);
                                        setlinecolor(WHITE);
                                        for (int i = 1; i <= 7; i++) {
                                            for (int j = 1; j <= 7; j++) {
                                                rectangle(100 * i - 40, 100 * j - 40, 100 * i + 40, 100 * j + 40);
                                            }
                                        }
                                        goto CHOOSE_FIRST;
                                    }
                                    break;
                                }
                                else {
                                    //不合法，取消高亮
                                    setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 5);
                                    setlinecolor(WHITE);
                                    for (int i = 1; i <= 7; i++) {
                                        for (int j = 1; j <= 7; j++) {
                                            rectangle(100 * i - 40, 100 * j - 40, 100 * i + 40, 100 * j + 40);
                                        }
                                    }
                                    goto CHOOSE_FIRST;
                                }
                            }
                        }
                        move_chess(x1, y1, x2, y2);
                        break;
                    }
                    //认输
                    else if (x1 >= 1050 && x1 <= 1220 && y1 >= 520 && y1 <= 620) {
                        give_in_flag = 1;
                        break;
                    }
                    //存档
                    else if (x1 >= 820 && x1 <= 990 && y1 >= 650 && y1 <= 750) {
                        save(); closegraph(); return;
                    }
                    //退出
                    else if (x1 >= 1050 && x1 <= 1220 && y1 >= 650 && y1 <= 750) {
                        closegraph(); exit(0);
                    }
                    //悔棋
                    else if (x1 >= 820 && x1 <= 990 && y1 >= 520 && y1 <= 620) {
                        retract_a_move(single_or_double);
                        break;
                    }
                    //不合法操作
                    else goto CHOOSE_FIRST;
                }
            }
            if (judge() || give_in_flag) { cur_round--; break; }
        }
        else {
            int x1, y1, x2, y2; Case next_step;
            //AI分析
            if (difficulty > 0) {
                next_step = minimax(difficulty);
            }
            else {
                next_step = random_pick_a_step();
            }
            //移动
            x1 = next_step.x1; y1 = next_step.y1;
            x2 = next_step.x2; y2 = next_step.y2;
            move_chess(x1, y1, x2, y2);
            //显示AI走子高亮
            setlinecolor(RED);
            rectangle(x1 * 100 - 40, y1 * 100 - 40, x1 * 100 + 40, y1 * 100 + 40);
            Sleep(300);
            setlinecolor(GREEN);
            rectangle(x2 * 100 - 40, y2 * 100 - 40, x2 * 100 + 40, y2 * 100 + 40);
            Sleep(500);
            setlinecolor(WHITE);
            rectangle(x1 * 100 - 40, y1 * 100 - 40, x1 * 100 + 40, y1 * 100 + 40);
            rectangle(x2 * 100 - 40, y2 * 100 - 40, x2 * 100 + 40, y2 * 100 + 40);
            if (judge()) { cur_round--; break; }
        }
    }
    
    //显示最后一步棋的棋盘
    setbkcolor(WHITE);
    cleardevice();

    setlinecolor(BLACK);
    setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 5);
    for (int i = 1; i <= 7; i++) {
        for (int j = 1; j <= 7; j++) {
            rectangle(100 * i - 50, 100 * j - 50, 100 * i + 50, 100 * j + 50);
        }
    }

    setfillcolor(BLACK);
    for (int i = 1; i <= 7; i++) {
        for (int j = 1; j <= 7; j++) {
            if (chessboard[i][j] == 1)fillcircle(i * 100, j * 100, 30);
            else if (chessboard[i][j] == 2)circle(i * 100, j * 100, 30);
        }
    }

    gettextstyle(&f); f.lfHeight = 48; settextcolor(BLACK); settextstyle(&f);

    outtextxy(800, 80, _T("第")); outtextxy(950, 80, _T("回合"));
    TCHAR s_round[10]; _stprintf_s(s_round, _T("%d"), cur_round); outtextxy(860, 80, s_round);


    outtextxy(800, 230, _T("当前行动方"));
    if (cur_color == 1)fillcircle(1100, 250, 30);
    else if (cur_color == 2)circle(1100, 250, 30);
    if (single_or_double == 1 && init_or_gote != cur_color) {
        outtextxy(1150, 230, _T("(AI)"));
    }

    outtextxy(800, 390, _T("棋子数目"));
    gettextstyle(&f); f.lfHeight = 60; settextstyle(&f);
    fillcircle(1100, 370, 30); circle(1100, 470, 30);
    TCHAR s1[5]; _stprintf_s(s1, _T("%d"), chess[1][0].x); outtextxy(1150, 340, s1);
    TCHAR s2[5]; _stprintf_s(s2, _T("%d"), chess[2][0].x); outtextxy(1150, 440, s2);

    setfillcolor(BLACK);
    fillrectangle(820, 520, 990, 620);//悔棋
    fillrectangle(1050, 520, 1220, 620);//认输
    fillrectangle(820, 650, 990, 750);//存档
    fillrectangle(1050, 650, 1220, 750);//退出
    gettextstyle(&f); f.lfHeight = 60; settextcolor(WHITE); settextstyle(&f);
    outtextxy(845, 535, _T("悔棋"));
    outtextxy(1080, 535, _T("认输"));
    outtextxy(845, 665, _T("存档"));
    outtextxy(1080, 665, _T("退出"));


    setfillcolor(WHITE);
    fillrectangle(100, 300, 700, 500);
    gettextstyle(&f); f.lfHeight = 80; settextcolor(BLACK); settextstyle(&f);
    //游戏结束的情况
    if (!give_in_flag) {
        //指出谁获胜
        int winner = judge();
        if (winner == 1)outtextxy(250, 355, _T("●获胜！"));
        else if (winner == 2)outtextxy(250, 355, _T("○获胜！"));
        else if (winner == 3)outtextxy(250, 355, _T("双方平局"));
    }
    //有一方认输的情况
    else {
        give_in_flag = 0;
        if (cur_color == 1)outtextxy(125, 355, _T("●认输，○获胜"));
        else if (cur_color == 2)outtextxy(125, 355, _T("○认输，●获胜"));
    }

    setfillcolor(WHITE);
    fillrectangle(580, 490, 660, 530);
    gettextstyle(&f); f.lfHeight = 30; settextcolor(BLACK); settextstyle(&f);
    outtextxy(590, 495, _T("确定"));

    MOUSEMSG m;
    while (1) {
        m = GetMouseMsg();
        if (m.uMsg == WM_LBUTTONDOWN) {
            if (m.x >= 580 && m.x <= 660 && m.y >= 490 && m.y <= 530)break;
        }
    }

    //观看复盘
    setfillcolor(WHITE);
    fillrectangle(100, 300, 700, 500);
    gettextstyle(&f); f.lfHeight = 80; settextcolor(BLACK); settextstyle(&f);
    outtextxy(155, 355, _T("是否查看复盘？"));

    setfillcolor(WHITE);
    fillrectangle(480, 490, 560, 530); fillrectangle(580, 490, 660, 530);
    gettextstyle(&f); f.lfHeight = 30; settextcolor(BLACK); settextstyle(&f);
    outtextxy(510, 495, _T("是")); outtextxy(610, 495, _T("否"));

    int watch_reprogress = 0;
    while (1) {
        m = GetMouseMsg();
        if (m.uMsg == WM_LBUTTONDOWN) {
            if (m.y >= 490 && m.y <= 530) {
                if (m.x >= 580 && m.x <= 660)break;
                else if (m.x >= 480 && m.x <= 560) {
                    watch_reprogress = 1;
                    break;
                }
            }
        }
    }
    closegraph();//关闭窗口
    if (watch_reprogress == 1)watch_re_progress();
    return;
}
void retract_a_move(int single_or_double) {
    //成功悔棋
    if ((cur_round >= 3 && single_or_double == 1) || (cur_round >= 2 && single_or_double == 2)) {
        if (single_or_double == 2) {
            cur_round -= 1; cur_color = 3 - cur_color;
        }
        if (single_or_double == 1)cur_round -= 2;
        //读入棋盘和参数
        ifstream fin; fin.open("save.txt");
        //模式，先后手
        fin >> mode_choice >> init_or_gote;
        for (int i = 1; i <= cur_round; i++) {
            int t; fin >> t;
            for (int i = 1; i <= 7; i++) {
                for (int j = 1; j <= 7; j++)fin >> chessboard[i][j];
            }
            fin >> t >> t >> t >> t;
        }
        fin.close();

        //悔棋后修改"save.txt"文件
        //创建临时文件
        ifstream save_in("save.txt");
        ofstream save_out("save_tmp.txt", ios::out | ios::trunc);
        for (int i = 0; i <= cur_round * 9; i++) {
            char save_tmp[100] = { '\0' };
            save_in.getline(save_tmp, 100);
            save_out << save_tmp << endl;
        }
        save_in.close(); save_out.close();
        //修改原文件
        save_in.open("save_tmp.txt");
        save_out.open("save.txt", ios::out | ios::trunc);
        for (int i = 0; i <= cur_round * 9; i++) {
            char save_tmp[100] = { '\0' };
            save_in.getline(save_tmp, 100);
            save_out << save_tmp << endl;
        }
        save_in.close(); save_out.close();
        //删除临时文件
        remove("./save_tmp.txt");
    }
    //不合法操作
    else {
        LOGFONT f;
        gettextstyle(&f);						// 获取当前字体设置
        f.lfHeight = 80;						// 设置字体高度为 48
        _tcscpy_s(f.lfFaceName, _T("黑体"));	// 设置字体为“黑体”(高版本 VC 推荐使用 _tcscpy_s 函数)
        f.lfQuality = ANTIALIASED_QUALITY;		// 设置输出效果为抗锯齿  
        settextstyle(&f);						// 设置字体样式
        setbkmode(TRANSPARENT);
        settextcolor(BLACK);

        setfillcolor(WHITE);
        fillrectangle(100, 300, 700, 500);

        outtextxy(245, 355, _T("已到开头"));
        Sleep(300);
    }
    return;
}
void mode_choose(int new_or_old) {
    START_THE_MODE_CHOOSE_PEOGRESS:

    initgraph(400, 450);
    setbkcolor(WHITE);
    cleardevice();
    setlinecolor(BLACK);
    setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 5);

    setfillcolor(BLACK);
    fillrectangle(100, 25, 300, 125);
    fillrectangle(100, 175, 300, 275);
    fillrectangle(100, 325, 300, 425);

    LOGFONT f;
    gettextstyle(&f);						// 获取当前字体设置
    f.lfHeight = 48;						// 设置字体高度为 48
    _tcscpy_s(f.lfFaceName, _T("黑体"));	// 设置字体为“黑体”(高版本 VC 推荐使用 _tcscpy_s 函数)
    f.lfQuality = ANTIALIASED_QUALITY;		// 设置输出效果为抗锯齿  
    settextstyle(&f);						// 设置字体样式
    setbkmode(TRANSPARENT);
    settextcolor(WHITE);
    outtextxy(105, 50, _T("单人游戏"));
    outtextxy(105, 200, _T("双人游戏"));
    outtextxy(105, 350, _T("返回上层"));


    MOUSEMSG m;
    while (1) {
        m = GetMouseMsg();
        if (m.uMsg == WM_LBUTTONDOWN) {
            if (m.x >= 100 && m.x <= 300) {
                if (m.y >= 25 && m.y <= 125) {
                    mode_choice = 1; break;
                }
                if (m.y >= 175 && m.y <= 275) {
                    mode_choice = 2; break;
                }
                if (m.y >= 325 && m.y <= 425) {
                    mode_choice = 3; break;
                }
            }
        }
    }
    closegraph();//关闭窗口
    if (mode_choice == 3) {
        mode_choice = 0;
        return;
    }

    //调用初始化模块
    chessboard_init(new_or_old, mode_choice);

    //当新开单人游戏，选择先后手
    START_THE_INIT_OR_GOTE_CHOOSE_PROCESS:
    if (mode_choice == 1 && new_or_old == 1) {
        initgraph(400, 450);
        setbkcolor(WHITE);
        cleardevice();
        setlinecolor(BLACK);
        setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 5);

        setfillcolor(BLACK);
        fillrectangle(100, 25, 300, 125);
        fillrectangle(100, 175, 300, 275);
        fillrectangle(100, 325, 300, 425);

        LOGFONT f;
        gettextstyle(&f);						// 获取当前字体设置
        f.lfHeight = 48;						// 设置字体高度为 48
        _tcscpy_s(f.lfFaceName, _T("黑体"));	// 设置字体为“黑体”(高版本 VC 推荐使用 _tcscpy_s 函数)
        f.lfQuality = ANTIALIASED_QUALITY;		// 设置输出效果为抗锯齿  
        settextstyle(&f);						// 设置字体样式
        setbkmode(TRANSPARENT);
        settextcolor(WHITE);
        outtextxy(155, 50, _T("先手"));
        outtextxy(155, 200, _T("后手"));
        outtextxy(105, 350, _T("返回上层"));


        MOUSEMSG m;
        while (1) {
            m = GetMouseMsg();
            if (m.uMsg == WM_LBUTTONDOWN) {
                if (m.x >= 100 && m.x <= 300) {
                    if (m.y >= 25 && m.y <= 125) {
                        init_or_gote = 1; break;
                    }
                    if (m.y >= 175 && m.y <= 275) {
                        init_or_gote = 2; break;
                    }
                    if (m.y >= 325 && m.y <= 425) {
                        init_or_gote = 3; break;
                    }
                }
            }
        }
        closegraph();//关闭窗口
    }
    if (init_or_gote == 3) {
        init_or_gote = 0;
        goto START_THE_MODE_CHOOSE_PEOGRESS;
    }
    else if (mode_choice == 1) {
        //选择难度
        //界面
        initgraph(400, 450);
        setbkcolor(WHITE);
        cleardevice();
        setlinecolor(BLACK);
        setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 5);

        setfillcolor(BLACK);
        fillrectangle(50, 25, 180, 125);
        fillrectangle(220, 25, 350, 125);
        fillrectangle(50, 175, 180, 275);
        fillrectangle(220, 175, 350, 275);
        fillrectangle(50, 325, 350, 425);

        gettextstyle(&f);						// 获取当前字体设置
        f.lfHeight = 40;						// 设置字体高度
        _tcscpy_s(f.lfFaceName, _T("黑体"));	// 设置字体为“黑体”(高版本 VC 推荐使用 _tcscpy_s 函数)
        f.lfQuality = ANTIALIASED_QUALITY;		// 设置输出效果为抗锯齿  
        settextstyle(&f);						// 设置字体样式
        setbkmode(TRANSPARENT);
        settextcolor(WHITE);
        outtextxy(55, 55, _T("找自信"));
        gettextstyle(&f); f.lfHeight = 48; settextstyle(&f);
        outtextxy(240, 50, _T("简单"));
        outtextxy(70, 200, _T("普通"));
        outtextxy(240, 200, _T("困难"));
        outtextxy(105, 350, _T("返回上层"));

        while (1) {
            m = GetMouseMsg();
            if (m.uMsg == WM_LBUTTONDOWN) {
                if (m.x >= 50 && m.x <= 180 && m.y >= 25 && m.y <= 125) {
                    difficulty = 0; break;
                }
                else if (m.x >= 50 && m.x <= 180 && m.y >= 175 && m.y <= 275) {
                    difficulty = 2; break;
                }
                else if (m.x >= 220 && m.x <= 350 && m.y >= 25 && m.y <= 125) {
                    difficulty = 1; break;
                }
                else if (m.x >= 220 && m.x <= 350 && m.y >= 175 && m.y <= 275) {
                    difficulty = 3; break;
                }
                else if (m.x >= 50 && m.x <= 350 && m.y >= 325 && m.y <= 425) {
                    goto START_THE_INIT_OR_GOTE_CHOOSE_PROCESS;
                }
            }
        }
        closegraph();//关闭窗口
    }

    //建立复盘文件
    ofstream fout("save.txt", ios::out | ios::trunc);
    fout << mode_choice << ' ' << init_or_gote << endl;
    save_progress(0,0,0,0);
    fout.close();

    //开始游戏
    print_chessboard_and_play(mode_choice, init_or_gote, difficulty);

    return;
}
int judge() {
    //判断游戏能否继续进行
    int result = 0;
    if (chess[1][0].x == 0)result = 2;
    else if (chess[2][0].x == 0)result = 1;
    else {
        for (int i = 1; i <= 7; i++) {
            for (int j = 1; j <= 7; j++) {
                if (
                    chessboard[i][j] == cur_color
                    && can_move(i, j)
                    )return 0;
            }
        }
        if (chess[1][0].x > chess[2][0].x)result = 1;
        else if (chess[2][0].x > chess[1][0].x)result = 2;
        else result = 3;
    }
    return result;
}
int can_move(int x, int y) {
    for (int i = -2; i <= 2; i++) {
        for (int j = -2; j <= 2; j++) {
            if (
                (i || j)
                && can_move_to(x + i, y + j)
                )return 1;
        }
    }
    return 0;
}
int can_move_to(int x, int y) {
    if (
        in_chessboard(x, y)
        && chessboard[x][y] == 0
        )return 1;
    return 0;
}
void move_chess(int x1, int y1, int x2, int y2) {
    int judge_tmp = move_judge(x1, y1, x2, y2);
    if (judge_tmp != 0) {
        chessboard[x2][y2] = cur_color;
        reverse_chess(x2, y2, cur_color);
        if (judge_tmp == 2)chessboard[x1][y1] = 0;
        //对新的情况，统计棋子
        board_to_chess();
        //交换执手
        cur_color = 3 - cur_color;
        cur_round++;
        save_progress(x1,y1,x2,y2);
    }
    return;
}
int move_judge(int x1, int y1, int x2, int y2) {
    /*若移动不合理，返回0
    **若移动到临近一格，返回1
    **若移动到临近两格，返回2*/
    int result = 0;
    if (in_chessboard(x1, y1) && in_chessboard(x2, y2)) {
        result = max(abs(x2 - x1), abs(y2 - y1));
        //落子位置是否在24个格子中
        if (result >= 3)return 0;
        //判断落子位置有无棋子
        if (chessboard[x2][y2])return 0;
        //判断起手位置棋子是否正确
        if (chessboard[x1][y1] != cur_color)return 0;
    }
    return result;
}
void reverse_chess(int x, int y, int color) {
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i || j)do_reverse(x + i, y + j, color);
        }
    }
    return;
}
void do_reverse(int x, int y, int color) {
    if (
        in_chessboard(x, y)
        && chessboard[x][y] == 3 - color
        )chessboard[x][y] = color;
    return;
}
void save() {
    string name;
    if (mode_choice == 1)name = "single.txt";
    else name = "double.txt";
    ofstream fout(name, ios::out | ios::trunc);
    //第1行保存cur_color
    //第2行保存单人游戏时玩家选择的执方
    //第3行保存单人游戏时玩家选择的难度
    //第4至10行每行7个数，保存chessboard[][]
    fout << cur_color << endl;
    fout << init_or_gote << endl;
    fout << difficulty << endl;
    for (int i = 1; i <= 7; i++) {
        for (int j = 1; j <= 7; j++) {
            fout << chessboard[i][j] << ' ';
        }
        fout << "\n";
    }
    fout.close();
    return;
}
void watch_re_progress() {
    int re_progress_round = 1;
    int re_progress_round_max = 0;
    //读入总回合数
    ifstream fin;
    fin.open("save.txt");
    fin >> re_progress_round_max >> re_progress_round_max;
    while (fin >> re_progress_round_max) {
        int find_round_max = 0;
        for (int i = 1; i <= 53; i++)fin >> find_round_max;
    }
    fin.close();

    //显示棋盘
    initgraph(1300, 800);

    LOGFONT f;
    gettextstyle(&f);						// 获取当前字体设置
    f.lfHeight = 48;						// 设置字体高度为 48
    _tcscpy_s(f.lfFaceName, _T("黑体"));	// 设置字体为“黑体”(高版本 VC 推荐使用 _tcscpy_s 函数)
    f.lfQuality = ANTIALIASED_QUALITY;		// 设置输出效果为抗锯齿  
    settextstyle(&f);						// 设置字体样式
    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    while (true) {
        //读入棋盘和参数
        ifstream fin; fin.open("save.txt");
        //模式，先后手
        fin >> mode_choice >> init_or_gote;
        //当前执子颜色
        if (welcome_choose == 1)cur_color = 1;
        else {
            string name;
            if (mode_choice == 1)name = "single.txt";
            else name = "double.txt";
            ifstream fin_read;
            fin_read.open(name);
            fin_read >> cur_color;
            fin_read.close();
        }
        for (int i = 1; i <= re_progress_round; i++) {
            int t; fin >> t;
            for (int i = 1; i <= 7; i++) {
                for (int j = 1; j <= 7; j++)fin >> chessboard_tmp[i][j];
            }
            fin >> chessboard_tmp[0][4] >> chessboard_tmp[0][5]
                >> chessboard_tmp[0][6] >> chessboard_tmp[0][7];
            cur_color = 3 - cur_color;
        }
        chessboard_tmp[0][2] = 0; chessboard_tmp[0][3] = 0;
        for (int i = 1; i <= 7; i++) {
            for (int j = 1; j <= 7; j++) {
                if (chessboard_tmp[i][j] == 1) {
                    chessboard_tmp[0][2]++;
                }
                else if (chessboard_tmp[i][j] == 2) {
                    chessboard_tmp[0][3]++;
                }
            }
        }
        chessboard_tmp[0][1] = cur_color;
        chessboard_tmp[0][0] = 0;
        if (mode_choice == 1 && cur_color == init_or_gote) {
            chessboard_tmp[0][0] = 1;
        }
        fin.close();

        //界面
        setbkcolor(WHITE);
        cleardevice();

        setlinecolor(BLACK);
        setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 5);
        for (int i = 1; i <= 7; i++) {
            for (int j = 1; j <= 7; j++) {
                rectangle(100 * i - 50, 100 * j - 50, 100 * i + 50, 100 * j + 50);
            }
        }

        setfillcolor(BLACK);
        for (int i = 1; i <= 7; i++) {
            for (int j = 1; j <= 7; j++) {
                if (chessboard_tmp[i][j] == 1)fillcircle(i * 100, j * 100, 30);
                else if (chessboard_tmp[i][j] == 2)circle(i * 100, j * 100, 30);
            }
        }

        gettextstyle(&f); f.lfHeight = 48; settextcolor(BLACK); settextstyle(&f);

        outtextxy(800, 80, _T("第")); outtextxy(950, 80, _T("回合"));
        TCHAR s_round[10]; _stprintf_s(s_round, _T("%d"), re_progress_round); outtextxy(860, 80, s_round);


        outtextxy(800, 230, _T("当前行动方"));
        if (chessboard_tmp[0][1]==2)fillcircle(1100, 250, 30);
        else if (chessboard_tmp[0][1] == 1)circle(1100, 250, 30);
        if (chessboard_tmp[0][0]) {
            outtextxy(1150, 230, _T("(AI)"));
        }

        outtextxy(800, 390, _T("棋子数目"));
        gettextstyle(&f); f.lfHeight = 60; settextstyle(&f);
        fillcircle(1100, 370, 30); circle(1100, 470, 30);
        TCHAR s1[5]; _stprintf_s(s1, _T("%d"), chessboard_tmp[0][2]); outtextxy(1150, 340, s1);
        TCHAR s2[5]; _stprintf_s(s2, _T("%d"), chessboard_tmp[0][3]); outtextxy(1150, 440, s2);

        setfillcolor(BLACK);
        fillrectangle(820, 520, 990, 620);//存档
        fillrectangle(1050, 520, 1220, 620);//认输
        fillrectangle(820, 650, 1220, 750);//退出
        gettextstyle(&f); f.lfHeight = 48; settextcolor(WHITE); settextstyle(&f);
        outtextxy(835, 550, _T("上一步"));
        outtextxy(1070, 550, _T("下一步"));
        gettextstyle(&f); f.lfHeight = 60; settextcolor(WHITE); settextstyle(&f);
        outtextxy(960, 665, _T("返回"));

        //走子高亮
        int x1, y1, x2, y2;
        x1 = chessboard_tmp[0][4]; y1 = chessboard_tmp[0][5];
        x2 = chessboard_tmp[0][6]; y2 = chessboard_tmp[0][7];
        if (x1) {
            setlinecolor(RED);
            rectangle(x1 * 100 - 40, y1 * 100 - 40, x1 * 100 + 40, y1 * 100 + 40);
            setlinecolor(GREEN);
            rectangle(x2 * 100 - 40, y2 * 100 - 40, x2 * 100 + 40, y2 * 100 + 40);
            setlinecolor(BLACK);
        }
        
        //鼠标输入
        MOUSEMSG m;
        while (1) {
            m = GetMouseMsg();
            if (m.uMsg == WM_LBUTTONDOWN) {
                //下一步
                if (m.x >= 1050 && m.x <= 1220 && m.y >= 520 && m.y <= 620) {
                    if (re_progress_round == re_progress_round_max) {
                        setfillcolor(WHITE);
                        fillrectangle(100, 300, 700, 500);
                        gettextstyle(&f); f.lfHeight = 80; settextcolor(BLACK); settextstyle(&f);
                        outtextxy(245, 355, _T("已到结尾"));
                        Sleep(300);
                        break;
                    }
                    else {
                        re_progress_round++;break;
                    }
                }
                //上一步
                else if (m.x >= 820 && m.x <= 990 && m.y >= 520 && m.y <= 620) {
                    if (re_progress_round == 1) {
                        setfillcolor(WHITE);
                        fillrectangle(100, 300, 700, 500);
                        gettextstyle(&f); f.lfHeight = 80; settextcolor(BLACK); settextstyle(&f);
                        outtextxy(245, 355, _T("已到开局"));
                        Sleep(300);
                        break;
                    }
                    else {
                        re_progress_round--; break;
                    }
                }
                //返回
                else if (m.x >= 820 && m.x <= 1220 && m.y >= 650 && m.y <= 750) {
                    closegraph(); return;
                }
                //不合法操作
                else continue;
            }
        }
    }
    return;
}
void board_to_chess() {
    chess[1][0].x = 0; chess[2][0].x = 0;
    for (int i = 1; i <= 7; i++) {
        for (int j = 1; j <= 7; j++) {
            if (chessboard[i][j] == 1) {
                chess[1][++chess[1][0].x].x = i;
                chess[1][chess[1][0].x].y = j;
            }
            else if (chessboard[i][j] == 2) {
                chess[2][++chess[2][0].x].x = i;
                chess[2][chess[2][0].x].y = j;
            }
        }
    }
}
void save_progress(int x1, int y1, int x2, int y2) {
    ofstream fout("save.txt", ios::out | ios::app);
    fout << cur_round << endl;
    for (int i = 1; i <= 7; i++) {
        for (int j = 1; j <= 7; j++)fout << chessboard[i][j] << ' ';
        fout << endl;
    }
    fout << x1 << ' ' << y1 << ' ' << x2 << ' ' << y2 << endl;
    fout.close();
    return;
}
int can_absorb(int x2, int y2) {
    int result = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (
                (i || j)
                && in_chessboard(x2 + i, y2 + j)
                && chessboard[x2 + i][y2 + j] == 3 - cur_color
                )result++;
        }
    }
    return result;
}
int in_chessboard(int x, int y) {
    if (x >= 1 && x <= 7 && y >= 1 && y <= 7)return true;
    return false;
}
Case random_pick_a_step() {
    Case next_step;
    srand(time(0));
    int a = rand() % chess[cur_color][0].x + 1;
    while (!can_move(chess[cur_color][a].x, chess[cur_color][a].y)) {
        a = a % chess[cur_color][0].x + 1;
    }
    next_step.x1 = chess[cur_color][a].x; next_step.y1 = chess[cur_color][a].y;
    int dx = rand() % 5 - 2, dy = rand() % 5 - 2;
    for (int i = dx;1; i = (i + 3) % 5 - 2) {
        int cnt = 0;
        for (int j = dy; cnt < 5; j = (j + 3) % 5 - 2) {
            cnt++;
            if (can_move_to(next_step.x1 + i, next_step.y1 + j)) {
                next_step.x2 = next_step.x1 + i;
                next_step.y2 = next_step.y1 + j;
                return next_step;
            }
        }
    }
    return next_step;
}
Case minimax(int minimax_depth) {
    int depth = 1;
    //寻找最优步
    Case next_step;
    step_choice[0].x1 = 0; step_choice[0].evaluation = -1000;
    //记录所有的最优步，其中step_choice[0].x1记录最优步个数，以便确定随机数范围
    int x1, y1, x2, y2;
    double evaluation = 0;
    for (int i0 = 1; i0 <= chess[cur_color][0].x; i0++) {
        x1 = chess[cur_color][i0].x; y1 = chess[cur_color][i0].y;
        for (int p = -2; p <= 2; p++) {
            for (int q = -2; q <= 2; q++) {
                x2 = x1 + p; y2 = y1 + q;
                if (can_move_to(x2, y2)) {
                    //留存副本，分析完成后恢复棋盘
                    int chessboard_tmp[8][8] = { 0 };
                    for (int i = 1; i <= 7; i++) {
                        for (int j = 1; j <= 7; j++)chessboard_tmp[i][j] = chessboard[i][j];
                    }
                    //走子，实现下一步判断
                    chessboard[x2][y2] = cur_color;
                    reverse_chess(x2, y2, cur_color);
                    if (max(abs(p), abs(q)) == 2)chessboard[x1][y1] = 0;
                    board_to_chess();
                    //估值
                    if (evaluate() >= next_step.evaluation) {
                        evaluation = human_play(depth, minimax_depth);
                        if (evaluation >= step_choice[0].evaluation) {
                            //若发现估价更大的情况，从零开始计
                            if (evaluation > step_choice[0].evaluation) {
                                step_choice[0].evaluation = evaluation;
                                step_choice[0].x1 = 0;
                            }
                            //将最优步增加到选择项中
                            step_choice[++step_choice[0].x1].evaluation = evaluation;
                            step_choice[step_choice[0].x1].x1 = x1;
                            step_choice[step_choice[0].x1].x2 = x2;
                            step_choice[step_choice[0].x1].y1 = y1;
                            step_choice[step_choice[0].x1].y2 = y2;
                        }
                    }
                    //分析完毕，恢复棋盘
                    for (int i = 1; i <= 7; i++) {
                        for (int j = 1; j <= 7; j++)chessboard[i][j] = chessboard_tmp[i][j];
                    }
                    board_to_chess();
                }
            }
        }
    }
    //随机数种子挑了个看起来比较吉利的东西
    srand(time(0) * int(evaluation * 100));
    next_step = step_choice[rand() % step_choice[0].x1 + 1];
    return next_step;
}
double ai_play(int depth, int minimax_depth) {
    depth++;
    if (depth > minimax_depth || judge())return evaluate();
    //寻找最优步
    int x1, y1, x2, y2;
    double evaluation = 0;
    double max_evaluation = -1000;
    for (int i0 = 1; i0 <= chess[cur_color][0].x; i0++) {
        x1 = chess[cur_color][i0].x; y1 = chess[cur_color][i0].y;
        for (int p = -2; p <= 2; p++) {
            for (int q = -2; q <= 2; q++) {
                x2 = x1 + p; y2 = y1 + q;
                if (can_move_to(x2, y2)) {
                    //留存副本，分析完成后恢复棋盘
                    int chessboard_tmp[8][8] = { 0 };
                    for (int i = 1; i <= 7; i++) {
                        for (int j = 1; j <= 7; j++)chessboard_tmp[i][j] = chessboard[i][j];
                    }
                    //走子，实现下一步判断
                    chessboard[x2][y2] = cur_color;
                    reverse_chess(x2, y2, cur_color);
                    if (max(abs(p), abs(q)) == 2)chessboard[x1][y1] = 0;
                    board_to_chess();
                    //估值
                    if (evaluate() >= max_evaluation) {
                        evaluation = human_play(depth, minimax_depth);
                        if (evaluation > max_evaluation) {
                            max_evaluation = evaluation;
                        }
                    }
                    //分析完毕，恢复棋盘
                    for (int i = 1; i <= 7; i++) {
                        for (int j = 1; j <= 7; j++)chessboard[i][j] = chessboard_tmp[i][j];
                    }
                    board_to_chess();
                }
            }
        }
    }
    return max_evaluation;
}
double human_play(int depth, int minimax_depth) {
    depth++;
    if (depth > minimax_depth || judge())return evaluate();
    //寻找最优步
    int x1, y1, x2, y2;
    double evaluation = 0;
    double min_evaluation = 1000;
    for (int i0 = 1; i0 <= chess[3 - cur_color][0].x; i0++) {
        x1 = chess[3 - cur_color][i0].x; y1 = chess[3 - cur_color][i0].y;
        for (int p = -2; p <= 2; p++) {
            for (int q = -2; q <= 2; q++) {
                x2 = x1 + p; y2 = y1 + q;
                if (can_move_to(x2, y2)) {
                    //留存副本，分析完成后恢复棋盘
                    int chessboard_tmp[8][8] = { 0 };
                    for (int i = 1; i <= 7; i++) {
                        for (int j = 1; j <= 7; j++)chessboard_tmp[i][j] = chessboard[i][j];
                    }
                    //走子，实现下一步判断
                    chessboard[x2][y2] = 3 - cur_color;
                    reverse_chess(x2, y2, 3 - cur_color);
                    if (max(abs(p), abs(q)) == 2)chessboard[x1][y1] = 0;
                    board_to_chess();
                    //估值
                    if (evaluate() <= min_evaluation) {
                        evaluation = ai_play(depth, minimax_depth);
                        if (evaluation < min_evaluation) {
                            min_evaluation = evaluation;
                        }
                    }
                    //分析完毕，恢复棋盘
                    for (int i = 1; i <= 7; i++) {
                        for (int j = 1; j <= 7; j++)chessboard[i][j] = chessboard_tmp[i][j];
                    }
                    board_to_chess();
                }
            }
        }
    }
    return min_evaluation;
}
double evaluate() {
    if (judge() == cur_color)return 1;
    else if (judge() == 3 - cur_color)return -1;
    else return (double)chess[cur_color][0].x / (double)(chess[1][0].x + chess[2][0].x);
    return 0;
}