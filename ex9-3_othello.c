#include "stdio.h"
#include <stdbool.h>
#include <string.h>

#define WHITE 0
#define BLACK 1
#define HORIZONTAL_MASK 0x7e7e7e7e7e7e7e7e
#define VERTICAL_MASK 0x00FFFFFFFFFFFF00
#define DIAGONAL_MASK 0x007e7e7e7e7e7e00

typedef int Color;
typedef unsigned long long ull;
typedef unsigned int uint;
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

/// @brief 立っているビット数をカウントします
int countBit(ull bits) {
    bits = (bits & 0x5555555555555555) + (bits >> 1 & 0x5555555555555555);
    bits = (bits & 0x3333333333333333) + (bits >> 2 & 0x3333333333333333);
    bits = (bits & 0x0f0f0f0f0f0f0f0f) + (bits >> 4 & 0x0f0f0f0f0f0f0f0f);
    bits = (bits & 0x00ff00ff00ff00ff) + (bits >> 8 & 0x00ff00ff00ff00ff);
    bits = (bits & 0x0000ffff0000ffff) + (bits >>16 & 0x0000ffff0000ffff);
    bits = (bits & 0x00000000ffffffff) + (bits >>32 & 0x00000000ffffffff);

    return bits;
}

/// @brief ビット列を左右反転する.
/// @example 11010100 -> 00101011
uint reverseBit(uint bits) {
    // 0xAA = 10101010, 0x55 = 01010101
    bits = ((bits & 0xAAAAAAAA) >> 1) | ((bits & 0x55555555) << 1);
    bits = ((bits & 0xCCCCCCCC) >> 2) | ((bits & 0x33333333) << 2);
    bits = ((bits & 0xF0F0F0F0) >> 4) | ((bits & 0x0F0F0F0F) << 4);
    bits = ((bits & 0xFF00FF00) >> 8) | ((bits & 0x00FF00FF) << 8);
    bits = ((bits & 0xFFFF0000) >> 16) | ((bits & 0x0000FFFF) << 16);

    return bits;
}

void error(char* message) {
    printf("\033[31mError: %s\033[37m\n", message);
}

// endregion

// region Core

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
    return result; // FIXME: shift(result, n) ??
}

/// @brief 特定の方向の合法手を返す
/// @param player 自分の盤面
/// @param opponent 敵の盤面
/// @param mask シフトするときに無視しないといけない opponent の石を除く. 番兵.
/// @param shift シフトする数
ull partialLegal(ull player, ull opponent, ull mask, int shift) {
    ull legal, tmp;

    opponent &= mask;

    // 左右シフト
    tmp = line(player, opponent, shiftL, shift);
    legal = shiftL(tmp, shift);
    tmp = line(player, opponent, shiftR, shift);
    legal |= shiftR(tmp, shift);

    return legal;
}

/// @brief player の合法手を返す
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

bool isPass(ull playerLegal, ull opponentLegal) {
    return playerLegal == 0 && opponentLegal != 0;
}

bool isFinished(ull playerLegal, ull opponentLegal) {
    return playerLegal == 0 && opponentLegal == 0;
}

// endregion

// region AI

// 評価関数
/// @brief あるマスのスコアを返す
int scoreOfSlot(int x, int y) {
    static int slotScoreMap[8][8] = {
            { 30, -12,  0, -1, -1,  0, -12,  30},
            {-12, -15, -3, -3, -3, -3, -15, -12},
            {  0,  -3,  0, -1, -1,  0,  -3,   0},
            { -1,  -3, -1, -1, -1, -3,  -3,  -1},
            { -1,  -3, -1, -1, -1, -3,  -3,  -1},
            {  0,  -3,  0, -1, -1,  0,  -3,   0},
            {-12, -15, -3, -3, -3, -3, -15, -12},
            { 30, -12,  0, -1, -1,  0, -12,  30}
            };

    if (x < 0 || x > 7 || y < 0 || y > 7) {
        return 0;
    } else {
        return slotScoreMap[x][y];
    }
}

/// @brief (index + 1)番目の行の左半分のスコア. 上から４列しか受け付けない.
/// @param index 上から何行目か (0~3)
/// @param player プレイヤーの石の盤面 (4bit)
/// @param opponent 敵の石の盤面 (4bit)
int scoreOfHalfRow(int index, uint player, uint opponent) {
    /// 左上4*4マスの石のパターンを、行と石の配置 (player << 4 + opponent) をキーとして保存. 同じマスに石が重複する場合が無駄(e.g. 10001000)
    static int rowScoreMap[4][0b11111111];
    /// rowScoreMap を初期化したかどうか
    static int initialized = 0;

    uint pattern = (player << 4) + opponent;

    if (!initialized) {
        for (int row = 0; row < 4; row++) {
            for (int pat = 0; pat < 0b11111111; pat++) {
                int _player = pat >> 4;
                int _opponent = pat & 0b00001111;
                int score = 0;

                for (int i = 0; i < 4; i++) {
                    int slot = 0b1000 >> i;

                    // slot の場所に _player の石があったらスコアを追加
                    if (slot & _player) {
                        score += scoreOfSlot(i, row);
                    }

                    if (slot & _opponent) {
                        score -= scoreOfSlot(i, row);
                    }
                }

                rowScoreMap[row][pat] = score;
            }
        }

        initialized = 1;
    }

    if (index < 0 || index > 3 || pattern > 0b11111111 - 1) {
        error("scoreOfHalfRow(), index or pattern is out of range.");
        return 0;

    } else {
        return rowScoreMap[index][pattern];

    }
}

/// @brief 0~7 列目, 横 8 マスで行のスコアを返す
/// @param index 上から何列目か. 0 ~ 7
/// @param player プレイヤーの盤面. 8bit
/// @param opponent 敵の盤面. 8bit
int scoreOfRow(int index, uint player, uint opponent) {
    if (player > 0b11111111 | opponent > 0b11111111) {
        error("scoreOfRow(), player or opponent is greater than 0b11111111");
    }

    int score = 0;

    // index を 0~3 までに抑える
    if (index > 3) {
        index = 7 - index;
    }

    // 左側
    score += scoreOfHalfRow(index, (player & 0b11110000) >> 4, (opponent & 0b11110000) >> 4);
    // 右側. 左右反転するが、reverseBit は32bitまるごと反転するので32-4の28ビットシフトする. また、プレイヤーと敵が逆向きになるのでそれも入れ替える.
    score += scoreOfHalfRow(index, reverseBit(player & 0b00001111) >> 28, reverseBit(opponent & 0b00001111) >> 28);

    return score;
}

int evaluateScore(ull player, ull opponent) {
    int score = 0;

    for (int i = 0; i < 8; i++) {
        int shift = i * 8;
        /// mask & player のビットのずれを直すため
        int unshift = (7 - i) * 8;
        ull mask = 0xFF00000000000000 >> shift;
        // i 行目の盤面を取得
        uint playerRow = (mask & player) >> unshift;
        uint opponentRow = (mask & opponent) >> unshift;

        score += scoreOfRow(i, playerRow, opponentRow);
    }

    return score;
}

// endregion

// region IO

/// @brief オセロの盤面を出力する
void printBoard(ull player, ull opponent, Color playerColor) {
    printf("\033[1m");
    printf("   A   B   C   D   E   F   G   H\n");

    ull legal = legalBoard(player, opponent);

    for (int i = 0; i < 8; i++) {
        printf("%d \033[42m", i + 1);

        for (int j = 0; j < 8; j++) {

            ull thisBit = getBit(j, i);
            bool isPlayerStone = (player & thisBit) != 0;
            bool isOpponentStone = (opponent & thisBit) != 0;
            bool isLegalStone = (legal & thisBit) != 0;
            bool isEmpty = (isPlayerStone | isOpponentStone) == 0;

            if (isEmpty) {
                if (isLegalStone) {
                    printf("\033[33m + \033[37m");
                } else {
                    printf("   ");
                }

            } else if ((isPlayerStone && playerColor) || (isOpponentStone && !playerColor)) {
                printf(" O ");

            } else {
                printf("\033[30m @ \033[37m");
            }

            if (j < 7) {
                printf("\033[30m┃\033[37m");
            } else {
                printf("\033[40m");
            }
        }

        if (i < 7) {
            printf("\n  \033[42m\033[30m━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━\033[37m\033[40m\n");
        }
    }
    printf("\n");
    printf("\033[m");

    printf("Score: %d\n", evaluateScore(player, opponent));
}

/// @brief Stringの座標を int, int の座標に変換する
/// @example (A2) -> (0, 1)
void toIntCoordinate(char* coordinate, int* x, int* y) {
    if (strlen(coordinate) < 2) {
        // エラー
        *x = -1;
        *y = -1;
        return;
    } else {
        *x = coordinate[0] - 'A';
        *y = coordinate[1] - '1';

        if (*x < 0 || *x > 7 || *y < 0 || *y > 7) {
            // エラー
            *x = -1;
            *y = -1;
            return;
        }

        return;
    }
}

/// @brief 座標の入力から, 整数値の座標を返します. 合法手になるまで入力を繰り返します.
void input(ull player, ull opponent, int* x, int* y) {
    char coordinate[20] = "";
    int tmpX = -1, tmpY = -1;

    while (1) {
        printf("Where to place? ⋯ ");
        scanf("%s", coordinate);

        toIntCoordinate(coordinate, &tmpX, &tmpY);

        if (tmpX == -1 || tmpY == -1) {
            printf("\033[1AEnter in correct format, like C2, F6 or A3. \n");

        } else {
            ull tryToPut = getBit(tmpX, tmpY);
            ull blank = ~(player | opponent);

            if ((tryToPut & blank) == 0) {
                // 既に石がある
                printf("\033[1A%s is not empty. Enter empty coordinate. \n", coordinate);

            } else if (!canPut(player, opponent, tryToPut)) {
                // 合法手じゃない
                printf("\033[1AYou can't place to %s. Enter other coordinate. \n", coordinate);

            } else {
                break;
            }
        }
    }

    *x = tmpX;
    *y = tmpY;
}

// endregion

void game() {
    ull player = getBit(3, 3) | getBit(4, 4);
    ull opponent = getBit(3, 4) | getBit(4, 3);
    Color color = WHITE;

    while (1) {
        printBoard(player, opponent, color);

        printf("Current Player: %c\n", color ? 'O' : '@');

        // 入力
        int x = -1, y = -1;
        input(player, opponent, &x, &y);

        // 反転
        ull put = getBit(x, y);
        ull reverse = reversible(player, opponent, put);
        player |= reverse;
        player |= put;
        opponent ^= reverse;

        // 終了判定
        ull playerLegal = legalBoard(player, opponent);
        ull opponentLegal = legalBoard(opponent, player);

        if (isFinished(playerLegal, opponentLegal)) {
            break;

            // 反転させて次回のターンのパスを判定
        } else if (isPass(opponentLegal, playerLegal)) {
            continue;

        } else {
            ull tmp = player;
            player = opponent;
            opponent = tmp;
            color = !color;
            continue;
        }
    }
}

void test();

int main() {
//    test();
    game();
}

// region Test

void expectULL(char* testName, ull input, ull expect) {
    if (input == expect) {
        printf("\033[32m✓\033[37m Test %s passed\n", testName);
    } else {
        printf("\033[31mx\033[37m Test %s failed\n", testName);
        printf("input: %llu\n", input);
    }
}

void expectInt(char* testName, int input, int expect) {
    if (input == expect) {
        printf("\033[32m✓\033[37m Test %s passed\n", testName);
    } else {
        printf("\033[31mx\033[37m Test %s failed. ", testName);
        printf("(input: %d)\n", input);
    }
}

void test() {
    ull player = getBit(3, 3) | getBit(4, 4);
    ull opponent = getBit(3, 4) | getBit(4, 3);

    expectULL( "getBit(4, 4)", getBit(4, 4), 0x8000000);
    expectULL("legal()", legalBoard(getBit(3, 3) | getBit(4, 4), getBit(3, 4) | getBit(4, 3)), 0x80420100000);

//    printf("%llu", partialReversible(player, opponent, getBit(4, 2), VERTICAL_MASK, 8));
    expectInt("countBit(0)", countBit(0), 0);
    expectInt("countBit(1)", countBit(1), 1);
    expectInt("countBit(2)", countBit(1001), 2);
    expectInt("countBit(1010001010001010)", countBit(1010001010001010), 6);
    expectInt("countBit(0xAAAB2149AAAB2149)", countBit(0xAAAB2149AAAB2149), 14);
    expectInt("reverseBit()", reverseBit(0b11000101) >> 24, 0b10100011);

    expectInt("scoreOfRow()", scoreOfRow(0, 0b11000101, 0b00010010), (30 - 12 + 0 + 30) - (-1 - 12));
    expectInt("evaluateScore()", evaluateScore(player, opponent), 0);
}

// endregion