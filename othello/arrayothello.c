#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

// Board Element
#define EMPTY 0
#define FIRST 1 // 先手
#define SECOND 2 // 後手
#define OUTSIDE -1
// ASNI Color Code
#define CH_BLACK 30
#define CH_WHITE 37
#define BK_BLACK 40
#define BK_GREEN 42
// Board Divider
#define BETWEEN 0
#define TOP 1
#define BOTTOM 2

// region IO
/// @brief 出力のカーソルを上に line 行だけずらす
void print_overwrite(int line) {
    printf("\033[%dF", line);
}

/// @brief ANSIのカラーコードを出力する
void print_color(int color_code) {
    printf("\033[%dm", color_code);
}

/// @brief 盤面のサイズを入力
/// @return 正の偶数
int input_size() {
    int size = 0;
    bool first_loop = true;

    do {
        if (!first_loop) {
            print_overwrite(1);
            printf("%d is not valid size.\n", size);
        }

        printf("Enter board size, which should be positive even number: ");
        scanf("%d", &size);
        printf("\n");
        first_loop = false;

    } while (size <= 0 || size % 2 != 0);

    printf("\n");

    return size;
}

/// @brief オセロの枠線を表示する
void print_frame(char *frame) {
    print_color(CH_BLACK);

    printf("%s", frame);

    print_color(CH_WHITE);
}

void print_left_padding() {
    printf("   ");
}

/// @brief 盤面の枠の行間の横線を出力する
/// @param type BETWEEN, TOP, BOTTOM のいずれか
void print_board_divider(int board_size, int type) {
    char left[10];
    char right[13];
    char between[7];

    switch (type) {
        case BETWEEN:
            strcpy(left, "┣━━");
            strcpy(right, "━╋━━");
            strcpy(between, "━┫");
            break;
        case TOP:
            strcpy(left, "┏━━");
            strcpy(right, "━┳━━");
            strcpy(between, "━┓");
            break;
        case BOTTOM:
            strcpy(left, "┗━━");
            strcpy(right, "━┻━━");
            strcpy(between, "━┛");
            break;
    }

    print_left_padding();
    print_color(BK_GREEN);
    print_frame(left);
    for (int j = 0; j < board_size - 1; j++) {
        print_frame(right);
    }
    print_frame(between);
    print_color(BK_BLACK);
    printf("\n");
}

/// @brief 盤面を出力
void print_board(int board_size, int board[board_size + 2][board_size + 2]) {
    print_left_padding();
    printf(" ");
    for (int i = 0; i < board_size; i++) {
        printf(" %c  ", 'A' + i);
    }
    printf("\n");
    print_board_divider(board_size, TOP);

    // i = 1 からなのは番兵を無視するため
    for (int i = 1; i < board_size + 1; i++) {
        // 数字右詰め出力
        printf("%2d ", i);
        print_color(BK_GREEN);
        print_frame("┃");

        for (int j = 1; j < board_size + 1; j++) {
            int stone = board[i][j];

            switch (stone) {
                case EMPTY:
                    printf("   ");
                    break;
                case FIRST:
                    print_color(CH_BLACK);
                    printf(" @ ");
                    print_color(CH_WHITE);
                    break;
                case SECOND:
                    printf(" 0 ");
                    break;
            }

            // 枠線
            if (j < board_size) {
                print_frame("┃");
            }
        }

        print_frame("┃");
        print_color(BK_BLACK);
        printf("\n");

        if (i < board_size) {
            print_board_divider(board_size, BETWEEN);
        }
    }

    print_board_divider(board_size, BOTTOM);
    printf("\n");
}
// endregion

// region Board
/// @brief 盤面を初期化
void init_board(int board_size, int board[board_size + 2][board_size + 2]) {
    int array_size = board_size + 2;
    int center = array_size / 2;

    for (int i = 0; i < array_size; i++) {

        for (int j = 0; j < array_size; j++) {

            if (i == 0 || i == array_size || j == 0 || j == array_size) {
                // 番兵
                board[i][j] = OUTSIDE;

            } else if (i == j && i - center < 2) {
                // 後手
                board[i][j] = SECOND;

            } else if (abs(i - j) == 1 && min(i, j) == center) {
                // 先手
                board[i][j] = FIRST;

            } else {
                board[i][j] = EMPTY;
            }

        }

    }
}
// endregion

void game() {
    int size = input_size();

    int board[size + 2][size + 2];

    init_board(size, board);

    print_board(size, board);
}

int main() {
    game();
}