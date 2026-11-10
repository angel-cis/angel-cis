#include "Circle.h"
#include <GL/freeglut.h>
#include <cmath>

Circle::Circle() {
    x = 0.0;
    y = 0.0;
    radius = 0.2;
    r = 0.0;
    g = 0.0;
    b = 0.0;
}

Circle::Circle(float x, float y, float radius, float r, float g, float b) {
    this->x = x; 
    this->y = y; 
    this->radius = radius;
    this->r = r; 
    this->g = g; 
    this->b = b; 
}

void Circle::draw() {
    glColor3f(r, g, b);

    float inc = M_PI / 32;
    glBegin(GL_POLYGON);
        for (float theta = 0; theta <= 2 * M_PI; theta += inc){
            glVertex2d(x + cos(theta) * radius, y + sin(theta) * radius);
        }
    glEnd();
}

bool Circle::contains(float mx, float my) {
    float dx = mx - x;
    float dy = my - y;
    return (dx * dx + dy * dy) <= (radius * radius);
}

void Circle::drawOutline() const {
    glColor3f(0.0, 0.0, 0.0); // White
    glLineWidth(3);
    float inc = M_PI / 32;
    glBegin(GL_LINE_LOOP);
        for (float theta = 0; theta <= 2 * M_PI; theta += inc) {
            glVertex2d(x + cos(theta) * radius, y + sin(theta) * radius);
        }
    glEnd();
}
