#include "stdarg.h"
#include "string.h"
#include "stdbool.h"
#include "hull.h"

#include "stdio.h"

/// @brief 全ビット同じ hull を返す.
/// @param bit 0 か 1
/// @param result 結果を格納する hull
/// @param size hull 配列の大きさ
void fill(int bit, hull result, int size) {
    ull mask = bit ? ULL_ALL_MASK : 0;

    for (int i = 0; i < size; i++) {
        result[i] = mask;
    }
}

/// @brief hull の論理和
/// @param result 結果を格納する hull
/// @param size hull 配列の大きさ
/// @param h 論理和をとる hull (可変長). 一番最後に NULL を入れる.
void or(hull result, int size, hull h, ...) {
    va_list va;
    va_start(va, h);

    // resultを0で初期化 (論理和だから)
    fill(0, result, size);

    while(1) {
        printf("arg\n");
        hull_p h_p = va_arg(va, hull_p);

        if (h_p == NULL) {
            break;
        }

        // 1要素ずつ論理和をとる
        for (int j = 0; j < size; j++) {
            printf("%llu ", result[j]);
            result[j] |= (*h_p)[j];
            printf("%llu", result[j]);
        }
    }

    va_end(va);
}

/// @brief 入力された hull が等しいかどうかを判定
/// @param a
/// @param sizeA aの配列サイズ
/// @param b
/// @param sizeB bの配列サイズ
bool hull_equals(const hull a, int sizeA, const hull b, int sizeB) {
    if (sizeA != sizeB) {
        return false;
    }

    return memcmp(a, b, sizeA * sizeof(ull)) == 0;
}