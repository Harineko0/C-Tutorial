#include "stdio.h"
#include <stdbool.h>

#define WHITE 0
#define BLACK 1
#define HORIZONTAL_MASK 0x7e7e7e7e7e7e7e7e
#define VERTICAL_MASK 0x00FFFFFFFFFFFF00
#define DIAGONAL_MASK 0x007e7e7e7e7e7e00

typedef int Color;
typedef unsigned long long ull;
typedef ull (*shifter)(ull, int);

// region Util

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

/// @brief playerの石の隣にある、連続するopponentの石を求める.
/// @example o: player, x: opponent として, 盤面が xxxo ならば 1110 を返す
/// @param player プレイヤーの盤面
/// @param opponent 敵の盤面
/// @param shift シフトする関数. shiftR または shiftL
/// @param n シフトする数
ull line(ull player, ull opponent, shifter shift, int n) {
    ull result = shift(player, n) & opponent;
    result |= shift(result, n) & opponent;
    result |= shift(result, n) & opponent;
    result |= shift(result, n) & opponent;
    result |= shift(result, n) & opponent;
    result |= shift(result, n) & opponent;
    return shift(result, n);
}

/// @brief 特定の方向の合法手を返す
/// @param player 自分の盤面
/// @param opponent 敵の盤面
/// @param mask シフトするときに無視しないといけない opponent の石を除く. 番兵.
/// @param shift シフトする数
ull partialLegal(ull player, ull opponent, ull mask, int shift) {
    ull legal;

    opponent &= mask;

    // 左右シフト
    legal = line(player, opponent, shiftL, shift);
    legal |= line(player, opponent, shiftR, shift);

    return legal;
}

/// @brief プレイヤーの合法手を返す
/// @param player プレイヤーの盤面
/// @param opponent 敵の盤面
ull legalBoard(ull player, ull opponent) {
    ull blank = ~(player | opponent);

            // 水平
    return blank & (partialLegal(player, opponent, HORIZONTAL_MASK, 1) |
            // 垂直
            partialLegal(player, opponent, VERTICAL_MASK, 8) |
            // 斜め
            partialLegal(player, opponent, DIAGONAL_MASK, 7) |
                    partialLegal(player, opponent, DIAGONAL_MASK, 9));
}

/// @brief その場所に石を置けるか置かないかをboolで返す
/// @param player 自分の盤面
/// @param opponent 相手の盤面
/// @param try 置こうとする場所
bool canPut(ull player, ull opponent, ull try) {
    ull legal = legalBoard(player, opponent);
    return (legal & try) != 0;
}

/// @brief 特定の方向の反転可能な石を返す
/// @param player プレイヤーの盤面
/// @param opponent 敵の盤面
/// @param put 置いた石
/// @param mask シフトするときのマスク
/// @param shift 何個シフトするか
ull partialReversible(ull player, ull opponent, ull put, ull mask, int shift) {
    ull result = 0, tmp;

    opponent &= mask;

    tmp = line(put, opponent, shiftL, shift);
    if ((player & shiftL(tmp, shift)) != 0) {
        result = tmp;
    }
    tmp = line(put, opponent, shiftR, shift);
    if ((player & shiftR(tmp, shift)) != 0) {
        result |= tmp;
    }

    return result;
}

/// @brief 反転可能な石を返す
/// @param player プレイヤーの盤面
/// @param opponent 敵の盤面
/// @param put 置いた石
ull reversible(ull player, ull opponent, ull put) {
    return partialReversible(player, opponent, put, HORIZONTAL_MASK, 1) |
            partialReversible(player, opponent, put, VERTICAL_MASK, 8) |
            partialReversible(player, opponent, put, DIAGONAL_MASK, 7) |
            partialReversible(player, opponent, put, DIAGONAL_MASK, 9);
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
    expectULL("legal()", legal(getBit(3, 3) | getBit(4, 4), getBit(3, 4) | getBit(4, 3)), 0x80420100000);
}

// endregion