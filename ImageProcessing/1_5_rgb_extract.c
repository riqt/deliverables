#include "jpegio.h"

int main() {
    // 入出力画像
    char *filename = "a_back.jpg";
    char *fileout_R = "a_back_R.jpg";
    char *fileout_G = "a_back_G.jpg";
    char *fileout_B = "a_back_B.jpg";
    BYTE *dataIn = NULL;
    BYTE *dataOut_R = NULL, *dataOut_G = NULL, *dataOut_B = NULL;
    int imageWidth = 0, imageHeight = 0, imageBPP = 0, imageSize = 0;
  
    imageWidth = readHeader(filename, IMAGE_WIDTH);
    imageHeight = readHeader(filename, IMAGE_HEIGHT);
    imageBPP = readHeader(filename, IMAGE_BPP);
    imageSize = readJpeg(filename, &dataIn);
    if (imageSize == 0) return 1;

    dataOut_R = malloc(imageSize);
    dataOut_G = malloc(imageSize);
    dataOut_B = malloc(imageSize);

    // Red
    for (int i = 0; i < imageSize; i += 3) {
        dataOut_R[i] = dataIn[i];
        dataOut_R[i + 1] = 0;
        dataOut_R[i + 2] = 0;
    }
    // Green
    for (int i = 0; i < imageSize; i += 3) {
        dataOut_G[i] = 0;
        dataOut_G[i + 1] = dataIn[i + 1];
        dataOut_G[i + 2] = 0;
    }
    // Blue
    for (int i = 0; i < imageSize; i += 3) {
        dataOut_B[i] = 0;
        dataOut_B[i + 1] = 0;
        dataOut_B[i + 2] = dataIn[i + 2];
    }
  
    // 出力画像の画質は最大値(100)
    writeJpeg(fileout_R, dataOut_R, imageWidth, imageHeight, imageBPP, 100);
    writeJpeg(fileout_G, dataOut_G, imageWidth, imageHeight, imageBPP, 100);
    writeJpeg(fileout_B, dataOut_B, imageWidth, imageHeight, imageBPP, 100);

    free(dataIn);
    free(dataOut_R);
    free(dataOut_G);
    free(dataOut_B);

    return 0;
}