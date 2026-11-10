#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>

// Shape is an abstract class because it has a pure virtual function
// An abstract class cannot be instantiated on its own, it can only be inherited from
class Shape {
protected:
    float x;
    float y;
    float r;
    float g;
    float b;
    

public:
    Shape() {
        x = 0.0;
        y = 0.0;
        r = 0.0;
        g = 0.0;
        b = 0.0;
    }

    // pure virtual method
    // anyone who inherits from Shape class MUST implement this function
    virtual void draw() = 0;

    virtual bool contains(float mx, float my) {
        return false;
    }

    void setXY(float mx, float my) {
        x = mx;
        y = my;
    }

    // Abstract class must have a virtual destructor
    // this ensures that the child destructors are also called
    virtual ~Shape() {
        std::cout << "~Shape()" << std::endl;
    }

    float getX() const { return x; }
    float getY() const { return y; }

    virtual void drawOutline() const {
    // Default: do nothing
    }

    virtual void setRGB(float r, float g, float b) {
    this->r = r; 
    this->g = g;
    this->b = b;
    }



};

#endif