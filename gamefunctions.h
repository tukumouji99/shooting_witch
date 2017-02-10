#ifndef __GAMEFUNCTIONS_H__
#define __GAMEFUNCTIONS_H__

#include"TextureImage.h"

typedef struct{
    ImageData img;
    TextureImage tex;
    double posx;
    double posy;
    bool status;
} Object;

typedef struct _node{
    Object object;
    struct _node *next;
} Node;

typedef struct{
    Node *head;
    Node *tail;
} ObjectList;

Node *createNode(const char *filename, double posx, double posy);

void printNode(Node *node);

//(0,0)にオブジェクトを移動
void setPosObject(Object *obj, double xi, double yi, bool status_);

//Objectの移動;posx,posyを計算
void moveObject(Object *obj, double velocity);

//画面外へのはみ出しを防ぐ、offset分認める
void limitPosObject(Object *obj, int offset);

#endif