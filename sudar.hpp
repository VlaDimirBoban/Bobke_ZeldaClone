#include <math.h>

/*
Facing direction after collision.
*/
void sudarsmer(float x1, float x2, float y1, float y2, float *smerX, float *smerY)
{

    if ((x2 - x1) > 0)
    {
        if ((y2 - y1) < 0)
        {
            *smerX = -1;
            *smerY = 1;
        }
    }
    if ((x2 - x1) > 0)
    {
        if ((y2 - y1) > 0)
        {
            *smerX = -1;
            *smerY = -1;
        }
    }
    if ((x2 - x1) < 0)
    {
        if ((y2 - y1) > 0)
        {
            *smerX = 1;
            *smerY = -1;
        }
    }
    if ((x2 - x1) < 0)
    {
        if ((y2 - y1) < 0)
        {
            *smerX = 1;
            *smerY = 1;
        }
    }
    if (y2 == y1)
        *smerY = 0;
    if (x2 == x1)
        *smerX = 0;
}

void homing(Vector4 col, float x1, float x2, float y1, float y2, float move, int choice, bool *check, float *x, float *y)
{
    float a = x1 - x2;
    float b = y1 - y2;
    float c = sqrt(a * a + b * b);
    if (choice % 2 == 0)
    {
        if (abs(c) >= 250)
        {
            *check = false;
            if ((x2 - x1) < 0)
                *x -= col.w * abs(a / c) * move; // right
            if ((x2 - x1) > 0)
                *x += col.z * abs(a / c) * move; // left
            if ((y2 - y1) < 0)
                *y -= col.x * abs(b / c) * move; // up
            if ((y2 - y1) > 0)
                *y += col.y * abs(b / c) * move; // down
            // What now?
            // shoot
        }
        else
            *check = true;
    }
    else
    {
        *check = false;
        if ((x2 - x1) < 0)
            *x -= col.w * abs(a / c) * move; // right
        if ((x2 - x1) > 0)
            *x += col.z * abs(a / c) * move; // left
        if ((y2 - y1) < 0)
            *y -= col.x * abs(b / c) * move; // up
        if ((y2 - y1) > 0)
            *y += col.y * abs(b / c) * move; // down
    }
}

void homingnode(float x1, float x2, float y1, float y2, int move, float *x, float *y, int *smerX)
{
    float a = x1 - x2;
    float b = y1 - y2;
    float c = sqrt(a * a + b * b);
    *x += (a / c) * move;
    *y += (b / c) * move;
    if (a / c > 0)
        *smerX = 1;
    else
        *smerX = 0;
}

void homingmetak(float x1, float x2, float y1, float y2, float *x, float *y)
{
    float a = x1 - x2;
    float b = y1 - y2;
    float c = sqrt(a * a + b * b);
    *x -= (a / c) * 13;
    *y -= (b / c) * 13;
    // if ((x2-x1)<0) *x-=abs(a/c)*3; // right
    // if ((x2-x1)>0) *x+=abs(a/c)*3; // left
    // if ((y2-y1)<0) *y-=abs(b/c)*3; // up
    // if ((y2-y1)>0) *y+=abs(b/c)*3; // down
}

float dist(Vector2 cilj, Vector2 poc)
{
    return sqrt((poc.x - cilj.x) * (poc.x - cilj.x) + (poc.y - cilj.y) * (poc.y - cilj.y));
}

float cos(Vector2 x, Vector2 y)
{
    float a = abs(x.x - y.x);
    float b = abs(x.y - y.y);
    float c = sqrt(a * a + b * b);
    return b / c;
    // *x -= (a / c) * 13;
    // *y -= (b / c) * 13;
    // if ((x2-x1)<0) *x-=abs(a/c)*3; // right
    // if ((x2-x1)>0) *x+=abs(a/c)*3; // left
    // if ((y2-y1)<0) *y-=abs(b/c)*3; // up
    // if ((y2-y1)>0) *y+=abs(b/c)*3; // down
}

float sin(Vector2 x, Vector2 y)
{
    float a = x.x - y.x;
    float b = x.y - y.y;
    float c = sqrt(a * a + b * b);
    return a / c;
    // *x -= (a / c) * 13;
    // *y -= (b / c) * 13;
    // if ((x2-x1)<0) *x-=abs(a/c)*3; // right
    // if ((x2-x1)>0) *x+=abs(a/c)*3; // left
    // if ((y2-y1)<0) *y-=abs(b/c)*3; // up
    // if ((y2-y1)>0) *y+=abs(b/c)*3; // down
}
/*

Calculating the angle in degrees from a given sin value.

*/
float arccos(float x)
{
    float pi = 3.141599;
    return abs((acos(x) * 360) / pi);
}

/*

Calculating the angle in degrees from a given sin value.

*/

float arcsin(float x)
{
    float pi = 3.141599;
    return (asin(x) * 360) / pi;
}
/*
Funkcija za izracunavanje rotacije teksture,
uneses koordinate tacke jedne i druge i on ti da 45/135/225/315 ugao iscrtavanja; 4 smera
*/
float rot(Vector2 x1, Vector2 x2)
{
    if ((x2.x - x1.x) > 0 && (x2.y - x1.y) < 0) // I kvadrant
        return 45.0f;
    if ((x2.x - x1.x) > 0 && (x2.y - x1.y) > 0) // II kvadrant
        return 135.0f;
    if ((x2.x - x1.x) < 0 && (x2.y - x1.y) > 0) // III kvadrant
        return 225.0f;
    if ((x2.x - x1.x) < 0 && (x2.y - x1.y) < 0) // IV kvadrant
        return 315.0f;
    return 0;
}
// sranje funkcija =-=

/*

Calculates sin/cos between 2 point. Angular direction is the jist of it (after collision)

*/
void abc(float x1, float x2, float y1, float y2, float *a, float *b, float *c)
{
    *a = x1 - x2;
    *b = y1 - y2;
    *c = sqrt(*a * *a + *b * *b);
}

void macevanje(float x1, float x2, float y1, float y2, float *x, float *y)
{
    float move = 1.0f;
    float a = x2 - x1;
    float b = y2 - y1;
    float c = sqrt(a * a + b * b);
    *x += (a / c) * move;
    *y += (b / c) * move;
    // if ((x2-x1)<0) *x+=abs(a/c)*move; // right
    // if ((x2-x1)>0) *x-=abs(a/c)*move; // left
    // if ((y2-y1)<0) *y+=abs(b/c)*move; // up
    // if ((y2-y1)>0) *y-=abs(b/c)*move; // down
}

void macevanje1(Vector4 col, bool up, bool down, bool left, bool right, float *x, float *y)
{
    float move = 7.0f;
    if (up)
        *y -= col.x * move;
    if (down)
        *y += col.y * move;
    if (left)
        *x -= col.w * move;
    if (right)
        *x += col.z * move;
}

/*

vector direction in relation to the mouse position (used for sword position/direction)

*/

void sudar(float x1, float x2, float y1, float y2, float &x, float &y)
{
    float move = 50.0f;
    float a = x1 - x2;
    float b = y1 - y2;
    float c = sqrt(a * a + b * b);
    if ((x2 - x1) < 0)
        x = x2 + (a / c) * move; // desno
    if ((x2 - x1) > 0)
        x = x2 + (a / c) * move; // levo
    if ((y2 - y1) < 0)
        y = y2 + (b / c) * move; // gore
    if ((y2 - y1) > 0)
        y = y2 + (b / c) * move; // dole
}

/*
COLLISION BUFFED
*/

void sudar1(float x1, float x2, float y1, float y2, float *x, float *y)
{
    float move = 4.0f;
    float a = x1 - x2;
    float b = y1 - y2;
    float c = sqrt(a * a + b * b);
    float brzX = (a / c) * move;
    float brzY = (b / c) * move;
    *x = brzX;
    *y = brzY;
    // if ((x2 - x1) < 0)
    //     *x = brzX; // right
    // if ((x2 - x1) > 0)
    //     *x = brzX; // left
    // if ((y2 - y1) < 0)
    //     *y = brzY; // up
    // if ((y2 - y1) > 0)
    //     *y = brzY; // down
}

void combo(float move, float x1, float x2, float y1, float y2, float *x, float *y)
{
    float a = x1 - x2;
    float b = y1 - y2;
    float c = sqrt(a * a + b * b);
    float brzX = (a / c) * move;
    float brzY = (b / c) * move;
    *x = brzX;
    *y = brzY;
    // if ((x2 - x1) < 0)
    //     *x = brzX; // right
    // if ((x2 - x1) > 0)
    //     *x = brzX; // left
    // if ((y2 - y1) < 0)
    //     *y = brzY; // up
    // if ((y2 - y1) > 0)
    //     *y = brzY; // down
}

void sudarODB(float x1, float x2, float y1, float y2, float *x, float *y)
{
    float move = 15.0f;
    float a = x1 - x2;
    float b = y1 - y2;
    float c = sqrt(a * a + b * b);
    float brzX = (a / c) * move;
    float brzY = (b / c) * move;
    if ((x2 - x1) < 0)
        *x = brzX; // right
    if ((x2 - x1) > 0)
        *x = brzX; // left
    if ((y2 - y1) < 0)
        *y = brzY; // up
    if ((y2 - y1) > 0)
        *y = brzY; // down
}

void rush(float x1, float x2, float y1, float y2, float *x, float *y)
{
    float move = 1.0f;
    float a = x1 - x2;
    float b = y1 - y2;
    float c = sqrt(a * a + b * b);
    float brzX = (a / c) * move;
    float brzY = (b / c) * move;
    if ((x2 - x1) < 0)
        *x = -brzX; // right
    if ((x2 - x1) > 0)
        *x = -brzX; // left
    if ((y2 - y1) < 0)
        *y = -brzY; // up
    if ((y2 - y1) > 0)
        *y = -brzY; // down
}

int heuristic(float &x1, float &x2, float &y1, float &y2)
{
    int D = 10;
    // int dx, dy;
    // dx = abs(x1 - x2);
    // dy = abs(y1 - y2);
    return D * (abs(x1 - x2) + abs(y1 - y2));
}

float heuristicG(float x1, float x2, float y1, float y2)
{
    float dx, dy;
    dx = abs(x1 - x2);
    dy = abs(y1 - y2);
    return sqrt(dx * dx + dy * dy);
}
template <typename T>
struct Vector2U
{
public:
    T x;
    T y;
};

template <typename T>
void printVec(std::vector<T> x)
{
    for (auto &y : x)
        std::cout << y << " ";
    std::cout << '\n';
    std::cout << "Vector print is completed my lord. " << '\n';
}