#ifndef __GAMEFUNCTIONS_H__
#define __GAMEFUNCTIONS_H__

#include"TextureImage.h"

typedef struct{
    ImageData img;
    TextureImage tex;
    double posx;
    double posy;
} Object;

typedef struct _node{
    Object object;
    struct _node *next;
} Node;

typedef struct{
    Node *head;
    Node *tail;
} ObjectList;

//Objectの移動;posx,posyを計算
void moveObject(Object *obj, double velocity);

#endif