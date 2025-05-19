#include <stdio.h>

// Basit veri: X = hava (0: yağmurlu, 1: güneşli), Y = oyun (0: hayır, 1: evet)
#define N 6
int X[N] = {0, 0, 1, 1, 1, 0};
int Y[N] = {0, 1, 1, 1, 0, 0};

float olasilik(int x, int y) {
    int count_xy = 0, count_y = 0;
    for (int i = 0; i < N; i++) {
        if (Y[i] == y) {
            count_y++;
            if (X[i] == x) count_xy++;
        }
    }
    if (count_y == 0) return 0;
    return (float)count_xy / count_y;
}

float sinif_olasiligi(int y) {
    int count = 0;
    for (int i = 0; i < N; i++)
        if (Y[i] == y) count++;
    return (float)count / N;
}

int main() {
    int test_x = 3; // Güneşli
    float p_yes = olasilik(test_x, 1) * sinif_olasiligi(1);
    float p_no  = olasilik(test_x, 0) * sinif_olasiligi(0);

    if (p_yes > p_no)
        printf("Tahmin: Oyun oynanir (gunesli)\n");
    else
        printf("Tahmin: Oyun oynanmaz (gunesli)\n");

    return 0;
}