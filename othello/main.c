#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "util.h"

// Board Element
#define EMPTY 0
#define FIRST 1 // 先手
#define SECOND 2 // 後手
#define LEGAL 3
#define CURSOR 4
#define OUTSIDE -1
// ASNI Color Code
#define CH_BLACK 30
#define CH_YELLOW 33
#define CH_WHITE 37
#define BK_BLACK 40
#define BK_GREEN 42
// Board Divider
#define BETWEEN 0
#define TOP 1
#define BOTTOM 2
// input_arrow
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define ENTER 5
#define INVALID 0
#define QUIT 6

struct vector {
    int x;
    int y;
};
/// UP, DOWN, LEFT, RIGHT, ENTER, INVALID, QUIT
typedef int direction;

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

/// @brief 矢印キーの入力を direction 型として返します.
direction input_arrow_key() {
    int firstChar = _getch();

    if (firstChar == 0xe0) {
        // 矢印キーのとき
        int secondChar = _getch();

        if (secondChar == 0x48) {
            return UP;
        } else if (secondChar == 0x50) {
            return DOWN;
        } else if (secondChar == 0x4b) {
            return LEFT;
        } else if (secondChar == 0x4d) {
            return RIGHT;
        }

    } else if (firstChar == 13) {
        // エンターキーのとき
        return ENTER;
    } else if (firstChar == 'q') {
        return QUIT;
    }

    return INVALID;
}

/// @brief 左上から見て一番最初にある合法手の座標を返します
struct vector first_legal_coord(int board_size, int board[board_size + 2][board_size + 2]) {
    struct vector coord;
    coord.x = 0;
    coord.y = 0;
    for (int i = 1; i < board_size; i++) {
        for (int j = 1; j < board_size; j++) {
            if (board[i][j] == LEGAL) {
                coord.x = i;
                coord.y = j;
                break;
            }
        }
        if (coord.x != 0 || coord.y != 0) {
            break;
        }
    }
    return coord;
}

struct vector input_coord(int board_size, int board[board_size + 2][board_size + 2]) {
    struct vector tmp_legal = first_legal_coord(board_size, board);
    struct vector cursor = tmp_legal;
    board[cursor.x][cursor.y] = CURSOR;
    direction dire = INVALID;

    do {
        dire = input_arrow_key();

    } while (dire != ENTER);

    return cursor;
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
                case LEGAL:
                    print_color(CH_YELLOW);
                    printf(" - ");
                    print_color(CH_WHITE);
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
    int center = array_size / 2 - 1;

    for (int i = 0; i < array_size; i++) {

        for (int j = 0; j < array_size; j++) {

            if (i == 0 || i == array_size || j == 0 || j == array_size) {
                // 番兵
                board[i][j] = OUTSIDE;

            } else if (i == j && (i - center == 0 || i - center == 1)) {
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

/// @brief (x, y) の石についての合法手を board に設定する
void partial_legal_board(int board_size, int board[board_size + 2][board_size + 2], int current_turn, int x, int y) {
    int next_turn = current_turn == FIRST ? SECOND : FIRST;

    // 8方向について探索
    for (int i = 0; i < 9; i++) {
        int x_diff = (i - 1) % 3 - 1;
        int y_diff = i / 3 - 1;

        if (x_diff == 0 && y_diff == 0) {
            continue;
        }

        int _x = x;
        int _y = y;
        /// その方向に一つでも相手の石があるか
        bool continuous = false;

        for (int j = 0; j < board_size; j++) {
            _x += x_diff;
            _y += y_diff;

            int stone = board[_x][_y];

            if (stone == OUTSIDE || stone == current_turn) {
                break;
            } else if (stone == next_turn) {
                continuous = true;
                continue;
            } else if (stone == EMPTY) {
                if (continuous == true) {
                    board[_x][_y] = LEGAL;
                }

                break;
            }
        }
    }
}

/// @brief board に合法手を設定します
/// @param current_turn FIRST or SECOND
void legal_board(int board_size, int board[board_size + 2][board_size + 2], int current_turn) {
    for (int i = 0; i < board_size + 2; i++) {
        for (int j = 0; j < board_size + 2; j++) {
            // 自分の石のとき、判定を行う
            if (board[i][j] == current_turn) {
                partial_legal_board(board_size, board, current_turn, i, j);
            }

        }
    }
}
// endregion

void game() {
    int size = input_size();

    int board[size + 2][size + 2];
    int current_turn = FIRST;

    init_board(size, board);
    legal_board(size, board, current_turn);

    print_board(size, board);
}

int main() {
    game();
}