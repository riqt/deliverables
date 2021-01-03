#include "jpegio.h"

int main() {
    // ラプラシアンフィルタで処理されたエッジ強調画像は必須課題4-3のプログラムで既に得られているとする
    char *filename = "photo.jpg";     // 原画像
    char *file_lap = "fig4_3_n8.jpg"; // エッジ強調画像
    char *fileout = "fig4_4_n8_v2.jpg";    // 鮮鋭化画像
    BYTE *dataIn = NULL;
    BYTE *dataLap = NULL;
    BYTE *dataOut = NULL;
    int imageWidth = 0, imageHeight = 0, imageBPP = 0, imageSize = 0, imageSize_l = 0;
  
    imageWidth = readHeader(filename, IMAGE_WIDTH);
    imageHeight = readHeader(filename, IMAGE_HEIGHT);
    imageBPP = readHeader(filename, IMAGE_BPP);
    imageSize = readJpeg(filename, &dataIn);
    imageSize_l = readJpeg(file_lap, &dataLap);
  
    if (imageSize == 0 || imageSize_l == 0) return 1;

    dataOut = malloc(imageSize * sizeof(int));

    // 鮮鋭化画像は原画像とエッジ強調画像の差分
    for (int i = 0; i < imageSize; i++) {
        dataOut[i] = dataIn[i] + dataLap[i];
        if (dataOut[i] < 0) dataOut[i] = 0;
    }

    writeJpeg(fileout, dataOut, imageWidth, imageHeight, imageBPP, 100);

    free(dataIn);
    free(dataLap);
    free(dataOut);
    
    return 0;
}
