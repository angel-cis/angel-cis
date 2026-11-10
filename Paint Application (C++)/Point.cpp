#include "Point.h"
#include <GL/freeglut.h>

Point::Point() {
    x = 0.0;
    y = 0.0;
    r = 0.0;
    g = 0.0;
    b = 0.0;
    size = 7;
}

Point::Point(float x, float y) {
    this->x = x;
    this->y = y;
    r = 0.0;
    g = 0.0;
    b = 0.0;
    size = 7;
}

Point::Point(float x, float y, float r, float g, float b) {
    this->x = x;
    this->y = y;
    this->r = r;
    this->g = g;
    this->b = b;
    size = 7;
}

Point::Point(float x, float y, float r, float g, float b, int size) {
    this->x = x;
    this->y = y;
    this->r = r;
    this->g = g;
    this->b = b;
    this->size = size;
}

void Point::draw() {
    glColor3f(r, g, b);
    glPointSize(size);

    glBegin(GL_POINTS);
        glVertex2f(x, y);
    glEnd();
}

bool Point::contains(float mx, float my) {
    float halfSize = size / 200.0f; // scale so size=7 works in normalized coords
    return (mx >= x - halfSize && mx <= x + halfSize &&
            my >= y - halfSize && my <= y + halfSize);
}

void Point::drawOutline() const {
    glColor3f(0.0, 0.0, 0.0); // White
    glPointSize(size + 4);    // Bigger point size for highlight
    glBegin(GL_POINTS);
        glVertex2f(x, y);
    glEnd();
}
