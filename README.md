# polygon-draw
A polygon drawing application written in C using OpenGL and GLUT.

## Usage

### How to run program (MacOS)
```make run```

### How to use program
- Left-click to start drawing a new polygon
- Right-click to complete current polygon
- ESC to exit program

## Architecture

- C
- OpenGL 1.1
- GLUT

### Platform

Currently this code runs on MacOS, however to run on another platform, change `#include <GLUT/glut.h>` to `#include <GL/glut.h>`
