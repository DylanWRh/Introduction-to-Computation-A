#include <iostream>
#include <cstring>
using namespace std;

//���º�������ʵ�ֻ�������Ϸѡ����ݴ洢�����Ӻ�ʤ���ж�
void welcome();//��ʼѡ����棬ͬʱ�����û�ѡ������Ϸ�������ʼ������
void init(int);//��welcome()�б����ã���ʼ����������
void chessboard_init(int);//��welcome()�б����ã���ʼ������
void print_chessboard();//��move_chess()�б����ã���յ�ǰ��Ļ����ӡ����
void mode_choose();//��welcome()�б����ã�ѡ��˫�ˡ��Ⱥ���
void single_playing(int);//��mode_choose()�б����ã����е�����Ϸ����ֵ��ʾ���ֻ����
void double_playing();//��mode_choose()�б����ã�����˫����Ϸ
int judge();//�ж�ʤ����ÿ������֮��ִ��
int can_move(int,int);//��judge()�б����ã��ж��Ƿ��������ܶ�
int can_move_to(int,int);//��can_move()�б�����
void input_move();//��single_playing()��double_playing()�ж����û��������ƶ����ӡ����䡢�����˳�
void move_chess(int,int,int,int);//�ƶ����ӣ���ת���ӣ������ж�������ӡ����
int move_judge(int,int,int,int);//��move_chess()�б����ã��ж��ƶ��Ƿ����
void reverse_chess(int,int);//��move_chess()�б����ã���ת����
void do_reverse(int,int);//��reverse_chess()�б����ã�ʵ�����ӷ�ת���ж���ִ��
void save();//��input_move()�б����ã������˳�
void give_in();//��input_move()�б����ã�Ͷ������
void ai_move(int);//��single_playing()�б����ã�AI����
//���Ϻ�������ʵ�ֻ�������Ϸѡ����Ӻ�ʤ���ж�

//����ĺ������������������ai_move()����ʹ��
int get_random_number(int);
#include <cstdlib>
#include <ctime>
int get_random_number(int x){
    srand((unsigned)time(NULL));
    return rand()%x+1;
}
//����ĺ������������������ai_move()����ʹ��

int chessboard[8][8]={0};//���̣�0��ʾ�գ�1��ʾ��2��ʾ��
int cur_color=1;//��ǰ�ж�����1��ʾ��2��ʾ����cur_color=3-cur_color����ִ��
int chess_num[3]={0};//������ӵĸ�����chess_num[1]Ϊ������chess_num[2]Ϊ����
struct Chess{int x=0,y=0;}chess[3][50];//Chess[][0].x��ʾ�������Ӹ���
//chess[1][1]-chess[1][chessnum[1]]��ʾ���еġ�chess[2][1]-chess[2][chessnum[2]]��ʾ���еġ�
string chessman[3]={" ","��","��"};

int main(){
    WELCOME:
    welcome();
    goto WELCOME;
    return 0;
}
void welcome(){
    system("cls");
    cout << "������Ӧ������ʵ����Ӧ���ܣ�\n"
         << "1-��ʼ����Ϸ\n"
         << "2-��ȡ�浵\n"
         << "3-�˳���Ϸ\n";
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
    //else �����ļ�����Ҫcur_color�������Ϣ
    return;
}
void chessboard_init(int choose){
    //ȷ��������ĳλ�õ����ӡ�ĳ���������̺δ���ͳ�����Ӹ���
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
    //else �����ļ�����Ҫchessboard��chess��chess_num�������Ϣ
}
void print_chessboard(){
    system("cls");
    cout << "\t��ǰִ�ַ���" << chessman[cur_color]
         << "\t\t��������" << chess_num[1]
         << "\t��������" << chess_num[2] << "\n\n";
    cout << "\t    1       2       3       4       5       6       7\n\n";
    cout << "\t�����������������ש��������������ש��������������ש��������������ש��������������ש��������������ש���������������\n";
    for(int i=1;i<=7;i++){
        cout << "\t��\t��\t��\t��\t��\t��\t��\t��\n";
        cout << "  " << i << "\t��   ";
        for(int j=1;j<=7;j++){
            cout << chessman[chessboard[i][j]] << "\t��   ";
        }
        cout << "\n";
        cout << "\t��\t��\t��\t��\t��\t��\t��\t��\n";

        if(i==7){
            cout << "\t�����������������ߩ��������������ߩ��������������ߩ��������������ߩ��������������ߩ��������������ߩ���������������\n";
        }
        else {
            cout << "\t�ǩ��������������贈�������������贈�������������贈�������������贈�������������贈�������������贈��������������\n";
        }
    }

    cout << "�������\n"
         << "1. �����ÿո�ָ����ĸ�1-7֮�����\n"
         << "    ǰ������x1,y1��ʾҪ�ƶ�������λ�����������µ�x1��,�������ҵ�y1��\n"
         << "    ��������x2,y2��ʾ��Ҫ�ƶ�����λ�����������µ�x2��,�������ҵ�y2��\n"
         << "2. ����0��������Ϸ���˳�\n"
         << "3. ����8������\n";
    return;
}
void mode_choose(){
    cout << "��ѡ����Ϸģʽ\n"
         << "1-������Ϸ\n"
         << "2-˫����Ϸ\n";
    int mode_choice;cin >> mode_choice;
    if(mode_choice==2){
        print_chessboard();
        double_playing();
    }
    else {
        cout << "��ѡ����Ϸģʽ\n"
             << "1-����\n"
             << "2-����\n";
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
            system("pause");//����Ϊ������ҿ����
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
            cout << chessman[result] << "����ʤ\n";
            system("pause");
            break;
        }
        case 3:{
            cout << "ƽ��\n";
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
    /*�ȶ����һ������
    **��Ϊ1-7��˵���ƶ����̣��������룬�������ƶ�����
    **��Ϊ0��˵�������˳�
    **��Ϊ8��˵������*/
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
        cout << "������������������\n";
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
        //���µ������ͳ��������Ŀ
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
        //����ִ��
        cur_color=3-cur_color;
    }
    print_chessboard();
    if(judge_tmp==0)cout << "������������������\n";
    return;
}
int move_judge(int x1,int y1,int x2,int y2){
    /*���ƶ�����������0
    **���ƶ����ٽ�һ�񣬷���1
    **���ƶ����ٽ����񣬷���2*/
    int result=0;
    //�ж��Ƿ񳬳�����
    if(
        y1>=1 && y1<=7
        && x2>=1 && x2<=7
        && y2>=1 && y2<=7
    ){
        result=max(abs(x2-x1),abs(y2-y1));
        //����λ���Ƿ���24��������
        if(result>=3)return 0;
        //�ж�����λ����������
        if(chessboard[x2][y2])return 0;
        //�ж�����λ����������
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
    cout << chessman[cur_color] << "�����䣬"
         << chessman[3-cur_color] << "����ʤ\n";
    system("pause");
    welcome();
    return;
}
void ai_move(int color){
    //color��Ϊ1����ִ����Ϊ2����ִ��
    int x1,y1,x2,y2;
    //�����㷨���֣�Ѱ�Һ��ʵ�x1,y1,x2,y2

    //�����ǲ�����������㷨��ּ����֤���������������
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
            cout << "���Խ����Ӵ�(" << x1 << "," << y1 << ")"
                 << "�ƶ���(" << x2 << "," << y2 << ").\n";
            return;
        }
        random_step=random_step%24+1;
        if(random_step==random_step_tmp)return;
    }
    a=a%chess_num[color]+1;
    if(a==a_tmp)return;
    else goto FIND_APPROPRIATE_MOVES;
    //�����ǲ�����������㷨��ּ����֤���������������

    //move_chess(x1,y1,x2,y2);
    return;
}