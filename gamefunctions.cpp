#include<cstdio>
#include<cmath>
#include<cstring>
#include<string>
#include<GL/glut.h>
#include"ImageIO.h"
#include"ImageData.h"
#include"GenericTimer.h"
#include"gamefunctions.h"

extern bool 
    keyup,
    keyright,
    keydown,
    keyleft,
    keyz;

extern int
    g_WindowHeight,
    g_WindowWidth;

Enemy::Enemy(){

}

Enemy::Enemy(const char *filename, double _posx, double _posy, int _hp, int _shootmode, int _bulletnum, double _velocity, double _interval, double _appeartime, bool _status){
    InitImageData(&img);

    if(LoadPPMImage_alpha(filename, &img)){
        FlipImageData(&img);
        MakeTextureFromImage(&tex, &img);
    }

    posx    = _posx;
    posy    = _posy;
    status  = _status;
    hp      = _hp;
    bulletInterval  = _interval;
    appearTime      = _appeartime;
    bulletnum       = _bulletnum;
    bulletVelocity  = _velocity;
    shootmode       = _shootmode;
}

void Enemy::display(){
    if( IsImageDataAllocated(&img) && IsTextureAvailable(&tex) && status && GetSecond() > appearTime){
        DrawTexturedQuad_d(&tex, posx, posy, img.width, img.height);
    }
}

void Enemy::setpos(double _posx, double _posy, int _hp, bool _status){
    posx    = _posx;
    posy    = _posy;
    hp      = _hp;
    status  = _status;
}

void Enemy::move(int windowWidth, double velocity){
    if(GetSecond() > appearTime){
        if(status){
            posx -= velocity;
        }
        if(posx < -img.width){
            status = false;
        }
    }
}

bool Enemy::judgeHit(Object *obj, int offset){
    if(fabs((obj->posx + obj->img.width / 2.0) - (posx + img.width / 2.0)) < img.width && fabs((obj->posy + obj->img.height / 2.0) - (posy + img.height / 2.0)) < offset){
        if(obj->status && status){
            obj->status = false;
            hp -= 1;
            if(this->judgeAlive()){
                return true;
            }
        }
    }
    return false;
}

bool Enemy::judgeAlive(){
    if(hp <= 0){
        status = false;
        return true;
    }

    return false;
}

void Enemy::presetbullet(const char *filename){
    sameTextureMultiSet(E_bullet, filename, posx, posy, false, E_BULLET_NUM);
}

void Enemy::setbullet(){
    for(int i = 0; i < E_BULLET_NUM; i++){
        setPosObject(&E_bullet[i], posx, posy, false);
    }
}

void Enemy::shootbullet(Object *obj){
    int shooted = 0;
    if(GetSecond() > appearTime){
        if(GetRapTime(shootedtime) >= bulletInterval && status){
            for(int i = 0; i < E_BULLET_NUM; i++){
                if(!E_bullet[i].status){
                    if(shootmode == 0){
                        bulletVector[i].dirx    = bulletVelocity * cos(2 * M_PI / bulletnum * shooted + 50.0 / 360.0 * 2 * M_PI);
                        bulletVector[i].diry    = bulletVelocity * sin(2 * M_PI / bulletnum * shooted + 50.0 / 360.0 * 2 * M_PI);
                    }
                    else if(shootmode == 1){
                        double angle = atan2((obj->posy + obj->img.height / 2) - (posy), (obj->posx + obj->img.width / 2) - posx);
                        bulletVector[i].dirx    = bulletVelocity * cos(angle);
                        bulletVector[i].diry    = bulletVelocity * sin(angle);
                    }
                    setPosObject(&E_bullet[i], posx, posy, true);
                    shooted++;
                }
                if(shooted >= bulletnum){
                    shootedtime = GetTime();
                    break;
                }
            }
        }
    }
}

void Enemy::movebullet(){
    for(int i = 0; i < E_BULLET_NUM; i++){
        if(E_bullet[i].status){
            E_bullet[i].posx += bulletVector[i].dirx;
            E_bullet[i].posy += bulletVector[i].diry;
        }
    }
}

void Enemy::displaybullet(){
    displayObject(E_bullet, E_BULLET_NUM);
}

void Enemy::judgebullet(int windowWidth, int windowHeight){
    for(int i = 0; i < E_BULLET_NUM; i++){
        if( E_bullet[i].posx + E_bullet[i].img.width < 0 ||
            E_bullet[i].posx > windowWidth ||
            E_bullet[i].posy < 0 ||
            E_bullet[i].posy > windowHeight){
            if(E_bullet[i].status){
                E_bullet[i].status = false;
            }
        }
    }
}

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

void setTextureObject(Object *obj, const char *filename, double posx, double posy, bool status){
    InitImageData(&obj->img);    /* 画像データを初期化 */

    if(LoadPPMImage_alpha(filename ,&obj->img)){
        FlipImageData(&obj->img);    /* 画像の上下を反転する */
        MakeTextureFromImage(&obj->tex, &obj->img);
    }
    //初期位置
    setPosObject(obj, posx, posy, status);
}

void setTextureObject(Object *obj, const char *filename, double posx, double posy, bool status, int _remain){
    InitImageData(&obj->img);    /* 画像データを初期化 */

    if(LoadPPMImage_alpha(filename ,&obj->img)){
        FlipImageData(&obj->img);    /* 画像の上下を反転する */
        MakeTextureFromImage(&obj->tex, &obj->img);
    }
    //初期位置
    setPosObject(obj, posx, posy, status);
    obj->remain = _remain;
}

void sameTextureMultiSet(Object object[],const char *filename, double posx, double posy, bool status, int num){
    InitImageData(&object[0].img);

    if(LoadPPMImage_alpha(filename, &object[0].img)){
        FlipImageData(&object[0].img);
        MakeTextureFromImage(&object[0].tex, &object[0].img);
    }

    setPosObject(&object[0], posx, posy, status);

    for(int i = 1; i < num; i++){
        InitImageData(&object[i].img);
        object[i].img.width     = object[0].img.width;
        object[i].img.height    = object[0].img.height;
        object[i].img.channels  = object[0].img.channels;
        object[i].img.data      = object[0].img.data;
        
        // FlipImageData(&object[i].img);
        MakeTextureFromImage(&object[i].tex, &object[i].img);
        setPosObject(&object[i], posx, posy, status);
    }
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

void displayObject(Object *obj){
    if( IsImageDataAllocated(&obj->img) && IsTextureAvailable(&obj->tex) && obj->status){
        DrawTexturedQuad_d(&obj->tex, obj->posx, obj->posy, obj->img.width, obj->img.height);
    }
}

void displayObject(Object obj[], int num){
    
    for(int i = 0; i < num; i++){
        if( IsImageDataAllocated(&obj[i].img) && IsTextureAvailable(&obj[i].tex) && obj[i].status){
            DrawTexturedQuad_d(&obj[i].tex, obj[i].posx, obj[i].posy, obj[i].img.width, obj[i].img.height);
        }
    }

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

bool judgeHit(Object *obj1, Object *obj2, int offset){
    
    if(fabs((obj1->posx + obj1->img.width / 2.0) - (obj2->posx + obj2->img.width / 2.0)) < offset && fabs((obj1->posy + obj1->img.height / 2.0) - (obj2->posy + obj2->img.height / 2.0)) < offset){
        if(obj1->status && obj2->status){
            obj1->status = false;
            obj2->status = false;
            return true;
        }
    }

    return false;

}

bool judgeHit_mainchara(Object *hero, Object *obj2, int offset){
    
    if(fabs((hero->posx + hero->img.width / 2.0) - (obj2->posx + obj2->img.width / 2.0)) < offset && fabs((hero->posy + hero->img.height / 2.0) - (obj2->posy + obj2->img.height / 2.0)) < offset){
        if(hero->status && obj2->status){
            hero->remain--;
            obj2->status = false;
            return true;
        }
    }

    return false;
}

void DrawString(char *str, int length, void *font, int windowWidth, int windowHeight, int x0, int y0){
    glDisable(GL_LIGHTING);
    // 平行投影にする
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, windowHeight, 0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // 画面上にテキスト描画
    glRasterPos2f(x0, y0);
    int size = length;
    for(int i = 0; i < size; ++i){
        char ic = str[i];
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ic);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void DrawString(char *str, int length, void *font, int windowWidth, int windowHeight, int x0, int y0, const char *prestring){
    glDisable(GL_LIGHTING);
    // 平行投影にする
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, windowHeight, 0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // 画面上にテキスト描画
    glRasterPos2f(x0, y0);
    char pre[256];
    strcpy(pre, prestring);
    for(int i = 0; pre[i] != '\0'; i++){
        char ic = pre[i];
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ic);
    }
    int size = length;
    for(int i = 0; i < size; ++i){
        char ic = str[i];
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ic);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void initDoubleArray(double array[], int num){
    for(int i = 0; i < num; i++){
        array[i] = 0.0;
    }
}

int judgeSign(int num){
    if(num == 0){
        return 0;
    }
    return ((num > 0) ? 1 : -1);
}