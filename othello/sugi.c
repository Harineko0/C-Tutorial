#include<stdio.h>

#define N 9       //盤面8*8 + 1(番兵用周囲マス)
#define BLANK 0     //空マス
#define CANPLACE 3 //設置可能マス
#define PLAYER 1    //プレイヤー
#define COMPUTER 2 //コンピューター
#define true 1
#define false 0

/*関数：盤面の初期化*/
void reset(int board[N][N]){
    for(int y=0 ; y <= N ; y++){
        for(int x=0 ; x <= N ; x++){
            board[x][y] = BLANK;
        }
    }
    board[N/2][N/2] = board[N/2+1][N/2+1] = PLAYER;
    board[N/2][N/2+1] = board[N/2+1][N/2] = COMPUTER;

    // Test 1
//    board[4][4] = board[5][4] = board[4][5] = board[5][5] = COMPUTER;
//    board[6][4] = PLAYER;

    // Test 2
//    board[4][4] = board[5][5] = board[4][3] = board[5][3] = COMPUTER;
//    board[6][6] = PLAYER;
}

/*関数：PLAYERの駒数のカウント*/
int o_count(int board[N][N]){
    int counter=0;

    for(int y=1 ; y < N ; y++){
        for(int x=1 ; x < N ; x++){
            if(board[x][y] == PLAYER){
                counter++;
            }
        }
    }
    return(counter);
}

/*関数：COMPUTERの駒数のカウント*/
int x_count(int board[N][N]){
    int counter=0;

    for(int y=1 ; y < N ; y++){
        for(int x=1 ; x < N ; x++){
            if(board[x][y] == COMPUTER){
                counter++;
            }
        }
    }
    return(counter);
}

/*関数：プレイヤーの交代*/
int next_player(int turn){
    if(turn == PLAYER){
        turn = COMPUTER;
    }else if(turn == COMPUTER){
        turn = PLAYER;
    }
    return(turn);
}

/*関数：ターンの表示*/
void print_turn(int turn){
    printf("\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~\n");
    if(turn == PLAYER){
        printf(" PLAYER's turn\n");
    }else if(turn == COMPUTER){
        printf(" COMPUTER's turn\n");
    }
}

/*関数：先攻・後攻の選択*/
int first_turn(void){
    int turn;
    while((turn != 1)&&(turn != 2)){
        printf("\n");
        printf("Deside FIRST(1) or SECOND(2) : ");
        scanf("%d",&turn);
        if((turn != 1)&&(turn != 2)){

            printf("Input 1 or 2.\n");
        }
    }
    if(turn == 1){turn = PLAYER;}
    else{turn = COMPUTER;}
    return(turn);
}
/*関数：x座標の入力*/

int input_x(void){
    int x;
    printf("input x : ");
    scanf("%d",&x);
    return(x);
}
/*関数：y座標の入力*/

int input_y(void){
    int y;
    printf("input y : ");
    scanf("%d",&y);
    return(y);
}
/*関数：返せる石の数の計算*/

int can_reverse_num(int board[N][N],int x,int y,int turn){
    int counter[N];
    int sum=0;
    for(int i=0 ; i < N ; i++){

        counter[i] = 0;                             //カウンターの初期化
    }
    for(int i=0 ; i <= 8 ; i++){                    //各8方向に繰り返す

        if(i == 4){continue;}                       //除外ケース
        int xx , yy , dx , dy ;

        dx = i/3 - 1;                               //xの増分
        dy = i%3 - 1;                               //yの増分
        xx = x + dx;
        yy = y + dy;
        if(turn == PLAYER){

            while(board[xx][yy] == COMPUTER){
                counter[i]++;
                xx = xx + dx;
                yy = yy + dy;
            }
            if(board[xx][yy] != PLAYER){
                counter[i] = 0;
            }
        }
        if(turn == COMPUTER){
            while(board[xx][yy] == PLAYER){
                counter[i]++;
                xx = xx + dx;
                yy = yy + dy;
            }
            if(board[xx][yy] != COMPUTER){
                counter[i] = 0;
            }
        }
    }
    for(int i=0 ; i < N ; i++){
        sum = sum + counter[i];
    }
    return(sum);
}
/*関数：設置可能マスの判定*/
void can_place(int board[N][N],int x,int y,int turn){

    if((can_reverse_num(board,x,y,turn) > 0)
       &&(board[x][y] == BLANK)
       &&(1<=x)&&(x<=8)&&(1<=y)&&(y<=8)){
        board[x][y] = CANPLACE;
    }
}
/*関数：盤面の表示*/
void print_board(int board[N][N],int turn){

    char piece;                                     //数字を駒（文字）に対応させる媒介変数
    printf("~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("    ");                                 //x座標の表示

    for(int x=1 ; x < N ; x++){

        printf("%d ",x);
    }
    printf("\n");
    for(int y=1 ; y < N ; y++){
        for(int x=1 ; x < N ; x++){

            if(board[x][y] == CANPLACE){
                board[x][y] = BLANK;
            }
        }
    }
    for(int y=1 ; y < N ; y++){
        for(int x=1 ; x < N ; x++){
            can_place(board,x,y,turn);
        }
    }
    for(int y=1 ; y < N ; y++){                     //盤とy座標の表示
        for(int x=1 ; x < N ; x++){

            if(board[x][y] == BLANK){
                piece = ' ';
            }else if(board[x][y] == PLAYER){
                piece = 'o';
            }else if(board[x][y] == COMPUTER){
                piece = 'x';
            }else if(board[x][y] == CANPLACE){
                piece = '-';
            }
            if(x == 1){printf("  %d ",y);}
            printf("%c ",piece);
            if(x == N-1){printf("\n");}
        }
    }
    printf("~~~~~~~~~~~~~~~~~~~~~~\n");             //その他の表示
    printf(" PLAYER : o (%d)\n",o_count(board));

    printf(" COMPUTER : x (%d)\n",x_count(board));
    printf("\n");
}
/*関数：石の設置と裏返し*/
void put_stone(int board[N][N],int x,int y,int turn){

    int counter=0;
    for(int i=0 ; i <= 8 ; i++){                    //各8方向に繰り返す
        if(i == 4){continue;}                       //除外ケース

        int xx , yy , dx , dy ;
        dx = i/3 - 1;                               //xの増分
        dy = i%3 - 1;                               //yの増分
        xx = x + dx;
        yy = y + dy;

        if(turn == PLAYER){
            board[x][y] = PLAYER;                   //石を設置する

            while(board[xx][yy] == COMPUTER){       //その方向のCOMPUTERの石を数える
                counter++;
                xx = xx + dx;
                yy = yy + dy;
            }
            if(board[xx][yy] == PLAYER){
                for(int i=1 ; i <= counter ; i++){
                    board[x + dx*i][y + dy*i] = PLAYER;
                }
            }
            counter = 0;
        }
        if(turn == COMPUTER){
            board[x][y] = COMPUTER;
            while(board[xx][yy] == PLAYER){
                counter++;
                xx = xx + dx;
                yy = yy + dy;
            }
            if(board[xx][yy] == COMPUTER){
                for(int i=1 ; i <= counter ; i++){
                    board[x + dx*i][y + dy*i] = COMPUTER;
                }
            }
        }
    }
}
/*関数：COMPUTERの石設置*/
void com_put(int board[N][N]){

    int max=0;
    int xx,yy;
    for(int y=1 ; y < N ; y++){
        for(int x=1 ; x < N ; x++){
            if(board[x][y] == CANPLACE){
                if(max < can_reverse_num(board,x,y,COMPUTER)){
                    max = can_reverse_num(board,x,y,COMPUTER);  //一番多く返せるところに置く
                    xx = x; yy = y;
                }
            }
        }
    }
    put_stone(board,xx,yy,COMPUTER);
}
/*関数：ターンのパスの判定*/
int pass(int board[N][N],int turn){

    for(int y=1 ; y < N ; y++){
        for(int x=1 ; x < N ; x++){
            if(board[x][y] == CANPLACE){return(false);}
        }
    }
    return(true);
}
/*関数：ゲームの終了判定*/
int complete(int board[N][N]){

    if((pass(board,PLAYER) == true) && (pass(board,COMPUTER) == true)){
        return(true);
    }else{return(false);}
}
/*関数：勝者の判定と表示*/
void print_winner(int board[N][N]){

    if(o_count(board) > x_count(board)){
        print_board(board, PLAYER);
        printf(" PLAYER's WIN\n");
    }else if(x_count(board) > o_count(board)){
        printf(" COMPUTER's WIN\n");
    }else if(o_count(board) == x_count(board)){
        printf(" DRAW\n");
    }
}
int main(){
    int board[N][N];

    int turn;
    int x,y;
    reset(board);
    turn = first_turn();
    do{ print_turn(turn);
        print_board(board,turn);
        if(turn == PLAYER){

            if(pass(board,turn) == true){
                printf("  PASS\n");

                break;
            }
            do{ x = input_x();
                y = input_y();
                if(board[x][y] != CANPLACE){
                    printf("input the appropriate x,y\n");
                }
            }while(board[x][y] != CANPLACE);
            put_stone(board,x,y,turn);
        }else if(turn == COMPUTER){
            if(pass(board,turn) == true){
                printf(" PASS\n");
                printf("\n");
                break;
            }

            com_put(board);
        }
        turn = next_player(turn);
    }while(complete(board) == false);
    print_winner(board);
    return 0;
}