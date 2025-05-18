// a = [ n * Σ(xy) - Σx * Σy ] / [ n * Σ(x^2) - (Σx)^2 ]
// b = [ Σy - a * Σx ] /
#include <stdio.h>

int main() {
    // Veri seti
    float x[] = {1, 2, 3, 4, 5};
    float y[] = {2, 3, 5, 4, 6};
    int n = 5;

    float sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;

    // Gerekli toplamlar
    for (int i = 0; i < n; i++) {
        sum_x += x[i];
        sum_y += y[i];
        sum_xy += x[i] * y[i];
        sum_x2 += x[i] * x[i];
    }

    // a ve b hesapla
    float a = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x);
    float b = (sum_y - a * sum_x) / n;

    printf("Dogrusal regresyon denklemi: y = %.2fx + %.2f\n", a, b);

    // Örnek tahmin
    float test_x = 6;
    float predicted_y = a * test_x + b;
    printf("x = %.2f icin tahmini y = %.2f\n", test_x, predicted_y);

    return 0;
}
