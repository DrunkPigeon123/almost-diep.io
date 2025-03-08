#include "raylib.h"
#include <math.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_BULLETS 100
#define maxGummies 50
#define playerSpeed 0.17f

typedef struct {
    Vector2 pos;
    Vector2 dir;
    float speed;
    bool active;

} Projectile;

typedef struct {
    Vector2 pos;
    Vector2 dir;
    float speed;
    bool active;
    float health;

} Enemy;

Projectile bullets[MAX_BULLETS];
int bulletscount = 0;
int bosscount = 0;

void spawnBoss(Enemy* escort)
{
    if(bosscount == 0){
    escort->pos = (Vector2){GetRandomValue(100,5000),GetRandomValue(100,5000)};
    escort->speed = 0.14;
    escort->health = 100;
    escort->active = true;
    bosscount++;}
}

void DrawHealthbar(Enemy escort, Vector2 offset)
{
    DrawRectangle(escort.pos.x-50 - offset.x,escort.pos.y-80 - offset.y,escort.health,30,RED);
}

void hitBoss(Enemy* escort, Projectile bullets[])
{   
    for(int i=0;i<bulletscount;i++)
    {
    if(CheckCollisionCircles(escort->pos,50,bullets[i].pos,10))
    {
       escort->health -= 3;
       bullets[i].pos.x = 10000;
       bullets[i].active = false;
    }
    }
}


void mplayer(Vector2* playerPos)

{
    if(IsKeyDown(KEY_W))playerPos->y -= playerSpeed;
    if(IsKeyDown(KEY_S))playerPos->y += playerSpeed;
    if(IsKeyDown(KEY_A))playerPos->x -= playerSpeed;
    if(IsKeyDown(KEY_D))playerPos->x += playerSpeed;
}

float rotate(Vector2 screenPlayerPos)
{
    Vector2 mousePos = GetMousePosition();
    float dx = mousePos.x - SCREEN_WIDTH/2;
    float dy = mousePos.y - SCREEN_HEIGHT/2;
    return atan2f(dy, dx);
}

float rotateBoss(Enemy* escort,Vector2 playerPos)
{
    float dx = playerPos.x - escort->pos.x ;
    float dy = playerPos.y - escort->pos.y ;
    return atan2f(dy, dx);
}

void updateBoss(Enemy* escort, Vector2 playerPos)
{
    if(escort->health<0){escort->active=false;bosscount--;}
    if(escort->active == true){
    escort->dir.x = cos(rotateBoss(escort, playerPos)) + GetRandomValue(-0.2f,0.1f);
    escort->dir.y = sin(rotateBoss(escort, playerPos)) + GetRandomValue(-0.2f,0.1f);
    escort->pos.x += escort->dir.x * escort->speed;
    escort->pos.y += escort->dir.y * escort->speed;}
    
}

void hitormiss(Projectile bullets[],Vector2 gummies[])
{
    for(int i=0;i<bulletscount;i++)
    {
        for(int j=0;j<maxGummies;j++)
        if(CheckCollisionCircles(gummies[j],10,bullets[i].pos,10)){gummies[j].x = 10000;}
    }
}

float rotateDeg(Vector2 screenPlayerPos)
{
    Vector2 mousePos = GetMousePosition();
    float dx = mousePos.x - SCREEN_WIDTH/2;
    float dy = mousePos.y - SCREEN_HEIGHT/2;
    return atan2f(dy, dx) * (180/PI);
}

void shoot(Projectile bullets[], Vector2 playerPos)
{   
    
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
    bullets[bulletscount].pos.x = playerPos.x;
    bullets[bulletscount].pos.y = playerPos.y;
    bullets[bulletscount].dir.x = cos(rotate(playerPos)); 
    bullets[bulletscount].dir.y = sin(rotate(playerPos)); 
    bullets[bulletscount].speed = 0.3f;
    bullets[bulletscount].active = true;
    bulletscount++;}
}

void UpBullets(Projectile bullets[])
{
    for(int i=0;i<bulletscount;i++){
    bullets[i].pos.x += bullets[i].speed * bullets[i].dir.x;
    bullets[i].pos.y += bullets[i].speed * bullets[i].dir.y;}
}

void DrawBullets(Projectile bullets[], Vector2 cameraoffset)
{   
    for(int i=0;i<bulletscount;i++){
        if(bullets[i].active){
    DrawCircle(bullets[i].pos.x - cameraoffset.x,bullets[i].pos.y - cameraoffset.y,10,RED);}}
}

void DrawPlayer(Vector2 playerPos, Vector2 cameraoffset, Texture2D tank)
{
    float scale = 0.06f;
    playerPos.x -= cameraoffset.x;
    playerPos.y -= cameraoffset.y;
    Rectangle source = {0.0f,0.0f,(float)tank.width,(float)tank.height};
    Rectangle dest = { playerPos.x, playerPos.y, tank.width * scale, tank.height * scale };
    Vector2 origin = { (tank.width * scale) / 2.0f, (tank.height * scale) / 2.0f };
    float rotation = rotateDeg(playerPos);
    DrawTexturePro(tank,source,dest,origin,rotation,WHITE);
}

void DrawCircles(Vector2 gummies[], Vector2 cameraoffset)
{
    for(int i = 0; i < maxGummies; i++)
    {
    DrawCircle(gummies[i].x - cameraoffset.x,gummies[i].y - cameraoffset.y,10,BLACK);
    }
}

void DrawBoss(Enemy escort, Vector2 offset)
{
    if(escort.active)
    DrawCircle(escort.pos.x - offset.x, escort.pos.y - offset.y,50,BLACK);
}
void bossKill(Enemy escort,Vector2 playerPos,bool* gameover)
{
    if(CheckCollisionCircles(escort.pos,50,playerPos,20)){*gameover=true;}
    else *gameover = false;
}

int main()
{
    InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT,"");
    Vector2 playerPos = {SCREEN_WIDTH/2,SCREEN_HEIGHT/2};
     Vector2 gummies[maxGummies] = {0};
         Image tank = LoadImage("basetank.png");
    Texture2D ttank = LoadTextureFromImage(tank);
    UnloadImage(tank);
    Enemy ford = {0};
    bool gameover = false;
    for(int i=0;i<maxGummies;i++)
    {
        gummies[i].x= GetRandomValue(0,SCREEN_WIDTH);
        gummies[i].y= GetRandomValue(0,SCREEN_HEIGHT);
    }
    while(!WindowShouldClose() && gameover == false)
    {   
        Vector2 cameraoffset;
        cameraoffset.x = playerPos.x - SCREEN_WIDTH/2; 
        cameraoffset.y = playerPos.y - SCREEN_HEIGHT/2;
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawPlayer(playerPos, cameraoffset, ttank);
        mplayer(&playerPos);
        DrawCircles(gummies, cameraoffset);
        shoot(bullets, playerPos);
        UpBullets(bullets);
        DrawBullets(bullets, cameraoffset);
        hitormiss( bullets, gummies);
        spawnBoss(&ford);
        updateBoss(&ford, playerPos);
        hitBoss(&ford, bullets);
        DrawBoss(ford, cameraoffset);
        bossKill(ford,playerPos,&gameover);
        DrawHealthbar(ford, cameraoffset);
        EndDrawing();
    }
    return 0;
}