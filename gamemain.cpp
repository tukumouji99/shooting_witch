#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<GL/glut.h>

#include"gamefunctions.h"

int g_WindowWidth = 1200;    /* ウィンドウの横幅 */
int g_WindowHeight = 660;   /* ウィンドウの縦幅 */

int g_CurveCurrentIndex = 0;    /* 折れ線の配列において、いま操作している折れ線のインデックス */

void init(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);   /* ウィンドウを消去するときの色を設定 */
    glLineWidth(3.f);                   /* 線の太さを指定 */
    glEnable(GL_LINE_SMOOTH);           /* 線をなめらかに表示するための設定 */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
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

   
   
    glFlush();  /* ここまで指定した描画命令をウィンドウに反映 */
}
 
/* キーボード入力のためのコールバック関数 */
void keyboard(unsigned char key, int x, int y)
{
    int i, nCurves;

    switch (key)
    {
    case ' ':   /* スペースキーを押すと点をクリア */
    
        g_CurveCurrentIndex = 0;
 
        printf("curves cleared\n");
        break;
    case 's':   /* 's' を押すとファイルに折れ線データを書き込み */
        /* 折れ線データをファイルに保存する次の関数を自分で実装 */
            printf("%d curves saved in \"\"\n", g_CurveCurrentIndex+1);
        break;
    case 'l':   /* 'l' (エル) を押すとファイルから折れ線データを読み込み */
        /* 折れ線データをファイルから読み込む次の関数を自分で実装 */
            g_CurveCurrentIndex = (nCurves <= 0) ? 0 : nCurves-1;
           printf("%d curves loaded from \"\"\n", nCurves);
        break;
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
    int y = g_WindowHeight - _y;

    if (state == GLUT_DOWN) /* もしマウスのボタンがクリックされたら */
    {
        const int isShiftPressed = glutGetModifiers() & GLUT_ACTIVE_SHIFT;
        const int isCtrlPressed = glutGetModifiers() & GLUT_ACTIVE_CTRL;

        if ( isShiftPressed ) /* マウスクリック時に Shift キーが押されていたら */
        {
            int i;

            for (i=0; i<=g_CurveCurrentIndex; i++)
            {
                /* クリックした点 (x,y) から半径 5 ピクセル以内に頂点があれば削除 */
                /* 次の関数を自分で実装する */
                    break;
            }
        }
        else if ( isCtrlPressed ) /* マウスクリック時に Ctrl キーが押されていたら */
        {
            if ( g_CurveCurrentIndex < 1024-1 )
            {
                unsigned char r = rand() % 256;
                unsigned char g = rand() % 256;
                unsigned char b = rand() % 256;

                g_CurveCurrentIndex++;

                /* 折れ線に色を設定する関数を自分で実装 */
         
                /* 折れ線の末尾にクリックした点を頂点として追加する関数を自分で実装 */
         
                printf("new curve, added point at (%d,%d), color = (%d,%d,%d)\n", x, y, r, g, b);
            }
        }
        else
        {
            if ( g_CurveCurrentIndex <= 1024-1 )
            {
                /* 折れ線に頂点がまだ登録されていないかどうかを判定する関数を自分で実装 */
                    unsigned char r = rand() % 256;
                    unsigned char g = rand() % 256;
                    unsigned char b = rand() % 256;

                    /* 折れ線に色を設定する関数を自分で実装 */
            
                    /* 折れ線の末尾にクリックした点を頂点として追加する関数を自分で実装 */
            
                    printf("added point at (%d,%d), color = (%d,%d,%d)\n", x, y, r, g, b);
            }
            else
            {
                    /* 折れ線の末尾にクリックした点を頂点として追加する関数を自分で実装 */
            
                    printf("added point at (%d,%d)\n", x, y);
            }
        }
    }

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
    glutCreateWindow("Curves");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutReshapeFunc(reshape);
    init();
    glutMainLoop();
    return 0;
}