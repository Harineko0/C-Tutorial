#include "hull.h"

/// @brief 与えられた座標のビットのみが立ったビット列を返す
/// @param x 横座標.
/// @param y 縦座標.
/// @param width 盤面の横幅(縦でもいい)
/// @param ret 返り値を格納する hull
/// @param size ret の配列サイズ
/// @return 座標のビットのみが立ったビット列
void coordToBit(int x, int y, int width, hull ret, int size) {
    int index = x + y * width;
    left_1bit_mask(ret, size, index);
}