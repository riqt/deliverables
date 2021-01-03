#include "jpegio.h"

int main() {
    /* 入力画像 */
    char *file_person = "a_person.jpg";      // 人物画像
    char *file_chromakey = "a_chromakey.jpg";     // クロマキー画像
    char *file_back = "a_back.jpg";          // 背景画像
    /* 出力画像 (result2-1_d.jpg (d : d値) ) */
    char *file_mask = "result2-2-23.jpg";  // マスク画像(課題2-2)

    /* d値 (> 0) */
    double d = 23.0;
    
    BYTE *dataPerson = NULL;
    BYTE *dataChromakey = NULL;
    BYTE *dataMask = NULL;

    int imageWidth = 0, imageHeight = 0, imageBPP = 0, imageSize = 0;
    int imageSize_ch = 0;

    imageWidth = readHeader(file_person, IMAGE_WIDTH);
    imageHeight = readHeader(file_person, IMAGE_HEIGHT);
    imageBPP = readHeader(file_person, IMAGE_BPP);
    imageSize = readJpeg(file_person, &dataPerson);
    imageSize_ch = readJpeg(file_chromakey, &dataChromakey);
  
    if (imageSize == 0 || imageSize_ch == 0) return 1;
    
    dataMask = malloc(imageSize);

    // クロマキー画像のavereage (cf : 課題1-3)
    int pixel = imageWidth * imageHeight;  // 人物画像とクロマキー画像の画素数は同じ
    long long sumR = 0, sumG = 0, sumB = 0;
    for (int i = 0; i < pixel * 3; i += 3) {
        sumR += dataChromakey[i];
        sumG += dataChromakey[i + 1];
        sumB += dataChromakey[i + 2];
    }
    double avrR = sumR / pixel;
    double avrG = sumG / pixel;
    double avrB = sumB / pixel;

    /* マスク画像の作成 (白色に初期化) */
    for (int i = 0; i < imageSize; i++) {
        dataMask[i] = 255;
    }
    /* RGB値がすべてaverage ± dに含まれる場合，黒色(RGB:0)にする */ 
    for (int i = 0; i < imageSize; i += 3) {
        if (fabs(dataPerson[i] - avrR) <= d) {
            if (fabs(dataPerson[i + 1] - avrG) <= d) {
                if (fabs(dataPerson[i + 2] - avrB) <= d) {
                    dataMask[i] = 0;
                    dataMask[i + 1] = 0;
                    dataMask[i + 2] = 0;
                }
            }
        }
    }
    
    writeJpeg(file_mask, dataMask, imageWidth, imageHeight, imageBPP, 100);

    free(dataPerson);
    free(dataChromakey);
    free(dataMask);

    return 0;
}