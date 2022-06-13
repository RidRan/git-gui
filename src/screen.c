#include "object.c"

#include <stdlib.h>
#include <stdio.h>

const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 250;

Image *screen;

Point originOffset;

ObjectList *world;
ObjectList *items;
ObjectList *characters;
ObjectList *ui;
ObjectList *text;

void CreateScreen() {
    screen = CreateImage(SCREEN_WIDTH, SCREEN_HEIGHT);
    for (int i = 0; i < screen->width * screen->height; i++) {
        screen->pixels[i] = 0x00000000;
    }
	originOffset = CreatePoint(0, 0);
}

void ResetScreen() {
	for (int i = 0; i < screen->width * screen->height; i++) {
        screen->pixels[i] = 0x00000000;
    }
}

void CreateLayers() {
	world = CreateObjectList(1024);
	items = CreateObjectList(1024);
	characters = CreateObjectList(1024);
	ui = CreateObjectList(1024);
	text = CreateObjectList(1024);
}

int DrawImage(Image *image, Point position) {
	int i;
	for (i = 0; i < image->width * image->height; i++) {
		int x = position.x + i % image->width;
		int y = position.y + i / image->width;

		if (x >= originOffset.x &&
			x < originOffset.x + screen->width &&
			y >= originOffset.y &&
			y < originOffset.x + screen->height &&
			image->pixels[i] != transparent) 
		{
			screen->pixels[(y - originOffset.y) * screen->width + x - originOffset.x] = image->pixels[i];
		}
	}

	return image->width;
}

int DrawObject(Object *object) {
	DrawImage(object->image, object->position);

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
	if (x >= 0 && x < screen->width && y >= 0 && y < screen->height) {
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