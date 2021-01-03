#include "jpegio.h"

// 3×3の近傍フィルタを計算する関数
// 第2引数が0のとき，4近傍ラプラシアンフィルタ (図15(a))
// 第2引数が1のとき，8近傍ラプラシアンフィルタ (図15(b))
// 第2引数が2のとき，4近傍鮮鋭化フィルタ (図16(a))
// 第2引数が2のとき，8近傍鮮鋭化フィルタ (図16(b))
// を計算
int getNeighborhood(int array[], int num) {
    int value = 0;
    if (num == 0) value = array[1] + array[3] - 4 * array[4] + array[5] + array[7];
    else if (num == 1) value = array[0] + array[1] + array[2] + array[3] - 8 * array[4] + array[5] + array[6] + array[7] + array[8]; 
    else if (num == 2) value = - array[1] - array[3] + 5 * array[4] -  array[5] - array[7];
    else if (num == 3) value = - array[0] - array[1] - array[2] - array[3] + 9 * array[4] - array[5] - array[6] - array[7] - array[8]; 

    // 画素値がオーバーフローした場合の処理
    if (value < 0) value = 0;
    if (value > 255) value = 255;
    return value;
}


int main(void) {
    char *filename = "photo.jpg";
    char *fileout = "fig4_3_n8.jpg";
    BYTE *dataIn = NULL;
    BYTE *dataOut = NULL;
    BYTE **frame = NULL;
    int imageWidth = 0, imageHeight = 0, imageBPP = 0, imageSize = 0;
  
    imageWidth = readHeader(filename, IMAGE_WIDTH);
    imageHeight = readHeader(filename, IMAGE_HEIGHT);
    imageBPP = readHeader(filename, IMAGE_BPP);
    imageSize = readJpeg(filename, &dataIn);
  
    if (imageSize == 0) return 1;
    
    // ラプラシアンフィルタ
    int n = 3; // 3 * 3
    int len_n = n * n;
    int array_R[len_n], array_G[len_n], array_B[len_n]; 

    dataOut = malloc(imageSize * sizeof(int));

    // 画素の座標frame[x][y]を作成
    frame = malloc(sizeof(int *) * (imageWidth + n - 1) * 3);
    for (int i = 0; i < (imageWidth + n - 1) * 3; i++) {
        frame[i] = malloc(sizeof(int) * (imageHeight + n - 1));
    }
    
    // 外側の画素値は0
    for (int i = 0; i < (imageWidth + n - 1) * 3; i++) {
        for (int j = 0; j < (n - 1) / 2; j++) {
            frame[i][j] = 0;
        }
    }
    for (int i = 0; i < (imageWidth + n - 1) * 3; i++) {
        for (int j = imageHeight + (n - 1) / 2; j < imageHeight + n - 1; j++) {
            frame[i][j] = 0;
        }
    }
    for (int i = 0; i < 3 * (n - 1) / 2; i++) {
        for (int j = (n - 1) / 2; j < imageHeight + (n - 1) / 2; j++) {
            frame[i][j] = 0;
        }
    }
    for (int i = (imageWidth + (n - 1) / 2) * 3; i < (imageWidth + n - 1) * 3; i++) {
        for (int j = (n - 1) / 2; j < imageHeight + (n - 1) / 2; j++) {
            frame[i][j] = 0;
        }
    }

    // 入力画像の画素をframe[x][y]に流し込む
    for (int i = 3 * (n - 1) / 2; i < (imageWidth + (n - 1) / 2) * 3; i++) {
        for (int j = (n - 1) / 2; j < imageHeight; j++) {
            frame[i][j] = dataIn[(j - (n - 1) / 2) * imageWidth * 3 + i - 3 * (n - 1) / 2];
        }
    }

    // ラプラシアンフィルタ
    // 4近傍と8近傍を同時に実装
    int filtertype;
    printf("4近傍ラプラシアンフィルタ -> 0\n");
    printf("8近傍ラプラシアンフィルタ -> 1\n");
    printf("4近傍鮮鋭化フィルタ -> 2\n");
    printf("8近傍鮮鋭化フィルタ -> 3\n");
    printf("Input number : ");
    scanf("%d", &filtertype);
    if (filtertype != 0 && filtertype != 1 && filtertype != 2 && filtertype != 3) {
        printf("Input correct number.");
        return 1;
    }

    for (int i = 3 * (n - 1) / 2; i < (imageWidth + (n - 1) / 2) * 3; i += 3) {
        for (int j = (n - 1) / 2; j < imageHeight + (n - 1) / 2; j++) {
            int num = 0; // 近傍配列の添え字
            for (int k = i - 3 * (n - 1) / 2; k <= i + 3 * (n - 1 ) / 2; k += 3) { 
                for (int l = j - (n - 1) / 2; l <= j + (n - 1) / 2; l++) {
                    array_R[num] = frame[k][l];
                    array_G[num] = frame[k + 1][l];
                    array_B[num] = frame[k + 2][l];
                    num++;
                }
            }
            dataOut[(j - (n - 1) / 2) * imageWidth * 3 + i - 3 * (n - 1) / 2] = getNeighborhood(array_R, filtertype);
            dataOut[(j - (n - 1) / 2) * imageWidth * 3 + i - 3 * (n - 1) / 2 + 1] = getNeighborhood(array_G, filtertype);
            dataOut[(j - (n - 1) / 2) * imageWidth * 3 + i - 3 * (n - 1) / 2 + 2] = getNeighborhood(array_B, filtertype);
        }
    }

    writeJpeg(fileout, dataOut, imageWidth, imageHeight, imageBPP, 100);

    /* 検証用
    for (int i = 100 * 3 * imageWidth; i < 100 * 3 * imageWidth + 30; i += 3) {
        printf("%d, ", dataOut[i]);
    }
    */
    
    free(dataIn);
    free(frame);
    free(dataOut);

    return 0;
}