#include <iostream>
#include <Magick++.h>
#include <AE_Effect.h>
#include "DeKafkaMask.h"
using namespace std;
using namespace Magick;

void ApplyMask(PF_EffectWorld *world, PF_FpLong x, PF_FpLong y, PF_FpLong w, PF_FpLong h, PF_FpLong amount)
{
    Image *img = new Image(Geometry(world->width, world->height), Color(0, 0, 0));

    EffectWorldToImage(world, img);

    ApplyMaskToImage(img, x, y, w, h, amount);

    ImageToEffectWorld(img, world);
}

void EffectWorldToImage(PF_EffectWorld *world, Image *img)
{
    Quantum *imgPixels = img->getPixels(0, 0, world->width, world->height);

    A_long padding = (world->rowbytes / sizeof(PF_Pixel8)) - world->width;

    for (int pixelY = 0; pixelY < world->height; pixelY++)
    {
        for (int pixelX = 0; pixelX < world->width; pixelX++)
        {
            int inputOffsetY = pixelY * (world->width + padding);
            int inputPixel = inputOffsetY + pixelX;

            int outputOffsetY = pixelY * (world->width);
            int outputPixel = (outputOffsetY + pixelX) * img->channels();

            PF_Pixel8 *pixels = world->data;

            imgPixels[outputPixel] = pixels[inputPixel].red;
            imgPixels[outputPixel + 1] = pixels[inputPixel].green;
            imgPixels[outputPixel + 2] = pixels[inputPixel].blue;
        }
    }

    img->syncPixels();
}

void ImageToEffectWorld(Image *img, PF_EffectWorld *world)
{
    Quantum *imgPixels = img->getPixels(0, 0, world->width, world->height);

    A_long padding = (world->rowbytes / sizeof(PF_Pixel8)) - world->width;

    for (int pixelY = 0; pixelY < world->height; pixelY++)
    {
        for (int pixelX = 0; pixelX < world->width; pixelX++)
        {
            int inputOffsetY = pixelY * (world->width);
            int inputPixel = (inputOffsetY + pixelX) * img->channels();

            int outputOffsetY = pixelY * (world->width + padding);
            int outputPixel = outputOffsetY + pixelX;

            PF_Pixel8 *pixels = world->data;

            pixels[outputPixel].red = imgPixels[inputPixel];
            pixels[outputPixel].green = imgPixels[inputPixel + 1];
            pixels[outputPixel].blue = imgPixels[inputPixel + 2];
        }
    }
}

void ApplyMaskToImage(Image *img, int Left, int Top, int Width, int Height, unsigned int Amount)
{
    int X = 2;
    int Y = 2;
    int Right = X + Width;
    int Bottom = Y + Height;
    unsigned int Amount2 = (Top >= 2 * Left) ? 255 : 128 * Top / Left;

    InitializeMagick("C:\\Program Files\\ImageMagick-7.1.0-Q8");

    Image imgTop(*img);
    imgTop.chop(Geometry(X, Y - 2));
    imgTop.crop(Geometry(Width, 2));

    Image imgBottom(*img);
    imgBottom.chop(Geometry(X, Bottom));
    imgBottom.crop(Geometry(Width, 2));

    Image imgLeft(*img);
    imgLeft.chop(Geometry(X - 2, Y));
    imgLeft.crop(Geometry(2, Height));

    Image imgRight(*img);
    imgRight.chop(Geometry(Right, Y));
    imgRight.crop(Geometry(2, Height));

    Image imgHoriz(Geometry(Width, 4), Color(0, 0, 0));
    imgHoriz.composite(imgTop, 0, 0, OverCompositeOp);
    imgHoriz.composite(imgBottom, 0, 2, OverCompositeOp);
    imgHoriz.gaussianBlur(1.58, 1);

    Image imgVert(Geometry(4, Height), Color(0, 0, 0));
    imgVert.composite(imgLeft, 0, 0, OverCompositeOp);
    imgVert.composite(imgRight, 2, 0, OverCompositeOp);
    imgVert.gaussianBlur(1.58, 1);

    Geometry maskSize = Geometry(Width, Height);
    maskSize.aspect(true); // true = ignore aspect ratio

    imgHoriz.filterType(TriangleFilter); // Triangle = Bilinear
    imgHoriz.resize(maskSize);

    imgVert.filterType(TriangleFilter); // Triangle = Bilinear
    imgVert.resize(maskSize);

    imgVert.alpha(Amount2);
    imgHoriz.composite(imgVert, 0, 0, OverCompositeOp);
    imgHoriz.alpha(Amount);
    img->composite(imgHoriz, X, Y, OverCompositeOp);
}