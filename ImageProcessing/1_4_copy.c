#include "jpegio.h"

int main() {
    char *filename = "a_back.jpg";
    char *fileout = "a_back_1.jpg";
    BYTE *dataIn = NULL;
    BYTE *dataOut = NULL;
    int imageWidth = 0, imageHeight = 0, imageBPP = 0, imageSize = 0;
  
    // 画像の読み込み
    imageWidth = readHeader(filename, IMAGE_WIDTH);
    imageHeight = readHeader(filename, IMAGE_HEIGHT);
    imageBPP = readHeader(filename, IMAGE_BPP);
    imageSize = readJpeg(filename, &dataIn);
    if (imageSize == 0) return 1;

    // 出力画像データ領域の確保
    dataOut = malloc(imageSize);

    // 画像データの配列をコピー
    for (int i = 0; i < imageSize; i++) {
        dataOut[i] = dataIn[i];
    }
  
    // 画像を出力
    // writeJpeg関数の最後の引数が画質 (0 ~ 100)
    writeJpeg(fileout, dataOut, imageWidth, imageHeight, imageBPP, 1);

  return 0;
}