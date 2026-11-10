#ifndef CANVAS_H
#define CANVAS_H

#include <bobcat_ui/all.h>
#include <vector>
#include "Point.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Triangle.h"
#include "Shape.h"
#include "Polygon.h"


class Canvas : public bobcat::Canvas_ {
    std::vector<Shape*> shapes;
    Shape* highlightedShape = nullptr;


public:
    Canvas(int x, int y, int w, int h);

    void addPoint(float mx, float my, float r, float g, float b, int size);

    void addRectangle(float mx, float my, float width, float height, float r, float g, float b);

    void addCircle(float mx, float my, float radius, float r, float g, float b);

    void addTriangle(float mx, float my, float base, float height, float r, float g, float b);

    void addPolygon(float x, float y, int sides, float length, float r, float g, float b);

    Shape* getSelectedShape(float mx, float my);

    void undo();

    void clear();

    void render();

    void setHighlightedShape(Shape* shape);

};

#endif