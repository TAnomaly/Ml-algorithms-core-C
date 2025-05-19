#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdio.h>
#include <stdlib.h>

// Görüntüyü yükle ve MNIST formatına dönüştür
double *load_and_preprocess_image(const char *filename)
{
    int width, height, channels;
    unsigned char *img = stbi_load(filename, &width, &height, &channels, 0);

    if (!img)
    {
        printf("Goruntu yuklenemedi: %s\n", filename);
        return NULL;
    }

    // MNIST için 28x28 boyutunda bir görüntü oluştur
    double *mnist_image = (double *)malloc(28 * 28 * sizeof(double));

    // Görüntüyü yeniden boyutlandır ve gri tonlamaya çevir
    for (int y = 0; y < 28; y++)
    {
        for (int x = 0; x < 28; x++)
        {
            // Orijinal görüntüdeki karşılık gelen koordinatları hesapla
            int orig_x = x * width / 28;
            int orig_y = y * height / 28;

            // Pikseli al ve gri tonlamaya çevir
            int idx = (orig_y * width + orig_x) * channels;
            double gray;

            if (channels == 1)
            {
                // Zaten gri tonlamalı
                gray = img[idx];
            }
            else if (channels == 3 || channels == 4)
            {
                // RGB veya RGBA -> Gri tonlama
                gray = 0.299 * img[idx] +     // Kırmızı
                       0.587 * img[idx + 1] + // Yeşil
                       0.114 * img[idx + 2];  // Mavi
            }

            // 0-1 arasına normalize et
            mnist_image[y * 28 + x] = gray / 255.0;
        }
    }

    stbi_image_free(img);
    return mnist_image;
}

// Görüntüyü ekrana yazdır (ASCII art olarak)
void print_processed_image(double *image)
{
    for (int i = 0; i < 28; i++)
    {
        for (int j = 0; j < 28; j++)
        {
            double pixel = image[i * 28 + j];
            char c = ' ';
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