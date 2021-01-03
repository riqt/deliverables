#include "jpegio.h"

int main() {
    char *filename = "a_person.jpg";
    int imageWidth = 0;
    int imageHeight =0;
    int imageBPP = 0;
    int imageSize =0;

    // 画像読み込み
    imageWidth  = readHeader(filename, IMAGE_WIDTH);
    imageHeight =readHeader(filename, IMAGE_HEIGHT);
    imageBPP  = readHeader(filename, IMAGE_BPP);
    imageSize =readHeader(filename, IMAGE_SIZE);

    printf("Image Width:%d\n", imageWidth);
    printf("Image Height:%d\n", imageHeight);
    printf("Image BPP:%d\n", imageBPP);
    printf("Image SIZE:%d\n", imageSize);

    // 式(2-1)が成立するとき”True”を出力
    if (imageSize == imageWidth * imageHeight * imageBPP) printf("TRUE\n");
  
    return 0;
}
