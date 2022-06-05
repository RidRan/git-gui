#include "image.c"
#include "object.c"

#include <stdlib.h>
#include <stdio.h>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 800;

pixel *screen;

Point originOffset;

ObjectList *world;
ObjectList *items;
ObjectList *characters;
ObjectList *ui;
ObjectList *text;

void CreateScreen() {
    screen = CreatePixels(SCREEN_WIDTH, SCREEN_HEIGHT);
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        screen[i] = 0x00000000;
    }
	originOffset = CreatePoint(0, 0);
}

void CreateLayers() {
	world = CreateObjectList(1024);
	items = CreateObjectList(1024);
	characters = CreateObjectList(1024);
	ui = CreateObjectList(1024);
	text = CreateObjectList(1024);
}

int DrawObject(Object *object) {
	int i;
	for (i = 0; i < object->width * object->height; i++) {
		int x = object->position.x + i % object->width;
		int y = object->position.y + i / object->width;

		if (x >= originOffset.x &&
			x < originOffset.x + SCREEN_WIDTH &&
			y >= originOffset.y &&
			y < originOffset.x + SCREEN_HEIGHT &&
			object->pixels[i] != transparent) 
		{
			screen[(y - originOffset.y) * SCREEN_WIDTH + x - originOffset.x] = object->pixels[i];
		}
	}

	return object->width;
}

void DrawObjects(ObjectList *objectList) {
	int i;
	for (i = objectList->size - 1; i >= 0; i--) {
		DrawObject(&objectList->objects[i]);
	}
}

void DrawLayers() {
	DrawObjects(world);
	DrawObjects(items);
	DrawObjects(characters);
	DrawObjects(ui);	
	DrawObjects(text);	
}

int screenIndex(int x, int y) {
	return y * SCREEN_WIDTH + x;
}

void SetLayerPixel(pixel *layer, int x, int y, pixel c) {
	if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
		layer[screenIndex(x, y)] = c;
	}
}

int WriteChar(char cha, pixel col, int x, int y) {
	return 1;
}

int WriteString(char *str, pixel col, int x, int y) {
	int len = strlen(str);
	int currentX = x;
	int currentY = y;
	for (int i = 0; i < len; i++) {
		if (str[i] == '\n') {
			currentX = x;
			currentY += 10;
		} else {
			currentX += WriteChar(str[i], col, currentX, currentY) + 1;
		}
		
	}

	return currentX - x + 1;
}