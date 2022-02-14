# This Makefile only works for MacOS Systems

build:
	gcc main.c -o polygon_draw -framework OpenGL -framework GLUT

run: build;
	./polygon_draw