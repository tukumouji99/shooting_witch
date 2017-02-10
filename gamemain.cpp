#include<cstdio>
#include<cstdlib>
#include<cmath>
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

extern bool keyup,
     keyright,
     keydown,
     keyleft;

Object heroin;

void init(void)
{
    //白
    // glClearColor(1.0, 1.0, 1.0, 1.0);   /* ウィンドウを消去するときの色を設定 */
    glClearColor(0, 0, 0, 0);
    // glLineWidth(3.f);                   /* 線の太さを指定 */
    // glEnable(GL_LINE_SMOOTH);           /* 線をなめらかに表示するための設定 */
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);

    InitImageData(&heroin.img);    /* 画像データを初期化 */

    if(LoadPPMImage("test.ppm" ,&heroin.img)){
        FlipImageData(&heroin.img);    /* 画像の上下を反転する */
        MakeTextureFromImage(&heroin.tex, &heroin.img);
    }
    heroin.posx = 0;
    heroin.posy = 0;

    StartTimer();
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
    int i;

    /* ウィンドウを消去 … glClearColor で指定した色で塗りつぶし */
    glClear(GL_COLOR_BUFFER_BIT);

    if( IsImageDataAllocated(&heroin.img) && IsTextureAvailable(&heroin.tex)){
        glColor3f(1,1,1);
        DrawTexturedQuad_d(&heroin.tex, heroin.posx, heroin.posy, heroin.img.width, heroin.img.height);
    }
   
    glutSwapBuffers();
    // glFlush();  /* ここまで指定した描画命令をウィンドウに反映 */
}
 
/* 定期的に呼び出され、画像の位置を更新するための関数 */
void idle(void)
{
	/* もし前回の更新から一定時間が過ぎていたら */
	if ( GetRapTime(g_PrevTime) >= g_AnimationDulation )
	{
	  /* めりこみを許す */
	  int offset = 0;
	  
		/* x, y 座標を更新 */
		// g_ImagePosX += g_ImageVelocityX;
		// g_ImagePosY += g_ImageVelocityY;

        moveObject(&heroin, VELO_SPEED);
		
        /* 画面の外に出ないように座標を調整 */
		if (heroin.posx < -offset)
		{
			heroin.posx = -offset;
		}
		else if (g_WindowWidth - heroin.img.width + offset <= heroin.posx)
		{
			heroin.posx = g_WindowWidth - heroin.img.width + offset;
		}

		if (heroin.posy < -offset)
		{
			heroin.posy = -offset;
		}
		else if (g_WindowHeight - heroin.img.height + offset <= heroin.posy)
		{
			heroin.posy = g_WindowHeight - heroin.img.height + offset;
		}

		/* 最終更新時刻を記録する */
		g_PrevTime = GetTime();
	}

	display();
}

/* キーボード入力のためのコールバック関数 */
void keyboard(unsigned char key, int x, int y)
{
    int i, nCurves;

    switch (key)
    {
    case 'q':   /* キーボードの 'q' 'Q' 'ESC' を押すとプログラム終了 */
    case 'Q':
    case '\033':
        exit(-1);
        break;
    }

    glutPostRedisplay();    /* ウィンドウ描画関数を呼ぶ */
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
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowSize(g_WindowWidth, g_WindowHeight);
    glutCreateWindow("shooting_witch");
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialkey);
    glutSpecialUpFunc(specialkeyup);
    glutMouseFunc(mouse);
    glutReshapeFunc(reshape);
    init();
    glutMainLoop();
    return 0;
}