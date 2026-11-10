#ifndef COLOR_H
#define COLOR_H

class Color {
    float r;
    float g;
    float b;

public:
    Color();

    Color(float r, float g, float b);

    float getR();
    float getG();
    float getB();
};

#endif