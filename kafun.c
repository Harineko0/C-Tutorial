#include<stdio.h>
// SUN -> 0
// MON -> 1
// TUE: 2
// WEN: 3
// THU: 4

int main() {
    int ya, yb, ma, mb = 0, d, w, maxDate, k;

    printf("year:");
    scanf("%d", &ya);
    printf("month:");
    scanf("%d", &ma);

    if ((ma > 0) && (ma < 13)) {
        if ((ma == 1) || (ma == 2)) {
            mb = ma + 12;
            yb = ya - 1;
        } else {
            // mbの初期値?
            mb = ma;
            yb = ya;
        }
        printf("%d/%d\n", ya, ma);
        printf(" Sun");
        printf(" Mon");
        printf(" Tue");
        printf(" Wed");
        printf(" Thu");
        printf(" Fri");
        printf(" Sat\n");
        switch (ma) {
            case 1:

            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12:
                maxDate = 31;
                break;
            case 4:
            case 6:
            case 9:
            case 11:
                maxDate = 30;
                break;
            case 2:
                if (((ya % 4 == 0) && (ya % 100 != 0)) || (ya % 400 == 0)) {
                    maxDate = 29;
                } else {
                    maxDate = 28;
                }
                break;
        }

//        w = (yb + (yb / 4) - (yb / 100) + (yb / 400) + (13 * (mb + 1) / 5) + 1) % 7;
        int w = ((yb + (yb / 4) - (yb / 100) + (yb / 400) + (13 * (mb + 1) / 5))) % 7;

        for (k = 0; k < w; k = k + 1) {
            printf("    ");
        }

        for (d = 1; d <= maxDate; d = d + 1) {
            int currentW = (w + d - 1) % 7;

            if (currentW == 6) {

                printf("%4d\n", d);
            } else {
                printf("%4d", d);
            }
        }
    } else {
        printf("入力された月が適切ではありません\n");
    }
}