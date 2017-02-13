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

#define VELO_SPEED 5

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

#define PLAYER_SHOT_MAX 24
Object bullet[PLAYER_SHOT_MAX];
double bullet_Prevtime = 0.0;


#define ENEMY_NUM 64
#define ENEMY_IMAGE1 "reversed_enemy1.ppm"
#define ENEMY_IMAGE2 "reversed_enemy2.ppm"
#define ENEMY_IMAGE3 "reversed_enemy3.ppm"
#define ENEMY_IMAGE4 "reversed_enemy4.ppm"
#define ENEMY_HP1 10
#define ENEMY_HP2 50
#define ENEMY_HP3 20
#define ENEMY_BULLET_IMAGE "nc120941_fixed.ppm"
#define ENEMY_BULLET_IMAGE2 "daen_gray_fixed.ppm"
// #define ENEMY_BULLET_NUM 100
// Object enemy[ENEMY_NUM];
Enemy enemy[ENEMY_NUM];
double enemy_Prevtime[ENEMY_NUM];
// Enemy *enemy;
// double *enemy_Prevtime;
bool ebulleton = true;
Vector enemypos[ENEMY_NUM] = {
    {g_WindowWidth, g_WindowHeight / 4 * 3}, 
    {g_WindowWidth, g_WindowHeight / 4}
};

#define DEFEAT_SCORE 500
#define SCORE_DIGIT 10
double score = 0,
       displayscore = 0;
char scorec[10];

int displaymode = 0;
int dis2come = 0;

#define RANK_NUM 10
double score_showtime = 0;
Player player[RANK_NUM];

#define FILE_RANK_RECORD "rankrecord.txt"
FILE *rankrec;

char putname[32];
int namei = 0;
bool breakinput = false;
bool changerank = true;
bool return0 = false;

void myInit();

void init(void)
{
    myInit();
    //白
    // glClearColor(1.0, 1.0, 1.0, 1.0);   /* ウィンドウを消去するときの色を設定 */
}

void myInit(void){
    glClearColor(0.005, 0, 0.15, 1.0);


    initDoubleArray(enemy_Prevtime, ENEMY_NUM);

    setTextureObject(&heroin, "hero3.ppm", 0, 0, true, 5);
    // heroin.img.width     /= 1.5;
    // heroin.img.height    /= 1.5;

    sameTextureMultiSet(bullet, "hero_bullet.ppm", 0, 0, false, PLAYER_SHOT_MAX);
    // sameTextureMultiSet(enemy, ENEMY_IMAGE, g_WindowWidth, g_WindowHeight / 4 * 3, true, ENEMY_NUM);

    // enemy = new Enemy[ENEMY_NUM];
    // enemy_Prevtime = new double[ENEMY_NUM];
    bool first = true;
    int firstnum;
    double beforeTime;
    for(int i = 0; i < ENEMY_NUM; i++){
        if(i < 10){
            beforeTime = 2 + (int)(i / 2);
            if(first){
                new( &enemy[i] ) Enemy(ENEMY_IMAGE1 , enemypos[i%2].dirx, enemypos[i%2].diry, ENEMY_HP1, 1, 3, 2, 5000, beforeTime, 1, true);
                enemy[i].presetbullet(ENEMY_BULLET_IMAGE);
                first = false;
                firstnum = i;
            }
            else{
                // printf("else!!!\n");
                new( &enemy[i] ) Enemy( &enemy[firstnum].img, enemypos[i%2].dirx, enemypos[i%2].diry, ENEMY_HP1, 1, 1, 2, 5000, beforeTime, 1, true);
                enemy[i].presetbullet(&enemy[firstnum].E_bullet[0].img);
                if(i == 9){
                    first = true;
                }
            }
        }
        else if(10 <= i && i < 14){
            beforeTime = 10 + (int)((i - 10) / 2) * 2;
            if(first){
                new( &enemy[i] ) Enemy( ENEMY_IMAGE2, enemypos[i%2].dirx, g_WindowHeight / 2 - (enemypos[i%2].diry - g_WindowHeight) / 4, ENEMY_HP2, 0, 4, 1, 4000, beforeTime, 0.5, true);
                enemy[i].presetbullet(ENEMY_BULLET_IMAGE);
                first = false;
                firstnum = i;
            }
            else{
                // printf("speed:%d\n",2 - i + firstnum);
                new( &enemy[i] ) Enemy( &enemy[firstnum].img, enemypos[i%2].dirx, g_WindowHeight / 2 - (enemypos[i%2].diry - g_WindowHeight) / 4, ENEMY_HP2, 0, 4, 1, 4000, beforeTime, 0.5, true);
                enemy[i].presetbullet(&enemy[firstnum].E_bullet[0].img);
                if(i == 13){
                    first = true;
                }
            }
        }
        else if(14 <= i && i < 18){
            beforeTime = 18 + (int)((i - 10) / 2);
            if(first){
                new( &enemy[i] ) Enemy( ENEMY_IMAGE3, enemypos[i%2].dirx, enemypos[i%2].diry, ENEMY_HP3 + 20, 0, 10, 5, 4000, beforeTime, 1, true);
                enemy[i].presetbullet(ENEMY_BULLET_IMAGE);
                first = false;
                firstnum = i;
            }
            else{
                new( &enemy[i] ) Enemy( &enemy[firstnum].img, enemypos[i%2].dirx, enemypos[i%2].diry, ENEMY_HP3 + 20, 0, 10, 5, 4000, beforeTime, 1, true);
                enemy[i].presetbullet(&enemy[firstnum].E_bullet[0].img);
                if(i == 17){
                    first = true;
                }
            }
        }
        else if(18 <= i && i < 20){
            beforeTime = 28 + (i - 18);
            if(first){
                new( &enemy[i] ) Enemy( ENEMY_IMAGE1, enemypos[0].dirx, enemypos[0].diry, ENEMY_HP1, 1, 1, 5, 1800, beforeTime, 3, true);
                enemy[i].presetbullet(ENEMY_BULLET_IMAGE);
                first = false;
                firstnum = i;
            }
            else{
                new( &enemy[i] ) Enemy( &enemy[firstnum].img, enemypos[0].dirx, enemypos[0].diry, ENEMY_HP3, 1, 1, 5, 1800, beforeTime, 3, true);
                enemy[i].presetbullet(&enemy[firstnum].E_bullet[0].img);
                if(i == 19){
                    first = true;
                }
            }
        }
        else if(20 <= i && i < 22){
            beforeTime = 30 + i - 20;
            if(first){
                new( &enemy[i] ) Enemy( ENEMY_IMAGE2, enemypos[1].dirx, enemypos[1].diry, ENEMY_HP1, 1, 1, 5, 1200, beforeTime, 3, true);
                enemy[i].presetbullet(ENEMY_BULLET_IMAGE);
                first = false;
                firstnum = i;
            }
            else{
                new( &enemy[i] ) Enemy( &enemy[firstnum].img, enemypos[1].dirx, enemypos[1].diry, ENEMY_HP3, 1, 1, 5, 1800, beforeTime, 3, true);
                enemy[i].presetbullet(&enemy[firstnum].E_bullet[0].img);
                if(i == 21){
                    first = true;
                }
            }
        }
        else if(22 <= i && i < 24){
            beforeTime = 32 + i - 22;
            if(first){
                new( &enemy[i] ) Enemy( ENEMY_IMAGE1, enemypos[0].dirx, enemypos[0].diry, ENEMY_HP1, 1, 1, 5, 1200, beforeTime, 3, true);
                enemy[i].presetbullet(ENEMY_BULLET_IMAGE);
                first = false;
                firstnum = i;
            }
            else{
                new( &enemy[i] ) Enemy( &enemy[firstnum].img, enemypos[0].dirx, enemypos[0].diry, ENEMY_HP3, 1, 1, 5, 1800, beforeTime, 3, true);
                enemy[i].presetbullet(&enemy[firstnum].E_bullet[0].img);
                if(i == 23){
                    first = true;
                }
            }
        }
        else if(24 <= i && i < 26){
            beforeTime = 34 + i - 24;
            if(first){
                new( &enemy[i] ) Enemy( ENEMY_IMAGE1, enemypos[1].dirx, enemypos[1].diry, ENEMY_HP1, 1, 1, 5, 1200, beforeTime, 3, true);
                enemy[i].presetbullet(ENEMY_BULLET_IMAGE);
                first = false;
                firstnum = i;
            }
            else{
                new( &enemy[i] ) Enemy( &enemy[firstnum].img, enemypos[1].dirx, enemypos[1].diry, ENEMY_HP3, 1, 1, 5, 1800, beforeTime, 3, true);
                enemy[i].presetbullet(&enemy[firstnum].E_bullet[0].img);
                if(i == 25){
                    first = true;
                }
            }
        }
        else if(26 <= i && i < 30){
            beforeTime = 38 + i - 26;
            if(first){
                new( &enemy[i] ) Enemy( ENEMY_IMAGE1, enemypos[i%2].dirx, enemypos[i%2].diry, ENEMY_HP3, 2, 4, 5, 1800, beforeTime, 2, true);
                enemy[i].presetbullet(ENEMY_BULLET_IMAGE);
                first = false;
                firstnum = i;
            }
            else{
                new( &enemy[i] ) Enemy( &enemy[firstnum].img, enemypos[i%2].dirx, enemypos[i%2].diry, ENEMY_HP3, 2, 4, 5, 1800, beforeTime, 2, true);
                enemy[i].presetbullet(&enemy[firstnum].E_bullet[0].img);
                if(i == 29){
                    first = true;
                }
            }
        }
        else if(30 <= i && i < 34){
            beforeTime = 42 + i - 30;
            if(first){
                new( &enemy[i] ) Enemy( ENEMY_IMAGE2, enemypos[i%2].dirx, enemypos[i%2].diry, ENEMY_HP3, 2, 4, 5, 1800, beforeTime, 2, true);
                enemy[i].presetbullet(ENEMY_BULLET_IMAGE);
                first = false;
                firstnum = i;
            }
            else{
                new( &enemy[i] ) Enemy( &enemy[firstnum].img, enemypos[i%2].dirx, enemypos[i%2].diry, ENEMY_HP3, 2, 4, 5, 1800, beforeTime, 2, true);
                enemy[i].presetbullet(&enemy[firstnum].E_bullet[0].img);
                if(i == 33){
                    first = true;
                }
            }
        }
        else if(34 <= i && i < 40){
            beforeTime = 50;
            if(first){
                new( &enemy[i] ) Enemy( ENEMY_IMAGE2, enemypos[i%2].dirx, enemypos[1].diry + (i - 34) * 70, ENEMY_HP1 - 3, 2, 4, 5, 1800, beforeTime, 1, true);
                enemy[i].presetbullet(ENEMY_BULLET_IMAGE);
                first = false;
                firstnum = i;
            }
            else{
                new( &enemy[i] ) Enemy( &enemy[firstnum].img, enemypos[1].dirx, enemypos[1].diry + (i - 34) * 70, ENEMY_HP1 - 3, 2, 4, 5, 1800, beforeTime, 1, true);
                enemy[i].presetbullet(&enemy[firstnum].E_bullet[0].img);
                if(i == 39){
                    first = true;
                }
            }
        }
        else if(40 <= i && i < 46){
            beforeTime = 54;
            if(first){
                new( &enemy[i] ) Enemy( ENEMY_IMAGE2, enemypos[i%2].dirx, enemypos[0].diry - (i - 40) * 70, ENEMY_HP1 - 3, 2, 4, 5, 1800, beforeTime, 1, true);
                enemy[i].presetbullet(ENEMY_BULLET_IMAGE);
                first = false;
                firstnum = i;
            }
            else{
                new( &enemy[i] ) Enemy( &enemy[firstnum].img, enemypos[1].dirx, enemypos[0].diry - (i - firstnum) * 70, ENEMY_HP1 - 3, 2, 4, 5, 1800, beforeTime, 1, true);
                enemy[i].presetbullet(&enemy[firstnum].E_bullet[0].img);
                if(i == 45){
                    first = true;
                }
            }
        }
        else if(46 <= i && i < 52){
            beforeTime = 62;
            if(first){
                new( &enemy[i] ) Enemy( ENEMY_IMAGE2, enemypos[i%2].dirx, enemypos[0].diry - (i - 40) * 70, ENEMY_HP1 - 3, 2, 6, 5, 1400, beforeTime, 2, true);
                enemy[i].presetbullet(ENEMY_BULLET_IMAGE);
                first = false;
                firstnum = i;
            }
            else{
                new( &enemy[i] ) Enemy( &enemy[firstnum].img, enemypos[1].dirx, enemypos[0].diry - (i - firstnum) * 70, ENEMY_HP1 - 3, 2, 6, 5, 1400, beforeTime, 2, true);
                enemy[i].presetbullet(&enemy[firstnum].E_bullet[0].img);
                if(i == 51){
                    first = true;
                }
            }
        }
        else if(52 <= i && i < 58){
            beforeTime = 66;
            if(first){
                new( &enemy[i] ) Enemy( ENEMY_IMAGE1, enemypos[i%2].dirx, enemypos[1].diry + (i - 40) * 30, ENEMY_HP1 - 3, 2, 6, 5, 1400, beforeTime, 2, true);
                enemy[i].presetbullet(ENEMY_BULLET_IMAGE);
                first = false;
                firstnum = i;
            }
            else{
                new( &enemy[i] ) Enemy( &enemy[firstnum].img, enemypos[1].dirx, enemypos[1].diry + (i - firstnum) * 30, ENEMY_HP1 - 3, 2, 6, 5, 1400, beforeTime, 2, true);
                enemy[i].presetbullet(&enemy[firstnum].E_bullet[0].img);
                if(i == 57){
                    first = true;
                }
            }
        }
        else if(58 <= i && i < 64){
            beforeTime = 74;
            if(first){
                new( &enemy[i] ) Enemy( ENEMY_IMAGE1, enemypos[i%2].dirx, enemypos[0].diry - (i - 40) * 30, ENEMY_HP1 - 3, 2, 6, 5, 1400, beforeTime, 2, true);
                enemy[i].presetbullet(ENEMY_BULLET_IMAGE);
                first = false;
                firstnum = i;
            }
            else{
                new( &enemy[i] ) Enemy( &enemy[firstnum].img, enemypos[1].dirx, enemypos[0].diry - (i - firstnum) * 30, ENEMY_HP1 - 3, 2, 6, 5, 1400, beforeTime, 2, true);
                enemy[i].presetbullet(&enemy[firstnum].E_bullet[0].img);
                if(i == 63){
                    first = true;
                }
            }
        }
    }

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
    // int i;

    // printf("display: width: %d\n", heroin.img.width);
    
    /* ウィンドウを消去 … glClearColor で指定した色で塗りつぶし */
    glClear(GL_COLOR_BUFFER_BIT);

    if(displaymode == 0){
        char start[256];
        strcpy(start, "push any key to start");
        glColor3d(1.0, 1.0, 1.0);
        DrawString(start, 21, GLUT_BITMAP_HELVETICA_18, g_WindowWidth, g_WindowHeight, g_WindowWidth / 2 - 120, g_WindowHeight / 2);
        strcpy(start, "Mirage Shooting");
        glColor3d(1.0, 1.0, 1.0);
        DrawString(start, 16, GLUT_BITMAP_HELVETICA_18, g_WindowWidth, g_WindowHeight, g_WindowWidth / 2 - 100, g_WindowHeight / 2 - 100);
    
    }

    else if(displaymode == 1){
        //alpha値を有効に;
        glAlphaFunc(GL_GREATER,0.5);
        glEnable(GL_ALPHA_TEST);

        displayObject(&heroin);

        displayObject(bullet, PLAYER_SHOT_MAX);
        for(int i = 0; i < ENEMY_NUM; i++){
            enemy[i].display();
            enemy[i].displaybullet();
        }
        
        for(int i = 0; i < ENEMY_NUM; i++){
            for(int j = 0; j < PLAYER_SHOT_MAX; j++){
                if(enemy[i].judgeHit(&bullet[j], 20)){
                    enemy_Prevtime[i] = GetTime();
                    score += DEFEAT_SCORE;
                }
            }
            for(int j = 0; j < enemy[i].bulletnum; j++){
                judgeHit_mainchara(&heroin, &enemy[i].E_bullet[j], 20);
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

        if(GetSecond() > 82){
            displaymode = 2;
        }
    }

    else if(displaymode == 2){
        if(dis2come == 0){
            // if(heroin.remain > 0){
                score = score + heroin.remain * 1000;
            // }
            dis2come = 1;   
        }
        if(displayscore < score){
                displayscore+=2;
                score_showtime = GetTime();
        }
        sprintf(scorec, "%10.0lf", displayscore);
        DrawString(scorec, SCORE_DIGIT, GLUT_BITMAP_HELVETICA_18, g_WindowWidth, g_WindowHeight, g_WindowWidth / 2 - 120, g_WindowHeight / 2, "your score:");
    }
    else if(displaymode == 3){
        char ranking[256];
        strcpy(ranking, "Ranking");
        glColor3d(1.0, 1.0, 1.0);
        DrawString(ranking, 7, GLUT_BITMAP_HELVETICA_18, g_WindowWidth, g_WindowHeight, g_WindowWidth / 2 - 20, g_WindowHeight / 5);
        for(int i = 0; i < RANK_NUM; i++){
            char rank[3], rscorec[SCORE_DIGIT];
            sprintf(rank, "%3d", player[i].rank);
            DrawString(rank, 3, GLUT_BITMAP_HELVETICA_18, g_WindowWidth, g_WindowHeight, g_WindowWidth / 2 - 100, g_WindowHeight / 5 + 30 + 20 * i);
            DrawString(player[i].name, 32, GLUT_BITMAP_HELVETICA_18, g_WindowWidth, g_WindowHeight, g_WindowWidth / 2 - 50, g_WindowHeight / 5 + 30 + 20 * i);
            sprintf(rscorec, "%10.0lf", player[i].score);
            DrawString(rscorec, SCORE_DIGIT, GLUT_BITMAP_HELVETICA_18, g_WindowWidth, g_WindowHeight, g_WindowWidth / 2 + 50, g_WindowHeight / 5 + 30 + 20 * i);
        }
    }
    else if(displaymode == 4){
        bool inputname = false;
        int replace = 0;
        for(int i = 0; i < RANK_NUM; i++){
            if(score > player[i].score){
                inputname = true;
                replace = i;
                break;
            }
        }
        if(inputname && !breakinput){
            char inputs[256];
            strcpy(inputs, "Input your name");
            glColor3d(1.0, 1.0, 1.0);
            DrawString(inputs, 15, GLUT_BITMAP_HELVETICA_18, g_WindowWidth, g_WindowHeight, g_WindowWidth / 2 - 40, g_WindowHeight / 5);
            DrawString(putname, namei, GLUT_BITMAP_HELVETICA_18, g_WindowWidth, g_WindowHeight, g_WindowWidth / 2 - 40, g_WindowHeight / 2);
        }
        else if(breakinput){
            if(changerank){
                for(int i = RANK_NUM; i > replace; --i){
                    initString(player[i].name, 32);
                    strcpy(player[i].name, player[i - 1].name);
                    player[i].score = player[i - 1].score;
                }
                initString(player[replace].name, 32);
                strcpy(player[replace].name, putname);
                player[replace].score = score;
                changerank = false;
            }
            if(!return0){
                char ranking[256];
                strcpy(ranking, "Ranking");
                glColor3d(1.0, 1.0, 1.0);
                DrawString(ranking, 7, GLUT_BITMAP_HELVETICA_18, g_WindowWidth, g_WindowHeight, g_WindowWidth / 2 - 20, g_WindowHeight / 5);
                for(int i = 0; i < RANK_NUM; i++){
                    char rank[3], rscorec[SCORE_DIGIT];
                    sprintf(rank, "%3d", player[i].rank);
                    DrawString(rank, 3, GLUT_BITMAP_HELVETICA_18, g_WindowWidth, g_WindowHeight, g_WindowWidth / 2 - 100, g_WindowHeight / 5 + 30 + 20 * i);
                    DrawString(player[i].name, 32, GLUT_BITMAP_HELVETICA_18, g_WindowWidth, g_WindowHeight, g_WindowWidth / 2 - 50, g_WindowHeight / 5 + 30 + 20 * i);
                    sprintf(rscorec, "%10.0lf", player[i].score);
                    DrawString(rscorec, SCORE_DIGIT, GLUT_BITMAP_HELVETICA_18, g_WindowWidth, g_WindowHeight, g_WindowWidth / 2 + 50, g_WindowHeight / 5 + 30 + 20 * i);
                }
            }
            if(return0){
                rankrec = fopen(FILE_RANK_RECORD, "w");
                for(int i = 0; i < RANK_NUM; i++){
                    fprintf(rankrec, "%d %s %.0lf\n", player[i].rank, player[i].name, player[i].score);
                }
                char start[256];
                strcpy(start, "push ESC or q to quit");
                glColor3d(1.0, 1.0, 1.0);
                DrawString(start, 21, GLUT_BITMAP_HELVETICA_18, g_WindowWidth, g_WindowHeight, g_WindowWidth / 2 - 120, g_WindowHeight / 2);            
            }
        }
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
            limitPosObject(&heroin, 10);

            for(int i = 0; i < ENEMY_NUM; i++){
                enemy[i].move(g_WindowWidth);
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
    switch (key)
    {
        case 'z':
            keyz = true;
            break;
        case '3':
            // displaymode = 3;
            break;
        case 'q':   /* キーボードの 'q' 'Q' 'ESC' を押すとプログラム終了 */
        case 'Q':
        case '\033':
            // delete[] enemy;
            exit(-1);
            break;
    }
    if(displaymode == 4){
        printf("namei: %d\n", namei);
        if(key == '\010'){
            putname[namei] = '\0';
            if(namei > 0){
                namei--;
            }
        }
        else if(key == '\015'){
            if(breakinput && namei || !changerank){
                namei++;
                putname[namei] = '\0';
                return0 = true;
            }
            breakinput = true;
        }
        else{
            putname[namei] = key;
            namei++;
        }
    }
    if(displaymode == 0){
        StartTimer();
        displaymode = 1;
    }
    if(displaymode == 2){
        displaymode = 4;
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
    if((rankrec = fopen(FILE_RANK_RECORD, "r+")) == NULL){
        fprintf(stderr, "cannot open %s\n", FILE_RANK_RECORD);
        fprintf(stderr, "please make %s\n", FILE_RANK_RECORD);
        exit(1);
    }


    for(int i = 0; i < RANK_NUM; i++){
        fscanf(rankrec, "%d %s %lf", &player[i].rank, player[i].name, &player[i].score);
    }    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(g_WindowWidth, g_WindowHeight);
    glutCreateWindow("mirage_shooting");
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