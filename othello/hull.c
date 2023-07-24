#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "hull.h"

typedef ull (*calc_func)(ull, ull);

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

/// @brief ull の論理積
ull ull_and(ull a, ull b) {
    return a & b;
}

/// @brief ull の論理和
ull ull_or(ull a, ull b) {
    return a | b;
}

/// @brief 複数の hull の演算を行う
/// @param result 結果を格納する hull
/// @param size hull 配列の大きさ
/// @param fun hullに実行する演算
/// @param va 可変長配列の va_list. va_start と va_end は呼び出す関数内で行う.
void calc_multi(hull result, int size, calc_func fun, va_list va) {
    while(1) {
        hull_p h_p = va_arg(va, hull_p);

        if (h_p == NULL) {
            break;
        }

        // 1要素ずつ論理和をとる
        for (int j = 0; j < size; j++) {
            result[j] = fun(result[j], (*h_p)[j]);
        }
    }
}

/// @brief hull の論理和
/// @param result 結果を格納する hull
/// @param size hull 配列の大きさ
/// @param ... hull の可変長配列. 最後に NULL を入れる！
void or(hull result, int size, ...) {
    va_list va;
    va_start(va, size);
    // resultを0で初期化 (論理和だから)
    fill(0, result, size);
    calc_multi(result, size, ull_or, va);
    va_end(va);
}

/// @brief hull の論理積
/// @param result 結果を格納する hull
/// @param size hull 配列の大きさ
/// @param ... hull の可変長配列. 最後に NULL を入れる！
void and(hull result, int size, ...) {
    va_list va;
    va_start(va, size);
    // resultを0で初期化 (論理積だから)
    fill(1, result, size);
    calc_multi(result, size, ull_and, va);
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

/// @brief hull を出力する
void printHULL(const hull h, int size) {
    for (int i = 0; i < size; ++i) {
        printf("%llu", h[i]);
    }
    printf("\n");
}