#include "jpegio.h"

int main() {
    // 画像データの入力
    char s1[32], s2[32];

    printf("Input Image : ");
    scanf("%s", s1);
    printf("Output Image : ");
    scanf("%s", s2);

    char *file_in = s1;
    char *file_out = s2;
    BYTE *dataIn = NULL;
    BYTE *dataOut = NULL;

    double imageSize_in = 0, imageSize_out = 0;

    imageSize_in = readJpeg(file_in, &dataIn);
    imageSize_out = readJpeg(file_out, &dataOut);

    if (imageSize_in == 0 || imageSize_out == 0 || imageSize_in != imageSize_out) return 1;

    double sum = 0, mse, psnr;

    // 式(4-2)
    for (int i = 0; i < imageSize_in; i++) {
        sum += pow(dataIn[i] - dataOut[i], 2.0);
    }

    mse = sum / (imageSize_in);

    // 式(4-1)
    psnr = 10 * log10(255.0 * 255.0 / mse);

    printf("PSNR : %lf[db]", psnr);

    free(dataIn);
    free(dataOut);
}