#include <stdio.h>
#include <stdlib.h>

// MNIST dosya başlıklarını okuma fonksiyonları
unsigned int read_int(FILE *fp)
{
    unsigned char bytes[4];
    fread(bytes, 1, 4, fp);
    return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
}

// MNIST veri seti yapısı
typedef struct
{
    unsigned char *images; // Görüntüler
    unsigned char *labels; // Etiketler (doğru rakamlar)
    int number_of_images;  // Görüntü sayısı
    int image_size;        // Her görüntünün boyutu (28*28=784)
} MNISTDataset;

// MNIST veri setini yükle
MNISTDataset *load_mnist(const char *image_file, const char *label_file)
{
    FILE *fp_images = fopen(image_file, "rb");
    FILE *fp_labels = fopen(label_file, "rb");

    if (!fp_images || !fp_labels)
    {
        printf("Dosyalar acilamadi!\n");
        return NULL;
    }

    MNISTDataset *dataset = (MNISTDataset *)malloc(sizeof(MNISTDataset));

    // Görüntü dosyası başlığını oku
    int magic_number = read_int(fp_images);
    dataset->number_of_images = read_int(fp_images);
    int n_rows = read_int(fp_images);
    int n_cols = read_int(fp_images);
    dataset->image_size = n_rows * n_cols;

    // Etiket dosyası başlığını oku
    magic_number = read_int(fp_labels);
    int number_of_labels = read_int(fp_labels);

    // Görüntüleri ve etiketleri oku
    dataset->images = (unsigned char *)malloc(dataset->number_of_images * dataset->image_size);
    dataset->labels = (unsigned char *)malloc(dataset->number_of_images);

    fread(dataset->images, 1, dataset->number_of_images * dataset->image_size, fp_images);
    fread(dataset->labels, 1, dataset->number_of_images, fp_labels);

    fclose(fp_images);
    fclose(fp_labels);

    printf("Yuklenen veri seti:\n");
    printf("Goruntu sayisi: %d\n", dataset->number_of_images);
    printf("Goruntu boyutu: %dx%d\n", n_rows, n_cols);

    return dataset;
}

// Görüntüyü normalize et (0-255 -> 0-1)
void normalize_image(unsigned char *raw_image, double *normalized_image, int size)
{
    for (int i = 0; i < size; i++)
    {
        normalized_image[i] = (double)raw_image[i] / 255.0;
    }
}

// One-hot encoding (rakam -> 10 boyutlu çıktı vektörü)
void create_target_output(unsigned char label, double *target, int output_size)
{
    for (int i = 0; i < output_size; i++)
    {
        target[i] = (i == label) ? 1.0 : 0.0;
    }
}

// Veri setini temizle
void free_dataset(MNISTDataset *dataset)
{
    free(dataset->images);
    free(dataset->labels);
    free(dataset);
}