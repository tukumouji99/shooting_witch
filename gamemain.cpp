#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<string>
#include<new>
#include<GL/glut.h>

#include"ImageData.h"
#include"ImageIO.h"
#include"TextureImage.h"
#include"GenericTimer.h"

#include"gamefunctions.h"

#define VELO_SPEED 2

ImageData g_Image;
TextureImage g_Tex;
int g_ImagePosX = 256;	/* 画像の左下点の、初期 x 座標 */
int g_ImagePosY = 0;	/* 画像の左下点の、初期 y 座標 */

int g_WindowWidth = 1200;    /* ウィンドウの横幅 */
int g_WindowHeight = 660;   /* ウィンドウの縦幅 */
int g_ImageVelocityX = 0;	/* 座標更新時の、x 方向の移動距離 */
int g_ImageVelocityY = 0;	/* 座標更新時の、y 方向の移動距離 */
int g_AnimationDulation = 10;/* 何ミリ秒ごとに更新するかの刻み幅 */

double g_PrevTime = 0.0;

// int g_CurveCurrentIndex = 0;    /* 折れ線の配列において、いま操作している折れ線のインデックス */

bool keyup      = false,
     keyright   = false,
     keydown    = false,
     keyleft    = false,
     keyz       = false;

#define TIMER_DIGIT 10
// std::string timer(TIMER_DIGIT, '\0');
char timer[TIMER_DIGIT];

Object heroin;

#define PLAYER_SHOT_MAX 20
Object bullet[PLAYER_SHOT_MAX];
double bullet_Prevtime = 0.0;

#define ENEMY_IMAGE "mon_272_reversed.ppm"
#define ENEMY_NUM 4
#define ENEMY_HP 20
#define ENEMY_BULLET_IMAGE "nc120941_fixed.ppm"
#define ENEMY_BULLET_IMAGE2 "daen_gray_fixed.ppm"
// #define ENEMY_BULLET_NUM 100
// Object enemy[ENEMY_NUM];
Enemy enemy[ENEMY_NUM];
double enemy_Prevtime[ENEMY_NUM];
bool ebulleton = true;
Vector enemypos[ENEMY_NUM] = {{g_WindowWidth, g_WindowHeight / 4 * 3}, {g_WindowWidth, g_WindowHeight / 4}, {g_WindowWidth, g_WindowHeight / 4 * 3}, {g_WindowWidth, g_WindowHeight / 4}};

#define DEFEAT_SCORE 500
#define SCORE_DIGIT 10
double score = 0,
       displayscore = 0;
char scorec[10];

int displaymode = 0;

void init(void)
{
    //白
    // glClearColor(1.0, 1.0, 1.0, 1.0);   /* ウィンドウを消去するときの色を設定 */
    glClearColor(0, 0, 0, 1.0);

    initDoubleArray(enemy_Prevtime, ENEMY_NUM);

    setTextureObject(&heroin, "test.ppm", 0, 0, true, 3);

    sameTextureMultiSet(bullet, "buy_bullet.ppm", 0, 0, false, PLAYER_SHOT_MAX);

    // sameTextureMultiSet(enemy, ENEMY_IMAGE, g_WindowWidth, g_WindowHeight / 4 * 3, true, ENEMY_NUM);
    
    for(int i = 0; i < ENEMY_NUM; i++){
        if(i < 2){
            new( &enemy[i] ) Enemy( ENEMY_IMAGE, enemypos[i].dirx, enemypos[i].diry, ENEMY_HP, 0, 10, 3, 1000, 10,true);
            enemy[i].presetbullet(ENEMY_BULLET_IMAGE);
        }
        else{
            new( &enemy[i] ) Enemy( ENEMY_IMAGE, enemypos[i].dirx, enemypos[i].diry, ENEMY_HP, 1, 1, 10, 100, 2, true);
            enemy[i].presetbullet(ENEMY_BULLET_IMAGE);
            // enemyAppearTime[i] = 10;
        }
    }

    StartTimer();
    // printf("width: %d\n", heroin.img.width);
}

void DrawCircle(int xi, int yi, int radius)
{
    int i;
    const int nDivision = 16;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2i(xi, yi);
    for (i=0; i<=nDivision; i++)
    {
        const double radian = 2*M_PI*(i/(double)nDivision);
        const double x = cos(radian) * radius + xi;
        const double y = sin(radian) * radius + yi;
        glVertex2d(x, y);
    }
    glEnd();
}

/* 表示処理のためのコールバック関数 */
void display(void)
{
    // int i;

    // printf("display: width: %d\n", heroin.img.width);
    
    /* ウィンドウを消去 … glClearColor で指定した色で塗りつぶし */
    glClear(GL_COLOR_BUFFER_BIT);

    if(displaymode == 0){
        char start[256];
        strcpy(start, "push any key to start");
        glColor3d(1.0, 1.0, 1.0);
        DrawString(start, 21, GLUT_BITMAP_HELVETICA_18, g_WindowWidth, g_WindowHeight, g_WindowWidth / 2 - 120, g_WindowHeight / 2);

    }
    else if(displaymode == 1){
        //alpha値を有効に
        glAlphaFunc(GL_GREATER,0.5);
        glEnable(GL_ALPHA_TEST);

        displayObject(&heroin);

        displayObject(bullet, PLAYER_SHOT_MAX);
        for(int i = 0; i < ENEMY_NUM; i++){
            // if(enemyAppearTime[i] < GetTime()/1000.0){
                enemy[i].display();
                enemy[i].displaybullet();
            // }
        }
        
        for(int i = 0; i < ENEMY_NUM; i++){
            for(int j = 0; j < PLAYER_SHOT_MAX; j++){
                if(enemy[i].judgeHit(&bullet[j], 20)){
                    enemy_Prevtime[i] = GetTime();
                    score += DEFEAT_SCORE;
                }
            }
            for(int j = 0; j < enemy[i].bulletnum; j++){
                judgeHit_mainchara(&heroin, &enemy[i].E_bullet[j],20);
            }
        }

        //alphaを無効に
        glDisable(GL_ALPHA_TEST);

        sprintf(timer, "%10.2lf", GetTime()/1000.0);
        glColor3d(1.0, 1.0, 1.0);
        DrawString(timer, TIMER_DIGIT, GLUT_BITMAP_HELVETICA_18, g_WindowWidth, g_WindowHeight, g_WindowWidth / 5 * 4, 18, "timer:");

        if(displayscore < score){
            displayscore+=2;
        }
        sprintf(scorec, "%10.0lf", displayscore);
        DrawString(scorec, SCORE_DIGIT, GLUT_BITMAP_HELVETICA_18, g_WindowWidth, g_WindowHeight, g_WindowWidth / 5 * 4 - 200, 18, "score:");

        char remain[3] = {'\0','\0','\0'};
        sprintf(remain, "%3d", heroin.remain);
        DrawString(remain, 3, GLUT_BITMAP_HELVETICA_18, g_WindowWidth, g_WindowHeight, 0, 18, "player:");
    }

    glutSwapBuffers();
    // glFlush();  /* ここまで指定した描画命令をウィンドウに反映 */
}
 
/* 定期的に呼び出され、画像の位置を更新するための関数 */
void idle(void)
{
	/* もし前回の更新から一定時間が過ぎていたら */
    if(displaymode == 1){
        if ( GetRapTime(g_PrevTime) >= g_AnimationDulation )
        {
            
            moveObject(&heroin, VELO_SPEED);
            limitPosObject(&heroin, 200);

            for(int i = 0; i < ENEMY_NUM; i++){
                enemy[i].move(g_WindowWidth, 2);
                enemy[i].shootbullet(&heroin);
                enemy[i].movebullet();
                enemy[i].judgebullet(g_WindowWidth, g_WindowHeight);
            }

            /* 最終更新時刻を記録する */
            g_PrevTime = GetTime();
        }

        if( GetRapTime(bullet_Prevtime) >= 20){
            if(keyz){
                for(int i = 0; i < PLAYER_SHOT_MAX; i++){
                    if(!bullet[i].status){
                        setPosObject(&bullet[i], heroin.posx + heroin.img.width / 2.0, heroin.posy + heroin.img.height/ 2.0 , true);
                        break;
                    }
                }
            }

            for(int i = 0; i < PLAYER_SHOT_MAX; i++){
                bullet[i].posx += 50;
                if(bullet[i].posx >= g_WindowWidth){
                    bullet[i].status = false;
                }
            }

            bullet_Prevtime = GetTime();
        }
    }
	display();
}

/* キーボード入力のためのコールバック関数 */
void keyboard(unsigned char key, int x, int y)
{
    displaymode = 1;
    switch (key)
    {
        case 'z':
            keyz = true;
            break;
        case 'q':   /* キーボードの 'q' 'Q' 'ESC' を押すとプログラム終了 */
        case 'Q':
        case '\033':
            exit(-1);
            break;
    }

    glutPostRedisplay();    /* ウィンドウ描画関数を呼ぶ */
}

void keyboardup(unsigned char key, int x, int y){
    if(key == 'z'){
        keyz = false;
    } 
}

void specialkey(int key, int x, int y){
    if(key == GLUT_KEY_UP){
        keyup = true;
    }
    if(key == GLUT_KEY_DOWN){
        keydown = true;
    }
    if(key == GLUT_KEY_RIGHT){
        keyright = true;
    }
    if(key == GLUT_KEY_LEFT){
        keyleft = true; 
    }
}

void specialkeyup(int key, int x, int y){
    if( key == GLUT_KEY_UP ){
        keyup = false;
    }
    if( key == GLUT_KEY_DOWN ){
        keydown = false;
    }
    if( key == GLUT_KEY_LEFT ){
        keyleft = false;
    }
    if( key == GLUT_KEY_RIGHT ){
        keyright = false;
    }
}

/* マウス入力のためのコールバック関数 */
void mouse(int button, int state, int x, int _y)
{
    glutPostRedisplay();    /* ウィンドウ描画関数を呼ぶ */
}
 
/* ウィンドウサイズが変更されたときのためのコールバック関数 */
void reshape(int w, int h)
{
    if (h < 1) return;
 
    g_WindowWidth = w;
    g_WindowHeight = h;
 
    glViewport(0, 0, w, h); /* ウィンドウの描画範囲を指定 */
 
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
 
    glutPostRedisplay();    /* ウィンドウ描画関数を呼ぶ */
}
 
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(g_WindowWidth, g_WindowHeight);
    glutCreateWindow("shooting_witch");
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardup);
    glutSpecialFunc(specialkey);
    glutSpecialUpFunc(specialkeyup);
    glutMouseFunc(mouse);
    glutReshapeFunc(reshape);
    init();
    glutMainLoop();
    return 0;
}