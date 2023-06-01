#include <stdio.h>

enum Day {
    MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY
};

enum Day calcDay(int year, int month, int day) {
    int y = year;
    int m = month;

    if (m < 3) {
        // 1, 2月のとき 年は１年減らし, 月は12こ増やす
        y -= 1;
        m += 12;
    }

    int w = ((497 * y + 1040 * m + 400 * day + 640) / 400) % 7;

    // 曜日は、0～6で土曜日～金曜日を表す。）
    switch (w) {
        case 0:
            printf("SATURDAY");
            return SATURDAY;
        case 1:
            printf("SUNDAY");
            return SUNDAY;
        case 2:
            printf("MONDAY");
            return MONDAY;
        case 3:
            printf("TUESDAY");
            return TUESDAY;
        case 4:
            printf("WEDNESDAY");
            return WEDNESDAY;
        case 5:
            printf("THURSDAY");
            return THURSDAY;
        case 6:
            printf("FRIDAY");
            return FRIDAY;
    }
}

int main() {
    int year, month, d;

    printf("Please enter year\n");
    scanf("%d", &year);

    printf("Please enter month\n");
    scanf("%d", &month);

    printf("Please enter day\n");
    scanf("%d", &d);

    enum Day result = calcDay(year, month, d);

}