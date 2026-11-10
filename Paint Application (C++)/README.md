# Paint Application

A desktop paint program written in C++ using the Bobcat UI library (built on FLTK) with OpenGL rendering. Draw freehand with a pencil, stamp shapes onto the canvas, pick them up and move them around, resize them, and undo or clear your work.

## Features

- **Pencil:** click or drag to draw in the selected color
- **Eraser:** paints white over existing marks (at twice the brush size)
- **Shapes:** click to place a circle, rectangle, triangle, or hexagon in the selected color
- **Select and move:** the mouse tool highlights a shape with a black outline, and dragging moves it
- **Resize:** the `+` and `-` buttons change the brush size, or scale the selected shape by 10% per click
- **Undo** removes the most recent item, and **Clear** wipes the canvas
- **Color palette:** seven rainbow colors (red, orange, yellow, green, blue, indigo, violet) with the active color marked by a small square

## Controls

The window is 500 × 500 pixels, with the toolbar on the left, the canvas in the middle, and the color palette along the bottom.

| Toolbar button | What it does |
| --- | --- |
| Pencil | Draws points while you click or drag |
| Eraser | Draws white points while you click or drag |
| Circle | Click the canvas to place a circle |
| Triangle | Click the canvas to place a triangle |
| Rectangle | Click the canvas to place a rectangle |
| Mouse | Click a shape to select it, then drag to move it |
| Polygon | Click the canvas to place a hexagon |
| Undo | Removes the most recently added item |
| Clear | Removes everything from the canvas |
| `+` / `-` | Increases or decreases the brush size, or the size of the selected shape |

**Default sizes:** the pencil starts at size 7. Shapes are placed at a fixed size: a circle of radius 0.1, a rectangle and triangle of 0.2 × 0.2, and a hexagon of side length 0.1 (in the canvas's normalized coordinates).

## Requirements

- A C++ compiler with C++11 or later (`g++` or `clang++`)
- [FLTK](https://www.fltk.org/)
- The Bobcat UI library (`bobcat_ui`)
- OpenGL and FreeGLUT
- An `assets/` folder containing the toolbar icons (see below)

## Toolbar Icons

The toolbar loads its icons from `./assets/` relative to the directory you run the program from. You need these files there:

```
assets/
├── pencil.png
├── eraser.png
├── circle.png
├── triangle.png
├── rectangle.png
├── mouse.png
├── polygon.png
├── undo.png
└── clear.png
```

## Building and Running

No build file is included in the project, so you'll need to compile the sources yourself and link against FLTK, Bobcat UI, OpenGL, and FreeGLUT. The exact flags depend on where those libraries are installed on your machine. A typical command looks like this:

```bash
g++ -std=c++17 src/*.cpp -o paint \
    $(fltk-config --cxxflags --ldflags --use-gl) \
    -lbobcat_ui -lglut -lGLU -lGL
```

Then run it from the directory that contains your `assets/` folder:

```bash
./paint
```

## Project Structure

```
src/
├── main.cpp             # Entry point: creates and runs the Application
├── Application.h/.cpp   # Wires the toolbar, canvas, and color selector together
├── Toolbar.h/.cpp       # Tool and action buttons
├── ColorSelector.h/.cpp # Color palette
├── Canvas.h/.cpp        # Stores shapes and renders them with OpenGL
├── Color.h/.cpp         # Simple RGB color value
├── Enums.h              # TOOL, COLOR, and ACTION enumerations
├── Shape.h              # Abstract base class for everything drawn
├── Point.h/.cpp         # Pencil/eraser dot
├── Circle.h/.cpp
├── Rectangle.h/.cpp
├── Triangle.h/.cpp
└── Polygon.h/.cpp       # Regular polygon (placed as a hexagon)
```

## How It Works

| Component | Role |
| --- | --- |
| `Application` | Receives events from the canvas and toolbar. It reads the active tool, color, and brush size, then tells the canvas what to add, select, move, or resize |
| `Toolbar` | Tracks the selected tool and the last action (undo, clear, size up, size down), and notifies the application on every click |
| `ColorSelector` | Tracks the chosen color and returns it as an RGB value |
| `Canvas` | Keeps a list of shapes, draws them in order each frame, and draws a black outline around the highlighted shape |
| `Shape` | Abstract base class with `draw()`, `contains()` (hit testing), and `drawOutline()`, which each shape implements |

Each pencil or eraser stamp is stored as its own `Point` shape. This is why shapes, strokes, and erasing all use the same undo list.

## Known Limitations

- **Undo works one item at a time.** Because every pencil dot is a separate shape, a single brush stroke takes many undo clicks to remove.
- **The eraser paints rather than deletes.** It draws white points on top of existing content, so it also adds to the undo history.
- **Selection picks the oldest shape.** When shapes overlap, the mouse tool selects the first one added, not the one on top.
- **The selection stays active after switching tools.** If a shape is still selected, `+` and `-` will resize that shape instead of changing the brush size, until you click Undo or Clear.
- **Hexagon hit testing is circular.** Clicking just outside a hexagon's corners may still select it.
- **Fixed placement sizes.** New shapes always appear at the default size and can only be resized afterward with `+` and `-`.
- **Debug output.** The program prints messages to the console when shapes are selected, created, or destroyed.
