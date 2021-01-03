#include "jpegio.h"

int main() {
    char *filename = "image.jpg";
    BYTE *data = NULL;
    int imageSize = 0;
    int imageWidth = 0;
    int x, y, i;
    imageWidth = readHeader(filename, IMAGE_WIDTH);
    imageSize = readJpeg(filename, &data);
    if (imageSize == 0) return 1;

    // 座標を入力
    printf("x = ");
    scanf("%d", &x);
    printf("y = ");
    scanf("%d", &y);

    // RGBを考慮
    i = (imageWidth * y + x) * 3;
    fprintf(stdout, "R:%3d  G:%3d  B:%3d\n", data[i], data[i+1], data[i+2]);

    // メモリ領域解放
    free(data);
  
    return 0;
}