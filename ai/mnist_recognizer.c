#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define INPUT_SIZE 784  // 28x28 piksel
#define HIDDEN_SIZE 128 // Gizli katman nöron sayısı
#define OUTPUT_SIZE 10  // 0-9 arası rakamlar
#define LEARNING_RATE 0.1
#define MAX_EPOCH 10

typedef struct
{
    double *input_layer;
    double *hidden_layer;
    double *output_layer;

    double *hidden_bias;
    double *output_bias;

    double **input_hidden_weights;
    double **hidden_output_weights;
} NeuralNetwork;

// Sigmoid aktivasyon fonksiyonu
double sigmoid(double x)
{
    return 1.0 / (1.0 + exp(-x));
}

// Rastgele ağırlık başlatma
double random_weight()
{
    return ((double)rand() / RAND_MAX) * 2.0 - 1.0;
}

// Sinir ağını başlat
NeuralNetwork *init_network()
{
    NeuralNetwork *nn = (NeuralNetwork *)malloc(sizeof(NeuralNetwork));

    // Katmanları oluştur
    nn->input_layer = (double *)calloc(INPUT_SIZE, sizeof(double));
    nn->hidden_layer = (double *)calloc(HIDDEN_SIZE, sizeof(double));
    nn->output_layer = (double *)calloc(OUTPUT_SIZE, sizeof(double));

    // Bias değerlerini oluştur
    nn->hidden_bias = (double *)calloc(HIDDEN_SIZE, sizeof(double));
    nn->output_bias = (double *)calloc(OUTPUT_SIZE, sizeof(double));

    // Ağırlık matrislerini oluştur
    nn->input_hidden_weights = (double **)malloc(INPUT_SIZE * sizeof(double *));
    for (int i = 0; i < INPUT_SIZE; i++)
    {
        nn->input_hidden_weights[i] = (double *)malloc(HIDDEN_SIZE * sizeof(double));
        for (int j = 0; j < HIDDEN_SIZE; j++)
        {
            nn->input_hidden_weights[i][j] = random_weight();
        }
    }

    nn->hidden_output_weights = (double **)malloc(HIDDEN_SIZE * sizeof(double *));
    for (int i = 0; i < HIDDEN_SIZE; i++)
    {
        nn->hidden_output_weights[i] = (double *)malloc(OUTPUT_SIZE * sizeof(double));
        for (int j = 0; j < OUTPUT_SIZE; j++)
        {
            nn->hidden_output_weights[i][j] = random_weight();
        }
    }

    return nn;
}

// İleri yayılım
void forward_propagation(NeuralNetwork *nn, double *input)
{
    // Giriş katmanını ayarla
    memcpy(nn->input_layer, input, INPUT_SIZE * sizeof(double));

    // Gizli katman hesaplamaları
    for (int i = 0; i < HIDDEN_SIZE; i++)
    {
        double sum = nn->hidden_bias[i];
        for (int j = 0; j < INPUT_SIZE; j++)
        {
            sum += nn->input_layer[j] * nn->input_hidden_weights[j][i];
        }
        nn->hidden_layer[i] = sigmoid(sum);
    }

    // Çıkış katmanı hesaplamaları
    for (int i = 0; i < OUTPUT_SIZE; i++)
    {
        double sum = nn->output_bias[i];
        for (int j = 0; j < HIDDEN_SIZE; j++)
        {
            sum += nn->hidden_layer[j] * nn->hidden_output_weights[j][i];
        }
        nn->output_layer[i] = sigmoid(sum);
    }
}

// Görüntüyü tanı
int recognize_digit(NeuralNetwork *nn, double *image)
{
    forward_propagation(nn, image);

    // En yüksek olasılıklı rakamı bul
    int max_digit = 0;
    double max_probability = nn->output_layer[0];

    for (int i = 1; i < OUTPUT_SIZE; i++)
    {
        if (nn->output_layer[i] > max_probability)
        {
            max_probability = nn->output_layer[i];
            max_digit = i;
        }
    }

    return max_digit;
}

// Görüntüyü yükle (28x28 piksel, gri tonlamalı)
double *load_image(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        printf("Dosya acilamadi: %s\n", filename);
        return NULL;
    }

    double *image = (double *)malloc(INPUT_SIZE * sizeof(double));
    unsigned char pixel;

    for (int i = 0; i < INPUT_SIZE; i++)
    {
        fread(&pixel, 1, 1, file);
        // Pikseli 0-1 arasına normalize et
        image[i] = (double)pixel / 255.0;
    }

    fclose(file);
    return image;
}

// Görüntüyü ekrana yazdır
void print_image(double *image)
{
    for (int i = 0; i < 28; i++)
    {
        for (int j = 0; j < 28; j++)
        {
            // Pikselin yoğunluğuna göre karakter seç
            char c = ' ';
            double pixel = image[i * 28 + j];
            if (pixel > 0.75)
                c = '#';
            else if (pixel > 0.5)
                c = '+';
            else if (pixel > 0.25)
                c = '.';
            printf("%c", c);
        }
        printf("\n");
    }
}

int main()
{
    // Sinir ağını başlat
    NeuralNetwork *nn = init_network();

    printf("El yazisi rakam tanima sistemi hazir!\n");
    printf("Not: Bu ornekte sadece ag yapisi gosterilmistir.\n");
    printf("Gercek bir uygulama icin MNIST veri seti ile egitim gereklidir.\n");

    // Örnek kullanım:
    printf("\nBir rakam goruntusu yuklemek icin dosya yolu girin:\n");
    char filename[256];
    scanf("%255s", filename);

    double *image = load_image(filename);
    if (image)
    {
        printf("\nYuklenen goruntu:\n");
        print_image(image);

        int digit = recognize_digit(nn, image);
        printf("\nTaninan rakam: %d\n", digit);

        free(image);
    }

    // Belleği temizle
    free(nn->input_layer);
    free(nn->hidden_layer);
    free(nn->output_layer);
    free(nn->hidden_bias);
    free(nn->output_bias);

    for (int i = 0; i < INPUT_SIZE; i++)
    {
        free(nn->input_hidden_weights[i]);
    }
    for (int i = 0; i < HIDDEN_SIZE; i++)
    {
        free(nn->hidden_output_weights[i]);
    }

    free(nn->input_hidden_weights);
    free(nn->hidden_output_weights);
    free(nn);

    return 0;
}