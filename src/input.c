#include "game.c"

int order = 0;

Object *selected = NULL;

int pressCounter = 0;
int pressDelay = 60;
Object *pressed = NULL;

void onStart() {
    start();

    Image *button = ParseImage(button_wide);
    Image *chkbox = ParseImage(checkbox);

    AddObject(
        world,
        CreateObject(
            "Button",
            CreatePoint(screen->width / 2, 150),
            order++,
            CreatePoint(0,0),
            button->width,
            button->height,
            button
        )
    );

    AddObject(
        world,
        CreateObject(
            "Checkbox",
            CreatePoint(100, 100),
            order++,
            CreatePoint(0,0),
            chkbox->width,
            chkbox->height,
            chkbox
        )
    );
}

void onClose() {

}

void onTimer() {
    loop();
}

void onMouseMove(int x, int y) {
    byte highlight = 15;

    if (selected != NULL) {
        DecreaseLight(selected->image, highlight);
        selected = NULL;
    }

    selected = ObjectAtPosition(world, x, y);

    if (selected != NULL) {
        IncreaseLight(selected->image, highlight);
    }
}

void onMouseLeftDown(int x, int y) {
    byte press = 15;

    Object *hit = ObjectAtPosition(world, x, y);
    if (hit != NULL) {
        printf("Clicked on object %s\n", hit->name);
        DecreaseLight(hit->image, press);
    }
}

void onMouseLeftUp(int x, int y) {

}

void onMouseRightDown(int x, int y) {

}

void onMouseRightUp(int x, int y) {

}

void onKeyDown(char c) {

}

void onKeyUp(char c) {

}

void onKeyPress(char c) {

}