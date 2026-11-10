#include "Canvas.h"
#include <GL/freeglut.h>
#include <cstddef>
#include "Polygon.h"

// constructor
Canvas::Canvas(int x, int y, int w, int h) : Canvas_(x, y, w, h) {
    highlightedShape = nullptr;
}



void Canvas::setHighlightedShape(Shape* shape) {
    highlightedShape = shape;
}

void Canvas::addPoint(float mx, float my, float r, float g, float b, int size) {
    shapes.push_back(new Point(mx, my, r, g, b, size));
}

void Canvas::addRectangle(float mx, float my, float width, float height, float r, float g, float b) {
    shapes.push_back(new Rectangle(mx, my, width, height, r, g, b));
}

void Canvas::addCircle(float mx, float my, float radius, float r, float g, float b) {
    shapes.push_back(new Circle(mx, my, radius, r, g, b));
}

void Canvas::addTriangle(float mx, float my, float base, float height, float r, float g, float b) {
    shapes.push_back(new Triangle(mx, my, base, height, r, g, b));
}

void Canvas::addPolygon(float x, float y, int sides, float length, float r, float g, float b) {
    shapes.push_back(new Polygon(x, y, sides, length, r, g, b));
}

Shape* Canvas::getSelectedShape(float mx, float my) {
    Shape* selectedShape = nullptr;

    for (unsigned int i = 0; i < shapes.size(); i++) {
        // ask every shape if we clicked on it
        if (shapes[i]->contains(mx, my)) {
            std::cout << "Clicked on shape[" << i << "]" << std::endl;
            selectedShape = shapes[i];
            break;
        }
    }
    return selectedShape;
}

void Canvas::undo() {
    if (shapes.size() > 0) {
        if (shapes[shapes.size() - 1] == highlightedShape) {
            highlightedShape = nullptr;
        }
        delete shapes[shapes.size() - 1];
        shapes.pop_back();
    }
}

void Canvas::clear() {
    for (unsigned int i = 0; i < shapes.size(); i++) {
        delete shapes[i];
    }
    shapes.clear();
    highlightedShape = nullptr;
}

// render OpenGL objects
void Canvas::render() {
    for (unsigned int i = 0; i < shapes.size(); i++) {
        shapes[i]->draw();
    }

    if (highlightedShape) {
        highlightedShape->drawOutline();
    }
}

