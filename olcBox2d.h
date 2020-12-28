#ifndef B2_OLC_H
#define B2_OLC_H

#include <box2d/box2d.h>
#include "olcPixelGameEngine/olcPixelGameEngine.h"

namespace b2olc
{

    float transX = 320.0f;
    float transY = 200.0f;
    float screenHeight = 200.0f;
    float scaleFactor = 10.0f;

    float map(float x, float in_min, float in_max, float out_min, float out_max)
    {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

    olc::vf2d worldToPixels(float x, float y)
    {
        float _x = map(x, 0.0f, 1.0f, transX, transX + scaleFactor);
        float _y = map(y, 0.0f, 1.0f, transY, transY + scaleFactor);
        _y = map(_y, 0.0f, screenHeight, screenHeight, 0.0f);

        return olc::vf2d(_x, _y);
    }

    b2Vec2 pixelsToWorld(float x, float y)
    {
        float _x = map(x, transX, transX + scaleFactor, 0.0f, 1.0f);
        float _y = y;
        _y = map(y, screenHeight, 0.0f, 0.0f, screenHeight);
        _y = map(_y, transY, transY + scaleFactor, 0.0f, 1.0f);

        return b2Vec2(_x, _y);
    }

    float scalarPixelsToWorld(float v)
    {
        return v / scaleFactor;
    }

    float scalarWorldToPixels(float v)
    {
        return v * scaleFactor;
    }

} // namespace b2olc

#endif