#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "mnist_loader.c"

// Geri yayılım için türev fonksiyonları
double sigmoid_derivative(double x)
{
    return x * (1.0 - x);
}

// Geri yayılım algoritması
void backpropagation(NeuralNetwork *nn, double *input, double *target)
{
    // İleri yayılım
    forward_propagation(nn, input);

    // Çıkış katmanı hata ve delta hesaplama
    double *output_delta = (double *)malloc(OUTPUT_SIZE * sizeof(double));
    for (int i = 0; i < OUTPUT_SIZE; i++)
    {
        double error = target[i] - nn->output_layer[i];
        output_delta[i] = error * sigmoid_derivative(nn->output_layer[i]);
    }

    // Gizli katman hata ve delta hesaplama
    double *hidden_delta = (double *)malloc(HIDDEN_SIZE * sizeof(double));
    for (int i = 0; i < HIDDEN_SIZE; i++)
    {
        double error = 0.0;
        for (int j = 0; j < OUTPUT_SIZE; j++)
        {
            error += output_delta[j] * nn->hidden_output_weights[i][j];
        }
        hidden_delta[i] = error * sigmoid_derivative(nn->hidden_layer[i]);
    }

    // Çıkış katmanı ağırlıklarını güncelle
    for (int i = 0; i < HIDDEN_SIZE; i++)
    {
        for (int j = 0; j < OUTPUT_SIZE; j++)
        {
            nn->hidden_output_weights[i][j] += LEARNING_RATE * output_delta[j] * nn->hidden_layer[i];
        }
    }

    // Gizli katman ağırlıklarını güncelle
    for (int i = 0; i < INPUT_SIZE; i++)
    {
        for (int j = 0; j < HIDDEN_SIZE; j++)
        {
            nn->input_hidden_weights[i][j] += LEARNING_RATE * hidden_delta[j] * nn->input_layer[i];
        }
    }

    // Bias değerlerini güncelle
    for (int i = 0; i < OUTPUT_SIZE; i++)
    {
        nn->output_bias[i] += LEARNING_RATE * output_delta[i];
    }
    for (int i = 0; i < HIDDEN_SIZE; i++)
    {
        nn->hidden_bias[i] += LEARNING_RATE * hidden_delta[i];
    }

    free(output_delta);
    free(hidden_delta);
}

// Eğitim fonksiyonu
void train_network(NeuralNetwork *nn, MNISTDataset *train_data, int epochs)
{
    double *normalized_image = (double *)malloc(INPUT_SIZE * sizeof(double));
    double *target = (double *)malloc(OUTPUT_SIZE * sizeof(double));

    for (int epoch = 0; epoch < epochs; epoch++)
    {
        double total_error = 0.0;
        int correct_predictions = 0;

        // Her görüntü için eğitim
        for (int i = 0; i < train_data->number_of_images; i++)
        {
            // Görüntüyü normalize et
            normalize_image(&train_data->images[i * INPUT_SIZE], normalized_image, INPUT_SIZE);

            // Hedef çıktıyı oluştur
            create_target_output(train_data->labels[i], target, OUTPUT_SIZE);

            // Geri yayılım ile ağırlıkları güncelle
            backpropagation(nn, normalized_image, target);

            // Tahmin doğruluğunu kontrol et
            int predicted = recognize_digit(nn, normalized_image);
            if (predicted == train_data->labels[i])
            {
                correct_predictions++;
            }

            // Toplam hatayı hesapla
            for (int j = 0; j < OUTPUT_SIZE; j++)
            {
                double error = target[j] - nn->output_layer[j];
                total_error += error * error;
            }
        }

        // Epoch sonuçlarını yazdır
        double accuracy = (double)correct_predictions / train_data->number_of_images * 100;
        double avg_error = total_error / (train_data->number_of_images * OUTPUT_SIZE);

        printf("Epoch %d/%d - Hata: %.6f - Dogruluk: %.2f%%\n",
               epoch + 1, epochs, avg_error, accuracy);
    }

    free(normalized_image);
    free(target);
}

// Test fonksiyonu
void test_network(NeuralNetwork *nn, MNISTDataset *test_data)
{
    double *normalized_image = (double *)malloc(INPUT_SIZE * sizeof(double));
    int correct_predictions = 0;

    for (int i = 0; i < test_data->number_of_images; i++)
    {
        normalize_image(&test_data->images[i * INPUT_SIZE], normalized_image, INPUT_SIZE);
        int predicted = recognize_digit(nn, normalized_image);

        if (predicted == test_data->labels[i])
        {
            correct_predictions++;
        }
    }

    double accuracy = (double)correct_predictions / test_data->number_of_images * 100;
    printf("\nTest sonuclari:\n");
    printf("Toplam test sayisi: %d\n", test_data->number_of_images);
    printf("Dogru tahmin: %d\n", correct_predictions);
    printf("Dogruluk orani: %.2f%%\n", accuracy);

    free(normalized_image);
}