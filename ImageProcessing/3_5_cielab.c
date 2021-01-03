#include "jpegio.h"

// CIELABの導出に必要な関数f(t)  (式(4-6))
double func_LAB(double t) {
    if (t > 0.008856) {
        return pow(t, 1.0/3.0);
    } else {
        return 7.7871 * t + 16.0 / 116.0;
    }
}

int main(){
    char s1[256], s2[256];

    printf("Input Image : ");
    scanf("%s", s1);
    printf("Output Image : ");
    scanf("%s", s2);

    char *file_in = s1;
    char *file_out = s2;
    BYTE *dataIn = NULL;
    BYTE *dataOut = NULL;

    double imageSize_in = 0, imageSize_out = 0;

    imageSize_in = readJpeg(file_in, &dataIn);
    imageSize_out = readJpeg(file_out, &dataOut);

    if (imageSize_in == 0 || imageSize_out == 0 || imageSize_in != imageSize_out) return 1;

    // RGB -> リニアRGB  (式(4-3))
    double *data_Lenear_in;
    double *data_Lenear_out;
    data_Lenear_in = (double *)malloc(imageSize_in * sizeof(double));
    data_Lenear_out = (double *)malloc(imageSize_out * sizeof(double));

    for (int i = 0; i < imageSize_in; i += 3) {
        data_Lenear_in[i] = pow(dataIn[i] / 255.0, 2.2);
        data_Lenear_in[i + 1] = pow(dataIn[i + 1] / 255.0, 2.2);
        data_Lenear_in[i + 2] = pow(dataIn[i + 2] / 255.0, 2.2);
        
        data_Lenear_out[i] = pow(dataOut[i] / 255.0, 2.2);
        data_Lenear_out[i + 1] = pow(dataOut[i + 1] / 255.0, 2.2);
        data_Lenear_out[i + 2] = pow(dataOut[i + 2] / 255.0, 2.2);
    }

    // リニアRGB -> CIEXYZ  (式(4-4))
    double *data_XYZ_in;
    double *data_XYZ_out;
    data_XYZ_in = (double *)malloc(imageSize_in * sizeof(double));
    data_XYZ_out = (double *)malloc(imageSize_out * sizeof(double));

    for (int i = 0; i < imageSize_in; i += 3) {
        data_XYZ_in[i] = 100.0 * (0.4124 * data_Lenear_in[i] + 0.3576 * data_Lenear_in[i + 1] + 0.1805 * data_Lenear_in[i + 2]);
        data_XYZ_in[i + 1] = 100.0 * (0.2126 * data_Lenear_in[i] + 0.7152 * data_Lenear_in[i + 1] + 0.0722 * data_Lenear_in[i + 2]);
        data_XYZ_in[i + 2] = 100.0 * (0.0193 * data_Lenear_in[i] + 0.1192 * data_Lenear_in[i + 1] + 0.9504 * data_Lenear_in[i + 2]);
        
        data_XYZ_out[i] = 100.0 * (0.4124 * data_Lenear_out[i] + 0.3576 * data_Lenear_out[i + 1] + 0.1805 * data_Lenear_out[i + 2]);
        data_XYZ_out[i + 1] = 100.0 * (0.2126 * data_Lenear_out[i] + 0.7152 * data_Lenear_out[i + 1] + 0.0722 * data_Lenear_out[i + 2]);
        data_XYZ_out[i + 2] = 100.0 * (0.0193 * data_Lenear_out[i] + 0.1192 * data_Lenear_out[i + 1] + 0.9504 * data_Lenear_out[i + 2]);
    }
    free(data_Lenear_in);
    free(data_Lenear_out);

    // CIEXYZ -> CIELAB  (式(4-5))
    double *data_LAB_in;
    double *data_LAB_out;
    data_LAB_in = (double *)malloc(imageSize_in * sizeof(double));
    data_LAB_out = (double *)malloc(imageSize_out * sizeof(double));

    // D65 光源下の完全拡散反射面の三刺激値
    double Xn = 95.05, Yn = 100.0, Zn = 108.89;

    for (int i = 0; i < imageSize_in; i += 3) {
        data_LAB_in[i] = 116.0 * func_LAB(data_XYZ_in[i + 1] / Xn) - 16.0;
        data_LAB_in[i + 1] = 500.0 * (func_LAB(data_XYZ_in[i] / Xn) - func_LAB(data_XYZ_in[i + 1] / Yn));
        data_LAB_in[i + 2] = 200.0 * (func_LAB(data_XYZ_in[i + 1] / Yn) - func_LAB(data_XYZ_in[i + 2] / Zn));
        
        data_LAB_out[i] = 116.0 * func_LAB(data_XYZ_out[i + 1] / Xn) - 16.0;
        data_LAB_out[i + 1] = 500.0 * (func_LAB(data_XYZ_out[i] / Xn) - func_LAB(data_XYZ_out[i + 1] / Yn));
        data_LAB_out[i + 2] = 200.0 * (func_LAB(data_XYZ_out[i + 1] / Yn) - func_LAB(data_XYZ_out[i + 2] / Zn));
    }
    free(data_XYZ_in);
    free(data_XYZ_out);

    // 色差ΔE*ab  (式(4-7))
    long double dE = 0;
    for (int i = 0; i < imageSize_in; i += 3) {
        dE += sqrt((data_LAB_in[i] - data_LAB_out[i]) * (data_LAB_in[i] - data_LAB_out[i]) + (data_LAB_in[i + 1] - data_LAB_out[i + 1]) * (data_LAB_in[i + 1] - data_LAB_out[i + 1]) + (data_LAB_in[i + 2] - data_LAB_out[i + 2]) * (data_LAB_in[i + 2] - data_LAB_out[i + 2]));
    }
    printf("色差ΔE*ab = %Lf\n", dE / (double)imageSize_in * 3.0);

    free(data_LAB_in);
    free(data_LAB_out);
}