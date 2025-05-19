#include <stdio.h>

#define N 4     // Veri sayısı
#define EPOCHS 10
#define LEARNING_RATE 0.1

int main() {
    // Girişler (AND kapısı örneği)
    float X[N][2] = {
        {0, 0},
        {0, 1},
        {1, 0},
        {1, 1}
    };
    // Hedef çıktılar
    int Y[N] = {0, 0, 0, 1};

    // Ağırlıklar ve bias
    float w1 = 0, w2 = 0, b = 0;

    // Eğitim döngüsü
    for (int epoch = 0; epoch < EPOCHS; epoch++) {
        for (int i = 0; i < N; i++) {
            // Lineer kombinasyon
            float z = X[i][0] * w1 + X[i][1] * w2 + b;
            // Aktivasyon (adım fonksiyonu)
            int y_pred = z >= 0 ? 1 : 0;
            // Hata
            int error = Y[i] - y_pred;
            // Ağırlık güncelleme
            w1 += LEARNING_RATE * error * X[i][0];
            w2 += LEARNING_RATE * error * X[i][1];
            b  += LEARNING_RATE * error;
        }
    }

    printf("Egitim tamamlandi.\nSon agirliklar: w1=%.2f, w2=%.2f, b=%.2f\n", w1, w2, b);

    // Test
    printf("Test Sonuclari:\n");
    for (int i = 0; i < N; i++) {
        float z = X[i][0] * w1 + X[i][1] * w2 + b;
        int y_pred = z >= 0 ? 1 : 0;
        printf("Giris: [%.0f, %.0f] -> Tahmin: %d, Gercek: %d\n", X[i][0], X[i][1], y_pred, Y[i]);
    }

    return 0;
}