#include "jpegio.h"

// 配列の中央値を取得する関数 (引数；配列, 配列の長さn*n)
int getMedian(int array[], int len){
    // sort
    for(int i = 0; i < len; i++){
        for(int j = i + 1; j < len; j++){
            if (array[i] < array[j]){
                int tmp = array[j];
                array[j] = array[i];
                array[i] = tmp;
            }
        }
    }
    int median = array[(len - 1) / 2];
    return  median;
}

int main(void) {
    char *filename = "noise2_s.jpg";
    char *fileout = "fig4_2_2.jpg";
    BYTE *dataIn = NULL;
    BYTE *dataOut = NULL;
    BYTE **frame = NULL;
    int imageWidth = 0, imageHeight = 0, imageBPP = 0, imageSize = 0;
  
    imageWidth = readHeader(filename, IMAGE_WIDTH);
    imageHeight = readHeader(filename, IMAGE_HEIGHT);
    imageBPP = readHeader(filename, IMAGE_BPP);
    imageSize = readJpeg(filename, &dataIn);
  
    if (imageSize == 0) return 1;
    
    // n * nのメディアンフィルタ
    int n;
    printf("Input filter size(odd) : ");
    scanf("%d", &n);
    if (n % 2 != 1 || n < 1) {
        printf("Input a positive odd number.\n");
        return 1;
    }
    int len_n = n * n; // メディアンフィルタのサイズ
    int array_R[len_n], array_G[len_n], array_B[len_n]; 

    dataOut = malloc(imageSize * sizeof(int));

    // 画素の座標frame[x][y]を作成
    frame = malloc(sizeof(int *) * (imageWidth + n - 1) * 3);
    for (int i = 0; i < (imageWidth + n - 1) * 3; i++) {
        frame[i] = malloc(sizeof(int) * (imageHeight + n - 1));
    }
    
    // 外側の画素値は0
    // 拡張範囲は上下左右にn画素ずつ
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

    // メディアンフィルタ
    for (int i = 3 * (n - 1) / 2; i < (imageWidth + (n - 1) / 2) * 3; i += 3) {
        for (int j = (n - 1) / 2; j < imageHeight + (n - 1) / 2; j++) {
            // (i, j)を中心としたn * nフィルタの中央値を(i, j)に代入
            int num = 0; // n * n配列の添え字
            for (int k = i - 3 * (n - 1) / 2; k <= i + 3 * (n - 1 ) / 2; k += 3) { 
                for (int l = j - (n - 1) / 2; l <= j + (n - 1) / 2; l++) {
                    array_R[num] = frame[k][l];
                    array_G[num] = frame[k + 1][l];
                    array_B[num] = frame[k + 2][l];
                    num++;
                }
            }
            dataOut[(j - (n - 1) / 2) * imageWidth * 3 + i - 3 * (n - 1) / 2] = getMedian(array_R, len_n);
            dataOut[(j - (n - 1) / 2) * imageWidth * 3 + i - 3 * (n - 1) / 2 + 1] = getMedian(array_G, len_n);
            dataOut[(j - (n - 1) / 2) * imageWidth * 3 + i - 3 * (n - 1) / 2 + 2] = getMedian(array_B, len_n);
        }
    }

    writeJpeg(fileout, dataOut, imageWidth, imageHeight, imageBPP, 100);

    /* 検証用出力
    for (int i = 0; i < 30; i += 3) {
        printf("%d, ", dataOut[i]);
    }
    */
    
    free(frame);
    free(dataOut);

    return 0;
}