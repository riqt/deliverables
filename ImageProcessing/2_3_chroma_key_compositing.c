#include "jpegio.h"

int main() {
    /* 入力画像 */
    char *file_person = "a_person.jpg";      // 人物画像
    char *file_chromakey = "a_chromakey.jpg";     // クロマキー画像
    char *file_back = "a_back.jpg";          // 背景画像
    /* 出力画像 (d : d値) */
    char *file_out = "result2-3-30.jpg";   // クロマキー合成の結果 (課題2-3の結果"result2-2-d.jpg")
    
    /* d値 */
    double d = 30.0;
    
    BYTE *dataPerson = NULL;
    BYTE *dataChromakey = NULL;
    BYTE *dataBack = NULL;
    BYTE *dataMask = NULL;
    BYTE *dataOut = NULL;

    int imageWidth = 0, imageHeight = 0, imageBPP = 0, imageSize = 0;
    int imageSize_ch = 0, imageSize_back = 0;

    imageWidth = readHeader(file_person, IMAGE_WIDTH);
    imageHeight = readHeader(file_person, IMAGE_HEIGHT);
    imageBPP = readHeader(file_person, IMAGE_BPP);
    imageSize = readJpeg(file_person, &dataPerson);
    imageSize_ch = readJpeg(file_chromakey, &dataChromakey);
    imageSize_back = readJpeg(file_back, &dataBack);
  
    if (imageSize == 0 || imageSize_back == 0) return 1;
    
    dataMask = malloc(imageSize);
    dataOut = malloc(imageSize);

    // クロマキー画像のavereage
    int pixel = imageWidth * imageHeight;
    long long sumR = 0, sumG = 0, sumB = 0;
    for (int i = 0; i < pixel * 3; i += 3) {
        sumR += dataChromakey[i];
        sumG += dataChromakey[i + 1];
        sumB += dataChromakey[i + 2];
    }
    double avrR = sumR / pixel;
    double avrG = sumG / pixel;
    double avrB = sumB / pixel;

    /* マスク画像の作成 */
    for (int i = 0; i < imageSize; i++) {
        dataMask[i] = 255;
    }
    /* RGB値がaverage ± dに含まれる場合，黒色(RGB:0)にする */ 
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

    for (int i = 0; i < imageSize; i += 3) {
        /* マスク画像の黒色部分は背景画像の画素値 */
        /* マスク画像の白色部分は人物画像の画素値 */
        if (dataMask[i] == 0) {
            if (dataMask[i + 1] == 0) {
                if (dataMask[i + 2] == 0) {
                    dataOut[i] = dataBack[i];
                    dataOut[i + 1] = dataBack[i + 1];
                    dataOut[i + 2] = dataBack[i + 2];
                }
            }
        } else {
            dataOut[i] = dataPerson[i];
            dataOut[i + 1] = dataPerson[i + 1];
            dataOut[i + 2] = dataPerson[i + 2];
        }
    }

    /* クロマキー合成の画像の生成 */
    writeJpeg(file_out, dataOut, imageWidth, imageHeight, imageBPP, 100);

    free(dataPerson);
    free(dataChromakey);
    free(dataMask);

    return 0;
}