#ifndef POINT_H
#define POINT_H

#include "Shape.h"

class Point : public Shape {
    int size;

public:
    Point();

    Point(float x, float y);

    Point(float x, float y, float r, float g, float b);
    
    Point(float x, float y, float r, float g, float b, int size);

    //void draw();

    void draw() override;
    bool contains(float mx, float my) override;
    void drawOutline() const override;

    int getSize() const { return size; }
    void setSize(int s) { size = s; }


    ~Point() {
        std::cout << "~Point()" << std::endl;
    }

    
};

#endif