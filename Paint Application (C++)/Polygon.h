#ifndef POLYGON_H
#define POLYGON_H

#include "Shape.h"
#include <cmath>

class Polygon : public Shape {
    int sides;
    float length;

public:
    Polygon();
    Polygon(float x, float y, int sides, float length, float r, float g, float b);

    void draw() override;
    bool contains(float mx, float my) override;
    void drawOutline() const override;

    float getLength() const { return length; }
    void setLength(float l) { length = l; }

};

#endif
