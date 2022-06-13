#include "point.c"
#include "image.c"

typedef struct Object {
    char *name;
    Point position;
    int order;
    Point pivot;
    int width;
    int height;
    Image *image;
} Object;

Object *CreateObject(char *name, Point position, int order, Point pivot, int width, int height, Image *image) {
    Object *object = (Object *) malloc(sizeof(Object));
    object->name = name;
    object->position = position;
    object->order = order;
    object->pivot = pivot;
    object->width = width;
    object->height = height;
    object->image = image;

    return object;
}

void ClearObject(Object *object) {
    ClearImage(object->image);
    free(object);
}

typedef struct ObjectList {
    Object *objects;
    int size;
    int maxSize;
} ObjectList;

ObjectList *CreateObjectList(int maxSize) {
    ObjectList *objectList = (ObjectList *) malloc(sizeof(ObjectList));

    objectList->objects = (Object *) malloc(sizeof(Object) * maxSize);
    objectList->size = 0;
    objectList->maxSize = maxSize;

    return objectList;
}

void ClearObjectList(ObjectList *objectList) {
    int i;
    for (i = 0; i < objectList->size; i++) {
        ClearObject(&objectList->objects[i]);
    }

    free(objectList->objects);
    free(objectList);
}

void InsertObject(ObjectList *objectList, Object *o, int index) {
    if (index > objectList->size) {
        ThrowError("Index exceeds objectList size", "object.c", 46, 0);
        return;
    }

    if (index < 0) {
        ThrowError("Index cannot be negative", "object.c", 46, 0);
        return;
    }

    if (objectList->size == objectList->maxSize) {
        ThrowError("ObjectList is at maximum capacity", "object.c", 46, 0);
        return;
    }

    int i;
    for (i = objectList->size; i > index; i--) {
        objectList->objects[i] = objectList->objects[i - 1];
    }

    objectList->objects[index] = *o;
    
    objectList->size = objectList->size + 1;
}

int AddObject(ObjectList *objectList, Object *o) {
    int i;
    for (i = 0; i < objectList->size; i++) {
        if (objectList->objects[i].order <= o->order) {
            InsertObject(objectList, o, i);
            return i;
        }
    }

    InsertObject(objectList, o, objectList->size);
    return objectList->size;
}

bool PositionWithinObject(Object *object, int x, int y) {
    int objectX = object->position.x;
    int objectY = object->position.y;
    int objectWidth = object->width;
    int objectHeight = object->height;

    if (x >= objectX && 
        x < objectX + objectWidth &&
        y >= objectY &&
        y < objectY + objectHeight) 
    {
        return true;
    }

    return false;
}

Object *ObjectAtPosition(ObjectList *objectList, int x, int y) {
    int i;
    for (i = 0; i < objectList->size; i++) {
        Object *object = &objectList->objects[i];
        if (PositionWithinObject(object, x, y)) {
            return object;
        }
    }

    return NULL;
}