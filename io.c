#include "raylib.h"
#include <math.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_BULLETS 100
#define maxGummies 50

typedef struct {
    Vector2 pos;
    Vector2 dir;
    float speed;
    bool active;

} Projectile;

Projectile bullets[MAX_BULLETS];
int bulletscount = 0;

void mplayer(Vector2* playerPos)
{
    if(IsKeyDown(KEY_W))playerPos->y -= 0.08;
    if(IsKeyDown(KEY_S))playerPos->y += 0.08;
    if(IsKeyDown(KEY_A))playerPos->x -= 0.08;
    if(IsKeyDown(KEY_D))playerPos->x += 0.08;
}

float rotate(Vector2 screenPlayerPos)
{
    Vector2 mousePos = GetMousePosition();
    float dx = mousePos.x - SCREEN_WIDTH/2;
    float dy = mousePos.y - SCREEN_HEIGHT/2;
    return atan2f(dy, dx);
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
    bulletscount++;}
}

void UpBullets(Projectile bullets[])
{
    for(int i=0;i<MAX_BULLETS;i++){
    bullets[i].pos.x += bullets[i].speed * bullets[i].dir.x;
    bullets[i].pos.y += bullets[i].speed * bullets[i].dir.y;}
}

void DrawBullets(Projectile bullets[], Vector2 cameraoffset)
{   
    for(int i=0;i<MAX_BULLETS;i++){
    DrawCircle(bullets[i].pos.x - cameraoffset.x,bullets[i].pos.y - cameraoffset.y,10,RED);}
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

int main()
{
    InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT,"");
    Vector2 playerPos = {SCREEN_WIDTH/2,SCREEN_HEIGHT/2};
     Vector2 gummies[maxGummies] = {0};
         Image tank = LoadImage("basetank.png");
    Texture2D ttank = LoadTextureFromImage(tank);
    UnloadImage(tank);
    for(int i=0;i<maxGummies;i++)
    {
        gummies[i].x= GetRandomValue(0,SCREEN_WIDTH);
        gummies[i].y= GetRandomValue(0,SCREEN_HEIGHT);
    }
    while(!WindowShouldClose())
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
        EndDrawing();
    }
    return 0;
}