#include "hull.h"

/// @brief 与えられた座標のビットのみが立ったビット列を返す
/// @param x 横座標.
/// @param y 縦座標.
/// @param width 盤面の幅(マス数)
/// @param ret 返り値を格納する hull
/// @param size ret の配列サイズ
/// @return 座標のビットのみが立ったビット列
void coord_to_bit(int x, int y, int width, hull ret, int size) {
    int index = x + y * width;
    left_1bit_mask(ret, size, index);
}

void bit_to_coord(hull bit, int size, int width, int *x, int *y) {
    for (int i = 0; i < width; i++) {
        int shift = i * width;
        hull mask =
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