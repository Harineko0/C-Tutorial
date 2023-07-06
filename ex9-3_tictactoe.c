#include "stdio.h"
#include "string.h"
#define N 3 // N目並べ
#define O 0
#define X 1
#define E (-1) // empty
#define PLAYER1 1
#define PLAYER2 2
#define HORIZONTAL 0
#define VERTICAL 1
#define DIAGONAL 2

void printTable(int length, int table[length][length]) {
    for (int i = 0; i < length; i++) {

        for (int j = 0; j < length; j++) {

            int cell = table[j][i];

            switch (cell) {
                case O:
                    printf(" O ");
                    break;
                case X:
                    printf(" X ");
                    break;
                case E:
                    printf("   ");
                    break;
                default:
                    break;
            }

            if (j != length - 1) {
                printf("┃");
            } else {
                printf("\n");
            }
        }

        if (i != length - 1) {
            printf("━━━╋━━━╋━━━\n");
        }
    }
}

void inputCell(int currentPlayer, int *x, int *y) {
    int tmpX, tmpY;

    printf("Player%d\n", currentPlayer);
    printf("  x: ");
    scanf(" %d", &tmpX);

    while (tmpX > N) {
        printf("x should be under %d\n", N);
        printf("  reenter x: ");
        scanf(" %d", &tmpX);
    }

    printf("\n  y: ");
    scanf(" %d", &tmpY);

    while (tmpY > N) {
        printf("y should be under %d\n", N);
        printf("  reenter y: ");
        scanf(" %d", &tmpY);
    }

    printf("\n");

//    x = tmpX;
//    y
}

int max(int a, int b) {
    return a > b ? a : b;
}

/// 連続しているか
/// return: 0 or 1
int isContinuous(int length, int table[length][length], int direction, int x, int y) {
    int count = 0;
    int cell = table[x][y];
    int tmpX = x, tmpY = y;

    if (cell == E) return 0;

    for (int i = 0; i < length - 1 - max(x, y); i++) {
        int diff;

        switch (direction) {
            case HORIZONTAL:
                tmpX += x == length - 1 ? -1 : 1;
                break;
            case VERTICAL:
                tmpY += y == length - 1 ? -1 : 1;
                break;
            case DIAGONAL:
                diff = (x == length - 1 && y == length - 1) ? -1 : 1;
                tmpX += diff;
                tmpY += diff;
                break;
        }

        int thisCell = table[tmpX][tmpY];

        if (thisCell != cell) {
            return 0;
        } else {
            count++;
        }
    }

    return count == length - 1 ? 1 : 0;
}

/// return o or 1
int isDone(int length, int table[length][length]) {
    // 縦1列と横１列だけで良い

    // 横
    for (int i = 0; i < length; i++) {
        int continuous = isContinuous(length, table, VERTICAL, i, 0)
                | isContinuous(length, table, HORIZONTAL, 0, i);

        if (i == 0 || i == length - 1) {
            continuous |= isContinuous(length, table, DIAGONAL, i, 0)
                    |  isContinuous(length, table, DIAGONAL, 0, i);
        }

        if (continuous == 1) {
            return  1;
        }
    }

    return 0;
}

int main() {
//    int table[N][N] = {{O, O, O},
//                       {E, E, E},
//                       {E, E, E}};
    int table[N][N];
    int currentPlayer = PLAYER1; // 0 or 1
    int done = 0; // 0: false, 1: true

    // 全てemptyに初期化
    memset(table, E, sizeof(table));

    while (done == 0) {

        printf("-------------------------------------\n");
        printTable(N, table);

        int x = -1, y = -1;
//        inputCell(currentPlayer, &x, &y);
        // region inputKey
        while (1) {

            printf("Player%d\n", currentPlayer);
            printf("  x: ");
            scanf(" %d", &x);

            while (x > N) {
                printf("x should be under %d\n", N);
                printf("  reenter x: ");
                scanf(" %d", &x);
            }

            printf("  y: ");
            scanf(" %d", &y);

            while (y > N) {
                printf("y should be under %d\n", N);
                printf("  reenter y: ");
                scanf(" %d", &y);
            }

            int isNotEmpty = table[x][y] != E;

            if (isNotEmpty) {
                printf("(%d, %d) is not empty.\n", x, y);
            } else {
                break;
            }
        }
        // endregion

        table[x][y] = currentPlayer == PLAYER1 ? O : X;

        done = isDone(N, table);

        if (done) {
            printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
            printTable(N, table);
            printf("WINNER Player%d", currentPlayer);
            break;
        }

        currentPlayer = currentPlayer == PLAYER1 ? PLAYER2 : PLAYER1;
    }
}