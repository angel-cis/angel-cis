#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Shape.h"

class Triangle : public Shape {
    float base;
    float height;

public:
    Triangle();

    Triangle(float x, float y, float base, float height, float r, float g, float b);

    //void draw();

    void draw() override;
    bool contains(float mx, float my) override;
    void drawOutline() const override;

    float getBase() const { return base; }
    float getHeight() const { return height; }
    void setSize(float b, float h) { base = b; height = h; }


    ~Triangle() {
        std::cout << "~Triangle()" << std::endl;
    }
};

#endif