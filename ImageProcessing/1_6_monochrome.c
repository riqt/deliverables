#include "jpegio.h"

int main() {
    char *filename = "a_back.jpg";
    char *fileout_mono_1 = "a_back_mono_1.jpg";
    char *fileout_mono_2 = "a_back_mono_2.jpg";
    BYTE *dataIn = NULL;
    BYTE *dataOut_mono_1 = NULL, *dataOut_mono_2 = NULL;
    int imageWidth = 0, imageHeight = 0, imageBPP = 0, imageSize = 0;
  
    imageWidth = readHeader(filename, IMAGE_WIDTH);
    imageHeight = readHeader(filename, IMAGE_HEIGHT);
    imageBPP = readHeader(filename, IMAGE_BPP);
    imageSize = readJpeg(filename, &dataIn);
  
    if (imageSize == 0) return 1;

    dataOut_mono_1 = malloc(imageSize);
    dataOut_mono_2 = malloc(imageSize);

    // method 1 (2-3)
    for (int i = 0; i < imageSize; i += 3) {
        dataOut_mono_1[i] = (dataIn[i] + dataIn[i + 1] + dataIn[i + 2]) / 3;
        dataOut_mono_1[i + 1] = dataOut_mono_1[i];
        dataOut_mono_1[i + 2] = dataOut_mono_1[i];
    }
    // method 2 (2-3)
    for (int i = 0; i < imageSize; i += 3) {
        dataOut_mono_2[i] = 0.299 * dataIn[i] + 0.587 * dataIn[i + 1] + 0.114 * dataIn[i + 2];
        dataOut_mono_2[i + 1] =  dataOut_mono_2[i];
        dataOut_mono_2[i + 2] =  dataOut_mono_2[i];
    }
  
    // 最後の引数が画質
    writeJpeg(fileout_mono_1, dataOut_mono_1, imageWidth, imageHeight, imageBPP, 100);
    writeJpeg(fileout_mono_2, dataOut_mono_2, imageWidth, imageHeight, imageBPP, 100);

    free(dataIn);
    free(dataOut_mono_1);
    free(dataOut_mono_2);

    return 0;
}