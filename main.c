#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#include "ImageData.h"
#include "ImageIO.h"
#include "TextureImage.h"

#include "GenericTimer.h"

int g_WindowWidth = 512;    /* ウィンドウの横幅 */
int g_WindowHeight = 512;   /* ウィンドウの縦幅 */

#define IMAGE_FILE "lenna128x128.ppm"
/* #define IMAGE_FILE "kanamori.ppm" */

ImageData g_Image;
TextureImage g_Tex;
int g_ImagePosX = 256;	/* 画像の左下点の、初期 x 座標 */
int g_ImagePosY = 0;	/* 画像の左下点の、初期 y 座標 */
int g_ImageVelocityX = -3;	/* 座標更新時の、x 方向の移動距離 */
int g_ImageVelocityY = 2;	/* 座標更新時の、y 方向の移動距離 */
int g_AnimationDulation = 10;/* 何ミリ秒ごとに更新するかの刻み幅 */

double g_PrevTime = 0.0;

/* 初期化処理 */
void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);   /* ウィンドウを消去するときの色を設定 */

	InitImageData(&g_Image);	/* 画像データを初期化 */

	if ( LoadPPMImage(IMAGE_FILE, &g_Image) ) /* 画像データの読み込みに成功 */
	{
		FlipImageData(&g_Image); /* 画像の上下を反転する */
		MakeTextureFromImage(&g_Tex, &g_Image);
	}

	StartTimer();	/* 時間計測の開始 */
}

/* 表示処理のためのコールバック関数 */
void display(void)
{
	/* ウィンドウを消去 … glClearColor で指定した色で塗りつぶし */
	glClear(GL_COLOR_BUFFER_BIT);

	if ( IsImageDataAllocated(&g_Image) && IsTextureAvailable(&g_Tex)) /* もし画像が読み込めていたら */
	{
	  /* 座標 (g_ImagePosX, g_ImagePosY) の位置に、画像を表示 */
	  /* DrawImageData(&g_Image, g_ImagePosX, g_ImagePosY); */
	  glColor3f(1,1,1);
	  DrawTexturedQuad(&g_Tex, g_ImagePosX, g_ImagePosY, g_Image.width, g_Image.height);
	}

	glutSwapBuffers();
}

/* 定期的に呼び出され、画像の位置を更新するための関数 */
void idle(void)
{
	/* もし前回の更新から一定時間が過ぎていたら */
	if ( GetRapTime(g_PrevTime) >= g_AnimationDulation )
	{
	  /* めりこみを許す */
	  int offset = 50;
	  
		/* x, y 座標を更新 */
		g_ImagePosX += g_ImageVelocityX;
		g_ImagePosY += g_ImageVelocityY;

		/* 画面の外に出ないように座標を調整 */
		
		if (g_ImagePosX < -offset)
		{
			g_ImagePosX = -offset;
			g_ImageVelocityX = -g_ImageVelocityX;
		}
		else if (g_WindowWidth - g_Image.width + offset <= g_ImagePosX)
		{
			g_ImagePosX = g_WindowWidth - g_Image.width + offset;
			g_ImageVelocityX = -g_ImageVelocityX;
		}

		if (g_ImagePosY < -offset)
		{
			g_ImagePosY = -offset;
			g_ImageVelocityY = -g_ImageVelocityY;
		}
		else if (g_WindowHeight - g_Image.height + offset <= g_ImagePosY)
		{
			g_ImagePosY = g_WindowHeight - g_Image.height + offset;
			g_ImageVelocityY = -g_ImageVelocityY;
		}

		/* 最終更新時刻を記録する */
		g_PrevTime = GetTime();
	}

	display();
}

/* キーボード入力のためのコールバック関数 */
void keyboard(unsigned char key, int x, int y)
{
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
	glutCreateWindow("Animation");
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutReshapeFunc(reshape);
	init();
	glutMainLoop();

	return 0;
}
