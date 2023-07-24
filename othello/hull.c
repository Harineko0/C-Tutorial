#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "hull.h"

typedef ull (*calc_func)(ull, ull);
/// @brief hull のポインタ
typedef unsigned long long (*hull_p)[];

// region ull
/// @brief ull の論理積
ull ull_and(ull a, ull b) {
    return a & b;
}

/// @brief ull の論理和
ull ull_or(ull a, ull b) {
    return a | b;
}

/// @brief ull の論理否定
/// @param b これは使わない
ull ull_not(ull a) {
    return ~a;
}

/// @brief ull の排他的論理和
ull ull_xor(ull a, ull b) {
    return a ^ b;
}

/// @brief 左から index (>= 0) 番目のビットのみが立ったビットマスクを返す
ull ull_left_1bit_mask(int index) {
    return 0x8000000000000000 >> index;
}

/// @brief から index (>= 0) 番目のビットのみが立ったビットマスクを返す
ull ull_right_1bit_mask(int index) {
    return 1 << index;
}

/// @brief 左から num 番目までのビットが立っているマスクを返す
ull ull_leftside_mask(int num) {
    // キャッシュ
    static ull masks[64];
    static bool initialized = false;

    // 初期化
    if (!initialized) {
        for (int i = 0; i < 64; i++) {
            ull mask = 0;

            for (int j = 0; j < i; j++) {
                mask |= ull_left_1bit_mask(j);
            }

            masks[i] = mask;
        }
        initialized = true;
    }

    return masks[num];
}

/// @brief 右から num 番目までのビットが立っているマスクを返す
ull ull_rightside_mask(int num) {
    // キャッシュ
    static ull masks[64];
    static bool initialized = false;

    // 初期化
    if (!initialized) {
        for (int i = 0; i < 64; i++) {
            ull mask = 0;

            for (int j = 0; j < i; j++) {
                mask |= ull_right_1bit_mask(j);
            }

            masks[i] = mask;
        }
        initialized = true;
    }

    return masks[num];
}
// endregion

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

/// @brief hull の排他的論理和
/// @param result 結果を格納する hull
/// @param size hull 配列の大きさ
/// @param ... hull の可変長配列. 最後に NULL を入れる！
void xor(hull result, int size, ...) {
    va_list va;
    va_start(va, size);
    // resultを0で初期化 (排他的論理和だから)
    fill(0, result, size);
    calc_multi(result, size, ull_xor, va);
    va_end(va);
}

/// @brief hull の論理否定
/// @param result 結果を格納する hull
/// @param size hull 配列の大きさ
/// @param input 入力の hull
void not(hull result, int size, hull input) {
    for (int i = 0; i < size; i++) {
        result[i] = ull_not(input[i]);
    }
}

/// @brief 右シフト演算
/// @param h hull
/// @param size hull の size
/// @param amount シフトする量
void shiftR(hull h, int size, int amount) {
    ull mask = ull_rightside_mask(amount);

    // 配列の一番右の要素からシフト
    for (int i = size - 1; i >= 0; i--) {
        h[i] >>= amount;

        // 仮想的に次の要素からもシフトさせる. i=0 のときはやらない(一番左だから次の要素がない)
        if (i > 0) {
            ull next = h[i - 1];
            next &= mask;
            next <<= 64 - amount;
            h[i] |= next;
        }
    }
}

/// @brief 右シフト演算
/// @param h hull
/// @param size hull の size
/// @param amount シフトする量
void shiftL(hull h, int size, int amount) {
    ull mask = ull_leftside_mask(amount);

    // 配列の一番右の要素からシフト
    for (int i = 0; i < size; i++) {
        h[i] <<= amount;

        // 仮想的に次の要素からもシフトさせる.
        if (i + 1 < size) {
            ull next = h[i + 1];
            next &= mask;
            next >>= 64 - amount;
            h[i] |= next;
        }
    }
}

/// @brief ull の立っているビット数をカウントする
int ull_count_bit(ull bits) {
    bits = (bits & 0x5555555555555555) + (bits >> 1 & 0x5555555555555555);
    bits = (bits & 0x3333333333333333) + (bits >> 2 & 0x3333333333333333);
    bits = (bits & 0x0f0f0f0f0f0f0f0f) + (bits >> 4 & 0x0f0f0f0f0f0f0f0f);
    bits = (bits & 0x00ff00ff00ff00ff) + (bits >> 8 & 0x00ff00ff00ff00ff);
    bits = (bits & 0x0000ffff0000ffff) + (bits >>16 & 0x0000ffff0000ffff);
    bits = (bits & 0x00000000ffffffff) + (bits >>32 & 0x00000000ffffffff);

    return bits;
}

/// @brief hull の立っているビット数をカウントする
/// @param h
/// @param size h の配列サイズ
int count_bit(hull h, int size) {
    int count = 0;

    for (int i = 0; i < size; ++i) {
        count += ull_count_bit(h[i]);
    }

    return count;
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
        printf("%llu ", h[i]);
    }
    printf("\n");
}