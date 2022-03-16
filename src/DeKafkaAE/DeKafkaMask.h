#pragma once

#ifndef DEKAFKAMASK_H
#define DEKAFKAMASK_H

#include <AE_Effect.h>
#include <Magick++.h>
using namespace Magick;

void ApplyMask(PF_EffectWorld *world, PF_FpLong x, PF_FpLong y, PF_FpLong w, PF_FpLong h, PF_FpLong amount);

void EffectWorldToImage(PF_EffectWorld *world, Image *img);

void ImageToEffectWorld(Image *img, PF_EffectWorld *world);

void ApplyMaskToImage(Image *img, int Left, int Top, int Width, int Height, unsigned int Amount);

#endif // DEKAFKAMASK_H