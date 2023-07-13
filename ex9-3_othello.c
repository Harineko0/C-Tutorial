#include "stdio.h"
#include "stdlib.h"
#include <stdbool.h>
#include <string.h>
#include <conio.h>
// sleep
#include <unistd.h>

#define WHITE 0
#define BLACK 1
#define HORIZONTAL_MASK 0x7e7e7e7e7e7e7e7e
#define VERTICAL_MASK 0x00FFFFFFFFFFFF00
#define DIAGONAL_MASK 0x007e7e7e7e7e7e00
#define INFINITY 2147483648
// getArrowKey()
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define ENTER 5
#define INVALID 0
#define QUIT 6
// AI
#define DEPTH 8

typedef int Color;
typedef int Direction;
typedef unsigned long long ull;
typedef unsigned int uint;
typedef ull (*shifter)(ull, int);

const ull ONE_BIT_MASK_MAX = 0x8000000000000000;

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

/// @brief 左から index (>= 0) 番目のビットのみが立ったビットマスクを返す
ull get1BitMask(int index) {
    return 0x8000000000000000 >> index;
}

/// @brief 左から index (>= 0) 番目の8ビットのみが立ったビットマスクを返す
ull get8BitMask(int index) {
    return 0xFF00000000000000 >> index;
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

/// @brief 入力されたビット列の最初に立っているビットの index (左から数えて) を返します. 入力が 0 の場合, -1 を返します.
/// @param max ビット列の長さ. (ビット列が 00101100 だった場合、max は 8 となる)
int getFirstBitIndex(ull bits, int max) {
    for (int i = 0; i < max; i++) {
        ull mask = ONE_BIT_MASK_MAX >> i;

        if ((bits & mask) != 0) {
            return i;
        }

    }

    return -1;
}

int max(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
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

/// @brief 入力された bit 列を 8*8 と解釈し、そのうえでの座標を x, y に格納します
void bitToCoord(ull bit, int *x, int *y) {
    for (int i = 0; i < 8; i++) {
        int shift = i * 8;
        ull row = ((0xFF00000000000000 >> shift) & bit);
        if (row != 0) {
            int unshift = (7 - i) * 8;
            row >>= unshift;

            for (int j = 0; j < 8; j++) {
                if (((0b10000000 >> j) & row) != 0) {
                    *x = j;
                    *y = i;
                    return;

                }
            }

        }
    }
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

/// @brief 盤面に put の石を置き、ひっくり返す.
/// @param player プレイヤーの盤面で, ひっくり返される.
/// @param opponent
/// @param put
void place(ull* player, ull* opponent, ull put) {
    ull reverse = reversible(*player, *opponent, put);
    *player |= reverse;
    *player |= put;
    *opponent ^= reverse;
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

/// @brief プレイヤーに対する盤面のスコアを計算する
/// @param player プレイヤーの盤面
/// @param opponent 敵の盤面
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

/// @brief nega-max法で盤面のスコアを求める
int negMaxScore(ull player, ull opponent, int depth, bool passed) {
    // 葉ノード -> スコアを返す
    if (depth == 0) {
        return evaluateScore(player, opponent);

    } else {
        ull legal = legalBoard(player, opponent);
        int score = -INFINITY;

        // 合法手について同じ探索
        for (int i = 0; i < 64; i++) {
            ull slot = 0x1 << i;

            if (slot & legal) {
                // slotが合法手ならば
                ull newPlayer = player;
                ull newOpponent = opponent;
                place(&newPlayer, &newOpponent, slot);

                score = max(score, -negMaxScore(newPlayer, newOpponent, depth - 1, false));

            }
        }

        // パス・終局の処理
        if (score == -INFINITY) {
            // 2連パス(終局) -> evaluateScore
            if (passed) {
                return evaluateScore(player, opponent);
            } else {
                return -negMaxScore(opponent, player, depth - 1, true);
            }
        }

        return score;
    }
}

/// @brief 最もいい手を返す
ull searchBest(ull player, ull opponent, int depth) {
    ull legal = legalBoard(player, opponent), bestStone = 0;
    int maxScore = -INFINITY, score;

    for (int i = 0; i < 64; i++) {
        ull slot = 0x8000000000000000 >> i;

        if (slot & legal) {
            score = -negMaxScore(player, opponent, depth - 1, false);

            if (maxScore < score) {
                maxScore = score;
                bestStone = slot;

            }
        }
    }

    return bestStone;
}

ull getRandom(ull player, ull opponent) {
    ull legal = legalBoard(player, opponent);
    int legalCount = countBit(legal);
    int randomPlace = rand() % legalCount;
    int counter = 0;

    for (int i = 0; i < 64; i++) {
        ull mask = ONE_BIT_MASK_MAX >> i;

        if (mask & legal) {
            counter++;

            if (counter == randomPlace) {
                return mask;
            }
        }
    }

    return 0;
}
// endregion

// region IO

/// @brief オセロと同じ量改行する
void printBoardSpacer() {
    for (int i = 0; i < 16; i++) {
        printf("\n");
    }
}

/// @brief オセロの盤面を置き換えるエスケープを出力する
void printBoardReplacer() {
    printf("\033[16F");
}

/// @brief オセロの盤面を出力する
void printBoard(ull player, ull opponent, Color playerColor, ull cursor) {
    printBoardReplacer();
    printf("\033[1m");
    printf("   A   B   C   D   E   F   G   H\n");

    ull legal = legalBoard(player, opponent);

    bool isPlayer = !playerColor;
    ull human = isPlayer ? player : opponent;
    ull computer = isPlayer ? opponent : player;

    for (int i = 0; i < 8; i++) {
        printf("%d \033[42m", i + 1);

        // ボード本体
        for (int j = 0; j < 8; j++) {

            ull thisBit = getBit(j, i);
            bool isHumanStone = (human & thisBit) != 0;
            bool isComputerStone = (computer & thisBit) != 0;
            bool isLegalStone = (legal & thisBit) != 0;
            bool isEmpty = (isHumanStone | isComputerStone) == 0;
            bool isCursor = (cursor & thisBit) != 0;

            if (isEmpty) {
                if (isCursor && isPlayer) {
                    printf("\033[33m + \033[37m");
                } else if (isLegalStone && isPlayer) {
                    printf("\033[33m - \033[37m");
                } else {
                    printf("   ");
                }

            } else if (isComputerStone) {
                printf(" O ");

            } else if (isHumanStone) {
                printf("\033[30m @ \033[37m");
            }

            if (j < 7) {
                printf("\033[30m┃\033[37m");
            } else {
                printf("\033[40m");
            }
        }

        // 右側に表示するサブ情報
        if (i == 0) {
            // FIXME: 数値がバグる
            printf("   You:      \033[30m@\033[37m - %d, human: %llu", countBit(human), human);

        } else if (i == 1) {
            printf("   Computer: O - %d, compu: %llu", countBit(computer), computer);

        }

        if (i < 7) {
            printf("\n  \033[42m\033[30m━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━\033[37m\033[40m\n");
        }
    }
    printf("\n");
    printf("\033[m");
}

void printWon(ull player, ull opponent, bool playerIsHuman) {
    int playerStone = countBit(player);
    int opponentStone = countBit(opponent);
    bool humanWon = (playerStone > opponentStone) ^ playerIsHuman;

    printf("\n~~~~~~~ %s WON! ~~~~~~~", humanWon ? "YOU" : "COMPUTER" );
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

/// @brief 座標の入力から, ボード上の座標を返します. 合法手になるまで入力を繰り返します.
void inputKey(ull player, ull opponent, int* x, int* y) {
    char coordinate[20] = "";
    int tmpX = -1, tmpY = -1;

    while (1) {
        printf("\nWhere to place? ⋯ ");
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

/// @brief 矢印キーの入力を得ます.
/// @return INVALID(0), UP(1), DOWN(2), LEFT(3), RIGHT(4), ENTER(5)
Direction getArrowKey() {
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

/// @brief 水平方向でカーソルに最も近い合法手を返します
ull nearestLegalHorizontal(ull cursor, ull legal) {
    ull leftCursor = cursor;
    ull rightCursor = cursor;

    int x, y;
    bitToCoord(cursor, &x, &y);

    ull legalRow = legal & get8BitMask(y * 8);

    // 同じ列に合法手があるとき
    if ((cursor & legalRow) != 0) {
        return cursor;
    }

    for (int j = 0; j < max(7 - x, x); j++) {

        leftCursor <<= 1;
        rightCursor >>= 1;

        if (leftCursor & legalRow) {
            return leftCursor;
        } else if (rightCursor & legalRow) {
            return  rightCursor;
        }
    }
    return 0;
}

// FIXME: カーソルが左上にあるときに動かなくなるバグ
/// @brief カーソルの入力から, ボード上の座標を返します.
void inputCursor(ull player, ull opponent, Color playerColor, int* x, int* y) {
    ull legal = legalBoard(player, opponent);
    int firstIndex = getFirstBitIndex(legal, 64);
    ull firstBit = ONE_BIT_MASK_MAX >> firstIndex;
    ull cursor = firstBit;
    // 座標選択で使う
    int tmpX, tmpY;
    bitToCoord(cursor, &tmpX, &tmpY);
    Direction direction;

    do {
        printBoard(player, opponent, playerColor, cursor);
        direction = getArrowKey();

        if (direction == LEFT || direction == RIGHT) {
            shifter shift = direction == LEFT ? shiftL : shiftR;
            int searchDist = direction == LEFT ? tmpX : 7 - tmpX;
            ull tmpCursor = cursor;

            for (int i = 0; i < searchDist; i++) {
                tmpCursor = shift(tmpCursor, 1);

                if ((legal & tmpCursor) != 0) {
                    cursor = tmpCursor;
                    bitToCoord(cursor, &tmpX, &tmpY);
                    break;
                } else {
                    continue;
                }

            }

            if (searchDist == 0) {
                cursor = tmpCursor;
            }

        } else if (direction == UP || direction == DOWN) {
            shifter shift = direction == UP ? shiftL : shiftR;
            int searchDist = direction == UP ? tmpY : 7 - tmpY;
            ull tmpCursor = cursor;
            /// 探索する量

            for (int i = 0; i < searchDist; i++) {
                tmpCursor = shift(tmpCursor, 8);

                ull nearestLegal = nearestLegalHorizontal(tmpCursor, legal);

                if (nearestLegal != 0) {
                    cursor = nearestLegal;
                    bitToCoord(cursor, &tmpX, &tmpY);
                    break;
                } else {
                    continue;
                }

            }

            if (searchDist == 0) {
                cursor = tmpCursor;
            }

        } else if (direction == QUIT) {
            exit(0);
        }

    } while (direction != ENTER);
    // エンター入力するまでループ
    // 矢印取得
    // 矢印の方向に石があるか確認
    // あったら座標を移動

    bitToCoord(cursor, x, y);
}

// endregion

void game() {
    ull player = getBit(3, 3) | getBit(4, 4);
    ull opponent = getBit(3, 4) | getBit(4, 3);
    Color color = WHITE;

    printBoardSpacer();

    while (1) {
        printBoard(player, opponent, color, 0);
//        printf("Current Player: %s\n", !color ? "You" : "Computer");

        ull put;
        /// 初期化
        int x = -1, y = -1;

        if (color == WHITE) {
            // Player(Human)
//            inputKey(player, opponent, &x, &y);
//            inputCursor(player, opponent, color, &x, &y);
//            put = getBit(x, y);

            sleep(1);
            put = getRandom(player, opponent);
            bitToCoord(put, &x, &y);

        } else {
            // Computer
            // 1秒スリープ
            sleep(1);
            put = searchBest(player, opponent, DEPTH);
            bitToCoord(put, &x, &y);
        }


        // 反転
        place(&player, &opponent, put);

        // 終了判定
        ull playerLegal = legalBoard(player, opponent);
        ull opponentLegal = legalBoard(opponent, player);

        if (isFinished(playerLegal, opponentLegal)) {
            printBoard(player, opponent, color, 0);
            printWon(player, opponent, color == BLACK);
            break;

            // 反転させて次回のターンのパスを判定
        } else if (isPass(playerLegal, opponentLegal)) {
            // FIXME: パスしたときにカーソルバグる
            printf("Passed!");
            ull tmp = player;
            player = opponent;
            opponent = tmp;
            color = !color;
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
        printf("\033[31mx\033[37m Test %s failed (input: %llu)\n", testName, input);
    }
}

void expectInt(char* testName, int input, int expect) {
    if (input == expect) {
        printf("\033[32m✓\033[37m Test %s passed\n", testName);
    } else {
        printf("\033[31mx\033[37m Test %s failed. (input: %d)\n", testName, input);
    }
}

void test() {
    ull player = getBit(3, 3) | getBit(4, 4);
    ull opponent = getBit(3, 4) | getBit(4, 3);
    int x, y;

    expectULL( "getBit(4, 4)", getBit(4, 4), 0x8000000);
    expectULL("legal()", legalBoard(getBit(3, 3) | getBit(4, 4), getBit(3, 4) | getBit(4, 3)), 0x80420100000);

    bitToCoord(getBit(3, 4), &x, &y);
    expectInt("bitToCoord() of x", x, 3);
    expectInt("bitToCoord() of y", y, 4);

//    printf("%llu", partialReversible(player, opponent, getBit(4, 2), VERTICAL_MASK, 8));
    expectInt("countBit(0)", countBit(0), 0);
    expectInt("countBit(1)", countBit(1), 1);
    expectInt("countBit(2)", countBit(1001), 2);
    expectInt("countBit(1010001010001010)", countBit(1010001010001010), 6);
    expectInt("countBit(0xAAAB2149AAAB2149)", countBit(0xAAAB2149AAAB2149), 14);
    expectInt("reverseBit()", reverseBit(0b11000101) >> 24, 0b10100011);

    expectInt("scoreOfRow()", scoreOfRow(0, 0b11000101, 0b00010010), (30 - 12 + 0 + 30) - (-1 - 12));
    expectInt("evaluateScore()", evaluateScore(player, opponent), 0);

    expectInt("getFirstBitIndex()", getFirstBitIndex(0b00101000, 8), 2);
    expectInt("getFirstBitIndex()", getFirstBitIndex(0b10101100, 8), 0);
    expectInt("getFirstBitIndex()", getFirstBitIndex(0b00000001, 8), 7);
    expectInt("getFirstBitIndex()", getFirstBitIndex(0b00000000, 8), -1);

    expectULL("nearestLegalHorizontal()", nearestLegalHorizontal(0b00100000, 0b01000110), 0b01000000);
    expectULL("nearestLegalHorizontal()", nearestLegalHorizontal(0b00100000, 0b00100110), 0b00100000);
    expectULL("nearestLegalHorizontal()", nearestLegalHorizontal(0b10000000, 0b10100110), 0b10000000);
    expectULL("nearestLegalHorizontal()", nearestLegalHorizontal(0b10000000, 0b01100110), 0b01000000);
    expectULL("nearestLegalHorizontal()", nearestLegalHorizontal(0b00000001, 0b00100111), 0b00000001);
    expectULL("nearestLegalHorizontal()", nearestLegalHorizontal(0b00000001, 0b00100110), 0b00000010);
}

// endregion