#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mnist_trainer.c"
#include "image_processor.c"

int main()
{
    srand(time(NULL));

    // MNIST veri setini yükle
    printf("MNIST veri seti yukleniyor...\n");
    MNISTDataset *train_data = load_mnist("train-images.idx3-ubyte", "train-labels.idx1-ubyte");
    MNISTDataset *test_data = load_mnist("t10k-images.idx3-ubyte", "t10k-labels.idx1-ubyte");

    if (!train_data || !test_data)
    {
        printf("MNIST veri seti yuklenemedi!\n");
        printf("Lutfen asagidaki dosyalarin mevcut oldugunu kontrol edin:\n");
        printf("- train-images.idx3-ubyte\n");
        printf("- train-labels.idx1-ubyte\n");
        printf("- t10k-images.idx3-ubyte\n");
        printf("- t10k-labels.idx1-ubyte\n");
        return 1;
    }

    // Sinir ağını başlat
    NeuralNetwork *nn = init_network();
    printf("\nSinir agi baslatildi.\n");

    // Eğitim parametreleri
    int epochs = 10;
    printf("\nEgitim basliyor...\n");
    printf("Egitim veri seti boyutu: %d\n", train_data->number_of_images);
    printf("Test veri seti boyutu: %d\n", test_data->number_of_images);
    printf("Epoch sayisi: %d\n\n", epochs);

    // Eğitim
    train_network(nn, train_data, epochs);

    // Test
    test_network(nn, test_data);

    // Kullanıcıdan görüntü testi
    printf("\nKendi rakam goruntulerinizi test etmek ister misiniz? (E/H): ");
    char response;
    scanf(" %c", &response);

    if (response == 'E' || response == 'e')
    {
        while (1)
        {
            printf("\nTest edilecek goruntu dosyasinin yolunu girin (JPEG, PNG, BMP) (Cikmak icin 'q' girin): ");
            char filename[256];
            scanf("%s", filename);

            if (filename[0] == 'q')
                break;

            double *image = load_and_preprocess_image(filename);
            if (image)
            {
                printf("\nIslenmus goruntu:\n");
                print_processed_image(image);

                int digit = recognize_digit(nn, image);
                printf("\nTaninan rakam: %d\n", digit);

                free(image);
            }
        }
    }

    // Belleği temizle
    free_dataset(train_data);
    free_dataset(test_data);

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