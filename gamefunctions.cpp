#include"ImageIO.h"
#include"gamefunctions.h"
#include<cstdio>

extern bool keyup,
     keyright,
     keydown,
     keyleft;

extern int
    g_WindowHeight,
    g_WindowWidth;

Node *createNode(const char *filename, double posx, double posy){
    Node *n = new Node[1];

    if(n == NULL){
        fprintf(stderr, "createNode: error: memory allocation failed\n");
        return NULL;
    }

    InitImageData(&n->object.img);    /* 画像データを初期化 */

    if(LoadPPMImage_alpha("test.ppm" ,&n->object.img)){
        FlipImageData(&n->object.img);    /* 画像の上下を反転する */
        MakeTextureFromImage(&n->object.tex, &n->object.img);
    }
    //初期位置
    setPosObject(&n->object, posx, posy, false);

    return n;
}

void printNode(Node *node){
    printf("posx:%6.2lf posy:%6.2lf\n", node->object.posx, node->object.posy);
}

void ListInit(ObjectList *list){
    list->head = NULL;
    list->tail = NULL;
}


void setPosObject(Object *obj, double xi, double yi, bool status_){
    obj->posx = xi;
    obj->posy = yi;
    obj->status = status_;
}

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

void limitPosObject(Object *obj, int offset){
        /* めりこみを許す */

        /* 画面の外に出ないように座標を調整 */
		if (obj->posx < -offset)
		{
			obj->posx = -offset;
		}
		else if (g_WindowWidth - obj->img.width + offset <= obj->posx)
		{
			obj->posx = g_WindowWidth - obj->img.width + offset;
		}

		if (obj->posy < -offset)
		{
			obj->posy = -offset;
		}
		else if (g_WindowHeight - obj->img.height + offset <= obj->posy)
		{
			obj->posy = g_WindowHeight - obj->img.height + offset;
		}

}