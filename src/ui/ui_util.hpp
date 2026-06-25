#pragma once

#include <cmath>
#include <imgui.h>

// Utility structure for realtime plot (directly copied from ImPlot)
struct ScrollingBuffer
{
    int MaxSize;
    int Offset;
    ImVector<ImVec2> Data;
    ScrollingBuffer(int max_size = 4500)
    {
        MaxSize = max_size;
        Offset = 0;
        Data.reserve(MaxSize);
    }
    void AddPoint(float x, float y)
    {
        if (Data.size() < MaxSize)
            Data.push_back(ImVec2(x, y));
        else
        {
            Data[Offset] = ImVec2(x, y);
            Offset = (Offset + 1) % MaxSize;
        }
    }
    void Erase()
    {
        if (Data.size() > 0)
        {
            Data.shrink(0);
            Offset = 0;
        }
    }
};

// Utility structure for realtime plot (directly copied from ImPlot)
struct RollingBuffer
{
    float Span;
    ImVector<ImVec2> Data;
    RollingBuffer(int initial_reserve = 2000)
    {
        Span = 10.0f;
        Data.reserve(initial_reserve);
    }
    void AddPoint(float x, float y)
    {
        float xmod = fmodf(x, Span);
        if (!Data.empty() && xmod < Data.back().x)
            Data.shrink(0);
        Data.push_back(ImVec2(xmod, y));
    }
};
