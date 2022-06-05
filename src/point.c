typedef struct Point {
    int x;
    int y;
} Point;

Point CreatePoint(int x, int y) {
    Point point;
    point.x = x;
    point.y = y;
    return point;
}

void AddPoint(Point *p, int x, int y) {
    p->x = p->x + x;
    p->y = p->y + y;
}