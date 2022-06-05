typedef unsigned long pixel;
typedef unsigned char byte;

pixel palette[] = {
    0x00FFFFFF,
    0x00000000,
    0x00FF0000
};

pixel transparent = 0x00FFFFFF;

pixel *CreatePixels(int width, int height) {
    pixel *pixels = (pixel *) malloc(sizeof(pixel) * width * height);
    return pixels;
} 

byte test_image_1[] = {
    16
};