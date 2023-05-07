#include "raylib.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include "sudar.hpp"

Color const PWHITE = {255, 255, 255, 90};
Color const PRED = {230, 41, 55, 90};

class grid
{
public:
    int i = 0;
    Rectangle rec;
    bool walk, check, goal, ow;
    Color color = PWHITE;
    Vector2 node;
    Vector2 lineage;
    Vector2 cor = {0, 0};
    int G, H, F; // G move cost, H heuristic, F = G + H
    grid()
    {
        check = false;
        goal = false;
        walk = true;
        ow = false;
        color = PWHITE;
        H = 0;
        G = 0;
        F = 0;
    }
    void fVal()
    {
        F = H + G;
    }
};

class lifeforms
{
public:
    Rectangle rec;
    Rectangle srcrec;
    Vector2 mouse;
    Vector2 cent;
    Vector2U<int> pos;
    int HP = 5, count = 0;
    // lifeforms(){
    //     cent.x = srcrec.x+30;
    //     cent.y = srcrec.y+30;
    // }
};

class player : public lifeforms
{

public:
    bool coll = false;
    bool check = false;
    bool show = false;
    bool wait = false, still = true;
    float rot;
    int i = 0, smerX = 0;
    Vector2 mac, dash{0, 0};
    Rectangle slash;
    Vector2 goal; //  = {0, 0};
    std::vector<Rectangle> presek;
    float brzX = 5.0f;
    float brzY = 5.0f;
    float move = 1.0f;
    int marker = 0;
    Vector4 col = {1, 1, 1, 1}; // col.x/y/w/z for movement and collision detection
};

class enemy : public lifeforms
{
private:
    int smerX1 = 1, smerY1 = 1;
    int count1 = 0;

public:
    int HP = 3;
    int smerX = 1, smerY = 1;
    int count, ftime, move, ani = 0;
    int collcount;
    float brzTX = 0;
    float brzTY = 0;
    bool colld, wait, ping, once, found, bull = false, pucaj, still = false;
    bool aggro, find;
    bool check;
    int type, broj, parr;
    int marker, waitfps, timemarkT, tsuite, bulltime;
    Vector4 col = {1, 1, 1, 1};
    Vector2 spawn;
    Vector2 circle = {spawn.x + 30.0f, spawn.y + 30.0f};
    Vector2 metak, cilj, poc;
    Vector2U<int> pos;
    std::vector<grid> tree, current;
    std::vector<Vector2> meci;
    float radius;
    Rectangle presek;
    enemy()
    {
        found = false;
        find = false;
        tsuite = 0;
        move = GetRandomValue(2, 6);
        // cent = {srcrec.x + 30.0f, srcrec.y + 30.0f};
    }
    void patrol(int choice)
    {
        if (choice % 2 == 0)
        {
            if (count1 <= 150)
            {
                srcrec.x -= smerX * 1;
                // srcrec.x = srcrec.x;
                // srcrec.y = srcrec.y;
                count1++;
            }
            else
            {
                count1 = 0;
                smerX = -smerX;
            }
        }
        else
        {
            if (count1 <= 150)
            {
                srcrec.y -= smerY * 1;
                // srcrec.x = srcrec.x;
                // srcrec.y = srcrec.y;
                count1++;
            }
            else
            {
                count1 = 0;
                smerY = -smerY;
            }
        }
        // std::cout << "count is: " << count1 << '\n';
    }
    void speed()
    {
        // move = 0;
        move = GetRandomValue(1, 6);
    }
};

class bullet : public enemy
{
public:
    Vector2 metak, cilj, poc;
    bool paljba;
    void fire(bool paljba)
    {
    }
};

class corpse
{
public:
    Vector2 vec;
    Vector2U<int> pos;
    Rectangle rec;
    int ani, smerX;
    bool go;
    corpse()
    {
        vec = {0, 0};
        ani = 0;
        go = true;
    };
    ~corpse(){};
};

class layering
{
public:
    Rectangle srcrec, rec;
    Texture2D tex;
};

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1400;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "Zelda");

    Color mapa[3][3] = {
        DARKGRAY, BLUE, DARKGRAY, BROWN, RAYWHITE, DARKGREEN, DARKGRAY, DARKBLUE, DARKGRAY};

    Texture2D mapa1[3][3] = {};
    Texture2D plyr = LoadTexture("./aset/Chars/Main-Sheet.png");
    Texture2D type1 = LoadTexture("./aset/Chars/Type1-Sheet.png");
    Texture2D type2 = LoadTexture("./aset/Chars/Type2-Sheet.png");
    Texture2D type3 = LoadTexture("./aset/Chars/Type3-Sheet.png");
    Texture2D ded = LoadTexture("./aset/Chars/ded.png");
    Texture2D slash = LoadTexture("./aset/Chars/slash-Sheet.png");
    Texture2D srce = LoadTexture("./aset/Chars/srce.png");

    player player;

    Vector4 col = {1, 1, 1, 1};

    Texture2D bg = LoadTexture("./aset/Chars/Tile1_bkg.png");
    Texture2D fg = LoadTexture("./aset/Chars/Tile1_fg.png");
    mapa1[0][0] = LoadTexture("./aset/Mapa00.png");
    mapa1[0][1] = LoadTexture("./aset/Mapa10.png");
    mapa1[0][2] = LoadTexture("./aset/Mapa20.png");
    mapa1[1][0] = LoadTexture("./aset/Mapa01.png");
    mapa1[1][1] = LoadTexture("./aset/Mapa11.png");
    mapa1[1][2] = LoadTexture("./aset/Mapa21.png");
    mapa1[2][0] = LoadTexture("./aset/Mapa02.png");
    mapa1[2][1] = LoadTexture("./aset/Mapa12.png");
    mapa1[2][2] = LoadTexture("./aset/Mapa22.png");

    std::ofstream myfile;

    float x = screenWidth / 2 - 50;
    float y = screenHeight / 2 - 50;
    int parr = 0;
    int posX = 1;
    int posY = 1;
    Vector2U<int> pos{posX, posY};
    float brzX = 0;
    float brzY = 0;
    std::vector<Rectangle> presek;
    std::vector<Rectangle> presek1;
    Rectangle sword;
    bool devmode = true, newface = true;
    bool collision = false; // dislike collision and movement atm, rework?
    bool key;
    bool checkP = true;
    bool pause = false;
    Color minimap = BLANK;
    Vector2 mousePoint;
    std::vector<int> dump; // = {0};
    std::vector<grid> current;
    int timewarp = 0;
    int timemark = 0;
    int aniCount = 0, anim = 0;
    int broj = 0;
    int broj1 = 0;
    int g = 0;
    int counter = 0;
    int choice = 1;

    std::vector<lifeforms> crtajV;
    std::vector<enemy> tekiV;

    class metax
    {
    public:
        Vector2 metak;
        Vector2 poc, cilj;
    };

    std::vector<metax> metak1;

    // C++ vectors
    std::vector<corpse> lesina1;
    std::vector<grid> tree, sideBitch, path;
    std::vector<grid> checkset, dickset;
    std::vector<grid> checked;
    std::ifstream myFile;
    std::string test;

    float *pX = new float{0};
    float *pY = new float{0};

    float dim = 60;

    for (int i = 0; i < (std::ceil(screenWidth / dim) * std::ceil(screenHeight / dim)); i++) // rounding needed
    {
        if (*pX > screenWidth / dim)
        {
            *pY += 1;
            *pX = 0;
        }
        path.emplace_back();
        path.back().cor = {*pX, *pY};
        path.back().rec = {*pX * dim, *pY * dim, dim, dim};
        path.back().i = i;
        path.back().color = PWHITE;
        *pX += 1;
    }

    float up = std::ceil(screenWidth / dim);

    std::cout << *pY << ' ' << *pX << '\n';
    std::cout << "Totalni broj celija je" << (std::ceil(screenWidth / dim) * std::ceil(screenHeight / dim)) << '\n';

    /* Delete them raw pointer boiz */
    delete pX;
    delete pY;

    ////////////////////////////
    // GRID INITIALIZATION
    ////////////////////////////

    /*
    // izracunaj za dati format na koliko celija se deli ekran

    brcelija = (screenWidth*screenHeight)/(grid_cell^2);

    for (int i=0; i<brcelija; i++)
    {
        path.emplace.back()
        path.i = i;
        path.cor.x = ; red
        path.cor.y = ; kolona
        path.rec = {.cor.x*30, .cor.y*30, 60,60};
    }

    */
    ////////////////////
    // GRID INIT
    ////////////////////

    auto vektorCheck = [](Vector2 x1, Vector2 x2)
    {
        return (x1.x == x2.x) && (x1.y == x2.y);
    };

    auto oBounds = [&](int x)
    {
        return (x >= 0 && x < (std::ceil(screenWidth / dim) * std::ceil(screenHeight / dim)));
    };

    auto wrap = [&](Vector2 x, Vector2 y)
    {
        return (x.y == y.y);
    };

    /*

    HITBOXES

    */

    myFile.open("mapa.csv");

    while (myFile)
    {
        std::string line;
        // std::cout<< broj << std::endl;
        getline(myFile, line, ',');
        if (!myFile)
            break;
        std::stringstream ss(line);
        while (getline(ss, line, ','))
        {
            // std::cout << line << " ";
            dump.push_back(std::stoi(line)); // I had no idea what i was writing at the time, 100% :D
        }
    }

    // cout DEBUG

    for (int i : dump)
    {
        if (broj1 % 7 == 0 && broj1 != 0)
            std::cout << std::endl;
        std::cout << i << ' ';
        broj1++;
    }

    myFile.close();

    std::cout << "Velicina vektora " << dump.size() << std::endl;

    /*
    guba.open("mapa.csv");

    Input from file

    while (guba){
        std::string s;
        getline(guba,s,',');
        std::stringstream sts(s);
        while (getline(sts,s,','){
            dump.push_back(std::stoi(s));
        }
    }

    guba.close("mapa.csv");
    */

    int *br = new int;
    *br = 0;

    for (auto &x : dump)
    {
        if (*br == 0)
            crtajV.emplace_back();
        switch (*br)
        {
        case 0:
            crtajV.back().rec.x = x;
        case 1:
            crtajV.back().rec.y = x;
        case 2:
            crtajV.back().rec.width = x;
        case 3:
            crtajV.back().rec.height = x;
        case 4:
            crtajV.back().pos.x = x;
        case 5:
            crtajV.back().pos.y = x;
        case 6:
            crtajV.back().count = x;
        }
        *br += 1;
        if (*br == 7)
            *br = 0;
    }

    dump.clear();

    /*

    SPAWN POINTS

    */
    myFile.open("mapa_spawn.csv");

    while (myFile)
    {
        std::string line;
        // std::cout<< broj << std::endl;
        getline(myFile, line, ',');
        if (!myFile)
            break;
        std::stringstream ss(line);
        while (getline(ss, line, ','))
        {
            // std::cout << line << " ";
            dump.push_back(std::stoi(line));
        }
        // lump.push_back(line);
    }

    broj1 = 0;

    std::cout << "Vektor spawn-ova " << std::endl;

    for (int i : dump)
    {
        if (broj1 % 5 == 0 && broj1 != 0)
            std::cout << std::endl;
        std::cout << i << ' ';
        broj1++;
    }

    myFile.close();

    *br = 0;

    for (auto &x : dump)
    {
        if (*br == 0)
            tekiV.emplace_back();
        switch (*br)
        {
        case 0:
            tekiV.back().srcrec.x = x;
        case 1:
            tekiV.back().srcrec.y = x;
        case 2:
            tekiV.back().pos.x = x;
        case 3:
            tekiV.back().pos.y = x;
        case 4:
            tekiV.back().type = x;
        case 5:
            tekiV.back().count = x;
        }
        tekiV.back().srcrec.width = 60;
        tekiV.back().srcrec.height = 60;
        *br += 1;
        if (*br == 6)
            *br = 0;
    }

    delete br;

    /*

    SPAWN POINTS

    */

    SetTargetFPS(60);

    /*

    BEGIN!

    */

    int swordani = 0;
    int comboC = -1;

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // frame ticker

        timewarp++;
        player.srcrec = {x, y, 60, 60};
        player.cent = {x + 30, y + 30};
        mousePoint = GetMousePosition();
        pos = {posX, posY};

        sword = {(float)swordani * 60, 60, 60, 60};

        // animation system

        ///////////////////////
        /// LAMBDAS
        ///////////////////////

        // very kewl little lambda which points the enemy when they are "aggroed"
        auto smer = [](float &x, float &y)
        {
            return !(x > y);
        };

        auto vechck = [&pos](Vector2U<int> &x)
        {
            if (pos.x == x.x)
                if (pos.y == x.y)
                    return true;
            return false;
        };

        auto playerCheck = [&]()
        {
            return ((player.i >= 0) && (player.i < std::ceil(screenWidth / dim) * std::ceil(screenHeight / dim))); //((player.cent.x > 10 && player.cent.x < 1390) && (player.cent.y > 10 && player.cent.y < 890));
        };

        ///////////////////////
        /// LAMBDAS
        ///////////////////////

        player.rec = {(float)anim * 120, 120, 120, 120};
        player.slash = {(float)swordani * 60, 60, 60, 60};

        player.rot = rot(player.cent, mousePoint);

        if (timewarp % 7 == 0)
        {
            anim++;
            // corpse ani counter/rec
            for (auto &value : lesina1)
            {
                value.rec = {(float)value.ani * 120, 120, 120, 120};
                if (value.go)
                {
                    if (value.smerX == 0)
                    {
                        if (value.ani < 3)
                            value.ani++;
                        else
                            value.go = false;
                    }
                    if (value.smerX == 1)
                    {
                        if (value.ani < 7)
                            value.ani++;
                        else
                            value.go = false;
                    }
                }
            }
        }

        if (timewarp % 3 == 0)
        {
            if (swordani >= 6)
                swordani = 0;
            if (swordani >= 2)
                swordani++;
        }

        if (player.show)
        {
            swordani = 2;
        }

        if (player.still)
        {
            if (player.smerX == 1)
            {
                if (anim >= 6)
                    anim = 1;
            }
            else
            {
                if (anim >= 12)
                    anim = 7;
            }
        }
        else
        {
            if (player.smerX == 1)
            {
                if (anim >= 16)
                    anim = 13;
            }
            else
            {
                if (anim >= 20)
                    anim = 17;
            }
        }

        //////////////
        /// SWORD VEcTOR // sword draw in relation to mouse position
        //////////////

        sudar(mousePoint.x, x + 30, mousePoint.y, y + 30, player.mac.x, player.mac.y);

        // shit, i don't like it

        //////////////
        /// SWORD VEcTOR // sword draw in relation to mouse position
        //////////////

        /*

        AGGRO CHECK || PATROL || COLLISION ENEMY vs BOX

        */

        // Vektor rewrite
        if (!pause)
        {
            for (auto &x : tekiV)
            {
                if (vechck(x.pos))
                {
                    x.circle = {x.srcrec.x + 30, x.srcrec.y + 30};
                    if (CheckCollisionCircleRec(x.circle, 350, player.srcrec))
                    {
                        x.tsuite = timewarp;
                        x.find = true;
                        x.aggro = true;
                    }
                    else if ((timewarp - x.tsuite >= 120) || (x.tsuite == 0))
                    {
                        x.aggro = false;
                        x.find = false;
                    }

                    if (!x.aggro)
                    {
                        x.patrol(x.type);
                        for (auto &y : crtajV)
                        {
                            if (vechck(y.pos))
                            {
                                if (CheckCollisionRecs(x.srcrec, y.rec))
                                {
                                    x.presek = GetCollisionRec(x.srcrec, y.rec);
                                    if (x.presek.y > x.srcrec.y + 30 && x.presek.width > x.presek.height)
                                    {
                                        x.smerY = 1;
                                        x.count = 0;
                                    }
                                    if (x.presek.y < x.srcrec.y + 30 && x.presek.width > x.presek.height)
                                    {
                                        x.smerY = -1;
                                        x.count = 0;
                                    }
                                    if (x.presek.x < x.srcrec.x + 30 && x.presek.height > x.presek.width)
                                    {
                                        x.smerX = -1;
                                        x.count = 0;
                                    }
                                    if (x.presek.x > x.srcrec.x + 30 && x.presek.height > x.presek.width)
                                    {
                                        x.smerX = 1;
                                        x.count = 0;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        /*

        AGGRO CHECK || PATROL || COLLISION ENEMY vs BOX

        */

        if (IsKeyReleased(KEY_Q))
        {
            std::cout << "Cord x je " << player.cent.x << '\n';
            std::cout << "Cord y je " << player.cent.y << '\n';
            if (!playerCheck())
                std::cout << "Centar je van ekrana " << '\n';
        }

        // Boop; if find -> homing node until you reach the player
        // that's it for now, keep it simple

        /////////////////////
        // GAS vector rewrite
        /////////////////////

        if (!pause)
        {
            for (auto &tek : tekiV)
            {
                if (vechck(tek.pos))
                {
                    tek.rec = {(float)tek.ani * 120, 120, 120, 120};
                    if (tek.type == 2)
                        tek.rec = {(float)tek.ani * 150, 150, 150, 150};
                    if (timewarp % 7 == 0)
                        tek.ani++;

                    if (tek.aggro)
                    {
                        if (timewarp - tek.waitfps >= 60)
                        {
                            tek.wait = false;
                        }
                        if (!tek.wait)
                        {
                        }
                    }

                    for (auto &crt : crtajV)
                    {
                        if (CheckCollisionRecs(tek.srcrec, crt.srcrec))
                        {
                            tek.presek = GetCollisionRec(tek.srcrec, crt.srcrec);
                            // keep track of intersected area related to the cube itself. This could be simpler? :D

                            if (tek.presek.y < tek.srcrec.y + 30 && tek.presek.width > tek.presek.height)
                            {
                                tek.col.x = 0;
                                tek.colld = true;
                            }
                            if (tek.presek.y > tek.srcrec.y + 30 && tek.presek.width > tek.presek.height)
                            {
                                tek.col.y = 0;
                                tek.colld = true;
                            }
                            if (tek.presek.x < tek.srcrec.x + 30 && tek.presek.height > tek.presek.width)
                            {
                                tek.col.w = 0;
                                tek.colld = true;
                            }
                            if (tek.presek.x > tek.srcrec.x + 30 && tek.presek.height > tek.presek.width)
                            {
                                tek.col.z = 0;
                                tek.colld = true;
                            }
                            tek.collcount = 0;
                        }
                        else
                        {
                            tek.collcount++;
                            tek.colld = false;
                            tek.col = {1, 1, 1, 1};
                        }

                        /*

                        When you damage the enemy, "freeze" his movement and give him a slight push back

                        */

                        if (CheckCollisionRecs(player.srcrec, tek.srcrec) && vechck(tek.pos))
                        {
                            player.wait = true;
                            tek.wait = true;
                            tek.find = false;
                            tek.waitfps = timewarp;
                            timemark = timewarp;
                            for (auto &crt : crtajV)
                            {
                                if (CheckCollisionRecs(player.srcrec, crt.srcrec) && vechck(crt.pos))
                                {
                                    presek1.push_back(GetCollisionRec(player.srcrec, crt.srcrec));
                                    // keep track of intersected area related to the cube itself. This could be simpler? :D || Yes, it is simpler.
                                    if (presek1.back().y > y + 30 && presek1.back().width > presek1.back().height)
                                    { // down
                                        player.col.y = 0;
                                        brzY = 0;
                                        player.coll = true;
                                        y -= presek1.back().height;
                                    }
                                    if (presek1.back().y < y + 30 && presek1.back().width > presek1.back().height)
                                    { // up
                                        player.col.x = 0;
                                        brzY = 0;
                                        player.coll = true;
                                        y += presek1.back().height;
                                    }
                                    if (presek1.back().x < x + 30 && presek1.back().height > presek1.back().width)
                                    { // left
                                        player.col.w = 0;
                                        brzX = 0;
                                        player.coll = true;
                                        x += presek1.back().width;
                                    }
                                    if (presek1.back().x > x + 30 && presek1.back().height > presek1.back().width)
                                    { // right
                                        player.col.z = 0;
                                        brzX = 0;
                                        player.coll = true;
                                        x -= presek1.back().width;
                                    }
                                }
                            }
                            if (player.coll == false)
                            {
                                player.brzX = 0;
                                player.brzY = 0;
                                player.move = 1;
                                sudar1(x, tek.srcrec.x, y, tek.srcrec.y, &brzX, &brzY);
                            }
                        }
                        else
                            player.coll = false;

                        /// BULLET COLLISION || ON

                        int *mitt = new int{};
                        for (auto &met : metak1)
                        {
                            if (vechck(crt.pos))
                            {
                                if (CheckCollisionCircleRec(met.metak, 5, crt.rec))
                                {
                                    metak1.erase(metak1.begin() + *mitt);
                                }
                            }
                            if (CheckCollisionPointRec(met.metak, player.srcrec))
                            {
                                metak1.erase(metak1.begin() + *mitt);
                                player.HP--;
                            }
                            *mitt += 1;
                        }

                        delete mitt;

                        /// BULLET COLLISION || ON

                        if (CheckCollisionCircleRec(player.mac, 30.0f, tek.srcrec) && (swordani >= 3)) //&& player.show) //
                        {
                            macevanje(x, tek.srcrec.x, y, tek.srcrec.y, &tek.brzTX, &tek.brzTY);
                            // into the oblivion :|
                            if (!tek.once) // You can hit enemy only once per swoosh
                            {
                                tek.HP--;
                                tek.once = true;
                            }
                            tek.ping = true;
                            tek.wait = true;
                            tek.waitfps = timewarp;
                            tek.timemarkT = timewarp;
                            std::cout << "tek.x je " << tek.srcrec.x << '\n';
                        }

                        if ((timewarp - aniCount) >= 15)
                            tek.once = false;
                    }
                }
            }
        }

        /////////////////////
        // GAS vector rewrite
        /////////////////////

        /*

            Collision system of revenge, player <->static hitboxes

        */
        if (collision == false)
        {
            player.col = {1, 1, 1, 1};
        }

        for (auto &crt : crtajV)
        {
            if (vechck(crt.pos))
            {
                if (CheckCollisionRecs(player.srcrec, crt.rec))
                {
                    // std::cout << "Presek " << '\n';
                    player.presek.push_back(GetCollisionRec(player.srcrec, crt.rec));
                    // pracenje presek povrsine u odnosu na samu kocku, to je potencijalno prostije? :D

                    if (player.presek.back().y > y + 30 && player.presek.back().width > player.presek.back().height)
                    { // na dole
                        player.col.y = 0;
                        // col.y = 0;
                        collision = true;
                    }
                    if (player.presek.back().y < y + 30 && player.presek.back().width > player.presek.back().height)
                    { // na gore
                        player.col.x = 0;
                        // col.x = 0;
                        collision = true;
                    }
                    if (player.presek.back().x < x + 30 && player.presek.back().height > player.presek.back().width)
                    { // na levo
                        player.col.w = 0;
                        // col.w = 0;
                        collision = true;
                    }
                    if (player.presek.back().x > x + 30 && player.presek.back().height > player.presek.back().width)
                    { // na desno
                        player.col.z = 0;
                        // col.z = 0;
                        collision = true;
                    }
                }
                else
                {
                    // col = {1,1,1,1};
                    // presek = {0,0,0,0};
                    collision = false;
                    player.presek.clear();
                }
            }
        }

        /*

            Collision system of revenge, player <->static hitboxes

        */

        for (auto &crt : crtajV)
        {
            if (CheckCollisionRecs(player.srcrec, crt.rec) && vechck(crt.pos))
            {
                player.move = 0;
            }
        }

        ///////////////////////////////
        // Vecrewrite
        ///////////////////////////////

        if (!pause)
        {
            int *itt = new int{};
            for (auto &tek : tekiV)
            {

                /*

                Enemy destruction, shitty atm. There's a bit of lag when hit/dead. Fix it you goose.

                */
                if (vechck(tek.pos))
                {
                    if (tek.HP == 0)
                    {
                        lesina1.emplace_back();
                        lesina1.back().vec = {tek.srcrec.x, tek.srcrec.y};
                        lesina1.back().smerX = tek.smerX;
                        lesina1.back().pos = {posY, posX};
                        if (tek.smerX == 1)
                            lesina1.back().ani = 4;
                        else
                            lesina1.back().ani = 1;
                        tekiV.erase(tekiV.begin() + *itt);
                    }

                    /*

                    Enemy destruction, shitty atm. There's a bit of lag when hit/dead. Fix it you goose.

                    */
                    for (auto &crt : crtajV)
                    {
                        if (vechck(crt.pos))
                        {
                            if (CheckCollisionRecs(tek.srcrec, crt.rec))
                            {
                                // std::cout << "Collision detected o.o" << '\n';
                                tek.brzTX = 0;
                                tek.brzTY = 0;
                            }
                        }
                    }

                    if ((timewarp - tek.timemarkT) <= 15)
                    {
                        // pushback enemies after collision with the sword smoothly -> falloff
                        if (tek.ping)
                        {
                            tek.srcrec.x += tek.brzTX;
                            tek.srcrec.y += tek.brzTY;
                            tek.brzTX /= 1.5;
                            tek.brzTY /= 1.5;
                            // std::cout << "ding ding ding ding " << tek.brzTX << '\n';
                        }
                    }
                    else
                        tek.ping = false;
                }
                *itt += 1;
            }
            delete itt;
        }

        ///////////////////////////////
        // Vecrewrite
        ///////////////////////////////

        // SHIT!!!

        /*
        Gotta limit the sword swipe hitbox. Else i keep melting the enemies -> NOT FUN
        */

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {

            if (aniCount > 0)
            {
                if ((timewarp - aniCount) >= 30) // This is problematic, can't remember why atm
                {
                    player.show = true;
                    aniCount = timewarp;
                }
            }
            else
                player.show = true;
            if (aniCount == 0)
                aniCount = timewarp;
        }

        // Sword swipe duration 5x16.7ms (for now)

        if ((timewarp - aniCount) >= 3)
        {
            player.show = false;
        }

        /*
        Time limited sword hitbox
        */

        /*
        Smooth collision with enemies
        */

        x += brzX * player.move;
        y += brzY * player.move;

        if ((timewarp - timemark) >= 15)
        {
            brzX = 0;
            brzY = 0;
        }

        if (brzX == 0 && brzY == 0)
        {
            player.brzX = 7.0f;
            player.brzY = 7.0f;
        }

        /*
        HP -- IF player is damaged -> wait for 5 frames before -> damage enable
        */

        if (player.wait)
        {
            if ((timewarp - timemark) >= 5)
            {
                if (player.dash.x <= 0.3 && player.dash.y <= 0.3)
                    player.HP--;
                player.wait = false;
            }
        }

        /*

        MOVEMENT INPUT

        */

        if (IsKeyDown(KEY_W) && !(IsKeyDown(KEY_S)))
        {
            y -= player.col.x * player.brzY;
            player.still = false;
        }

        if (IsKeyDown(KEY_S) && !(IsKeyDown(KEY_W)))
        {
            y += player.col.y * player.brzY;
            player.still = false;
        }
        if (IsKeyDown(KEY_A) && !(IsKeyDown(KEY_D)))
        {
            if (player.still)
                anim = 13;
            player.still = false;
            x -= player.col.w * player.brzX;
            player.smerX = 1;
        }
        if (IsKeyDown(KEY_D) && !(IsKeyDown(KEY_A)))
        {
            if (player.still)
                anim = 17; // zamrzotina
            player.still = false;
            x += player.col.z * player.brzX;
            player.smerX = 0;
        }

        ////////////////////////
        // PLAYER IDLE ANIMATION
        ////////////////////////

        if ((!IsKeyDown(KEY_W) && !IsKeyDown(KEY_A) && !IsKeyDown(KEY_S) && !IsKeyDown(KEY_D)))
        {

            if (!player.still)
            {
                anim = 0;
                if (player.smerX == 0)
                    anim = 7;
                else
                    anim = 0;
            }
            player.still = true;
        }

        ////////////////////////
        // PLAYER IDLE ANIMATION
        ////////////////////////

        /////////////////////////////
        // DASH -----  iFrames
        /////////////////////////////

        // ADD invincibility frames

        if (IsKeyReleased(KEY_LEFT_SHIFT))
        {
            sudar1(player.cent.x, mousePoint.x, player.cent.y, mousePoint.y, &player.dash.x, &player.dash.y);
        }

        for (auto &crt : crtajV)
        {
            if (vechck(crt.pos))
            {
                if (CheckCollisionRecs(player.srcrec, crt.rec))
                {
                    player.dash = {0.0};
                }
            }
        }

        x -= player.dash.x * 6.0f;
        y -= player.dash.y * 6.0f;

        player.dash.x /= 1.1;
        player.dash.y /= 1.1;

        /////////////////////////////
        // DASH -----  iFrames
        /////////////////////////////

        /*

        OVERFLOW PROTECTION:
        we must save ourselves from segfaults. When Player leaves the map, engine tries to expand the pathfinding grid and loses its mind.
        Better safe than sorry <3

        */

        ////////////////////////////////
        // Map coordinates update
        ////////////////////////////////

        if (y < -100 && posY > 0)
        {
            posY--;
            y = 900;
        };
        if (y > 950 && posY < 2)
        {
            posY++;
            y = -100;
        }
        if (x > 1400 && posX < 2)
        {
            posX++;
            x = -100;
        }
        if (x + 100 < 0 && posX > 0)
        {
            posX--;
            x = 1400;
        }

        if (devmode)
        {
            /*

            Pathfinding GRID

            */
            if (tekiV.size() > 0)
            {
                if (playerCheck())
                {
                    for (auto &path : path)
                    {
                        // INIT only once, do not redu. SAVE THE CPU
                        path.color = PWHITE;
                        path.walk = true;
                        for (auto &crt : crtajV)
                        {
                            if (vechck(crt.pos))
                            {
                                if (CheckCollisionRecs(path.rec, crt.rec))
                                {
                                    path.color = PRED;
                                    path.walk = false;
                                }
                            }
                        }
                        if (CheckCollisionPointRec(player.cent, path.rec))
                        {
                            if (!path.walk)
                            {
                                checkP = false;
                            }
                            else
                            {
                                checkP = true;
                                player.goal = path.cor;
                                player.i = path.i;
                            }
                        }

                        // marks the enemy position on the grid
                        // It finds the path but has issues drawing it on the debug grid (there is some kind of cutoff)

                        for (auto &tek : tekiV)
                        {
                            if (vechck(tek.pos))
                            {
                                tek.cent = {tek.srcrec.x + 30, tek.srcrec.y + 30};

                                if (path.walk && checkP && (tekiV.size() > 0))
                                {
                                    if (CheckCollisionPointRec(tek.cent, path.rec))
                                    {
                                        // gotta limit the pathfinding calculation to lighten the CPU load;
                                        // recalculate only if something changes
                                        tek.check = true;
                                        tek.broj = path.i;
                                        tek.parr = path.i;
                                        tek.tree.clear();
                                        tek.current.clear();
                                        tek.tree.push_back(path);
                                        tek.tree.back().G = 0;
                                        tek.tree.back().H = heuristic(path.cor.x, player.goal.x, path.cor.y, player.goal.y);
                                        tek.tree.back().fVal();
                                        tek.tree.back().check = true;
                                    }
                                }
                            }
                        }
                    }
                }

                /*

                Pathfinding GRID

                */

                /*

                PATHFINDING AUTO

                */

                for (auto &tek : tekiV)
                {
                    if (vechck(tek.pos))
                    {
                        if (tek.find)
                        {
                            if ((tek.tree.size() > 0) && ((tekiV.size() > 0)))
                            {
                                parr = tek.parr;
                                broj = parr;
                                while (tek.check)
                                {
                                    // std::cout << "Hello Mom" << '\n';
                                    checkset.clear();
                                    // dickset.clear();
                                    // count = 0;
                                    // if ((path[parr - 1].walk) && (oBounds(path[parr - 1].i))
                                    if ((path[parr - 1].walk) && wrap(path[parr].cor, path[parr - 1].cor)) //(oBounds(path[parr - 1].i)))
                                    {
                                        key = false;
                                        for (auto &value : tek.tree)
                                        {
                                            if (value.i == path[parr - 1].i)
                                            {
                                                key = true;
                                            }
                                        }
                                        if (!key)
                                        {
                                            checkset.push_back(path[parr - 1]);
                                            // count++;
                                        }
                                    }
                                    if ((path[parr - up].walk) && (oBounds(path[parr - up].i)))
                                    {
                                        key = false;
                                        for (auto &value : tek.tree)
                                        {
                                            if (value.i == path[parr - up].i)
                                            {
                                                key = true;
                                            }
                                        }
                                        if (!key)
                                        {
                                            checkset.push_back(path[parr - up]);
                                            // count++;
                                        }
                                    }
                                    if ((path[parr + 1].walk) && wrap(path[parr].cor, path[parr + 1].cor)) //(oBounds(path[parr + 1].i)))
                                    {
                                        key = false;
                                        for (auto &value : tek.tree)
                                        {
                                            if (value.i == path[parr + 1].i)
                                            {
                                                key = true;
                                            }
                                        }
                                        if (!key)
                                        {
                                            checkset.push_back(path[parr + 1]);
                                            // count++;
                                        }
                                    }
                                    if ((path[parr + up].walk) && (oBounds(path[parr + up].i)))
                                    {
                                        key = false;
                                        for (auto &value : tek.tree)
                                        {
                                            if (value.i == path[parr + up].i)
                                            {
                                                key = true;
                                            }
                                        }
                                        if (!key)
                                        {
                                            checkset.push_back(path[parr + up]);
                                            // count++;
                                        }
                                    }

                                    // Just dump all 4 and calculate heuristics
                                    // Why? So I can do heuristic & GMove for the whole vector tree quickly <3
                                    // Overwrite funcionality for optimal path
                                    // Consistency(real-time)

                                    // std::cout << "count je " << count << '\n' << "count1 je " << count1 << '\n';
                                    // if (checkset.size() == 0) check = false;
                                    // std::cout << "Checkset size " << checkset.size() << '\n';

                                    if (checkset.size() > 0)
                                    {
                                        for (auto &value : checkset)
                                        {
                                            value.lineage = path[parr].cor;
                                            value.H = heuristic(value.cor.x, player.goal.x, value.cor.y, player.goal.y);
                                            value.G += g + 10;
                                            value.fVal();
                                        }
                                    }
                                    //     // tree.insert(tree.end(), checkset.begin(), checkset.end());
                                    //     // std::sort(tree.begin(), tree.end(), [](const grid &lhs, const grid &rhs)
                                    //     //           { return lhs.F < rhs.F; });

                                    // std::cout << "Checkset size: " << checkset.size() << '\n';

                                    tek.tree.insert(tek.tree.end(), checkset.begin(), checkset.end());

                                    // for (auto &value : tree)
                                    // {
                                    //     std::cout << value.F << ' ';
                                    //     std::cout << value.H << ' ';
                                    //     std::cout << value.G << ' ';
                                    // }
                                    // std::cout << '\n';

                                    std::sort(tek.tree.begin(), tek.tree.end(), [](const grid &lhs, const grid &rhs)
                                              { return lhs.F < rhs.F; });

                                    // std::sort(tree.begin(), tree.end(), [](const grid &lhs, const grid &rhs)
                                    //           { return lhs.F < rhs.F; });

                                    for (auto &value : tek.tree) //
                                    {
                                        if (!value.check)
                                        {
                                            // std::cout << "I'm in, boss" << '\n';
                                            parr = value.i;
                                            value.check = true;
                                            g = value.G;
                                            if (value.H == 0) //(vektorCheck(value.cor, player.goal))
                                            {
                                                tek.current.push_back(value);
                                                tek.check = false;
                                            }
                                            break;
                                        }
                                    }

                                    //     // for (auto &value : tree)
                                    //     // {
                                    //     //     if (!value.check)
                                    //     //     {
                                    //     //         parr = value.i;
                                    //     //         value.check = true;
                                    //     //         c = value.G;
                                    //     //         if (value.H == 0)
                                    //     //         {
                                    //     //             current.push_back(value);
                                    //     //             check = false;
                                    //     //             // teki.check = false;
                                    //     //         }
                                    //     //         break;
                                    //     //     }
                                    //     // }

                                    //     /* NO PATH */

                                    counter = 0;
                                    for (auto &value : tek.tree) //
                                    {
                                        if (value.check)
                                            counter++;
                                    }

                                    if (counter == (int)tek.tree.size()) //
                                        tek.check = false;

                                    //  This part saves it.
                                    //  teki.check = false;

                                    // /*
                                    //     NO PATH
                                    //     shitty solution
                                    // */
                                    // // }

                                    // std::cout << "counter je " << counter << '\n';
                                    // it keeps adding 2 per cycle into the vecor, no idea why though
                                    // the issue is the order and then it keeps adding them one by one
                                    // MUST FIX; EZ EZ
                                }

                                // Reconstructing the whole path from found goal to origin (via lineage)
                                // Small bug keeps poping up here, especially with C-Style part of the code

                                if (tek.current.size() > 0)
                                {
                                    // std::cout << "tick je " << tick << '\n';
                                    while (tek.current.back().i != broj)
                                    {
                                        for (auto &value : tek.tree)
                                        {
                                            if (vektorCheck(value.cor, tek.current.back().lineage)) // check lineage; constructh full path
                                                tek.current.push_back(value);
                                            //  if ((value.cor.x == current.back().lineage.x) && (value.cor.y == current.back().lineage.y)) current.push_back(value);
                                        }
                                        if (tek.current.back().i == 0)
                                            break;
                                    }
                                    // std::cout << "We still breathing, AMEN JESUS !" << '\n';
                                }
                            }
                        }
                    }
                }
            }
        }

        // GAS SYSTEM

        //////////////////////////////////
        // ENEMY ANIMATION || VEC-REWRITE
        //////////////////////////////////

        if (!pause)
        {
            for (auto &tek : tekiV)
            {
                if (vechck(tek.pos))
                {
                    if ((tek.find) && (tek.current.size() > 0) && (playerCheck() && (!tek.wait)))
                    {
                        if (tek.type == 1)
                        {
                            homingnode(tek.current[tek.current.size() - 2].rec.x + dim / 2, tek.srcrec.x + 30, tek.current[tek.current.size() - 2].rec.y + dim / 2, tek.srcrec.y + 30, tek.move, &tek.srcrec.x, &tek.srcrec.y, &tek.smerX);
                            if (smer(player.cent.x, tek.cent.x))
                            {
                                if ((tek.ani == 6))
                                    tek.still = false;
                                if (!tek.still)
                                {
                                    tek.ani = 1;
                                    tek.still = true;
                                }
                            }
                            else
                            {
                                if ((tek.ani == 12))
                                    tek.still = true;
                                if (tek.still)
                                {
                                    tek.ani = 7;
                                    tek.still = false;
                                }
                            }
                        }
                        if ((tek.type == 2) && (tek.current.size() >= 15))
                        {
                            homingnode(tek.current[tek.current.size() - 3].rec.x + 15, tek.srcrec.x + 30, tek.current[tek.current.size() - 3].rec.y + 15, tek.srcrec.y + 30, tek.move, &tek.srcrec.x, &tek.srcrec.y, &tek.smerX);
                            if (smer(player.cent.x, tek.cent.x))
                            {
                                if ((tek.ani >= 6))
                                    tek.still = false;
                                if (!tek.still)
                                {
                                    tek.ani = 1;
                                    tek.still = true;
                                }
                            }
                            else
                            {
                                if ((tek.ani >= 12))
                                {
                                    tek.still = false;
                                }
                                if (!tek.still)
                                {
                                    tek.ani = 7;
                                    tek.still = true;
                                }
                            }
                        }

                        else if ((tek.type == 2) && (tek.current.size() < 15))
                        {
                            // pathfinding off; not needed kinda
                            if ((timewarp - tek.bulltime >= 60) || (tek.bulltime == 0))
                            {
                                tek.bulltime = timewarp;
                                tek.cilj = player.cent;
                                tek.poc = tek.cent;
                                if (smer(player.cent.x, tek.cent.x))
                                {
                                    tek.ani = 13;
                                }
                                else
                                {
                                    tek.ani = 19;
                                }
                            }
                            if (smer(player.cent.x, tek.cent.x))
                            {
                                if (tek.ani >= 18)
                                    tek.ani = 1;
                            }
                            else if (tek.ani >= 24)
                                tek.ani = 7;

                            if ((timewarp - tek.bulltime >= 25) && (timewarp - tek.bulltime <= 59))
                            {
                                // enemy wiggle if sucsessfully avoided. I need another "if" to clean it up
                                if (dist(tek.cent, tek.poc) <= 400.0f)
                                {
                                    homingnode(tek.cilj.x, tek.poc.x, tek.cilj.y, tek.poc.y, 20, &tek.srcrec.x, &tek.srcrec.y, &tek.smerX);
                                }
                            }
                            // if (timer) rush(x1,x2,y1,y2) // Rush after timer ticks off func
                            // if rushed turn on pathfinding
                        }

                        if (tek.type == 3) // current size ga okida i spamuje metkove, to je prvi ocigledan problem
                        {
                            if (tek.current.size() <= 15)
                            {
                                if (smer(player.cent.x, tek.cent.x))
                                {
                                    if (tek.still)
                                    {
                                        tek.ani = 13;
                                        tek.still = false;
                                    }
                                    if (tek.ani > 19)
                                    {
                                        tek.ani = 13;
                                        tek.still = true;
                                    }
                                }

                                else
                                {
                                    if (tek.still)
                                    {
                                        tek.ani = 21;
                                        tek.still = false;
                                    }
                                    if (tek.ani > 27)
                                        tek.still = true;
                                }
                                // spawnovanje metaka problem
                                if ((timewarp - tek.bulltime >= 30))
                                {
                                    if ((tek.ani == 27) || (tek.ani == 19))
                                    {
                                        tek.bulltime = timewarp;
                                        tek.bull = true;
                                        metak1.emplace_back();
                                        metak1.back().metak = {tek.srcrec.x + 30.0f, tek.srcrec.y + 30.0f};
                                        metak1.back().poc = {tek.srcrec.x + 30.0f, tek.srcrec.y + 30.0f};
                                        metak1.back().cilj = player.cent;
                                    }
                                }
                            }

                            if (tek.current.size() >= 15)
                            {
                                // pljuckanje animacija! 13-20; 21-28; levo/desno
                                homingnode(tek.current[tek.current.size() - 3].rec.x + 15, tek.srcrec.x + 30, tek.current[tek.current.size() - 3].rec.y + 15, tek.srcrec.y + 30, tek.move, &tek.srcrec.x, &tek.srcrec.y, &tek.smerX);
                                if (smer(player.cent.x, tek.cent.x))
                                {
                                    if ((tek.ani == 6))
                                        tek.still = false;
                                    if (!tek.still)
                                    {
                                        tek.ani = 1;
                                        tek.still = true;
                                    }
                                }
                                else
                                {
                                    if ((tek.ani == 12))
                                    {
                                        tek.still = false;
                                    }
                                    if (!tek.still)
                                    {
                                        tek.ani = 7;
                                        tek.still = true;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        if ((tek.smerX == 0) || (tek.smerX == -1))
                        {
                            if ((tek.ani == 12))
                                tek.still = false;
                            if (!tek.still)
                            {
                                tek.ani = 7;
                                tek.still = true;
                            }
                        }
                        if (tek.smerX == 1)
                        {
                            if ((tek.ani == 6))
                                tek.still = true;
                            if (tek.still)
                            {
                                tek.ani = 1;
                                tek.still = false;
                            }
                        }
                    }
                }
            }
        }

        /////////////////////////////////
        // PATHFINDING GRID || GRID DRAW
        ////////////////////////////////

        for (int i = 0; i < std::ceil(screenWidth / dim) * std::ceil(screenHeight / dim); i++)
        {
            for (auto &tek : tekiV)
            {
                if (vechck(tek.pos))
                {
                    for (auto &value : tek.current)
                    {
                        if (i == value.i)
                            path[i].color = BLUE;
                    }
                }
            }
        }

        auto oBounds = [](Vector2 a)
        {
            return ((a.x >= -50) && (a.x < 1460) && (a.y >= -50) && (a.y < 950));
        };

        // BULLET driver

        int *mitt = new int{};

        for (auto &met : metak1)
        {
            // speed keeps stacking for each bulet, that's why only the first one is "normal" hmmmm..
            // bullet driver
            homingmetak(met.poc.x, met.cilj.x, met.poc.y, met.cilj.y, &met.metak.x, &met.metak.y);
            if (!oBounds(met.metak))
            {
                metak1.erase(metak1.begin() + *mitt); // delete bullets that venture off screen
            }
            *mitt += 1;
            //}
            // delete bullets after they leave the screen
        }
        delete mitt;

        // }

        /*

        PATHFINDING AUTO

        */

        /*

        Save hitbox postions & enemy spawn positions into .csv file

        */

        if (IsKeyReleased(KEY_F2))
        {
            for (auto &tek : tekiV)
            {
                tek.count = tekiV.size();
            }
            myfile.open("mapa.csv");

            for (auto &x : crtajV)
            {
                myfile << "," << x.rec.x << "," << x.rec.y << "," << x.rec.width << "," << x.rec.height << "," << x.pos.x << "," << x.pos.y << "," << x.count << '\n';
            }

            myfile.close();

            myfile.open("mapa_spawn.csv");

            for (auto &x : tekiV)
            {
                myfile << "," << x.srcrec.x << "," << x.srcrec.y << "," << x.pos.x << "," << x.pos.y << "," << x.type << "," << x.count << '\n';
                ;
            }

            myfile.close();
        }

        /*

        Save hitbox postions into .csv file

        */

        // RESET

        if (IsKeyReleased(KEY_R))
        {
            // color = WHITE;
            x = screenWidth / 2 - 50;
            y = screenHeight / 2 - 50;
            // posX = 1;
            // posY = 1;
            col = {1, 1, 1, 1};
        }

        // RESET

        // TOGGLE Devmode

        if (IsKeyReleased(KEY_TAB))
        {
            devmode = !devmode;
            player.HP = 5;
        }

        // TOGGLE Pause || Pause the gameplay before adding hitboxes and spawning new enemies (less hectic this way)

        if (IsKeyReleased(KEY_SPACE))
        {
            pause = !pause;
        }

        // TOGGLE newface || this swaps the "fancy" graphics with regular cubes and also swaps the background
        // for now i only have one pretty background drawn, was too frickin lazy to add 8 more

        if (IsKeyReleased(KEY_F1))
            newface = !newface;

        if (IsKeyReleased(KEY_ONE))
        {
            choice = 1;
        }
        if (IsKeyReleased(KEY_TWO))
        {
            choice = 2;
        }
        if (IsKeyReleased(KEY_THREE))
        {
            choice = 3;
        }

        // TOGGLE Devmode

        ///////////////////////////////////////////////////
        // DRAG & DROP HITBOX || SPAWNING ENEMIES || TOOL
        //////////////////////////////////////////////////

        if (devmode && pause)
        {

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) // instanciraj hitbox
            {
                crtajV.emplace_back();
                // crtajV.back().rec.x = mousePoint.x;
                // crtajV.back().rec.y = mousePoint.y;
                crtajV.back().rec = {mousePoint.x, mousePoint.y, 10, 10};
                crtajV.back().pos = {posX, posY};
            }

            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) // drag & drop
            {
                // else if ((mousePoint.x>pointX) && (mousePoint.y>pointY)) {
                crtajV.back().rec.width = mousePoint.x - crtajV.back().rec.x;
                crtajV.back().rec.height = mousePoint.y - crtajV.back().rec.y;
                //}
            }

            if (IsMouseButtonReleased(MOUSE_BUTTON_MIDDLE))
            {
                tekiV.emplace_back();
                tekiV.back().pos = {posX, posY};
                tekiV.back().srcrec = {mousePoint.x, mousePoint.y, 60, 60};
                tekiV.back().type = choice;
                std::cout << "mousePoint.x :" << mousePoint.x << '\n';
                std::cout << "mousePoint.y :" << mousePoint.y << '\n';
            }

            // Brisanje neprijatelja
            int *itt = new int{0};

            for (auto &tek : tekiV)
            {
                if (vechck(tek.pos))
                {
                    if (CheckCollisionPointRec(mousePoint, tek.srcrec))
                    {
                        // std::cout << "kolizija!" << '\n';
                        if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
                        {
                            std::cout << "kliknuo si i obrisao jednog neprijatelja" << '\n';
                            tekiV.erase(tekiV.begin() + *itt);
                        }
                    }
                }
                *itt += 1;
            }

            *itt = 0;

            // Brisanje hitboxova; radi samo malo neintuitivno (toggle flag)
            for (auto &crt : crtajV)
            {
                if (vechck(crt.pos))
                {
                    if (CheckCollisionPointRec(mousePoint, crt.rec))
                    {
                        if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
                        {
                            crtajV.erase(crtajV.begin() + *itt);
                        }
                    }
                }
                *itt += 1;
            }

            delete itt;
        }

        ///////////////////////////////////////////////////
        // DRAG & DROP HITBOX || SPAWNING ENEMIES || TOOL
        //////////////////////////////////////////////////

        /*


        // DEATH STATE
        // reload enemy positions
        // reset player position
        // reset health


        */

        ////////////////////////
        // DEATH STATE -> RESET
        ////////////////////////

        if (!devmode)
        {
            if (player.HP == 0)
            {
                int *br = new int{};
                tekiV.clear();
                for (auto &x : dump)
                {
                    if (*br == 0)
                        tekiV.emplace_back();
                    switch (*br)
                    {
                    case 0:
                        tekiV.back().srcrec.x = x;
                    case 1:
                        tekiV.back().srcrec.y = x;
                    case 2:
                        tekiV.back().pos.x = x;
                    case 3:
                        tekiV.back().pos.y = x;
                    case 4:
                        tekiV.back().type = x;
                    case 5:
                        tekiV.back().count = x;
                    }
                    tekiV.back().srcrec.width = 60;
                    tekiV.back().srcrec.height = 60;
                    *br += 1;
                    if (*br == 6)
                        *br = 0;
                }
                x = screenWidth / 2 - 30.0f;
                y = screenHeight / 2 - 30.0f;
                posY = posX = 0;
                player.HP = 5;
                // RESET STATE
                // GAME OVER SCREEN (ye)
                delete br;
            }
        }

        ////////////////////////
        // DEATH STATE -> RESET
        ////////////////////////

        /*

        Lambda eskiviranje (malo me drka sto stalno mora vucem vechck(pos) lambdicu; dosta gresaka pravio)

        Ovo treba da se okida samo

        if (!vechck(cell.back().pos))
        {
            for (auto& tek: tekiV)
            {
                if (vechck(tek.pos)){
                    cell.push_back(tek);
                }
            }
        }

        - Databaza s'teksturice
        - crtanje
        - layering
        - .csv dump i load
        sadja

        Layering

        1) Background
        2) z-buffer vector (player, decals, teki)
           vector.sort(vector.begin(), vector.end(), [](const lhs, const rhs){return lhs.y < rhs.y});
        3) overlay

        par problema:
        nece punimo vektor i da ga brisemo svaki put dok smo na istom ekranu.
        Problem sa ovim je sto neprijatelji umiru
        Punjenje i clear se rade samo kad napustimo tekuci blok u mapi

        if (vechck(pos))
        {
            vec.emplace_back()
            vec.back().srcrec = player.srcrec;
            vec.back().rec  = {x,y,60,60};
            vec.back().zbuff = y+30;
            vec.back().tex  = plyr;
            for (auto& tek: tekiV){
                if (vechck(tek.pos)){
                    vec.emplace_back();
                    vec.back().srcrec =  tek.srcrec;
                    vec.back().rec = tek.rec;
                    vec.back().zbuff = tek.srcrec.y + tek.srcrec.height + tek.srcrec.height/2
                    if (tek.type == 1)
                    vec.back().tex = type1;
                    if (tek.type == 2)
                    vec.back().tex = type2;
                    if (tek.type == 3)
                    vec.back().tex = type3;
                }
            }
            vec.sort(vec.begin(),vec.end(), [](const lhs, const rhs){
                return lhs.y < rhs.y;}
            });
        }

        else
           vec.clear();

        DRAW

        for (auto& vec: vec){
            DrawTextureRec(vec.tex, vec.rec, {vec.srcrec.x - 30, vec.srcrec.y - 30}, WHITE);
        }

        very simples <3 <3 <3

        */

        //----------------------------------------------------------------------------------

        BeginDrawing();

        ///////////////////////
        // BACKGROUND CHOICE
        ///////////////////////

        if (!newface)
            DrawTexture(mapa1[posY][posX], 0, 0, WHITE);
        else if (posY == 1 && posX == 1)
        {
            DrawTexture(bg, 0, 0, WHITE);
            DrawTexture(fg, 0, 0, WHITE);
        }
        else
            DrawTexture(mapa1[posY][posX], 0, 0, WHITE);

        ///////////////////////
        // BACKGROUND CHOICE
        ///////////////////////

        DrawRectangleLines(x, y, 60, 60, WHITE); // playya

        if (newface)
        {
            for (auto &value : lesina1)
            {
                if (vechck(value.pos))
                    DrawTextureRec(ded, value.rec, value.vec, WHITE);
            }

            DrawTextureRec(plyr, player.rec, {x - 30, y - 60}, WHITE); // Player must be bigger

            for (auto &tek : tekiV)
            {
                if (vechck(tek.pos))
                {
                    if (tek.type == 1)
                        DrawTextureRec(type1, tek.rec, {tek.srcrec.x - 30, tek.srcrec.y - 30}, WHITE);
                    if (tek.type == 2)
                        DrawTextureRec(type2, tek.rec, {tek.srcrec.x - 30, tek.srcrec.y - 30}, WHITE);
                    if (tek.type == 3)
                        DrawTextureRec(type3, tek.rec, {tek.srcrec.x - 30, tek.srcrec.y - 30}, WHITE);

                    DrawRectangleLines(tek.srcrec.x, tek.srcrec.y, 60, 60, WHITE);

                    if (!devmode)
                        DrawLine(tek.srcrec.x + 30.0f, tek.srcrec.y + 30.0f, x + 30.0f, y + 30.0f, BLACK);
                }
            }
        }

        /////////////////////////////////
        /// DRAW CORPSES IF ANY
        /////////////////////////////////

        if (!newface)
        {
            for (auto &value : lesina1)
            {
                DrawRectangle(value.vec.x, value.vec.y, 60, 60, LIME);
            }
        }

        /////////////////////////////////
        /// DRAW CORPSES IF ANY
        /////////////////////////////////

        ////////////////////
        /* DEBUG LAYER */
        ////////////////////

        if (devmode)
        {
            for (auto &x : crtajV)
            {
                if (vechck(x.pos))
                    DrawRectangleLinesEx(x.rec, 5, BLUE);
            }
            DrawRectangleLinesEx(crtajV.back().rec, 10, BLUE);
            for (auto &tek : tekiV)
            {

                if (vechck(tek.pos))
                {
                    /*
                    if (tek.type == 1)
                        DrawRectangle(tek.srcrec.x, tek.srcrec.y, 60, 60, BROWN);
                    if (tek.type == 2)
                        DrawRectangle(tek.srcrec.x, tek.srcrec.y, 60, 60, DARKBLUE);
                    if (tek.type == 3)
                        DrawRectangle(tek.srcrec.x, tek.srcrec.y, 60, 60, DARKPURPLE);
                    */

                    DrawRectangleLines(tek.srcrec.x, tek.srcrec.y, 60, 60, WHITE);
                    DrawCircle(tek.srcrec.x, tek.srcrec.y, 15, RED);
                    DrawCircleLines(tek.circle.x, tek.circle.y, 350, RED);
                }
            }
            for (auto &path : path)
                DrawRectangleLinesEx(path.rec, 2.5f, path.color);

            for (auto &guzica : metak1)
                DrawCircle(guzica.metak.x, guzica.metak.y, 10, RED);
        }

        if (!newface)
        {
            for (auto &tek : tekiV)
            {
                if (vechck(tek.pos))
                {
                    if (tek.type == 1)
                        DrawRectangle(tek.srcrec.x, tek.srcrec.y, 60, 60, BROWN);
                    if (tek.type == 2)
                        DrawRectangle(tek.srcrec.x, tek.srcrec.y, 60, 60, DARKBLUE);
                    if (tek.type == 3)
                        DrawRectangle(tek.srcrec.x, tek.srcrec.y, 60, 60, DARKPURPLE);

                    DrawRectangleLines(tek.srcrec.x, tek.srcrec.y, 60, 60, WHITE);
                    DrawCircle(tek.srcrec.x, tek.srcrec.y, 15, RED);
                    DrawCircleLines(tek.circle.x, tek.circle.y, 350, RED);
                }
            }
        }

        // SWORD DRAW

        DrawTexturePro(slash, player.slash, {player.cent.x, player.cent.y, 120, 120}, {60, 60}, player.rot, WHITE); // "crosshair"
        DrawCircleLines(player.mac.x, player.mac.y, 30.0f, YELLOW);

        // SWORD DRAW

        DrawText(TextFormat("broj metaka: %d", metak1.size()), 1100, 50, 25, BLACK);
        DrawText(TextFormat("Enemy type : %.3d", choice), 1100, 20, 25, BLACK);
        DrawText(TextFormat("player.cent.x %f", player.cent.x), 1100, 80, 25, BLACK);
        DrawText(TextFormat("player.cent.y %f", player.cent.y), 1100, 110, 25, BLACK);
        DrawText(TextFormat("player.rot %f", player.rot), 1100, 140, 25, BLACK);
        DrawText(TextFormat("DEV MODE %d", devmode), 700, 50, 50, BLACK);
        if (pause)
            DrawText("Paused", 700, 100, 50, BLACK);
        else
            DrawText("UNPaused", 700, 100, 50, BLACK);
        // Left-hand side

        DrawText(TextFormat("player.dash.x %f", player.dash.x), 50, 100, 30, BLACK);
        DrawText(TextFormat("player.dash.y %f", player.dash.y), 50, 160, 30, BLACK);
        DrawText(TextFormat("combo counter %d", comboC), 50, 220, 30, BLACK);

        ////////////////////
        /* DEBUG LAYER */
        ////////////////////

        ///////////////////////////
        //// HP BAR
        //////////////////////////

        for (int i = 0; i < player.HP; i++)
        {
            if (!newface)
                DrawRectangle(50 * (i + 1), 30, 40, 40, RED);
            else
                DrawTexture(srce, 80 * (i + 0.5), 30, RAYWHITE);
        }

        DrawRectangle(1300, 800, 90, 90, BLACK);

        ////////////////////////////
        /// MINIMAP
        ///////////////////////////

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                minimap = mapa[j][i];
                DrawRectangle(1300 + i * 30, 800 + j * 30, 30, 30, minimap);
                DrawRectangle(1312 + posX * 30, 812 + posY * 30, 5, 5, RED); // minimap position
            }
        }

        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}