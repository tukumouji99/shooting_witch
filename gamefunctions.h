#ifndef __GAMEFUNCTIONS_H__
#define __GAMEFUNCTIONS_H__

#define E_BULLET_NUM 100

#include"TextureImage.h"

typedef struct{
    ImageData img;
    TextureImage tex;
    double posx;
    double posy;
    bool status;
} Object;

typedef struct{
    double dirx, diry;
} Vector;

class Enemy{
    private:
        ImageData img;
    public:
        TextureImage tex;
        double posx, posy;
        int hp;
        bool status;
        Object E_bullet[ E_BULLET_NUM ];
        Vector bulletVector[ E_BULLET_NUM ];

        Enemy();

        Enemy(const char *filename, double _posx, double _posy, bool _status,int _hp);

        Enemy(double _posx, double _posy, bool _status,int _hp);

        void display();

        void setpos(double _posx, double _posy, int _hp, bool _status);

        void move(int windowWidth, double velocity);

        bool judgeHit(Object *obj, int offset);

        bool judgeAlive();

        void presetbullet(const char *filename);

        void setbullet();

        void shootbullet(int num, int velocity);

        void movebullet(int num);

        void displaybullet();

        void judgebullet(int windowWidth, int windowHeight);
};

typedef struct _node{
    Object object;
    struct _node *next;
} Node;

typedef struct{
    Node *head;
    Node *tail;
} ObjectList;

Node *createNode(const char *filename, double posx, double posy);

void setTextureObject(Object *obj, const char *filename, double posx, double posy, bool status);

void sameTextureMultiSet(Object object[], const char *filename, double posx, double posy, bool status,int num);

void printNode(Node *node);

//(0,0)にオブジェクトを移動
void setPosObject(Object *obj, double xi, double yi, bool status_);

void displayObject(Object *obj);

void displayObject(Object obj[], int num);

//Objectの移動;posx,posyを計算
void moveObject(Object *obj, double velocity);

//画面外へのはみ出しを防ぐ、offset分認める
void limitPosObject(Object *obj, int offset);

bool judgeHit(Object *obj1, Object *obj2, int offset);

void DrawString(char *str, int length, void *font, int windowWidth, int windowHeight, int x0, int y0, const char *prestring);

void initDoubleArray(double array[], int num);

#endif