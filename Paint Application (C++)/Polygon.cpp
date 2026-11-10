#include "Polygon.h"
#include <GL/freeglut.h>

Polygon::Polygon() : sides(5), length(0.1f) {
    // Base default constructor of Shape is called automatically
    x = 0.0f;
    y = 0.0f;
    r = 0.0f;
    g = 0.0f;
    b = 0.0f;
}

Polygon::Polygon(float x, float y, int sides, float length, float r, float g, float b)
    : sides(sides), length(length) {
    // Base default constructor of Shape is called automatically
    this->x = x;
    this->y = y;
    this->r = r;
    this->g = g;
    this->b = b;
}


void Polygon::draw() {
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
    float inc = 2 * M_PI / sides;
    for (float theta = 0; theta < 2 * M_PI; theta += inc) {
        glVertex2f(x + length * cos(theta), y + length * sin(theta));
    }
    glEnd();
}

bool Polygon::contains(float mx, float my) {
    // Simple circular hit detection
    float dx = mx - x;
    float dy = my - y;
    float distSq = dx * dx + dy * dy;
    return distSq <= (length * length);
}

void Polygon::drawOutline() const {
    glColor3f(0.0f, 0.0f, 0.0f); // Black outline
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    float inc = 2 * M_PI / sides;
    for (float theta = 0; theta < 2 * M_PI; theta += inc) {
        glVertex2f(x + length * cos(theta), y + length * sin(theta));
    }
    glEnd();
}
