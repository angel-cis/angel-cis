#ifndef APPLICATION_H
#define APPLICATION_H

#include <bobcat_ui/all.h>
#include "Canvas.h"
#include "Toolbar.h"
#include "ColorSelector.h"
#include "Shape.h"
#include "Enums.h"

class Application : public bobcat::Application_ {
    bobcat::Window* window;
    Toolbar* toolbar;
    Canvas* canvas;
    ColorSelector* colorSelector;
    Shape* selectedShape;

    // New members for color and point size tracking
    float currentR, currentG, currentB;
    int pointSize;

    void onCanvasMouseDown(bobcat::Widget* sender, float mx, float my);
    void onCanvasDrag(bobcat::Widget* sender, float mx, float my);
    void onToolbarChange(bobcat::Widget* sender);
    void onColorSelectorClick(bobcat::Widget* sender, float mx, float my);
    
    void resizeSelectedShape(bool increase);

public:
    Application();

    friend struct::AppTest;
};

#endif
