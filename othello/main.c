#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "util.h"
#include "vector.h"

// Board Element
#define EMPTY 0
#define FIRST -1 // 先手
#define SECOND -2 // 後手
#define OUTSIDE -3
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

/// UP, DOWN, LEFT, RIGHT, ENTER, INVALID, QUIT
typedef int direction;

/// @brief オセロの board で合法手かどうか
bool is_legal(int stone) {
    return stone > 0;
}

/// @brief オセロの board でカーソルかどうか
bool is_cursor(int stone) {
    return (0b1000000000 & stone) != 0;
}

void set_cursor(int board_size, int board[board_size + 2][board_size + 2], struct vector coord) {
    if (board[coord.x][coord.y] <= 0) return;
    board[coord.x][coord.y] |= 0b1000000000;
}

// region IO
/// @brief 出力のカーソルを上に line 行だけずらす
void print_overwrite(int line) {
//    printf("\033[%dF", line);
}

void print_line(int line) {
    for (int i = 0; i < line; i++) {
        printf("\n");
    }
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
    print_overwrite(19);
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
            int stone = board[j][i];

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
                default:
                    if (is_cursor(stone)) {
                        print_color(CH_YELLOW);
                        printf(" + ");
                        print_color(CH_WHITE);
                    } else if (is_legal(stone)) {
                        print_color(CH_YELLOW);
                        printf(" - ");
                        print_color(CH_WHITE);
                    }
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

void print_board_debug(int board_size, int board[board_size + 2][board_size + 2]) {
    for (int i = 0; i < board_size + 2; i++) {
        for (int j = 0; j < board_size + 2; j++) {
            printf("%5d ", board[j][i]);
        }
        printf("\n");
    }
    printf("----------------------------------------\n");
}

/// @brief 左上から見て一番最初にある合法手の座標を返します
struct vector first_legal_coord(int board_size, int board[board_size + 2][board_size + 2]) {
    struct vector coord;
    coord.x = 0;
    coord.y = 0;
    for (int i = 1; i < board_size; i++) {
        for (int j = 1; j < board_size; j++) {
            if (is_legal(board[j][i])) {
                coord.x = j;
                coord.y = i;
                break;
            }
        }
        if (coord.x != 0 || coord.y != 0) {
            break;
        }
    }
    return coord;
}

/// @brief 水平方向で cursor に最も近い合法手の座標を返す. 合法手がない場合, 入力の cursor をそのまま返す.
struct vector horizontal_legal(int board_size, int board[board_size + 2][board_size + 2], struct vector cursor) {
    if (is_legal(board[cursor.x][cursor.y])) {
        return cursor;
    }

    struct vector leftCursor = cursor, rightCursor = cursor;

    for (int i = 0; i < board_size; i++) {
        leftCursor.x--;
        rightCursor.x++;

        if (leftCursor.x > 0 && is_legal(board[leftCursor.x][leftCursor.y])) {
            return leftCursor;

        } else if (rightCursor.x < board_size + 2 && is_legal(board[rightCursor.x][rightCursor.y])) {
            return rightCursor;

        }
    }

    return cursor;
}

/// @brief search_diff の方向にある合法手の座標を返します. search_diff が上下の場合, その方向にも探索します.
struct vector next_cursor(int board_size, int board[board_size + 2][board_size + 2], struct vector cursor, struct vector search_diff) {
    struct vector new_cursor = cursor;

    for (int i = 0; i < board_size; i++) {
        new_cursor = add_vector(new_cursor, search_diff);

        if (search_diff.y != 0) {
            new_cursor = horizontal_legal(board_size, board, new_cursor);
        }

        int stone = board[new_cursor.x][new_cursor.y];

        if (is_legal(stone)) {
            return new_cursor;

        } else if (stone == OUTSIDE) {
            break;
        }
    }

    return cursor;
}

struct vector input_coord(int board_size, int board[board_size + 2][board_size + 2]) {
    struct vector tmp_legal = first_legal_coord(board_size, board);
    struct vector cursor = tmp_legal;
    struct vector old_cursor;
    direction dire = INVALID;

    do {
        int original = board[cursor.x][cursor.y];
        set_cursor(board_size, board, cursor);

        print_board(board_size, board);
        dire = input_arrow_key();
        struct vector search_diff;
        search_diff.x = 0, search_diff.y = 0;

        switch (dire) {
            case UP:
                search_diff.y = -1;
                break;
            case DOWN:
                search_diff.y = 1;
                break;
            case LEFT:
                search_diff.x = -1;
                break;
            case RIGHT:
                search_diff.x = 1;
                break;
            case QUIT:
                exit(0);
                break;
        }

        old_cursor = cursor;
        cursor = next_cursor(board_size, board, cursor, search_diff);

        board[old_cursor.x][old_cursor.y] = original;

    } while (dire != ENTER);

    return cursor;
}
// endregion

// region Board
int get_next_turn(int current_turn) {
    return current_turn == FIRST ? SECOND : FIRST;
}

/// @brief 盤面を初期化
void init_board(int board_size, int board[board_size + 2][board_size + 2]) {
    int array_size = board_size + 2;
    int center = array_size / 2 - 1;

    for (int i = 0; i < array_size; i++) {

        for (int j = 0; j < array_size; j++) {

            if (i == 0 || i == array_size - 1 || j == 0 || j == array_size - 1) {
                // 番兵
                board[j][i] = OUTSIDE;

            } else if (i == j && (i - center == 0 || i - center == 1)) {
                // 後手
                board[j][i] = SECOND;

            } else if (abs(i - j) == 1 && min(i, j) == center) {
                // 先手
                board[j][i] = FIRST;

            } else {
                board[j][i] = EMPTY;
            }

        }

    }
}

/// @brief 入力された数値 (0~8) から, xy座標における方向(縦横斜め)を返します.
struct vector vec_direction(int n) {
    struct vector vec;
    vec.x = n % 3 - 1;
    vec.y = n / 3 - 1;
    return vec;
}

/// @brief (x, y) の石についての合法手を board に設定する
void partial_legal_board(int board_size, int board[board_size + 2][board_size + 2], int current_turn, int x, int y) {
    int next_turn = get_next_turn(current_turn);
    printf("partial_legal_board (x, y) = (%d, %d)\n", x, y);
//    print_board_debug(board_size, board);

    // 8方向について探索
    for (int i = 0; i < 9; i++) {
        struct vector diff = vec_direction(i);
        printf(" diff (%d, %d): \n", diff.x, diff.y);

        if (diff.x == 0 && diff.y == 0) {
            continue;
        }

        int _x = x;
        int _y = y;
        /// その方向に一つでも相手の石があるか
        bool continuous = false;

        for (int j = 0; j < board_size; j++) {
            _x += diff.x;
            _y += diff.y;

            int stone = board[_x][_y];
            printf("  stone(%d, %d): %d\n", _x, _y, stone);

            if (stone == OUTSIDE || stone == current_turn) {
                break;
            } else if (stone == next_turn) {
                continuous = true;
                continue;
            } else if (stone == EMPTY || is_legal(stone)) {
                if (continuous == true) {
                    int legal_stone = 1 << i;
                    board[_x][_y] |= legal_stone;
                    printf("   legal: (%d, %d)\n", _x, _y);
                }

                break;
            }
        }
    }
}

/// @brief board に合法手を設定します
/// @param current_turn FIRST or SECOND
void legal_board(int board_size, int board[board_size + 2][board_size + 2],
                 int current_turn) {
    for (int i = 0; i < board_size + 2; i++) {
        for (int j = 0; j < board_size + 2; j++) {
            // 自分の石のとき, 判定を行う
            if (board[j][i] == current_turn) {
                partial_legal_board(board_size, board, current_turn, j, i);
            }

        }
    }
}

void remove_legal_board(int board_size, int board[board_size + 2][board_size + 2]) {
    for (int i = 0; i < board_size + 2; i++) {
        for (int j = 0; j < board_size + 2; j++) {
            int stone = board[j][i];
            if (is_legal(stone)) {
                board[j][i] = EMPTY;
            }

        }
    }
}

void place_stone(int board_size, int board[board_size + 2][board_size + 2],
                 struct vector put_coord, int current_turn) {
    int put = board[put_coord.x][put_coord.y];
    board[put_coord.x][put_coord.y] = current_turn;
    int next_turn = get_next_turn(current_turn);

    if (!is_legal(put)) return;

    for (int i = 0; i < 9; i++) {
        /// 合法手ビット列と合致したら
        if ((1 << i) & put) {
            struct vector diff = vec_direction(i);
            diff = scala_vector(diff, -1);
            struct vector reverse_coord = put_coord;

            for (int j = 0; j < board_size; j++) {
                reverse_coord = add_vector(reverse_coord, diff);
                int stone = board[reverse_coord.x][reverse_coord.y];

                if (stone == next_turn) {
                    board[reverse_coord.x][reverse_coord.y] = current_turn;
                } else if (stone == current_turn) {
                    break;
                } else if (stone == OUTSIDE) {
                    break;
                }
            }

        }
    }
}
// endregion

void game() {
    int size = input_size();
//    int size = 8;
//    print_line(19);

    int board[size + 2][size + 2];
    int current_turn = FIRST;

    init_board(size, board);

//    struct vector inputs[] = {{6, 5}, {5, 5}, {1, 1}};
//    int i = 0;

    do {
        legal_board(size, board, current_turn);
        struct vector put_coord = input_coord(size, board);

//        struct vector put_coord = inputs[i];
//        i++;

        place_stone(size, board, put_coord, current_turn);
        remove_legal_board(size, board);
//        print_board_debug(size, board);
        current_turn = get_next_turn(current_turn);
    } while(1);

    print_board(size, board);
}

int main() {
    game();
}