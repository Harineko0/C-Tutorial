#include "stdio.h"
#include <stdbool.h>

#define WHITE 0
#define BLACK 1

typedef int Color;
typedef unsigned long long ull;

// region Util

typedef ull (*shifter)(ull, int);

/// @brief 左シフトビット演算
/// @param a シフトする対象
/// @param n 何bitシフトするか
ull shiftL(ull a, int n) {
    return a << n;
}

/// @brief 右シフトビット演算
/// @param a シフトする対象
/// @param n 何bitシフトするか
ull shiftR(ull a, int n) {
    return a >> n;
}

// endregion

// region Calculate

/// @brief 与えられた座標のビットのみが立ったビット列を返す
/// @param x 横座標.
/// @param y 縦座標.
/// @return 座標のビットのみが立ったビット列
ull getBit(int x, int y) {
    // 4つのビット列をまとめて1つの16進数で表す. ex) 8 = 1000
    ull mask = 0x8000000000000000;

    // xにシフト
    mask >>= x;

    // yにシフト
    mask >>= y * 8;

    return mask;
}

ull line(ull player, ull opponent, shifter shift,  ull n) {
    ull result = ;
}

/// @brief direction の方向の合法手を返す
/// @param player 自分の盤面
/// @param opponent 敵の盤面
/// @param watcher 番兵
/// @param shift シフトする数
ull legal(ull player, ull opponent, ull watcher, int shift) {
    opponent &= watcher;

    ull tmp;
    ull legal;

    // 左シフト
    tmp = player << shift & opponent;
    tmp |= tmp << shift & opponent;
    tmp |= tmp << shift & opponent;
    tmp |= tmp << shift & opponent;
    tmp |= tmp << shift & opponent;
    tmp |= tmp << shift & opponent;
    legal = tmp << shift;

     // 右シフト
    tmp = player >> shift & opponent;
    tmp |= tmp >> shift & opponent;
    tmp |= tmp >> shift & opponent;
    tmp |= tmp >> shift & opponent;
    tmp |= tmp >> shift & opponent;
    tmp |= tmp >> shift & opponent;
    legal |= tmp >> shift;

    return legal;
}

/// @brief 合法手を返す
/// @param player プレイヤーの盤面
/// @param opponent 敵の盤面
ull legalBoard(ull player, ull opponent) {
    ull blank = ~(player | opponent);

            // 水平
    return blank & (legal(player, opponent, 0x7e7e7e7e7e7e7e7e, 1) |
            // 垂直
            legal(player, opponent, 0x00FFFFFFFFFFFF00, 8) |
            // 斜め
            legal(player, opponent, 0x007e7e7e7e7e7e00, 7) |
            legal(player, opponent, 0x007e7e7e7e7e7e00, 9));
}

/// @brief その場所に石を置けるか置かないかをboolで返す
/// @param player 自分の盤面
/// @param opponent 相手の盤面
/// @param try 置こうとする場所
bool canPut(ull player, ull opponent, ull try) {
    ull legal = legalBoard(player, opponent);
    return (legal & try) != 0;
}

// endregion

// region IO

void printBoard(ull player, ull opponent, Color playerColor) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {

            ull thisBit = getBit(j, i);
            bool playerStone = (player & thisBit) != 0;
            bool opponentStone = (opponent & thisBit) != 0;
            bool isEmpty = (playerStone | opponentStone) == 0;

            if (isEmpty) {
                printf("   ");

            } else if ((playerStone && playerColor) || (opponentStone && !playerColor)) {
                printf(" O ");

            } else {
                printf(" X ");
            }

            if (j < 7) {
                printf("┃");
            }
        }

        if (i < 7) {
            printf("\n━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━\n");
        }
    }
}

// endregion

int test();

int main() {
    ull player = 0x1008000000;
    ull opponent = 0x810000000;

    printBoard(player, opponent, WHITE);
//    test();
}

// region Test

void expectULL(char* testName, ull input, ull expect) {
    if (input == expect) {
        printf("✓ Test %s passed\n", testName);
    } else {
        printf("x Test %s failed\n", testName);
        printf("input: %llu", input);
    }
}

int test() {
    expectULL( "getBit(4, 4)", getBit(4, 4), 0x8000000);
    // expect:
    //    00001000
    //    00000100
    //    00100000
    //    00010000
    //    00000000
    //    00000000
    expectULL("legalBoard()", legalBoard(getBit(3,   3) | getBit(4, 4), getBit(3, 4) | getBit(4, 3)), 0x80420100000);
}

// endregion