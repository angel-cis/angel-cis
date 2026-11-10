#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <bobcat_ui/all.h>
#include <bobcat_ui/image.h>
#include "Enums.h"
#include <FL/Fl.H>

class Toolbar : public bobcat::Group {
    bobcat::Image* pencilButton;
    bobcat::Image* eraserButton;
    bobcat::Image* circleButton;
    bobcat::Image* triangleButton;
    bobcat::Image* rectangleButton;
    bobcat::Image* mouseButton;
    bobcat::Image* undoButton;
    bobcat::Image* clearButton;
    bobcat::Image* polygonButton;

    // NEW buttons for size control
    bobcat::Button* sizeUpButton;
    bobcat::Button* sizeDownButton;
    
    TOOL tool;
    ACTION action;
    void onClick(bobcat::Widget* sender);
    void deselectAllTools();
    void visualizeSelectedTool();

public:
    Toolbar(int x, int y, int w, int h);

    TOOL getTool();
    ACTION getAction();
};

#endif
