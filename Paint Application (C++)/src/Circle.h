#ifndef CIRCLE_H
#define CIRCLE_H

#include "Shape.h"

class Circle : public Shape {
    float radius;

public:
    Circle();
    
    Circle(float x, float y, float radius, float r, float g, float b);
    
    float getRadius() const { return radius; }
    void setRadius(float r) { radius = r; }

    //void draw();

    void draw() override;
    bool contains(float mx, float my) override;
    void drawOutline() const override;

    ~Circle() {
        std::cout << "~Circle()" << std::endl;
    }
};

#endif