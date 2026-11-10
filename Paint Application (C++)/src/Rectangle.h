#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Shape.h"

class Rectangle : public Shape {
    float width;
    float height;

public:
    Rectangle();

    Rectangle(float x, float y, float width, float height, float r, float g, float b);

    void draw() override;
    bool contains(float mx, float my) override;
    void drawOutline() const override;

    float getWidth() const { return width; }
    float getHeight() const { return height; }
    void setSize(float w, float h) { width = w; height = h; }




    ~Rectangle() {
        std::cout << "~Rectangle()" << std::endl;
    }
};

#endif