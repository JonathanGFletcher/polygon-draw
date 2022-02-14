// Jonathan Fletcher

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <GLUT/glut.h>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

typedef struct Position {
    int x;
    int y;
} Position;

int g_n_vertices;
bool g_b_dragging;

Position g_vertices[100];
Position g_last_line[2];

void init(void) {
    g_n_vertices = 0;
    g_b_dragging = false;

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glPointSize(5.0);
    glLogicOp(GL_XOR);
}

void points(void) {
    glBegin(GL_POINTS);
    glPointSize(3.0);
    glColor3f(0.0, 0.0, 1.0);
    for (int i = 0; i < g_n_vertices; i++) {
        glVertex2i(g_vertices[i].x, g_vertices[i].y);
    }
    glEnd();
}

void lines(void) {
    glBegin(GL_LINES);
    glColor3f(83.0 / 255.0, 27.0 / 255.0, 135.0 / 255.0);
    glLineWidth(3.0);
    for (int i = 0; i < g_n_vertices - 1; i++) {
        glVertex2i(g_vertices[i].x, g_vertices[i].y);
        glVertex2i(g_vertices[i + 1].x, g_vertices[i + 1].y);
    }
    glEnd();
}

void polygon(void) {
    if (!g_b_dragging) {

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBegin(GL_POLYGON);
        glColor3f(38.0 / 255.0, 128.0 / 255.0, 62.0 / 255.0);
        for (int i = 0; i < g_n_vertices; i++) {
            glVertex2i(g_vertices[i].x, g_vertices[i].y);
        }
        glEnd();
    }
}

void on_reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutPostRedisplay();
}

void redraw_last_line() {
    glColor3f(1.0, 1.0, 1.0);
    glEnable(GL_COLOR_LOGIC_OP);
    glBegin(GL_LINES);

    // redraw to erase the old line
    glVertex2i(g_last_line[0].x, g_last_line[0].y);
    glVertex2i(g_last_line[1].x, g_last_line[1].y);

    glEnd();
    glFlush();
    glDisable(GL_COLOR_LOGIC_OP);
}

void on_mouse_move(int mouse_x, int mouse_y) {
    if (g_b_dragging)
    {
        int x = mouse_x;
        int y = WINDOW_HEIGHT - mouse_y;

        glColor3f(1.0, 1.0, 1.0);
        glEnable(GL_COLOR_LOGIC_OP);
        glBegin(GL_LINES);

        // Erase old line
        glVertex2i(g_last_line[0].x, g_last_line[0].y);
        glVertex2i(g_last_line[1].x, g_last_line[1].y);
        // Draw new line
        glVertex2i(g_last_line[0].x, g_last_line[0].y);
        glVertex2i(x, y);

        glEnd();
        glFlush();
        glDisable(GL_COLOR_LOGIC_OP);

        // Update last line
        g_last_line[1].x = x;
        g_last_line[1].y = y;
    }
}

void mouse(int button, int state, int mouse_x, int mouse_y) {
    int x = mouse_x;
    int y = WINDOW_HEIGHT - mouse_y;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

        // If not currently dragging, start new polygon
        if (!g_b_dragging) {
            g_n_vertices = 0;
            g_b_dragging = true;

            g_last_line[0].x = x;
            g_last_line[0].y = y;
            g_last_line[1].x = x;
            g_last_line[1].y = y;
        }
        else {
            // Erase rubber band line
            redraw_last_line();
        }

        // Add new vertex
        Position new_position;
        new_position.x = x;
        new_position.y = y;
        g_vertices[g_n_vertices] = new_position;
        g_n_vertices++;

        // Update last line
        g_last_line[0].x = x;
        g_last_line[0].y = y;

        glutPostRedisplay();
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {

        // If current dragging, stop polygon creation
        if (g_b_dragging) {
            // Erase rubber band line
            redraw_last_line();

            // Add new vertex corresponding to first polygon vertex
            g_vertices[g_n_vertices] = g_vertices[0];
            g_n_vertices++;

            glutPostRedisplay();

            g_b_dragging = false;
        }
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) {
        exit(0);
    }
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    polygon();
    lines();
    points();
    glFlush();
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(on_reshape);
    glutPassiveMotionFunc(on_mouse_move);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}