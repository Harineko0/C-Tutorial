/// カレンダー

#include "stdio.h"
#include "stdbool.h"
#include "string.h"

// 曜日を取得する. 日 -> 0, 月 -> 1
int getDay(int year, int month, int day) {
    int y = year;
    int m = month;

    if (m < 3) {
        // 1, 2月のとき 年は１年減らし, 月は12こ増やす
        y -= 1;
        m += 12;
    }

    int w = ((y + (y / 4) - (y / 100) + (y / 400) + (13 * (m + 1) / 5) + day - 1)) % 7;

    if (w < 0) {
        w += 7;
    }

    return w;
}

// うるう年かどうかを返す
bool getIsLeapYear(int year) {
    return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
}

// うるう年であるかどうかと月を入力として、その月の最大の日付を返す
int getMaxDate(bool isLeap, int month) {
    if (month == 2) {
        return isLeap ? 29 : 28;
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    } else {
        return 31;
    }
}

// 入力した char[] にカレンダーを入れる
void calender(char *sCalender, int year, int month) {
    // １日(ついたち)の曜日
    int firstDay = getDay(year, month, 1);
    bool isLeapYear = getIsLeapYear(year);
    int maxDate = getMaxDate(isLeapYear, month);

    // 文字列初期化
    char sYear[5], sMonth[3], sDate[4];

    snprintf(sYear, 5, "%d", year);
    snprintf(sMonth, 3, "%d", month);

    strcat(sCalender, sYear);
    strcat(sCalender, " / ");
    strcat(sCalender, sMonth);
    strcat(sCalender, "\nSun Mon Tue Wed Thu Fri Sat");
    strcat(sCalender, "\n");

    // 最初の曜日まで空白を挿入
    for (int i = 0; i < firstDay; i++) {
        strcat(sCalender, "    ");
    }


    for (int date = 1; date <= maxDate; date++) {
        // 日曜日で改行
        if (((date + firstDay) % 7) == 1) {
            strcat(sCalender, "\n");

        }

        snprintf(sDate, 34, "%3d", date);
        strcat(sCalender, sDate);
        strcat(sCalender, " ");
    }
}

void testString(char *testName, char *input, const char *expect) {
    if (strstr(input, expect) != NULL) {
        printf("\no %s: success\n", testName);
    } else {
        printf("\nx %s: failure\ninput: \n%s", testName, input);
    }
}

void testInt(char *testName, int input, int expect) {
    if (input == expect) {
        printf("\no %s: success\n", testName);
    } else {
        printf("\nx %s: failure\ninput: \n%d\n", testName, input);
    }
}

void tests() {
    char sCalender1[200];
    calender(sCalender1, 2023, 5);
    testString("Test 2023/05", sCalender1,
               "2023 / 5\n"
               "Sun Mon Tue Wed Thu Fri Sat\n"
               "      1   2   3   4   5   6 \n"
               "  7   8   9  10  11  12  13 \n"
               " 14  15  16  17  18  19  20 \n"
               " 21  22  23  24  25  26  27 \n"
               " 28  29  30  31 ");

    char sCalender2[200];
    calender(sCalender2, 2023, 2);
    testString("Test 2023/02", sCalender2,
               "2023 / 2\n"
               "Sun Mon Tue Wed Thu Fri Sat\n"
               "               1   2   3   4\n"
               "  5   6   7   8   9  10  11\n"
               "  12  13  14  15  16  17  18\n"
               "  19  20  21  22  23  24  25\n"
               "  26  27  28");

    char sCalender3[200];
    calender(sCalender3, 2020, 4);
    testString("Test 2020/04", sCalender3,
               "2020 / 4\n"
               " Sun Mon Tue Wed Thu Fri Sat\n"
               "               1   2   3   4\n"
               "   5   6   7   8   9  10  11\n"
               "  12  13  14  15  16  17  18\n"
               "  19  20  21  22  23  24  25\n"
               "  26  27  28  29  30\n");

    testInt("Test getDay() 2000/02/01", getDay(2000, 2, 1), 2);
    testInt("Test getDay() 2023/05/01", getDay(2023, 5, 1), 1);
}

int main() {
//    tests();

    int year = 0, month = 0;

    do {
        printf("Please enter year (greater than 1): ");
        scanf("%d", &year);
    } while (year < 1);

    do {
        printf("Please enter month (1 ~ 12): ");
        scanf("%d", &month);
    } while (month > 12 || month < 1);

    printf("\n");

    char sCalender1[200];
    calender(sCalender1, year, month);

    printf("%s", sCalender1);
}