#ifndef __GAMEFUNCTIONS_H__
#define __GAMEFUNCTIONS_H__

#define E_BULLET_NUM 500

#include"TextureImage.h"

typedef struct{
    ImageData img;
    TextureImage tex;
    double posx;
    double posy;
    bool status;
    int remain;
} Object;

typedef struct{
    double dirx, diry;
} Vector;

typedef struct{
    int rank;
    char name[32];
    double score;
} Player;

class Enemy{
    private:
    public:
        ImageData img;
        TextureImage tex;
        double posx, posy;
        int hp;
        bool status;
        Object E_bullet[ E_BULLET_NUM ];
        Vector bulletVector[ E_BULLET_NUM ];
        double shootedtime ,bulletInterval, bulletVelocity;
        int bulletnum, shootmode;
        double appearTime, velocity;
        bool shown;

        Enemy();

        Enemy(const char *filename, double _posx, double _posy, int _hp, int _shootmode, int _bulletnum, double _bulletvelocity, double _interval, double _appeartime, double _velocity, bool _status);

        Enemy(ImageData *original_img, double _posx, double _posy, int _hp, int _shootmode, int _bulletnum, double _bulletvelocity, double _interval, double _appeartime, double _velocity, bool _status);

        Enemy(double _posx, double _posy, bool _status,int _hp);

        void display();

        void setpos(double _posx, double _posy, int _hp, bool _status);

        void move(int windowWidth);

        bool judgeHit(Object *obj, int offset);

        bool judgeAlive();

        void presetbullet(const char *filename);

        void presetbullet(ImageData *original_image);

        void setbullet();

        void shootbullet(Object *obj);

        void movebullet();

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

void setTextureObject(Object *obj, const char *filename, double posx, double posy, bool status, int _remain);

void sameTextureMultiSet(Object object[], const char *filename, double posx, double posy, bool status,int num);

void sameTextureMultiSet(Object object[], ImageData *original_image, double posx, double posy, bool status, int num);

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

bool judgeHit_mainchara(Object *hero, Object *obj2, int offset);

void DrawString(char *str, int length, void *font, int windowWidth, int windowHeight, int x0, int y0);

void DrawString(char *str, int length, void *font, int windowWidth, int windowHeight, int x0, int y0, const char *prestring);

void initDoubleArray(double array[], int num);

void initString(char str[], int num);

int judgeSign(int num);

#endif