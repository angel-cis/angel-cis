#include "Application.h"
#include "Enums.h"

using namespace bobcat;
using namespace std;

void Application::onCanvasMouseDown(Widget* sender, float mx, float my) {
    TOOL tool = toolbar->getTool();
    Color color = colorSelector->getColor(); // already supported

    if (tool == PENCIL) {
        canvas->addPoint(mx, my, color.getR(), color.getG(), color.getB(), pointSize);
        canvas->redraw();
    }
    else if (tool == ERASER) {
        canvas->addPoint(mx, my, 1.0f, 1.0f, 1.0f, pointSize * 2);
        canvas->redraw();
    }
    else if (tool == RECTANGLE) {
        canvas->addRectangle(mx, my, 0.2f, 0.2f, color.getR(), color.getG(), color.getB());
        canvas->redraw();
    }
    else if (tool == CIRCLE) {
        canvas->addCircle(mx, my, 0.1f, color.getR(), color.getG(), color.getB());
        canvas->redraw();
    }
    else if (tool == TRIANGLE) {
        canvas->addTriangle(mx, my, 0.2f, 0.2f, color.getR(), color.getG(), color.getB());
        canvas->redraw();
    }
    else if (tool == POLYGON) {
        canvas->addPolygon(mx, my, 6, 0.1f, color.getR(), color.getG(), color.getB());
        canvas->redraw();
    }
    else if (tool == MOUSE) {
        selectedShape = canvas->getSelectedShape(mx, my);
        if (selectedShape) {
            cout << "Selecting Highlighting shape" << endl;
            canvas->setHighlightedShape(selectedShape);
        } else {
            canvas->setHighlightedShape(nullptr);
        }
        canvas->redraw();
    }
}

void Application::onCanvasDrag(bobcat::Widget* sender, float mx, float my) {
    TOOL tool = toolbar->getTool();
    Color color = colorSelector->getColor();

    if (tool == PENCIL) {
        canvas->addPoint(mx, my, color.getR(), color.getG(), color.getB(), pointSize);
        canvas->redraw();
    }
    else if (tool == ERASER) {
        canvas->addPoint(mx, my, 1.0f, 1.0f, 1.0f, pointSize * 2);
        canvas->redraw();
    }
    else if (tool == MOUSE) {
        if (selectedShape) {
            selectedShape->setXY(mx, my);
            canvas->redraw();
        }
    }
}

void Application::onToolbarChange(bobcat::Widget* sender) {
    ACTION action = toolbar->getAction();

    if (action == CLEAR) {
        selectedShape = nullptr;
        cout << "Clear" << endl;
        canvas->clear();
        canvas->redraw();
    }
    else if (action == UNDO) {
        selectedShape = nullptr;
        canvas->undo();
        canvas->redraw();
    }
    else if (action == SIZE_UP) { // you'll define SIZE_UP / SIZE_DOWN in Enums.h
        if (selectedShape) {
            resizeSelectedShape(true);
        } else {
            pointSize += 1;
        }
    }
    else if (action == SIZE_DOWN) {
        if (selectedShape) {
            resizeSelectedShape(false);
        } else {
            if (pointSize > 1) pointSize -= 1;
        }
    }
}

void Application::resizeSelectedShape(bool increase) {
    if (!selectedShape) return;

    float factor = increase ? 1.1f : 0.9f; // 10% change

    if (auto circle = dynamic_cast<Circle*>(selectedShape)) {
        // Access radius directly by adding getter/setter in Circle
        circle->setRadius(circle->getRadius() * factor);
    }
    else if (auto rect = dynamic_cast<Rectangle*>(selectedShape)) {
        rect->setSize(rect->getWidth() * factor, rect->getHeight() * factor);
    }
    else if (auto tri = dynamic_cast<Triangle*>(selectedShape)) {
        tri->setSize(tri->getBase() * factor, tri->getHeight() * factor);
    }
    else if (auto poly = dynamic_cast<Polygon*>(selectedShape)) {
        poly->setLength(poly->getLength() * factor);
    }
    else if (auto point = dynamic_cast<Point*>(selectedShape)) {
        point->setSize(std::max(1, static_cast<int>(point->getSize() * factor)));
    }

    canvas->redraw();
}


Application::Application() {
    window = new Window(10, 75, 500, 500, "Bobcat UI - Paint Application Project");
    canvas = new Canvas(50, 0, 450, 450);
    toolbar = new Toolbar(0, 0, 50, 500);
    colorSelector = new ColorSelector(50, 450, 450, 50);

    selectedShape = nullptr;
    pointSize = 7; // default size

    window->add(toolbar);
    window->add(canvas);
    window->add(colorSelector);

    ON_MOUSE_DOWN(canvas, Application::onCanvasMouseDown);
    ON_DRAG(canvas, Application::onCanvasDrag);
    ON_CHANGE(toolbar, Application::onToolbarChange);

    window->show();
}

