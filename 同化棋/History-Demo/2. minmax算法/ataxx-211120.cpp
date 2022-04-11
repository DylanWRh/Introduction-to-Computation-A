#include <iostream>
#include <cstring>
#include <fstream>
#include <algorithm>
using namespace std;

//���º�������ʵ�ֻ�������Ϸѡ����ݴ洢�����Ӻ�ʤ���ж�
void welcome();//��ʼѡ����棬ͬʱ�����û�ѡ������Ϸ�������ʼ������
void init();//��welcome()�е��ã���ʼ�����б���
void chessboard_init(int,int);//��mode_choose()�б����ã���ʼ�����̺Ͳ���
void print_chessboard();//��move_chess()�б����ã���յ�ǰ��Ļ����ӡ����
void mode_choose(int);//��welcome()�б����ã�ѡ��˫�ˡ��Ⱥ��֣�������ʾ�¿��������ͬʱ�½��ļ��Թ�����
void single_playing(int);//��mode_choose()�б����ã����е�����Ϸ����ֵ��ʾ���ֻ����
void double_playing();//��mode_choose()�б����ã�����˫����Ϸ
int judge(int);//�ж�ʤ����ÿ������֮��ִ��
int can_move(int,int);//��judge()�б����ã��ж��Ƿ��������ܶ�
int can_move_to(int,int);//��can_move()�б�����
void input_move();//��single_playing()��double_playing()�ж����û��������ƶ����ӡ����䡢�����˳�
void move_chess(int,int,int,int);//�ƶ����ӣ���ת���ӣ������ж�������ӡ����
int move_judge(int,int,int,int);//��move_chess()�б����ã��ж��ƶ��Ƿ����
void reverse_chess(int,int,int);//��move_chess()�б����ã���ת����
void do_reverse(int,int,int);//��reverse_chess()�б����ã�ʵ�����ӷ�ת���ж���ִ��
void save();//��input_move()�б����ã������˳�
void give_in();//��input_move()�б����ã�Ͷ������
void ai_move();//��single_playing()�б����ã�AI����
void board_to_chess();//��move_chess()��chessboard_init()�б����ã�ͨ�����������chess[]��ֵ
void save_progress();//��move_chess()�б����ã�����ÿһ�����Ӷ�֧�ָ���
void re_progress();//���̺�������mode_choose()�б�����
int in_chessboard(int,int);//��֪��д�˶��ٴ���ŷ�����Ҫ����һ���������������
//���Ϻ�������ʵ�ֻ�������Ϸѡ����Ӻ�ʤ���ж�

int init_or_gote=0;//��mode_choose()����ֵ���¿���Ϸ������init()�б���ֵ����ȡ�浵��
//�����жϵ�����Ϸʱ���ִ�η�����print_chessboard()����Ϊ��Ϣ��ӡ
int mode_choice=0;//��mode_choose()�б���ֵ�������жϵ������滹��˫������
//ͬʱ��������save()��init()��chessboard_init()�У���print_chessboard()��Ӧ�����ж�Ӧ����/�����һ��浵
int welcome_choose=0;//��welcome()�б����ã������ж����¿���Ϸ���Ƕ�ȡ�浵
int save_flag=0;//�ж��Ƿ񱣴沢�˳�����save()�и�ֵ����judge()��ʹ��
int give_in_flag=0;//�ж��Ƿ񱣴沢�˳�����give_in()�и�ֵ����judge()��ʹ��
int chessboard[8][8]={0};//���̣�0��ʾ�գ�1��ʾ��2��ʾ��
int cur_color=1;//��ǰ�ж�����1��ʾ��2��ʾ����cur_color=3-cur_color����ִ��
struct Chess{int x=0,y=0;}chess[3][50];//Chess[][0].x��ʾ�������Ӹ���
//chess[1][1]-chess[1][chessnum[1]]��ʾ���еġ�chess[2][1]-chess[2][chessnum[2]]��ʾ���еġ�
string chessman[3]={" ","��","��"};

//��������ai_move()�е��õĺ��ĺ���
struct Case{int x1=0,y1=0,x2=0,y2=0;double evaluation=-1000;};//���ڷ��ؾ��߽����ai_move()��
Case minimax(int);
double human_play(int,int);
double ai_play(int,int);
double evaluate();
//��������ai_move()�е��õĺ��ĺ���

int main(){
    WELCOME:
    welcome();
    goto WELCOME;
    return 0;
}
void welcome(){
    system("cls");
    init();
    cout << "������Ӧ������ʵ����Ӧ���ܣ�\n"
         << "1-��ʼ����Ϸ\n"
         << "2-��ȡ�浵\n"
         << "3-�˳���Ϸ\n";
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
    //ȷ��������ĳλ�õ����ӡ�ĳ���������̺δ���ͳ�����Ӹ���
    //ȷ����ǰִ�ӷ��͵���ģʽ�����ѡ���ִ�ӷ�
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
        //����cur_color��init_or_gote
        fin >> cur_color >> init_or_gote;
        //����chessboard[][]
        for(int i=1;i<=7;i++){
            for(int j=1;j<=7;j++)fin >> chessboard[i][j];
        }
        //����chessboard[][]���chess[][]
        board_to_chess();

        fin.close();
    }
}
void print_chessboard(){
    system("cls");
    cout << "\t��ǰִ�ַ���" << chessman[cur_color];
    if(mode_choice==1){
        if(cur_color==init_or_gote)cout << "(HUMAN)";
        else cout << "(AI)";
    }
    cout << "\t\t��������" << chess[1][0].x
         << "\t��������" << chess[2][0].x << "\n\n";
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
void mode_choose(int new_or_old){
    cout << "��ѡ����Ϸģʽ\n"
         << "1-������Ϸ\n"
         << "2-˫����Ϸ\n";
    cin >> mode_choice;
    //���ó�ʼ��ģ��
    chessboard_init(new_or_old,mode_choice);

    //���¿�������Ϸ��ѡ���Ⱥ���
    if(mode_choice==1 && new_or_old==1){
        cout << "��ѡ����Ϸģʽ\n"
             << "1-����\n"
             << "2-����\n";
        cin >> init_or_gote;
    }
    //���������ļ�
    ofstream fout("save.txt",ios::out | ios::trunc);
    fout << mode_choice << ' ' << init_or_gote << endl;
    save_progress();
    fout.close();
    //��ʼ��Ϸ
    print_chessboard();
    if(mode_choice==2) double_playing();
    else single_playing(init_or_gote);

    //�ۿ�����
    cout << "�Ƿ�鿴����\n"
         << "1. �鿴����\n"
         << "2. ������Ϸ\n";
    int watch_re_progress;cin >> watch_re_progress;
    if(watch_re_progress==1)re_progress();
    return;
}
void single_playing(int init_or_gote){
    while(true){
        if(cur_color==init_or_gote){
            input_move();
            if(judge(1))break;
        }
        else {
            system("pause");//����Ϊ������ҿ����
            ai_move();
            if(judge(1))break;
        }
    }
    return;
}
void double_playing(){
    while(true){
        input_move();
        if(judge(1))break;
    }
    return;
}
int judge(int during_the_game){
    //�ж��Ƿ񱣴沢�˳�
    if(save_flag){
        save_flag=0;return 1;
    }
    //�ж��Ƿ�Ͷ��
    if(give_in_flag){
        give_in_flag=1;return 1;
    }
    //�ж���Ϸ�ܷ��������
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
    if(during_the_game){
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
        reverse_chess(x2,y2,cur_color);
        if(judge_tmp==2)chessboard[x1][y1]=0;
        //���µ������ͳ������
        board_to_chess();
        //����ִ��
        cur_color=3-cur_color;
        save_progress();
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
    if(in_chessboard(x1,y1) && in_chessboard(x2,y2)){
        result=max(abs(x2-x1),abs(y2-y1));
        //����λ���Ƿ���24��������
        if(result>=3)return 0;
        //�ж�����λ����������
        if(chessboard[x2][y2])return 0;
        //�ж�����λ�������Ƿ���ȷ
        if(chessboard[x1][y1]!=cur_color)return 0;
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
void save(){
    string name;
    if(mode_choice==1)name="single.txt";
    else name="double.txt";
    ofstream fout(name,ios::out | ios::trunc);
    //��1�б���cur_color
    //��2�б��浥����Ϸʱ���ѡ���ִ��
    //��3��9��ÿ��7����������chessboard[][]
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
    cout << chessman[cur_color] << "�����䣬"
         << chessman[3-cur_color] << "����ʤ\n";
    give_in_flag=1;
    system("pause");
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
    cout << "���̽���" << endl;
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
void ai_move(){
    int x1,y1,x2,y2;
    //�����㷨���֣�Ѱ�Һ��ʵ�x1,y1,x2,y2
    Case next_step=minimax(3);
    x1=next_step.x1;y1=next_step.y1;
    x2=next_step.x2;y2=next_step.y2;
    //�ƶ�����
    move_chess(x1,y1,x2,y2);
    cout << "���Խ����Ӵ�(" << x1 << "," << y1 << ")"
         << "�ƶ���(" << x2 << "," << y2 << ").\n";
    return;
}
Case minimax(int minimax_depth){
    int depth=1;
    //Ѱ�����Ų�
    Case next_step;
    int x1,y1,x2,y2;
    double evaluation=0;
    for(int i0=1;i0<=chess[cur_color][0].x;i0++){
        x1=chess[cur_color][i0].x;y1=chess[cur_color][i0].y;
        for(int p=-2;p<=2;p++){
            for(int q=-2;q<=2;q++){
                x2=x1+p;y2=y1+q;
                if(can_move_to(x2,y2)){
                    //���渱����������ɺ�ָ�����
                    int chessboard_tmp[8][8]={0};
                    for(int i=1;i<=7;i++){
                        for(int j=1;j<=7;j++)chessboard_tmp[i][j]=chessboard[i][j];
                    }
                    //���ӣ�ʵ����һ���ж�
                    chessboard[x2][y2]=cur_color;
                    reverse_chess(x2,y2,cur_color);
                    if(max(abs(p),abs(q))==2)chessboard[x1][y1]=0;
                    board_to_chess();
                    //��ֵ
                    if(evaluate()>=next_step.evaluation){
                        evaluation=human_play(depth,minimax_depth);
                        if(evaluation>next_step.evaluation){
                            next_step.evaluation=evaluation;
                            next_step.x1=x1;next_step.y1=y1;
                            next_step.x2=x2;next_step.y2=y2;
                        }
                    }
                    //������ϣ��ָ�����
                    for(int i=1;i<=7;i++){
                        for(int j=1;j<=7;j++)chessboard[i][j]=chessboard_tmp[i][j];
                    }
                    board_to_chess();
                }
            }
        }
    }
    return next_step;
}
double ai_play(int depth,int minimax_depth){
    depth++;
    if(depth>minimax_depth || judge(0))return evaluate();
    //Ѱ�����Ų�
    int x1,y1,x2,y2;
    double evaluation=0;
    double max_evaluation=-1000;
    for(int i0=1;i0<=chess[cur_color][0].x;i0++){
        x1=chess[cur_color][i0].x;y1=chess[cur_color][i0].y;
        for(int p=-2;p<=2;p++){
            for(int q=-2;q<=2;q++){
                x2=x1+p;y2=y1+q;
                if(can_move_to(x2,y2)){
                    //���渱����������ɺ�ָ�����
                    int chessboard_tmp[8][8]={0};
                    for(int i=1;i<=7;i++){
                        for(int j=1;j<=7;j++)chessboard_tmp[i][j]=chessboard[i][j];
                    }
                    //���ӣ�ʵ����һ���ж�
                    chessboard[x2][y2]=cur_color;
                    reverse_chess(x2,y2,cur_color);
                    if(max(abs(p),abs(q))==2)chessboard[x1][y1]=0;
                    board_to_chess();
                    //��ֵ
                    if(evaluate()>=max_evaluation){
                        evaluation=human_play(depth,minimax_depth);
                        if(evaluation>max_evaluation){
                            max_evaluation=evaluation;
                        }
                    }
                    //������ϣ��ָ�����
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
    if(depth>minimax_depth || judge(0))return evaluate();
    //Ѱ�����Ų�
    int x1,y1,x2,y2;
    double evaluation=0;
    double min_evaluation=1000;
    for(int i0=1;i0<=chess[3-cur_color][0].x;i0++){
        x1=chess[3-cur_color][i0].x;y1=chess[3-cur_color][i0].y;
        for(int p=-2;p<=2;p++){
            for(int q=-2;q<=2;q++){
                x2=x1+p;y2=y1+q;
                if(can_move_to(x2,y2)){
                    //���渱����������ɺ�ָ�����
                    int chessboard_tmp[8][8]={0};
                    for(int i=1;i<=7;i++){
                        for(int j=1;j<=7;j++)chessboard_tmp[i][j]=chessboard[i][j];
                    }
                    //���ӣ�ʵ����һ���ж�
                    chessboard[x2][y2]=3-cur_color;
                    reverse_chess(x2,y2,3-cur_color);
                    if(max(abs(p),abs(q))==2)chessboard[x1][y1]=0;
                    board_to_chess();
                    //��ֵ
                    if(evaluate()<=min_evaluation){
                        evaluation=ai_play(depth,minimax_depth);
                        if(evaluation<min_evaluation){
                            min_evaluation=evaluation;
                        }
                    }
                    //������ϣ��ָ�����
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
    if(judge(0)==cur_color)return 1;
    else if(judge(0)==3-cur_color)return -1;
    else return (double)chess[cur_color][0].x/(double)(chess[1][0].x+chess[2][0].x);
    return 0;
}