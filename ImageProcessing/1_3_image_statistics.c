#include "jpegio.h"

int main() {
    char *filename = "a_chromakey.jpg";
    BYTE *data = NULL;
    int imageWidth = 0, imageHeight = 0, imageBPP = 0, imageSize = 0;
    long long sumR = 0, sumG = 0, sumB = 0;

    imageSize = readJpeg(filename, &data);
    if (imageSize == 0) return 1;

  
    imageWidth = readHeader(filename, IMAGE_WIDTH);
    imageHeight = readHeader(filename, IMAGE_HEIGHT);
    imageBPP = readHeader(filename, IMAGE_BPP);
    imageSize = readHeader(filename, IMAGE_SIZE);

    int pixel = imageWidth * imageHeight;  // 画素の総数
    // 各RGB値に対して，画素値の総和を求めておく
    for (int i = 0; i < pixel * 3; i += 3) {
        sumR += data[i];
        sumG += data[i + 1];
        sumB += data[i + 2];
    }
    // 平均値
    double avrR = sumR / pixel;
    double avrG = sumG / pixel;
    double avrB = sumB / pixel;
    printf("Average R : %lf\n", avrR);
    printf("Average G : %lf\n", avrG);
    printf("Average B : %lf\n", avrB);

    // 標準偏差
    sumR = 0, sumG = 0, sumB = 0;
    for (int i = 0; i < pixel * 3; i += 3) {
        sumR += pow(data[i] - avrR, 2);
        sumG += pow(data[i + 1] - avrG, 2);
        sumB += pow(data[i + 2] - avrB, 2);
    }

    printf("Standard Deviation R : %lf\n", sqrt((double)sumR / pixel));
    printf("Standard Deviation G : %lf\n", sqrt((double)sumG / pixel));
    printf("Standard Deviation B : %lf\n", sqrt((double)sumB / pixel));

    return 0;
}