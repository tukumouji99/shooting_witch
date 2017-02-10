#include"gamefunctions.h"
#include<cstdio>

bool keyup = false,
     keyright = false,
     keydown = false,
     keyleft = false;

void moveObject(Object *obj, double velocity){
    if(keyup && keyright){
        obj->posx += velocity;
        obj->posy += velocity;
    }
    else if(keyup && keyleft){
        obj->posx -= velocity;
        obj->posy += velocity;
    }
    else if(keydown && keyleft){
        obj->posx -= velocity;
        obj->posy -= velocity;
    }
    else if(keydown && keyright){
        obj->posx += velocity;
        obj->posy -= velocity;
    }
    else if(keyup){
        // printf("obj: posx: %lf\n", obj->posx);
        obj->posy += velocity;
    }
    else if(keyright){
        obj->posx += velocity;
    }
    else if(keydown){
        obj->posy -= velocity;
    }
    else if(keyleft){
        obj->posx -= velocity;
    }
}