#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <math.h>

#define width 85
#define height 25

typedef struct {
    int x,y,w;
} TRacket;

typedef struct{
    float x,y;
    int ix, iy;
    float alfa;
    float speed;
} TBall;

char field[height][width+1];
TRacket racket;
TBall ball;
int hitCnt  = 0;
int maxHitCnt  = 0;


void initBall(){
    ball.x = 2;
    ball.y = 2;
    ball.alfa = -1;
    ball.speed = 0.5;
}

void putBall(){
    field[ball.iy][ball.ix] = '*';
}

void moveBall(float x, float y){
    ball.x = x;
    ball.y = y;
    ball.ix = (int)round(ball.x);
    ball.iy = (int)round(ball.y);
}

void autoMoveBall(){
    if(ball.alfa < 0) ball.alfa += M_PI*2;
    if(ball.alfa < M_PI*2) ball.alfa -= M_PI*2;

    TBall bl = ball;
    moveBall(ball.x + cos(ball.alfa) * ball.speed
             ,ball.y + sin(ball.alfa) * ball.speed);

    if((field[ball.iy][ball.ix] == '#') || (field[ball.iy][ball.ix] == '@')){
        if(field[ball.iy][ball.ix] == '@') hitCnt++;
        if((ball.ix != bl.ix) && (ball.iy != bl.iy)){
            if(field[bl.iy][ball.ix] == field[ball.iy][bl.ix]){
                bl.alfa = bl.alfa + M_PI;
            } else{
                if(field[bl.iy][ball.ix] == '#'){
                    bl.alfa = (2*M_PI - bl.alfa) + M_PI;
                } else{
                    bl.alfa = (2*M_PI - bl.alfa);
                }
            }
        }
        else if(ball.iy == bl.iy)
            bl.alfa = (2*M_PI - bl.alfa) + M_PI;
        else
            bl.alfa = (2*M_PI - bl.alfa);

        ball = bl;
        autoMoveBall();
    }
}

void initRacket(){
    racket.w = 7;
    racket.x = (width - racket.w) / 2;
    racket.y = height - 1;
}

void putRacket(){
    for(int i = racket.x; i < racket.x + racket.w; i++)
        field[racket.y][i] = '@';
}

void moveRacket(int x){
    racket.x = x;
    if(racket.x < 1)
        racket.x = 1;
    if(racket.x + racket.w >= width)
        racket.x = width - 1 -racket.w;
}

void init(){
    for(int i = 0; i < width; i++)
        field[0][i] = '#';

    field[0][width] = '\0';

    strncpy(field[1], field[0], width+1);

    for(int i = 1; i < width-1; i++)
        field[1][i] = ' ';

    for(int i = 2; i < height; i++)
        strncpy(field[i], field[1], width+1);
    for(int i = 10; i < 20; i++)
        field[9][i] = '#';
    for(int i = 65; i < 80; i++)
        field[16][i] = '#';
    for(int i = 45; i < 55; i++)
        field[3][i] = '#';
    for(int i = 30; i < 35; i++)
        field[7][i] = '#';
}

void show(){
    for(int i = 0; i < height; i++){
        printf("%s", field[i]);
        if(i == 3)
            printf("   hit %i   ", hitCnt);
        if(i == 4)
            printf("   Max %i   ", maxHitCnt);
        if(i < height - 1)
            printf("\n");
    }

}
void setcur(int x, int y){
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int main()
{
    BOOL run = FALSE;
    initRacket();
    initBall();
    do{
    setcur(0,0);
    if(run) autoMoveBall();
    if(ball.iy > height){
        run = FALSE;
        if(hitCnt > maxHitCnt)
            maxHitCnt = hitCnt;
        hitCnt = 0;
    }
    init();
    putRacket();
    putBall();
    show();

    if(GetKeyState('A') < 0) moveRacket(racket.x - 1);
    if(GetKeyState('D') < 0) moveRacket(racket.x + 1);
    if(GetKeyState('W') < 0) run = TRUE;
     if(!run)
        moveBall(racket.x + racket.w /2, racket.y - 1);
    Sleep(10);
    } while (GetKeyState(VK_ESCAPE) >= 0);

    return 0;
}
