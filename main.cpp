#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <mmsystem.h>
#include <string.h>

#pragma comment(lib, "winmm.lib")

// ==================== GLOBAL CONSTANTS ====================
const int SCREEN_W = 1200;
const int SCREEN_H = 500;

// Shared game state
int lives = 3;
int score = 0;

// Stage management (-1=cover, 0=instructions, 1,2,4=stages)
int currentStage = -1;
enum GameState { PLAYING, WIN, GAMEOVER };
GameState globalGameState = PLAYING;

// Stage win flags
bool stageWinTriggered = false;

// Forward declarations
void initStage(int stage);
void displayStage(int stage);

// ==================== COVER PAGE ====================
namespace CoverPage {
    void drawRect(float x, float y, float w, float h, float r, float g, float b) {
        glColor3f(r, g, b);
        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
        glEnd();
    }

    void renderBitmapString(float x, float y, void *font, const char *string) {
        glColor3f(1, 1, 1);
        glRasterPos2f(x, y);
        for (const char* c = string; *c != '\0'; c++) {
            glutBitmapCharacter(font, *c);
        }
    }

    void display() {
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();

        drawRect(0, 0, SCREEN_W, SCREEN_H, 0.051f, 0.514f, 0.612f);

        renderBitmapString(140, 450, GLUT_BITMAP_TIMES_ROMAN_24,
            "American International University - Bangladesh (AIUB)");
        renderBitmapString(280, 410, GLUT_BITMAP_TIMES_ROMAN_24,
            "Course Name : Computer Graphics");
        renderBitmapString(180, 370, GLUT_BITMAP_TIMES_ROMAN_24,
            "Project Name : Heart and Gold Collecting Game");
        renderBitmapString(760, 370, GLUT_BITMAP_TIMES_ROMAN_24,
            "Section : [I]");

        renderBitmapString(300, 310, GLUT_BITMAP_TIMES_ROMAN_24,
            "Course Instructor : Mahfujur Rahman");
        renderBitmapString(330, 280, GLUT_BITMAP_TIMES_ROMAN_24,
            "Assistant Professor, Computer Science");

        // Table Header
        drawRect(120, 220, 400, 35, 0.09f, 0.184f, 0.859f);
        drawRect(540, 220, 250, 35, 0.09f, 0.184f, 0.859f);
        renderBitmapString(290, 232, GLUT_BITMAP_HELVETICA_18, "Name");
        renderBitmapString(640, 232, GLUT_BITMAP_HELVETICA_18, "ID");

        // Row 1
        drawRect(120, 180, 400, 32, 0.09f, 0.184f, 0.859f);
        drawRect(540, 180, 250, 32, 0.09f, 0.184f, 0.859f);
        renderBitmapString(145, 190, GLUT_BITMAP_TIMES_ROMAN_24, "Rashed Ahammod Bin Azam");
        renderBitmapString(600, 190, GLUT_BITMAP_TIMES_ROMAN_24, "23-51203-1");

        // Row 2
        drawRect(120, 145, 400, 32, 0.09f, 0.184f, 0.859f);
        drawRect(540, 145, 250, 32, 0.09f, 0.184f, 0.859f);
        renderBitmapString(145, 155, GLUT_BITMAP_TIMES_ROMAN_24, "Asifur Rahman");
        renderBitmapString(600, 155, GLUT_BITMAP_TIMES_ROMAN_24, "23-51204-1");

        // Row 3
        drawRect(120, 110, 400, 32, 0.09f, 0.184f, 0.859f);
        drawRect(540, 110, 250, 32, 0.09f, 0.184f, 0.859f);
        renderBitmapString(145, 120, GLUT_BITMAP_TIMES_ROMAN_24, "Nabil Mohammed Nasim Uddin");
        renderBitmapString(600, 120, GLUT_BITMAP_TIMES_ROMAN_24, "23-51206-1");

        // Row 4
        drawRect(120, 75, 400, 32, 0.09f, 0.184f, 0.859f);
        drawRect(540, 75, 250, 32, 0.09f, 0.184f, 0.859f);
        renderBitmapString(145, 85, GLUT_BITMAP_TIMES_ROMAN_24, "Israt Jahan Chowdhury");
        renderBitmapString(600, 85, GLUT_BITMAP_TIMES_ROMAN_24, "23-51219-1");

        // Row 5
        drawRect(120, 40, 400, 32, 0.09f, 0.184f, 0.859f);
        drawRect(540, 40, 250, 32, 0.09f, 0.184f, 0.859f);
        renderBitmapString(145, 50, GLUT_BITMAP_TIMES_ROMAN_24, "MD. Redwanul Haque");
        renderBitmapString(600, 50, GLUT_BITMAP_TIMES_ROMAN_24, "23-50057-1");

        renderBitmapString(930, 15, GLUT_BITMAP_HELVETICA_18, "Press Enter to Start");
    }
}

// ==================== INSTRUCTION PAGE ====================
namespace InstructionPage {
    void drawRect(float x, float y, float w, float h, float r, float g, float b) {
        glColor3f(r, g, b);
        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
        glEnd();
    }

    void renderBitmapString(float x, float y, void *font, const char *string) {
        glColor3f(1, 1, 1);
        glRasterPos2f(x, y);
        for (const char* c = string; *c != '\0'; c++) {
            glutBitmapCharacter(font, *c);
        }
    }

    void display() {
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();

        // Background
        drawRect(0, 0, SCREEN_W, SCREEN_H, 0.05f, 0.15f, 0.25f);

        // Headline
        renderBitmapString(420, 465, GLUT_BITMAP_TIMES_ROMAN_24, "Instruction of the Game");

        // ==================== STAGE 1 ====================
        renderBitmapString(50, 420, GLUT_BITMAP_HELVETICA_18, "Stage 1 (Green Hills):");
        renderBitmapString(80, 390, GLUT_BITMAP_HELVETICA_12, "RIGHT ARROW : Move Right");
        renderBitmapString(80, 370, GLUT_BITMAP_HELVETICA_12, "LEFT ARROW  : Move Left");
        renderBitmapString(80, 350, GLUT_BITMAP_HELVETICA_12, "UP ARROW    : Jump");

        // ==================== STAGE 2 ====================
        renderBitmapString(380, 420, GLUT_BITMAP_HELVETICA_18, "Stage 2 (Cave Challenge):");
        renderBitmapString(410, 390, GLUT_BITMAP_HELVETICA_12, "SPACE / ENTER : Start Game / Jump");
        renderBitmapString(410, 370, GLUT_BITMAP_HELVETICA_12, "LEFT ARROW    : Move Left");
        renderBitmapString(410, 350, GLUT_BITMAP_HELVETICA_12, "RIGHT ARROW   : Move Right");
        renderBitmapString(410, 330, GLUT_BITMAP_HELVETICA_12, "UP ARROW      : Jump");
        renderBitmapString(410, 310, GLUT_BITMAP_HELVETICA_12, "DOWN ARROW    : Crouch");
        renderBitmapString(410, 290, GLUT_BITMAP_HELVETICA_12, "MOUSE CLICK   : Jump");

        // ==================== STAGE 4 ====================
        renderBitmapString(760, 420, GLUT_BITMAP_HELVETICA_18, "Stage 4 (Desert Temple):");
        renderBitmapString(790, 390, GLUT_BITMAP_HELVETICA_12, "LEFT ARROW  : Move Left");
        renderBitmapString(790, 370, GLUT_BITMAP_HELVETICA_12, "RIGHT ARROW : Move Right");
        renderBitmapString(790, 350, GLUT_BITMAP_HELVETICA_12, "UP ARROW    : Climb Ladder / Jump");
        renderBitmapString(790, 330, GLUT_BITMAP_HELVETICA_12, "DOWN ARROW  : Climb Down Ladder");
        renderBitmapString(790, 310, GLUT_BITMAP_HELVETICA_12, "SPACE       : Jump");

        // ==================== GENERAL ====================
        renderBitmapString(50, 240, GLUT_BITMAP_HELVETICA_18, "General:");
        renderBitmapString(80, 210, GLUT_BITMAP_HELVETICA_12, "Collect coins to increase score");
        renderBitmapString(80, 190, GLUT_BITMAP_HELVETICA_12, "Collect hearts to gain extra lives/health");
        renderBitmapString(80, 170, GLUT_BITMAP_HELVETICA_12, "Collect keys to unlock doors/finish");
        renderBitmapString(80, 150, GLUT_BITMAP_HELVETICA_12, "Avoid enemies, spikes, lava, and swinging obstacles");
        renderBitmapString(80, 130, GLUT_BITMAP_HELVETICA_12, "Falling off platforms costs lives");
        renderBitmapString(80, 110, GLUT_BITMAP_HELVETICA_12, "Losing all lives = GAME OVER");
        renderBitmapString(80, 90, GLUT_BITMAP_HELVETICA_12, "ESC : Exit Game at any time");

        // ==================== WIN/LOSE ====================
        renderBitmapString(380, 240, GLUT_BITMAP_HELVETICA_18, "Win / Lose:");
        renderBitmapString(410, 210, GLUT_BITMAP_HELVETICA_12, "Game Over screen shows RETRY button (click to restart)");
        renderBitmapString(410, 190, GLUT_BITMAP_HELVETICA_12, "Press R to restart from beginning");
        renderBitmapString(410, 170, GLUT_BITMAP_HELVETICA_12, "Stage 4 win shows final score, click NEXT to exit");

        // Footer
        renderBitmapString(880, 20, GLUT_BITMAP_HELVETICA_18, "Press ENTER to Continue");
    }
}

// ==================== STAGE 1 (redwan) ====================
namespace Stage1 {
    float playerX = 100;
    float playerY = 28;
    float velocityY = 0.0f;
    float gravity = -0.2f;
    bool isJumping = false;
    bool coin1 = true, coin2 = true, coin3 = true, coin4 = true, coin5 = true;
    bool hasKey = false;
    bool showKey = true;

    float cloud1X = 280, cloud2X = 500, cloud3X = 800, cloud4X = 1050;
    float cloudSpeed1 = 0.2f, cloudSpeed2 = 0.15f, cloudSpeed3 = 0.18f, cloudSpeed4 = 0.1f;

    struct PlatformS1 { float x, y, w, h; };
    PlatformS1 platformsS1[] = {
        {0, 0, 240, 28},
        {190, 28, 50, 72},
        {240, 0, 45, 100},
        {340, 0, 60, 120},
        {480, 50, 50, 70},
        {620, 0, 50, 100},
        {670, 0, 110, 80},
        {850, 0, 50, 120},
        {970, 0, 150, 28}
    };
    int platformCountS1 = 9;

    void drawRectS1(float x, float y, float w, float h, float r, float g, float b) {
        glColor3f(r, g, b);
        glBegin(GL_QUADS);
        glVertex2f(x, y); glVertex2f(x + w, y);
        glVertex2f(x + w, y + h); glVertex2f(x, y + h);
        glEnd();
    }

    void drawCircleS1(float cx, float cy, float radius, float r, float g, float b) {
        glColor3f(r, g, b);
        int triangleAmount = 30;
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        for (int i = 0; i <= triangleAmount; i++) {
            float angle = 2.0f * 3.14159265358979323846 * i / triangleAmount;
            glVertex2f(cx + cos(angle) * radius, cy + sin(angle) * radius);
        }
        glEnd();
    }

    void drawKeyS1(float x, float y) {
        drawCircleS1(x, y, 6, 1.0f, 0.84f, 0.0f);
        drawRectS1(x, y - 2, 20, 4, 1.0f, 0.84f, 0.0f);
        drawRectS1(x + 14, y - 6, 3, 4, 1.0f, 0.84f, 0.0f);
        drawRectS1(x + 18, y - 6, 3, 4, 1.0f, 0.84f, 0.0f);
    }

    void drawTextS1(float x, float y, const char* text, void* font) {
        glColor3f(1, 1, 1);
        glRasterPos2f(x, y);
        for (int i = 0; text[i] != '\0'; i++)
            glutBitmapCharacter(font, text[i]);
    }

    void drawCloudS1(float x, float y) {
        drawCircleS1(x, y, 10, 1, 1, 1);
        drawCircleS1(x + 12, y + 5, 12, 1, 1, 1);
        drawCircleS1(x + 25, y, 10, 1, 1, 1);
    }

    void drawGrassPlatformS1(float x, float y, float w, float h) {
        drawRectS1(x, y, w, h, 0.55f, 0.35f, 0.15f);
        drawRectS1(x, y + h - 6, w, 6, 0.3f, 0.7f, 0.1f);
    }

    void drawConretePlatformS1(float x, float y, float w, float h) {
        drawRectS1(x, y, w, h, 0.51f, 0.51f, 0.486f);
    }

    void drawPlayerS1(float x, float y) {
        drawRectS1(x+4, y+8, 16, 20, 0.3f, 0.55f, 0.85f);
        drawCircleS1(x+12, y+34, 9, 0.95f, 0.78f, 0.55f);
        drawRectS1(x+3, y+38, 18, 5, 0.72f, 0.52f, 0.15f);
        drawRectS1(x+6, y+43, 12, 7, 0.72f, 0.52f, 0.15f);
        drawCircleS1(x+9, y+35, 1.5f, 0.1f, 0.1f, 0.1f);
        drawCircleS1(x+14, y+35, 1.5f, 0.1f, 0.1f, 0.1f);
        drawRectS1(x+4, y, 7, 9, 0.25f, 0.35f, 0.55f);
        drawRectS1(x+12, y, 7, 9, 0.25f, 0.35f, 0.55f);
        drawRectS1(x+3, y-4, 9, 5, 0.4f, 0.22f, 0.05f);
        drawRectS1(x+11, y-4, 9, 5, 0.4f, 0.22f, 0.05f);
        drawRectS1(x+20, y+14, 6, 12, 0.95f, 0.78f, 0.55f);
        drawRectS1(x-2, y+14, 6, 12, 0.95f, 0.78f, 0.55f);
    }

    void drawLadderS1(float x, float y, float height) {
        drawRectS1(x, y, 3, height, 0.6f, 0.4f, 0.15f);
        drawRectS1(x + 14, y, 3, height, 0.6f, 0.4f, 0.15f);
        for (float stepY = y + 10; stepY < y + height - 5; stepY += 15)
            drawRectS1(x + 3, stepY, 12, 3, 0.7f, 0.5f, 0.2f);
    }

    void drawTrianglesS1(float x, float y, float width, float height) {
        glColor3f(0.51f, 0.51f, 0.486f);
        for (int i = 0; i < 4; i++) {
            glBegin(GL_TRIANGLES);
            glVertex2f(x + i * width, y);
            glVertex2f(x + width + i * width, y);
            glVertex2f(x + width/2 + i * width, y + height);
            glEnd();
        }
    }

    void drawBushS1(float x, float y) {
        drawCircleS1(x + 10, y + 8, 10, 0.15f, 0.55f, 0.1f);
        drawCircleS1(x + 20, y + 6, 8, 0.15f, 0.55f, 0.1f);
        drawCircleS1(x, y + 5, 7, 0.15f, 0.55f, 0.1f);
    }

    void drawCrateS1(float x, float y) {
        drawRectS1(x, y, 25, 25, 0.5f, 0.3f, 0.1f);
        glColor3f(0.3f, 0.2f, 0.05f);
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(x, y); glVertex2f(x+25, y);
        glVertex2f(x+25, y+25); glVertex2f(x, y+25);
        glEnd();
        glBegin(GL_LINES);
        glVertex2f(x, y); glVertex2f(x+25, y+25);
        glVertex2f(x+25, y); glVertex2f(x, y+25);
        glEnd();
    }

    void drawSnailS1(float x, float y) {
        drawCircleS1(x + 10, y + 10, 11, 0.5f, 0.3f, 0.7f);
        drawRectS1(x, y, 22, 5, 0.95f, 0.85f, 0.6f);
        drawCircleS1(x + 22, y + 8, 3, 0.95f, 0.85f, 0.6f);
        glColor3f(0, 0, 0);
        drawCircleS1(x + 23, y + 8, 1, 0, 0, 0);
    }

    void display() {
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
        drawRectS1(0, 0, SCREEN_W, SCREEN_H, 0.53f, 0.81f, 0.98f);
        drawCloudS1(cloud1X, 270);
        drawCloudS1(cloud2X, 280);
        drawCloudS1(cloud3X, 275);
        drawCloudS1(cloud4X, 280);

        drawGrassPlatformS1(0, 0, 240, 28);
        drawGrassPlatformS1(970, 0, 150, 28);
        drawRectS1(25, 28, 5, 20, 0.4f, 0.2f, 0.1f);
        drawRectS1(10, 48, 53, 18, 0.45f, 0.25f, 0.1f);
        drawTextS1(15, 52, "START", GLUT_BITMAP_HELVETICA_12);

        drawBushS1(140, 28);
        drawGrassPlatformS1(190, 28, 50, 72);
        drawGrassPlatformS1(240, 0, 45, 100);
        if (coin1) drawCircleS1(215, 114, 8, 1, 1, 0);
        if (coin2) drawCircleS1(235, 114, 8, 1, 1, 0);
        if (coin3) drawCircleS1(255, 114, 8, 1, 1, 0);
        drawLadderS1(320, 18, 100);
        drawGrassPlatformS1(340, 0, 60, 120);
        drawCrateS1(358, 120);
        drawConretePlatformS1(400, 0, 80, 15);
        drawTrianglesS1(400, 15, 20, 18);
        drawGrassPlatformS1(480, 50, 50, 70);
        drawLadderS1(600, 0, 100);
        drawGrassPlatformS1(620, 0, 50, 100);
        if (coin4) drawCircleS1(630, 114, 8, 1, 1, 0);
        if (coin5) drawCircleS1(650, 114, 8, 1, 1, 0);
        drawGrassPlatformS1(670, 0, 110, 80);
        drawSnailS1(680, 80);
        if (showKey) drawKeyS1(865, 145);
        drawGrassPlatformS1(850, 0, 50, 120);
        drawConretePlatformS1(898, 0, 73, 15);
        drawTrianglesS1(898, 15, 18, 16);
        drawRectS1(1050, 28, 5, 50, 0.4f, 0.2f, 0.1f);
        drawRectS1(1020, 68, 60, 18, 0.45f, 0.25f, 0.1f);
        drawTextS1(1030, 72, "FINISH", GLUT_BITMAP_HELVETICA_12);
        drawPlayerS1(playerX, playerY);

        char buf[50];
        sprintf(buf, "Score: %d", score);
        drawTextS1(200, SCREEN_H - 18, buf, GLUT_BITMAP_HELVETICA_18);
        sprintf(buf, "Lives: %d", lives);
        drawTextS1(320, SCREEN_H - 18, buf, GLUT_BITMAP_HELVETICA_18);
        drawTextS1(450, SCREEN_H - 18, hasKey ? "KEY: YES" : "KEY: NO", GLUT_BITMAP_HELVETICA_18);
        drawRectS1(10, SCREEN_H - 25, 150, 20, 0.05f, 0.3f, 0.05f);
        drawTextS1(15, SCREEN_H - 18, "STAGE 1 : GREEN HILLS", GLUT_BITMAP_HELVETICA_12);
    }

    void update() {
        if (globalGameState != PLAYING) return;

        cloud1X += cloudSpeed1;        cloud2X += cloudSpeed2;
        cloud3X += cloudSpeed3;        cloud4X += cloudSpeed4;
        if (cloud1X > 350 || cloud1X < 200) cloudSpeed1 = -cloudSpeed1;
        if (cloud2X > 600 || cloud2X < 420) cloudSpeed2 = -cloudSpeed2;
        if (cloud3X > 900 || cloud3X < 700) cloudSpeed3 = -cloudSpeed3;
        if (cloud4X > 1120 || cloud4X < 950) cloudSpeed4 = -cloudSpeed4;

        float prevY = playerY;
        velocityY += gravity;
        playerY += velocityY;
        bool onPlatform = false;

        for (int i = 0; i < platformCountS1; i++) {
            PlatformS1& p = platformsS1[i];
            if (playerX + 18 > p.x && playerX + 2 < p.x + p.w) {
                float platTop = p.y + p.h;
                if (prevY >= platTop && playerY <= platTop && velocityY <= 0) {
                    playerY = platTop;
                    velocityY = 0;
                    isJumping = false;
                    onPlatform = true;
                }
            }
        }

        if (coin1 && fabs((playerX + 6) - 215) < 8 && fabs((playerY + 20) - 114) < 10) { coin1 = false; score++; }
        if (coin2 && fabs((playerX + 12) - 235) < 8 && fabs((playerY + 20) - 114) < 10) { coin2 = false; score++; }
        if (coin3 && fabs((playerX + 12) - 255) < 8 && fabs((playerY + 20) - 114) < 10) { coin3 = false; score++; }
        if (coin4 && fabs((playerX + 12) - 630) < 8 && fabs((playerY + 20) - 114) < 10) { coin4 = false; score++; }
        if (coin5 && fabs((playerX + 12) - 650) < 8 && fabs((playerY + 20) - 114) < 10) { coin5 = false; score++; }
        if (showKey && fabs((playerX + 12) - 865) < 15 && fabs((playerY + 20) - 145) < 15) { showKey = false; hasKey = true; }

        if (!onPlatform) isJumping = true;

        if (playerY < -50) {
            lives--;
            if (lives <= 0) { globalGameState = GAMEOVER; PlaySound(NULL, 0, 0); PlaySound(TEXT("gameoverbgm.wav"), NULL, SND_FILENAME | SND_ASYNC); return; }
            playerX = 100; playerY = 28; velocityY = 0; isJumping = false;
        }

        if (hasKey && playerX > 1020 && playerY >= 28) {
            stageWinTriggered = true;
        }
    }

    void init() {
        playerX = 100; playerY = 28; velocityY = 0; isJumping = false;
        coin1 = coin2 = coin3 = coin4 = coin5 = true;
        hasKey = false; showKey = true;
        cloud1X = 280; cloud2X = 500; cloud3X = 800; cloud4X = 1050;
        cloudSpeed1 = 0.2f; cloudSpeed2 = 0.15f; cloudSpeed3 = 0.18f; cloudSpeed4 = 0.1f;
        globalGameState = PLAYING;
        stageWinTriggered = false;
        PlaySound(TEXT("s.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    }

    void keyboard(unsigned char key, int x, int y) {
        if (key == 27) exit(0);
    }

    void special(int key, int x, int y) {
        if (key == GLUT_KEY_RIGHT) playerX += 8;
        if (key == GLUT_KEY_LEFT) playerX -= 8;
        if (key == GLUT_KEY_UP && !isJumping) { velocityY = 8.0f; isJumping = true; }
    }
}

// ==================== STAGE 2 (rashed) ====================
namespace Stage2 {
    float charX2 = 70.0f, charY2 = 110.0f, velY2 = 0.0f, velX2 = 0.0f;
    bool onGround2 = false, isCrouching2 = false;
    bool keyLeft2 = false, keyRight2 = false, keyJump2 = false, keyCrouch2 = false;
    int invincibleTimer2 = 0;
    bool coin12 = true, coin22 = true, coin32 = true, coin42 = true;
    bool heartCollected = false, keyCollected = false;
    int hasKey2 = 0;
    bool gameStarted2 = false;

    struct Plat2 { float x, y, w, h; };
    Plat2 platforms2[] = {
        {130, 120, 130, 100}, {380, 120, 100, 180}, {560, 120, 130, 160},
        {780, 120, 160, 140}, {1020, 120, 180, 180}
    };
    int numPlats2 = 5;

    float swingAngle2 = 0.0f;
    float batX2 = 320.0f, batDir2 = 1.0f;
    float coinBob2 = 0.0f;
    int frameCount2 = 0;

    const float GROUND_TOP2 = 110.0f;
    const float CHAR_W2 = 38.0f, CHAR_H2 = 65.0f;
    const float SWING_PX2 = 300.0f, SWING_PY2 = 370.0f, ROPE_L2 = 80.0f, PLANK_W2 = 70.0f, PLANK_H2 = 16.0f;

    void drawText2(float x, float y, const char* str, void* font = GLUT_BITMAP_HELVETICA_18) {
        glRasterPos2f(x, y);
        for (const char* c = str; *c; c++) glutBitmapCharacter(font, *c);
    }
    void drawRect2(float x, float y, float w, float h) {
        glBegin(GL_QUADS); glVertex2f(x,y); glVertex2f(x+w,y); glVertex2f(x+w,y+h); glVertex2f(x,y+h); glEnd();
    }
    void drawRectOutline2(float x, float y, float w, float h) {
        glBegin(GL_LINE_LOOP); glVertex2f(x,y); glVertex2f(x+w,y); glVertex2f(x+w,y+h); glVertex2f(x,y+h); glEnd();
    }
    void drawCircle2(float cx, float cy, float r, int segs=40) {
        glBegin(GL_POLYGON);
        for (int i=0; i<segs; i++) { float a=2.0f*3.14159f*i/segs; glVertex2f(cx+r*cos(a), cy+r*sin(a)); }
        glEnd();
    }
    void drawTriangle2(float x1,float y1,float x2,float y2,float x3,float y3){
        glBegin(GL_TRIANGLES); glVertex2f(x1,y1); glVertex2f(x2,y2); glVertex2f(x3,y3); glEnd();
    }

    void drawSky2() {
        glBegin(GL_QUADS);
        glColor3f(0.53f,0.81f,0.98f); glVertex2f(0,SCREEN_H); glVertex2f(SCREEN_W,SCREEN_H);
        glColor3f(0.75f,0.90f,1.0f); glVertex2f(SCREEN_W,0); glVertex2f(0,0);
        glEnd();
    }
    void drawCloud2(float x, float y) {
        glColor3f(1,1,1);
        drawCircle2(x,y,22); drawCircle2(x+22,y+8,28); drawCircle2(x+50,y,22); drawCircle2(x+25,y-5,18);
    }
    void drawStonePlatform2(float x, float y, float w, float h) {
        glColor3f(0.50f,0.50f,0.55f); drawRect2(x,y,w,h);
        glColor3f(0.30f,0.30f,0.33f); glLineWidth(1.0f);
        float bH=18, bW=36;
        for (float r=y; r<=y+h; r+=bH){ glBegin(GL_LINES); glVertex2f(x,r); glVertex2f(x+w,r); glEnd(); }
        int ri=0;
        for (float r=y; r<y+h; r+=bH, ri++){
            float off=(ri%2==0)?0:bW*0.5f;
            for(float c=x+off; c<=x+w; c+=bW){ glBegin(GL_LINES); glVertex2f(c,r); glVertex2f(c,r+bH); glEnd(); }
        }
        glColor3f(0.65f,0.65f,0.70f); glLineWidth(2);
        glBegin(GL_LINES); glVertex2f(x,y+h); glVertex2f(x+w,y+h); glEnd();
        glLineWidth(1);
    }
    void drawGround2() {
        drawStonePlatform2(0,0,SCREEN_W,110);
        glColor3f(0.20f,0.65f,0.20f); drawRect2(0,108,SCREEN_W,12);
        glColor3f(0.15f,0.55f,0.15f);
        for (float gx=5; gx<SCREEN_W; gx+=18) drawTriangle2(gx,120,gx+9,138,gx+18,120);
    }
    void drawPlatform2(float x,float y,float w,float h) {
        drawStonePlatform2(x,y,w,h);
        glColor3f(0.20f,0.65f,0.20f); drawRect2(x,y+h-4,w,8);
    }
    void drawStartSign2(float x, float y) {
        glColor3f(0.40f,0.22f,0.08f); drawRect2(x+13,y,8,60);
        glColor3f(0.55f,0.30f,0.10f); drawRect2(x,y+32,80,36);
        glColor3f(0.30f,0.15f,0.05f); drawRectOutline2(x,y+32,80,36);
        glColor3f(1,1,1); drawText2(x+8,y+45,"START",GLUT_BITMAP_HELVETICA_12);
    }
    void drawLockedDoor2(float x, float y) {
        glColor3f(0.40f,0.40f,0.45f);
        drawRect2(x-5,y,80,5); drawRect2(x-5,y,5,90); drawRect2(x+70,y,5,90);
        glColor3f(0.45f,0.25f,0.08f); drawRect2(x,y,70,90);
        glColor3f(0.35f,0.18f,0.05f);
        for(float py=y+15;py<y+90;py+=18){ glBegin(GL_LINES); glVertex2f(x,py); glVertex2f(x+70,py); glEnd(); }
        glColor3f(0.10f,0.08f,0.05f); drawCircle2(x+35,y+55,8); drawRect2(x+31,y+40,8,18);
        glColor3f(0.85f,0.65f,0); glLineWidth(2);
        for(int i=0;i<40;i++){
            float a1=2*3.14159f*i/40,a2=2*3.14159f*(i+1)/40;
            glBegin(GL_LINES); glVertex2f(x+35+8*cos(a1),y+55+8*sin(a1)); glVertex2f(x+35+8*cos(a2),y+55+8*sin(a2)); glEnd();
        }
        glLineWidth(1);
    }
    void drawOpenDoor2(float x, float y) {
        glColor3f(0.40f,0.40f,0.45f);
        drawRect2(x-5,y,80,5); drawRect2(x-5,y,5,90); drawRect2(x+70,y,5,90);
        glColor3f(0.15f,0.35f,0.15f); drawRect2(x,y,70,90);
        glColor3f(0.10f,0.25f,0.10f);
        for(float py=y+15;py<y+90;py+=18){ glBegin(GL_LINES); glVertex2f(x,py); glVertex2f(x+70,py); glEnd(); }
        glColor3f(1,1,0); drawText2(x+5,y+45,"OPEN!",GLUT_BITMAP_HELVETICA_12);
    }

    void drawCharacter2(float x, float y) {
        glColor3f(0.18f, 0.10f, 0.04f); drawRect2(x+4, y, 12, 12); drawRect2(x+22, y, 12, 12);
        glColor3f(0.30f, 0.16f, 0.06f); drawRect2(x+4, y+9, 12, 4); drawRect2(x+22, y+9, 12, 4);
        glColor3f(0.35f, 0.20f, 0.08f); drawRect2(x+5, y+1, 5, 3); drawRect2(x+23, y+1, 5, 3);
        glColor3f(0.20f, 0.35f, 0.70f); drawRect2(x+5, y+12, 12, 22); drawRect2(x+21, y+12, 12, 22);
        glColor3f(0.14f, 0.26f, 0.55f); drawRect2(x+5, y+12, 4, 22); drawRect2(x+21, y+12, 4, 22);
        glColor3f(0.28f, 0.14f, 0.04f); drawRect2(x+4, y+33, 30, 5);
        glColor3f(0.88f, 0.72f, 0.0f); drawRect2(x+15, y+34, 8, 4); drawRect2(x+16, y+35, 6, 2);
        glColor3f(0.85f, 0.22f, 0.22f); drawRect2(x+4, y+37, 30, 22);
        glColor3f(0.68f, 0.14f, 0.14f); drawRect2(x+4, y+37, 8, 22);
        glColor3f(0.93f, 0.76f, 0.58f); drawRect2(x+14, y+55, 10, 5);
        glColor3f(0.85f, 0.22f, 0.22f); drawRect2(x-3, y+37, 8, 18); drawRect2(x+33, y+37, 8, 18);
        glColor3f(0.68f, 0.14f, 0.14f); drawRect2(x-3, y+37, 3, 18); drawRect2(x+38, y+37, 3, 18);
        glColor3f(0.93f, 0.76f, 0.58f); drawCircle2(x+1, y+56, 5.5f); drawCircle2(x+37, y+56, 5.5f);
        glColor3f(0.82f, 0.65f, 0.48f); drawRect2(x-2, y+52, 6, 3); drawRect2(x+34, y+52, 6, 3);
        glColor3f(0.93f, 0.76f, 0.58f); drawRect2(x+14, y+59, 10, 7);
        drawCircle2(x+19, y+73, 13);
        glColor3f(0.88f, 0.70f, 0.53f); drawRect2(x+10, y+60, 18, 10); drawCircle2(x+19, y+60, 8);
        drawCircle2(x+7, y+71, 4); drawCircle2(x+31, y+71, 4);
        glColor3f(0.78f, 0.58f, 0.43f); drawCircle2(x+7, y+71, 2); drawCircle2(x+31, y+71, 2);
        glColor3f(0.22f, 0.12f, 0.04f); drawCircle2(x+19, y+82, 12); drawRect2(x+7, y+78, 24, 8);
        drawRect2(x+7, y+66, 5, 14); drawRect2(x+26, y+66, 5, 14);
        glColor3f(0.40f, 0.24f, 0.08f); drawRect2(x+16, y+79, 5, 7);
        glColor3f(1.0f, 1.0f, 1.0f); drawCircle2(x+14, y+72, 4.5f); drawCircle2(x+24, y+72, 4.5f);
        glColor3f(0.15f, 0.45f, 0.85f); drawCircle2(x+14, y+72, 3.0f); drawCircle2(x+24, y+72, 3.0f);
        glColor3f(0.05f, 0.05f, 0.05f); drawCircle2(x+14, y+72, 1.5f); drawCircle2(x+24, y+72, 1.5f);
        glColor3f(1,1,1); drawCircle2(x+15, y+73, 0.9f); drawCircle2(x+25, y+73, 0.9f);
        glColor3f(0.18f, 0.10f, 0.04f); glLineWidth(2.2f);
        glBegin(GL_LINES); glVertex2f(x+10, y+77); glVertex2f(x+18, y+78); glVertex2f(x+20, y+78); glVertex2f(x+28, y+77); glEnd();
        glLineWidth(1.0f);
        glColor3f(0.80f, 0.60f, 0.44f); drawCircle2(x+19, y+69, 2.2f);
        glColor3f(0.72f, 0.52f, 0.38f); drawCircle2(x+16, y+68, 1.5f); drawCircle2(x+22, y+68, 1.5f);
        glColor3f(0.60f, 0.25f, 0.20f); glLineWidth(1.8f);
        glBegin(GL_LINE_STRIP);
        for(int i=0;i<=8;i++){ float a=3.14159f+3.14159f*i/8; glVertex2f(x+19+5*cos(a), y+64+2.0f*sin(a)); }
        glEnd(); glLineWidth(1.0f);
        glColor3f(0.15f, 0.32f, 0.62f); drawRect2(x+6, y+80, 26, 8); drawRect2(x+10, y+88, 18, 10);
        glColor3f(0.88f, 0.72f, 0.0f); drawRect2(x+10, y+88, 18, 3);
        glColor3f(0.22f, 0.44f, 0.80f); drawRect2(x+12, y+91, 6, 6);
    }

    void drawBat2(float x, float y) {
        glPushMatrix(); glTranslatef(x,y,0);
        glColor3f(0.15f,0.10f,0.20f); drawCircle2(0,0,10);
        glColor3f(0.9f,0.1f,0.1f); drawCircle2(-4,3,2); drawCircle2(4,3,2);
        glColor3f(0.20f,0.10f,0.30f);
        glBegin(GL_POLYGON);
        glVertex2f(-10,5); glVertex2f(-38,18); glVertex2f(-28,2); glVertex2f(-38,-8); glVertex2f(-10,-5);
        glEnd();
        glBegin(GL_POLYGON);
        glVertex2f(10,5); glVertex2f(38,18); glVertex2f(28,2); glVertex2f(38,-8); glVertex2f(10,-5);
        glEnd();
        glColor3f(0.35f,0.15f,0.45f);
        glBegin(GL_LINES);
        glVertex2f(-10,0); glVertex2f(-38,5); glVertex2f(-10,0); glVertex2f(-32,-6);
        glVertex2f(10,0); glVertex2f(38,5); glVertex2f(10,0); glVertex2f(32,-6);
        glEnd();
        glPopMatrix();
    }

    void drawSwing2(float pivotX, float pivotY, float angle) {
        float rad = angle * 3.14159f / 180.0f;
        float plankCX = pivotX + ROPE_L2 * sin(rad);
        float plankCY = pivotY - ROPE_L2 * cos(rad);
        glColor3f(0.15f,0.10f,0.05f); drawCircle2(pivotX, pivotY, 5);
        glColor3f(0.30f,0.20f,0.08f); glLineWidth(3.0f);
        glBegin(GL_LINES); glVertex2f(pivotX, pivotY); glVertex2f(plankCX, plankCY); glEnd(); glLineWidth(1);
        glPushMatrix(); glTranslatef(plankCX, plankCY, 0); glRotatef(angle, 0, 0, 1);
        glColor3f(0.55f,0.32f,0.10f); drawRect2(-PLANK_W2/2, -PLANK_H2/2, PLANK_W2, PLANK_H2);
        glColor3f(0.40f,0.22f,0.06f);
        glBegin(GL_LINES);
        glVertex2f(-PLANK_W2/2+8, -PLANK_H2/2); glVertex2f(-PLANK_W2/2+8, PLANK_H2/2);
        glVertex2f(-PLANK_W2/2+20, -PLANK_H2/2); glVertex2f(-PLANK_W2/2+20, PLANK_H2/2);
        glVertex2f(-PLANK_W2/2+32, -PLANK_H2/2); glVertex2f(-PLANK_W2/2+32, PLANK_H2/2);
        glEnd();
        glPopMatrix();
    }

    void drawLava2(float x, float y, float w) {
        glColor3f(0.95f,0.30f,0.0f); drawRect2(x,y,w,22);
        glColor3f(1.0f,0.60f,0.0f); drawRect2(x,y+14,w,8);
        glColor3f(1.0f,0.45f,0.0f);
        for(float bx=x+10;bx<x+w-10;bx+=25) drawCircle2(bx,y+20,8);
    }

    void drawCoin2(float x, float y, float bob=0) {
        glPushMatrix(); glTranslatef(x,y+bob,0);
        glColor3f(0.90f,0.72f,0.0f); drawCircle2(0,0,13);
        glColor3f(1.0f,0.88f,0.20f); drawCircle2(0,0,9);
        glColor3f(1.0f,1.0f,0.70f); drawCircle2(-3,3,3);
        glPopMatrix();
    }

    void drawHeart2(float x, float y) {
        glColor3f(0.90f,0.10f,0.20f);
        drawCircle2(x-9,y+6,10); drawCircle2(x+9,y+6,10);
        drawTriangle2(x-18,y+6,x+18,y+6,x,y-12);
        glColor3f(1.0f,0.50f,0.55f); drawCircle2(x-6,y+10,4);
    }

    void drawCrate2(float x, float y, float s) {
        glColor3f(0.55f,0.32f,0.10f); drawRect2(x,y,s,s);
        glColor3f(0.35f,0.18f,0.05f); drawRectOutline2(x,y,s,s);
        glBegin(GL_LINES);
        glVertex2f(x,y); glVertex2f(x+s,y+s); glVertex2f(x+s,y); glVertex2f(x,y+s);
        glVertex2f(x,y+s/2); glVertex2f(x+s,y+s/2); glVertex2f(x+s/2,y); glVertex2f(x+s/2,y+s);
        glEnd();
    }

    void drawSlime2(float x, float y) {
        glColor3f(0.20f,0.75f,0.20f); drawCircle2(x,y+10,20);
        glColor3f(0.15f,0.65f,0.15f); drawRect2(x-20,y,40,12);
        glColor3f(1,1,1); drawCircle2(x-7,y+16,6); drawCircle2(x+7,y+16,6);
        glColor3f(0.1f,0.1f,0.1f); drawCircle2(x-7,y+15,3); drawCircle2(x+7,y+15,3);
        glColor3f(0.08f,0.45f,0.08f);
        glBegin(GL_LINE_STRIP);
        for(int i=0;i<=8;i++){ float a=3.14159f+3.14159f*i/8; glVertex2f(x+8*cos(a),y+9+4*sin(a)); }
        glEnd();
        glColor3f(0.25f,0.80f,0.25f); drawTriangle2(x-5,y+2,x+5,y+2,x,y-8);
    }

    void drawSpikes2(float x, float y, int count) {
        glColor3f(0.70f,0.70f,0.75f);
        float sw=18;
        for(int i=0;i<count;i++){
            float sx=x+i*sw;
            drawTriangle2(sx,y,sx+sw,y,sx+sw/2,y+32);
            glColor3f(0.88f,0.88f,0.92f);
            glBegin(GL_LINES); glVertex2f(sx+sw/2-2,y+5); glVertex2f(sx+sw/2,y+28); glEnd();
            glColor3f(0.70f,0.70f,0.75f);
        }
    }

    void drawKey2(float x, float y) {
        glColor3f(0.88f,0.70f,0.0f);
        for(int i=0;i<40;i++){
            float a1=2*3.14159f*i/40, a2=2*3.14159f*(i+1)/40;
            glBegin(GL_TRIANGLES); glVertex2f(x,y+14); glVertex2f(x+11*cos(a1),y+14+11*sin(a1)); glVertex2f(x+11*cos(a2),y+14+11*sin(a2)); glEnd();
        }
        glColor3f(0.53f,0.81f,0.98f); drawCircle2(x,y+14,5);
        glColor3f(0.88f,0.70f,0.0f); drawRect2(x-4,y-16,8,28); drawRect2(x+4,y-14,7,5); drawRect2(x+4,y-5,5,5);
    }

    void drawFlag2(float x, float y) {
        glColor3f(0.50f,0.50f,0.55f); glLineWidth(2.5f);
        glBegin(GL_LINES); glVertex2f(x,y); glVertex2f(x,y+70); glEnd(); glLineWidth(1);
        glColor3f(0.90f,0.10f,0.10f);
        glBegin(GL_TRIANGLES); glVertex2f(x,y+70); glVertex2f(x+30,y+58); glVertex2f(x,y+46); glEnd();
    }

    void drawHUD2() {
        glColor3f(0.10f,0.40f,0.10f); drawRect2(10,SCREEN_H-50,260,38);
        glColor3f(0.05f,0.25f,0.05f); drawRectOutline2(10,SCREEN_H-50,260,38);
        glColor3f(1,1,1); drawText2(18,SCREEN_H-26,"STAGE 2 : CAVE CHALLENGE",GLUT_BITMAP_HELVETICA_12);
        char buf[64];
        sprintf(buf,"SCORE : %d", score);
        glColor3f(0.10f,0.40f,0.10f); drawRect2(SCREEN_W-250,SCREEN_H-55,120,38);
        glColor3f(1,1,1); drawText2(SCREEN_W-242,SCREEN_H-30,buf,GLUT_BITMAP_HELVETICA_12);
        sprintf(buf,"LIFE : %d", lives);
        glColor3f(0.10f,0.40f,0.10f); drawRect2(SCREEN_W-120,SCREEN_H-55,115,38);
        glColor3f(1,1,1); drawText2(SCREEN_W-112,SCREEN_H-30,buf,GLUT_BITMAP_HELVETICA_12);
        sprintf(buf,"KEY : %s", hasKey2?"YES":"NO");
        glColor3f(0.10f,0.40f,0.10f); drawRect2(SCREEN_W-250,SCREEN_H-103,120,38);
        glColor3f(1,1,1); drawText2(SCREEN_W-242,SCREEN_H-78,buf,GLUT_BITMAP_HELVETICA_12);
        glColor3f(0.2f,0.2f,0.2f);
        drawText2(10,15,"Arrow Keys: Move/Crouch(Down) | Space/Up: Jump | ESC: Quit",GLUT_BITMAP_HELVETICA_12);
    }

    struct Rect2 { float x,y,w,h; };
    Rect2 getSwingPlankRect2() {
        float rad = swingAngle2 * 3.14159f / 180.0f;
        float plankCX = SWING_PX2 + ROPE_L2 * sin(rad);
        float plankCY = SWING_PY2 - ROPE_L2 * cos(rad);
        return {plankCX - PLANK_W2/2 - 4, plankCY - PLANK_H2/2, PLANK_W2 + 8, PLANK_H2 + 4};
    }

    bool rectsOverlap2(float ax,float ay,float aw,float ah, float bx,float by,float bw,float bh) {
        return ax<bx+bw && ax+aw>bx && ay<by+bh && ay+ah>by;
    }

    void hitPlayer2() {
        if(invincibleTimer2>0) return;
        lives--;
        invincibleTimer2 = 120;
        if(lives <= 0) { globalGameState = GAMEOVER; PlaySound(NULL,0,0); PlaySound(TEXT("gameoverbgm.wav"), NULL, SND_FILENAME|SND_ASYNC); return; }
        charX2=70; charY2=GROUND_TOP2; velX2=0; velY2=0;
    }

    void mouse(int button, int state, int x, int y) {
        if (globalGameState != PLAYING) return;
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            if (!gameStarted2) {
                gameStarted2 = true;
                return;
            }
            if (onGround2) {
                keyJump2 = true;
            }
        }
        if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
            keyJump2 = false;
        }
    }

    void update2() {
        if (globalGameState != PLAYING) return;

        frameCount2++;
        float swingAmplitude = 28.0f;
        float swingSpeed     = 0.022f;
        swingAngle2 = swingAmplitude * sin(frameCount2 * swingSpeed);

        batX2 += batDir2 * 1.8f;
        if (batX2 > 520) batDir2 = -1;
        if (batX2 < 270) batDir2 =  1;

        coinBob2 = 5.0f * sin(frameCount2 * 0.08f);

        if (!gameStarted2) return;

        const float GRAVITY = -0.55f, JUMP_VEL = 12.0f, MOVE_SPD = 3.8f;
        isCrouching2 = (keyCrouch2 && onGround2);
        float speed = isCrouching2 ? MOVE_SPD*0.5f : MOVE_SPD;
        velX2 = 0;
        if(keyLeft2 && charX2 > 0) velX2 = -speed;
        if(keyRight2 && charX2 < SCREEN_W-CHAR_W2) velX2 = speed;
        if(keyJump2 && onGround2) { velY2 = JUMP_VEL; onGround2 = false; keyJump2 = false; }
        velY2 += GRAVITY;
        charX2 += velX2; charY2 += velY2;
        onGround2 = false;
        if(charY2 <= GROUND_TOP2) { charY2 = GROUND_TOP2; velY2 = 0; onGround2 = true; }
        for(int i=0;i<numPlats2;i++) {
            Plat2& p = platforms2[i];
            if(charX2+CHAR_W2 > p.x && charX2 < p.x+p.w) {
                float pTop = p.y+p.h;
                if(velY2<=0 && charY2 <= pTop && charY2 >= pTop-20) { charY2 = pTop; velY2 = 0; onGround2 = true; }
                else if(charY2+CHAR_H2 > p.y && charY2 < pTop) {
                    if(velX2>0 && charX2+CHAR_W2 > p.x && charX2 < p.x) charX2 = p.x - CHAR_W2;
                    if(velX2<0 && charX2 < p.x+p.w && charX2+CHAR_W2 > p.x+p.w) charX2 = p.x+p.w;
                }
            }
        }
        Rect2 sr2 = getSwingPlankRect2();
        if(charX2+CHAR_W2/2 > sr2.x && charX2+CHAR_W2/2 < sr2.x+sr2.w)
            if(velY2<=0 && charY2 <= sr2.y+sr2.h+4 && charY2 >= sr2.y-20) { charY2 = sr2.y+sr2.h; velY2 = 0; onGround2 = true; }
        if(rectsOverlap2(charX2,charY2,CHAR_W2,CHAR_H2, 480,200,80,30)) { hitPlayer2(); return; }
        if(rectsOverlap2(charX2,charY2,CHAR_W2,CHAR_H2, 960,110,72,32)) { hitPlayer2(); return; }
        if(rectsOverlap2(charX2,charY2,CHAR_W2,CHAR_H2, 700,110,54,32)) { hitPlayer2(); return; }
        if(rectsOverlap2(charX2,charY2,CHAR_W2,CHAR_H2, batX2-38,430-10,76,20)) { hitPlayer2(); return; }
        float slimeFrontX = 840-20, slimeFrontW=25, slimeTopY=platforms2[3].y+platforms2[3].h;
        if(rectsOverlap2(charX2,charY2,CHAR_W2,CHAR_H2, slimeFrontX, slimeTopY, slimeFrontW, 30) ||
           rectsOverlap2(charX2,charY2,CHAR_W2,CHAR_H2, 840-20, slimeTopY+20, 40, 10)) { hitPlayer2(); return; }

        float p1top = platforms2[0].y+platforms2[0].h;
        if(coin12 && rectsOverlap2(charX2,charY2,CHAR_W2,CHAR_H2, 155-13,p1top+60+coinBob2,26,26)) { coin12=false; score+=10; PlaySound(TEXT("coinbgm.wav"),NULL,SND_ASYNC); }
        if(coin22 && rectsOverlap2(charX2,charY2,CHAR_W2,CHAR_H2, 200-13,p1top+60+coinBob2,26,26)) { coin22=false; score+=10; PlaySound(TEXT("coinbgm.wav"),NULL,SND_ASYNC); }
        float p4top = platforms2[3].y+platforms2[3].h;
        if(coin32 && rectsOverlap2(charX2,charY2,CHAR_W2,CHAR_H2, 810-13,p4top+40+coinBob2,26,26)) { coin32=false; score+=10; PlaySound(TEXT("coinbgm.wav"),NULL,SND_ASYNC); }
        if(coin42 && rectsOverlap2(charX2,charY2,CHAR_W2,CHAR_H2, 910-13,p4top+40+coinBob2,26,26)) { coin42=false; score+=10; PlaySound(TEXT("coinbgm.wav"),NULL,SND_ASYNC); }
        float p3top = platforms2[2].y+platforms2[2].h;
        if(!heartCollected && rectsOverlap2(charX2,charY2,CHAR_W2,CHAR_H2, 620-18,p3top+48,36,24)) { heartCollected=true; lives++; score+=20; }
        float p5top = platforms2[4].y+platforms2[4].h;
        if(!keyCollected && rectsOverlap2(charX2,charY2,CHAR_W2,CHAR_H2, 1090-11,p5top+20,22,30)) { keyCollected=true; hasKey2=1; score+=30; }
        if(hasKey2 && rectsOverlap2(charX2,charY2,CHAR_W2,CHAR_H2, 1060,platforms2[4].y+platforms2[4].h,70,90)) {
            stageWinTriggered = true;
        }
        if(invincibleTimer2>0) invincibleTimer2--;
        if(charY2 < -100) { hitPlayer2(); }
    }

    void display() {
        glClear(GL_COLOR_BUFFER_BIT);
        drawSky2();
        drawCloud2(520,450); drawCloud2(780,470); drawCloud2(980,450);
        drawGround2();
        glColor3f(0.15f,0.55f,0.15f); drawCircle2(870,145,28); drawCircle2(895,148,22); drawCircle2(855,142,18);
        for(int i=0;i<numPlats2;i++) drawPlatform2(platforms2[i].x, platforms2[i].y, platforms2[i].w, platforms2[i].h);
        drawLava2(480,120,80);
        drawSpikes2(960,110,4);
        drawSpikes2(700,110,3);
        drawSwing2(SWING_PX2, SWING_PY2, swingAngle2);
        float p1top=platforms2[0].y+platforms2[0].h, p4top=platforms2[3].y+platforms2[3].h;
        float p3top=platforms2[2].y+platforms2[2].h, p5top=platforms2[4].y+platforms2[4].h;
        if(coin12) drawCoin2(155, p1top+60, coinBob2);
        if(coin22) drawCoin2(200, p1top+60, coinBob2);
        if(!heartCollected) drawHeart2(620, p3top+48);
        if(coin32) drawCoin2(810, p4top+40, coinBob2);
        if(coin42) drawCoin2(910, p4top+40, coinBob2);
        if(!keyCollected) drawKey2(1090, p5top+20);
        drawBat2(batX2, 430);
        drawSlime2(850, p4top);
        if(hasKey2) drawOpenDoor2(1060, platforms2[4].y+platforms2[4].h);
        else drawLockedDoor2(1060, platforms2[4].y+platforms2[4].h);
        drawStartSign2(10, GROUND_TOP2);
        drawFlag2(1145, platforms2[4].y+platforms2[4].h);
        drawCharacter2(charX2, charY2);
        drawHUD2();

        if (!gameStarted2) {
            glColor4f(0,0,0.2f,0.85f);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
            drawRect2(0,0,SCREEN_W,SCREEN_H);
            glDisable(GL_BLEND);

            glColor3f(1,1,0.3f);
            drawText2(SCREEN_W/2-200,SCREEN_H/2+80,"STAGE 2 : CAVE CHALLENGE",GLUT_BITMAP_TIMES_ROMAN_24);
            glColor3f(1,1,1);
            drawText2(SCREEN_W/2-190,SCREEN_H/2+20,"Press ENTER or SPACE to Start",GLUT_BITMAP_HELVETICA_18);
            glColor3f(0.8f,0.9f,1.0f);
            drawText2(SCREEN_W/2-210,SCREEN_H/2-30,"Arrow Keys: Move | Down: Crouch | Space/Up: Jump",GLUT_BITMAP_HELVETICA_12);
            drawText2(SCREEN_W/2-200,SCREEN_H/2-55,"Collect coins, key, avoid enemies!",GLUT_BITMAP_HELVETICA_12);
        }
    }

    void init() {
        charX2=70; charY2=GROUND_TOP2; velX2=0; velY2=0; onGround2=false;
        coin12=coin22=coin32=coin42=true; heartCollected=false; keyCollected=false; hasKey2=0;
        invincibleTimer2=0; swingAngle2=0.0f; frameCount2=0; coinBob2=0.0f;
        batX2=320.0f; batDir2=1.0f;
        keyCrouch2=false; keyJump2=false; keyLeft2=false; keyRight2=false;
        gameStarted2 = false;
        globalGameState = PLAYING;
        stageWinTriggered = false;
        PlaySound(TEXT("background.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    }

    void keyboard(unsigned char key, int x, int y) {
        if (key == 27) exit(0);
        if ((key == ' ' || key == 13) && !gameStarted2) { gameStarted2 = true; return; }
        if (key == ' ') keyJump2 = true;
    }

    void keyboardUp(unsigned char key, int x, int y) {
        if (key == ' ') keyJump2 = false;
    }

    void special(int key, int x, int y) {
        if (key == GLUT_KEY_LEFT) keyLeft2 = true;
        if (key == GLUT_KEY_RIGHT) keyRight2 = true;
        if (key == GLUT_KEY_UP) keyJump2 = true;
        if (key == GLUT_KEY_DOWN) keyCrouch2 = true;
    }

    void specialUp(int key, int x, int y) {
        if (key == GLUT_KEY_LEFT) keyLeft2 = false;
        if (key == GLUT_KEY_RIGHT) keyRight2 = false;
        if (key == GLUT_KEY_UP) keyJump2 = false;
        if (key == GLUT_KEY_DOWN) keyCrouch2 = false;
    }
}

// ==================== STAGE 4 (nabil) WITH FINAL SCORE ====================
namespace Stage4 {
    const float GROUND_Y = 55;
    const float GRAVITY = -0.6f;
    const float JUMP_VEL = 11.0f;

    bool keyUp = false, keyDown = false;
    struct Player { float x, y, w, h, velX, velY; int health; bool onGround, hasKey; } player;
    struct Coin { float x, y; bool collected; } coins[5];
    int coinCount = 5;
    struct Heart { float x, y; bool collected; } hearts[1];
    int heartCount = 1;
    struct Spike { float x, y, w, h; } spikes[2];
    int spikeCount = 2;
    struct Platform { float x, y, w, h; } platforms[5];
    int platformCount = 5;
    struct MovingPlatform { float x, y, w, h, dx; float left, right; } movingPlat;
    struct Mace { float pivotX, pivotY, length; float angle, angleSpeed; } mace;
    struct Scorpion { float x, y, w, h, velX; float left, right; } scorpion;
    struct Key { float x, y; bool collected; } key;

    bool gameWin = false, gameOver = false;
    bool hasKey = false;

    float ladderX = 450, ladderY = GROUND_Y, ladderH = 80;

    float retryBtnX, retryBtnY, retryBtnW = 120, retryBtnH = 30;
    float nextBtnX, nextBtnY, nextBtnW = 120, nextBtnH = 30;

    void drawBrick(float x, float y, float w, float h, int r, int g, int b) {
        glColor3ub(r,g,b);
        glBegin(GL_QUADS); glVertex2f(x,y); glVertex2f(x+w,y); glVertex2f(x+w,y+h); glVertex2f(x,y+h); glEnd();
        glColor3ub(r-40,g-40,b-40);
        glBegin(GL_LINE_LOOP); glVertex2f(x,y); glVertex2f(x+w,y); glVertex2f(x+w,y+h); glVertex2f(x,y+h); glEnd();
    }

    void drawGround() {
        glColor3ub(180,110,30);
        glBegin(GL_QUADS); glVertex2f(0,0); glVertex2f(SCREEN_W,0); glVertex2f(SCREEN_W,GROUND_Y); glVertex2f(0,GROUND_Y); glEnd();
        glColor3ub(205,133,63);
        glBegin(GL_QUADS); glVertex2f(0,GROUND_Y-10); glVertex2f(SCREEN_W,GROUND_Y-10); glVertex2f(SCREEN_W,GROUND_Y); glVertex2f(0,GROUND_Y); glEnd();
    }

    void drawPlatform(float x, float y, float w, float h) {
        drawBrick(x,y,w,h,205,133,63);
        glColor3ub(160,90,20);
        for(float by=y; by<y+h; by+=20) { glBegin(GL_LINES); glVertex2f(x,by); glVertex2f(x+w,by); glEnd(); }
        for(float bx=x; bx<x+w; bx+=20) { glBegin(GL_LINES); glVertex2f(bx,y); glVertex2f(bx,y+h); glEnd(); }
    }

    void drawMovingPlatform() { drawBrick(movingPlat.x, movingPlat.y, movingPlat.w, movingPlat.h, 139,69,19); }

    void drawLadder(float x, float y, float h) {
        glColor3ub(139,69,19);
        glBegin(GL_QUADS); glVertex2f(x,y); glVertex2f(x+4,y); glVertex2f(x+4,y+h); glVertex2f(x,y+h); glEnd();
        glBegin(GL_QUADS); glVertex2f(x+20,y); glVertex2f(x+24,y); glVertex2f(x+24,y+h); glVertex2f(x+20,y+h); glEnd();
        for(float rungY=y+10; rungY<y+h; rungY+=15) {
            glBegin(GL_QUADS); glVertex2f(x,rungY); glVertex2f(x+24,rungY); glVertex2f(x+24,rungY+4); glVertex2f(x,rungY+4); glEnd();
        }
    }

    void drawSpikes() {
        for(int i=0;i<spikeCount;i++) {
            float sx=spikes[i].x, sy=spikes[i].y, sw=spikes[i].w, sh=spikes[i].h;
            drawBrick(sx,sy,sw,10,60,60,60);
            glColor3ub(169,169,169);
            int n=sw/15;
            for(int j=0;j<n;j++) {
                float spikeX=sx+j*(sw/n);
                glBegin(GL_TRIANGLES);
                glVertex2f(spikeX,sy+10); glVertex2f(spikeX+sw/n,sy+10); glVertex2f(spikeX+sw/(2*n),sy+sh+10);
                glEnd();
            }
        }
    }

    void drawCoins() {
        for(int i=0;i<coinCount;i++) {
            if(coins[i].collected) continue;
            float cx=coins[i].x, cy=coins[i].y;
            glColor3ub(255,215,0);
            glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
            for(int j=0;j<=20;j++) { float a=j*2*3.1416/20; glVertex2f(cx+cos(a)*10,cy+sin(a)*14); }
            glEnd();
        }
    }

    void drawHearts() {
        for(int i=0;i<heartCount;i++) {
            if(hearts[i].collected) continue;
            float hx=hearts[i].x, hy=hearts[i].y;
            glColor3ub(220,20,20);
            glBegin(GL_POLYGON);
            glVertex2f(hx,hy-10); glVertex2f(hx+10,hy+5); glVertex2f(hx+5,hy+10);
            glVertex2f(hx,hy+5); glVertex2f(hx-5,hy+10); glVertex2f(hx-10,hy+5);
            glEnd();
        }
    }

    void drawMace() {
        drawBrick(625,GROUND_Y,15,110,139,69,19);
        drawBrick(625,155,50,15,139,69,19);
        float ballX = mace.pivotX + sin(mace.angle)*mace.length;
        float ballY = mace.pivotY - cos(mace.angle)*mace.length;
        glColor3ub(50,50,50); glLineWidth(2);
        glBegin(GL_LINES); glVertex2f(mace.pivotX,mace.pivotY); glVertex2f(ballX,ballY); glEnd();
        glColor3ub(80,80,80);
        glBegin(GL_TRIANGLE_FAN); glVertex2f(ballX,ballY);
        for(int i=0;i<=16;i++) { float a=i*2*3.1416/16; glVertex2f(ballX+cos(a)*16,ballY+sin(a)*16); }
        glEnd();
    }

    void drawScorpion() {
        float sx=scorpion.x, sy=scorpion.y;
        glColor3ub(178,34,34);
        glBegin(GL_QUADS); glVertex2f(sx,sy); glVertex2f(sx+scorpion.w,sy);
        glVertex2f(sx+scorpion.w,sy+scorpion.h); glVertex2f(sx,sy+scorpion.h); glEnd();
        glBegin(GL_QUADS); glVertex2f(sx+5,sy+scorpion.h); glVertex2f(sx+10,sy+scorpion.h);
        glVertex2f(sx+15,sy+scorpion.h+15); glVertex2f(sx+5,sy+scorpion.h+15); glEnd();
    }

    void drawCactus(float x, float y) {
        glColor3ub(34,139,34);
        glBegin(GL_QUADS); glVertex2f(x-8,y); glVertex2f(x+8,y); glVertex2f(x+8,y+40); glVertex2f(x-8,y+40); glEnd();
        glBegin(GL_QUADS); glVertex2f(x-18,y+15); glVertex2f(x-8,y+15); glVertex2f(x-8,y+25); glVertex2f(x-18,y+25); glEnd();
        glBegin(GL_QUADS); glVertex2f(x+8,y+10); glVertex2f(x+18,y+10); glVertex2f(x+18,y+20); glVertex2f(x+8,y+20); glEnd();
    }

    void drawPlayer() {
        if(gameOver || gameWin) return;
        float px=player.x, py=player.y, pw=player.w, ph=player.h;
        glColor3ub(139,69,19);
        glBegin(GL_QUADS); glVertex2f(px+5,py); glVertex2f(px+pw-5,py); glVertex2f(px+pw-5,py+15); glVertex2f(px+5,py+15); glEnd();
        glColor3ub(210,180,140);
        glBegin(GL_QUADS); glVertex2f(px+2,py+15); glVertex2f(px+pw-2,py+15); glVertex2f(px+pw-2,py+35); glVertex2f(px+2,py+35); glEnd();
        glColor3ub(255,222,173);
        glBegin(GL_QUADS); glVertex2f(px+5,py+35); glVertex2f(px+pw-5,py+35); glVertex2f(px+pw-5,py+45); glVertex2f(px+5,py+45); glEnd();
        glColor3ub(160,82,45);
        glBegin(GL_QUADS); glVertex2f(px-2,py+45); glVertex2f(px+pw+2,py+45); glVertex2f(px+pw-2,py+52); glVertex2f(px+2,py+52); glEnd();
    }

    void drawKey() {
        if(key.collected) return;
        float kx=key.x, ky=key.y;
        glColor3ub(255,215,0);
        glBegin(GL_QUADS); glVertex2f(kx,ky); glVertex2f(kx+15,ky); glVertex2f(kx+15,ky+4); glVertex2f(kx,ky+4); glEnd();
        glBegin(GL_QUADS); glVertex2f(kx-6,ky-4); glVertex2f(kx,ky-4); glVertex2f(kx,ky+8); glVertex2f(kx-6,ky+8); glEnd();
    }

    void drawTemple(float x, float y) {
        glColor3ub(205,133,63);
        for(int row=0;row<5;row++) {
            float rowW=120-row*20, rowX=x+(120-rowW)/2, rowY=y+row*20;
            glBegin(GL_QUADS); glVertex2f(rowX,rowY); glVertex2f(rowX+rowW,rowY); glVertex2f(rowX+rowW,rowY+20); glVertex2f(rowX,rowY+20); glEnd();
            glColor3ub(160,90,20);
            glBegin(GL_LINE_LOOP); glVertex2f(rowX,rowY); glVertex2f(rowX+rowW,rowY); glVertex2f(rowX+rowW,rowY+20); glVertex2f(rowX,rowY+20); glEnd();
            glColor3ub(205,133,63);
        }
        glColor3ub(30,20,10);
        glBegin(GL_QUADS); glVertex2f(x+45,y); glVertex2f(x+75,y); glVertex2f(x+70,y+40); glVertex2f(x+50,y+40); glEnd();
    }

    void drawPalmTree(float x, float y) {
        glColor3ub(139,69,19);
        glBegin(GL_QUADS); glVertex2f(x,y); glVertex2f(x+8,y); glVertex2f(x+8,y+60); glVertex2f(x,y+60); glEnd();
        glColor3ub(34,139,34);
        glBegin(GL_TRIANGLES);
        glVertex2f(x+4,y+55); glVertex2f(x-20,y+45); glVertex2f(x+4,y+75);
        glVertex2f(x+4,y+55); glVertex2f(x+28,y+45); glVertex2f(x+4,y+75);
        glEnd();
    }

    void drawFlag(float x, float y) {
        glColor3ub(139,69,19);
        glBegin(GL_QUADS); glVertex2f(x,y); glVertex2f(x+4,y); glVertex2f(x+4,y+60); glVertex2f(x,y+60); glEnd();
        glColor3ub(220,20,20);
        glBegin(GL_TRIANGLES); glVertex2f(x+4,y+40); glVertex2f(x+25,y+50); glVertex2f(x+4,y+60); glEnd();
    }

    void drawSign(float x, float y, const char* text) {
        drawBrick(x,y+20,60,30,139,69,19);
        drawBrick(x+25,y,10,20,101,67,33);
        glColor3f(1,1,1); glRasterPos2f(x+8,y+30);
        for(const char *c=text; *c!='\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }

    void drawUI() {
        glColor3ub(0,100,0);
        glBegin(GL_QUADS); glVertex2f(10,SCREEN_H-40); glVertex2f(290,SCREEN_H-40); glVertex2f(290,SCREEN_H-10); glVertex2f(10,SCREEN_H-10); glEnd();
        glColor3f(1,1,1);
        glRasterPos2f(20,SCREEN_H-30);
        for(const char *c="STAGE 4 : DESERT TEMPLE"; *c; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        char text[50];
        sprintf(text,"Health: %d", player.health);
        glRasterPos2f(330,SCREEN_H-30);
        for(char *c=text; *c; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        sprintf(text,"Coins: %d", score);
        glRasterPos2f(480,SCREEN_H-30);
        for(char *c=text; *c; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

        if(gameOver) {
            glColor4f(0,0,0,0.5f);
            glBegin(GL_QUADS); glVertex2f(0,0); glVertex2f(SCREEN_W,0); glVertex2f(SCREEN_W,SCREEN_H); glVertex2f(0,SCREEN_H); glEnd();
            glColor3f(1,0,0);
            glRasterPos2f(SCREEN_W/2-70,SCREEN_H/2+40);
            for(const char *c="GAME OVER"; *c; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
            glColor3ub(0,180,0);
            glBegin(GL_QUADS); glVertex2f(retryBtnX,retryBtnY); glVertex2f(retryBtnX+retryBtnW,retryBtnY);
            glVertex2f(retryBtnX+retryBtnW,retryBtnY+retryBtnH); glVertex2f(retryBtnX,retryBtnY+retryBtnH); glEnd();
            glColor3f(1,1,1);
            glRasterPos2f(retryBtnX+30,retryBtnY+8);
            for(const char *c="RETRY"; *c; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
        if(gameWin) {
            glColor4f(0,0,0,0.5f);
            glBegin(GL_QUADS); glVertex2f(0,0); glVertex2f(SCREEN_W,0); glVertex2f(SCREEN_W,SCREEN_H); glVertex2f(0,SCREEN_H); glEnd();
            glColor3f(1,0.84f,0);
            glRasterPos2f(SCREEN_W/2-100,SCREEN_H/2+40);
            for(const char *c="LEVEL COMPLETE!"; *c; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

            char finalText[50];
            sprintf(finalText, "Final Score: %d", score);
            glColor3f(1,1,1);
            glRasterPos2f(SCREEN_W/2-80, SCREEN_H/2);
            for(char *c=finalText; *c; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

            glColor3ub(0,100,200);
            glBegin(GL_QUADS); glVertex2f(nextBtnX,nextBtnY); glVertex2f(nextBtnX+nextBtnW,nextBtnY);
            glVertex2f(nextBtnX+nextBtnW,nextBtnY+nextBtnH); glVertex2f(nextBtnX,nextBtnY+nextBtnH); glEnd();
            glColor3f(1,1,1);
            glRasterPos2f(nextBtnX+32,nextBtnY+8);
            for(const char *c="NEXT"; *c; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
    }

    void display() {
        glClear(GL_COLOR_BUFFER_BIT);
        drawTemple(1000,GROUND_Y);
        drawPalmTree(1150,GROUND_Y);
        drawFlag(1120,GROUND_Y);
        drawSign(20,GROUND_Y,"START");
        drawSign(1060,GROUND_Y,"FINISH");
        drawCactus(140,GROUND_Y);
        drawLadder(ladderX,ladderY,ladderH);
        drawMace();
        drawGround();
        for(int i=0;i<platformCount;i++) drawPlatform(platforms[i].x,platforms[i].y,platforms[i].w,platforms[i].h);
        drawMovingPlatform();
        drawSpikes();
        drawScorpion();
        drawCoins();
        drawHearts();
        drawKey();
        drawPlayer();
        drawUI();
    }

    bool rectCollide(float x1,float y1,float w1,float h1, float x2,float y2,float w2,float h2) {
        return x1<x2+w2 && x1+w1>x2 && y1<y2+h2 && y1+h1>y2;
    }

    void update() {
        if(globalGameState != PLAYING) return;
        if(gameWin || gameOver) return;
        player.x += player.velX;
        if(player.x<0) player.x=0;
        if(player.x+player.w>SCREEN_W) player.x=SCREEN_W-player.w;
        float oldBottom=player.y, oldX=player.x-player.velX;
        bool onLadder = (player.x+player.w/2 > ladderX && player.x+player.w/2 < ladderX+24 &&
                         player.y+player.h > ladderY && player.y < ladderY+ladderH);
        if(!onLadder) { player.velY+=GRAVITY; player.y+=player.velY; }
        else { if(keyUp) player.y+=3; if(keyDown) player.y-=3; player.velY=0; }
        player.onGround = false;
        if(player.y <= GROUND_Y) { player.y=GROUND_Y; player.velY=0; player.onGround=true; }
        for(int i=0;i<platformCount;i++) {
            if(rectCollide(player.x,player.y,player.w,player.h, platforms[i].x,platforms[i].y,platforms[i].w,platforms[i].h)) {
                float platTop=platforms[i].y+platforms[i].h;
                if(player.velY<=0 && oldBottom>=platTop-5) { player.y=platTop; player.velY=0; player.onGround=true; break; }
                else if(player.velX>0 && oldX+player.w <= platforms[i].x+2) player.x = platforms[i].x - player.w;
                else if(player.velX<0 && oldX >= platforms[i].x+platforms[i].w-2) player.x = platforms[i].x+platforms[i].w;
                else if(player.velY>0 && player.y < platforms[i].y) { player.y=platforms[i].y-player.h; player.velY=0; }
            }
        }
        float mpTop=movingPlat.y+movingPlat.h;
        if(rectCollide(player.x,player.y,player.w,player.h, movingPlat.x,movingPlat.y,movingPlat.w,movingPlat.h)) {
            if(player.velY<=0 && oldBottom>=mpTop-5) { player.y=mpTop; player.velY=0; player.onGround=true; player.x+=movingPlat.dx; }
        }
        scorpion.x+=scorpion.velX;
        if(scorpion.x<=scorpion.left || scorpion.x+scorpion.w>=scorpion.right) scorpion.velX=-scorpion.velX;
        mace.angle+=mace.angleSpeed;
        if(mace.angle>1.2f || mace.angle<-1.2f) mace.angleSpeed=-mace.angleSpeed;
        movingPlat.x+=movingPlat.dx;
        if(movingPlat.x<=movingPlat.left || movingPlat.x+movingPlat.w>=movingPlat.right) movingPlat.dx=-movingPlat.dx;

        for(int i=0;i<coinCount;i++) {
            if(!coins[i].collected && rectCollide(player.x,player.y,player.w,player.h, coins[i].x-10,coins[i].y-14,20,28)) {
                coins[i].collected=true; score++; PlaySound(TEXT("coinbgm.wav"),NULL,SND_ASYNC);
            }
        }
        for(int i=0;i<heartCount;i++) {
            if(!hearts[i].collected && rectCollide(player.x,player.y,player.w,player.h, hearts[i].x-10,hearts[i].y-10,20,20)) {
                hearts[i].collected=true; player.health++; score+=20; PlaySound(TEXT("heartbgm.wav"),NULL,SND_ASYNC);
            }
        }
        if(!key.collected && rectCollide(player.x,player.y,player.w,player.h, key.x-10,key.y-10,20,20)) {
            key.collected=true; player.hasKey=true; PlaySound(TEXT("keybgm.wav"),NULL,SND_ASYNC);
        }
        for(int i=0;i<spikeCount;i++) {
            if(rectCollide(player.x,player.y,player.w,player.h, spikes[i].x,spikes[i].y,spikes[i].w,spikes[i].h)) {
                player.health--; player.x=80; player.y=GROUND_Y; player.velY=0;
                if(player.health<=0) { gameOver=true; globalGameState=GAMEOVER; PlaySound(NULL,0,0); PlaySound(TEXT("gameoverbgm.wav"),NULL,SND_ASYNC); return; }
            }
        }
        if(rectCollide(player.x,player.y,player.w,player.h, scorpion.x,scorpion.y,scorpion.w,scorpion.h)) {
            player.health--; player.x=80; player.y=GROUND_Y; player.velY=0;
            if(player.health<=0) { gameOver=true; globalGameState=GAMEOVER; PlaySound(NULL,0,0); PlaySound(TEXT("gameoverbgm.wav"),NULL,SND_ASYNC); return; }
        }
        float ballX=mace.pivotX+sin(mace.angle)*mace.length, ballY=mace.pivotY-cos(mace.angle)*mace.length;
        if(rectCollide(player.x,player.y,player.w,player.h, ballX-16,ballY-16,32,32)) {
            player.health--; player.x=80; player.y=GROUND_Y; player.velY=0;
            if(player.health<=0) { gameOver=true; globalGameState=GAMEOVER; PlaySound(NULL,0,0); PlaySound(TEXT("gameoverbgm.wav"),NULL,SND_ASYNC); return; }
        }

        if(player.x+player.w > 1000 && player.y <= GROUND_Y+5 && player.hasKey) {
            gameWin = true;
            PlaySound(NULL, 0, 0);
            PlaySound(TEXT("gamewonbgm.wav"), NULL, SND_FILENAME | SND_ASYNC);
        }
    }

    void init() {
        player.x=80; player.y=GROUND_Y; player.w=30; player.h=46; player.velX=player.velY=0; player.health=3; player.onGround=true; player.hasKey=false;
        coins[0]={220,140,false}; coins[1]={260,140,false}; coins[2]={520,150,false}; coins[3]={550,150,false}; coins[4]={580,150,false};
        hearts[0]={410,155,false};
        platforms[0]={200,GROUND_Y,80,50}; platforms[1]={370,GROUND_Y,100,80}; platforms[2]={500,100,100,20}; platforms[3]={620,GROUND_Y,70,20}; platforms[4]={720,GROUND_Y,80,70};
        spikes[0]={800,GROUND_Y,60,20}; spikes[1]={940,GROUND_Y,60,20};
        movingPlat={860,90,80,20,1.5f,830,1000};
        mace={670,160,50,0,0.04f};
        scorpion={300,GROUND_Y,35,15,1,280,370};
        key={760,150,false};
        gameOver=false; gameWin=false; keyUp=false; keyDown=false;
        retryBtnX = SCREEN_W/2 - retryBtnW/2;
        retryBtnY = SCREEN_H/2 - 10;
        nextBtnX = SCREEN_W/2 - nextBtnW/2;
        nextBtnY = SCREEN_H/2 - 50;
        globalGameState = PLAYING;
        stageWinTriggered = false;
        PlaySound(TEXT("bgm.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    }

    void keyboard(unsigned char key_in, int x, int y) {
        if (key_in == 27) exit(0);
        if (key_in == ' ' && player.onGround && !gameOver && !gameWin) { player.velY = JUMP_VEL; player.onGround = false; }
    }

    void special(int key_in, int x, int y) {
        if (gameOver || gameWin) return;
        if (key_in == GLUT_KEY_LEFT) player.velX = -5;
        if (key_in == GLUT_KEY_RIGHT) player.velX = 5;
        if (key_in == GLUT_KEY_UP) keyUp = true;
        if (key_in == GLUT_KEY_DOWN) keyDown = true;
    }

    void specialUp(int key_in, int x, int y) {
        if (key_in == GLUT_KEY_LEFT || key_in == GLUT_KEY_RIGHT) player.velX = 0;
        if (key_in == GLUT_KEY_UP) keyUp = false;
        if (key_in == GLUT_KEY_DOWN) keyDown = false;
    }

    void mouse(int button, int state, int x, int y) {
        float worldX = (float)x / glutGet(GLUT_WINDOW_WIDTH) * SCREEN_W;
        float worldY = (float)(glutGet(GLUT_WINDOW_HEIGHT) - y) / glutGet(GLUT_WINDOW_HEIGHT) * SCREEN_H;
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            if (gameOver) {
                if (worldX >= retryBtnX && worldX <= retryBtnX+retryBtnW && worldY >= retryBtnY && worldY <= retryBtnY+retryBtnH) {
                    globalGameState = PLAYING;
                    init();
                }
            }
            if (gameWin) {
                if (worldX >= nextBtnX && worldX <= nextBtnX+nextBtnW && worldY >= nextBtnY && worldY <= nextBtnY+nextBtnH) {
                    char finalMsg[100];
                    sprintf(finalMsg, "Final Score: %d\nThanks for playing!", score);
                    MessageBox(NULL, finalMsg, TEXT("Stage 4 Complete"), MB_OK);
                    exit(0);
                }
            }
        }
    }
}

// ==================== GLOBAL DISPLAY & UTILITIES ====================
void drawGlobalGameOverOverlay() {
    if (globalGameState != GAMEOVER) return;
    glColor4f(0, 0, 0, 0.7f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
    glVertex2f(0, 0); glVertex2f(SCREEN_W, 0); glVertex2f(SCREEN_W, SCREEN_H); glVertex2f(0, SCREEN_H);
    glEnd();
    glDisable(GL_BLEND);

    glColor3f(1, 0, 0);
    glRasterPos2f(SCREEN_W/2 - 70, SCREEN_H/2 + 40);
    for (const char* c = "GAME OVER"; *c; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    float bx = SCREEN_W/2 - 60, by = SCREEN_H/2 - 30, bw = 120, bh = 30;
    glColor3ub(0, 180, 0);
    glBegin(GL_QUADS);
    glVertex2f(bx, by); glVertex2f(bx+bw, by); glVertex2f(bx+bw, by+bh); glVertex2f(bx, by+bh);
    glEnd();
    glColor3f(1,1,1);
    glRasterPos2f(bx + 35, by + 8);
    for (const char* c = "RETRY"; *c; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

void displayStage(int stage) {
    if (stage == -1) CoverPage::display();
    else if (stage == 0) InstructionPage::display();
    else if (stage == 1) Stage1::display();
    else if (stage == 2) Stage2::display();
    else if (stage == 4) Stage4::display();
}

void updateStage(int value) {
    if (currentStage == -1 || currentStage == 0) {
        glutPostRedisplay();
        glutTimerFunc(16, updateStage, 0);
        return;
    }
    if (globalGameState == GAMEOVER) {
        glutPostRedisplay();
        glutTimerFunc(16, updateStage, 0);
        return;
    }
    if (stageWinTriggered) {
        stageWinTriggered = false;
        int nextStage = (currentStage == 1) ? 2 : (currentStage == 2) ? 4 : currentStage;
        if (nextStage != currentStage) {
            currentStage = nextStage;
            initStage(currentStage);
        }
    }
    switch (currentStage) {
        case 1: Stage1::update(); break;
        case 2: Stage2::update2(); break;
        case 4: Stage4::update(); break;
    }
    glutPostRedisplay();
    glutTimerFunc(16, updateStage, 0);
}

void initStage(int stage) {
    PlaySound(NULL, 0, 0);
    switch (stage) {
        case 1: Stage1::init(); break;
        case 2: Stage2::init(); break;
        case 4: Stage4::init(); break;
    }
}

void dispatchKeyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0);
    if (currentStage == -1) {
        if (key == 13) { currentStage = 0; }
        return;
    }
    if (currentStage == 0) {
        if (key == 13) { currentStage = 1; initStage(1); }
        return;
    }
    if (globalGameState == GAMEOVER) {
        if (key == 'r' || key == 'R') {
            lives = 3; score = 0;
            currentStage = 1;
            initStage(1);
            globalGameState = PLAYING;
        }
        return;
    }
    switch (currentStage) {
        case 1: Stage1::keyboard(key, x, y); break;
        case 2: Stage2::keyboard(key, x, y); break;
        case 4: Stage4::keyboard(key, x, y); break;
    }
}

void dispatchSpecial(int key, int x, int y) {
    if (currentStage == -1 || currentStage == 0) return;
    if (globalGameState != PLAYING) return;
    switch (currentStage) {
        case 1: Stage1::special(key, x, y); break;
        case 2: Stage2::special(key, x, y); break;
        case 4: Stage4::special(key, x, y); break;
    }
}

void dispatchSpecialUp(int key, int x, int y) {
    if (currentStage == -1 || currentStage == 0) return;
    if (globalGameState != PLAYING) return;
    switch (currentStage) {
        case 2: Stage2::specialUp(key, x, y); break;
        case 4: Stage4::specialUp(key, x, y); break;
    }
}

void dispatchKeyboardUp(unsigned char key, int x, int y) {
    if (currentStage == -1 || currentStage == 0) return;
    if (globalGameState != PLAYING) return;
    switch (currentStage) {
        case 2: Stage2::keyboardUp(key, x, y); break;
    }
}

void dispatchMouse(int button, int state, int x, int y) {
    if (currentStage == -1 || currentStage == 0) return;
    float worldX = (float)x / glutGet(GLUT_WINDOW_WIDTH) * SCREEN_W;
    float worldY = (float)(glutGet(GLUT_WINDOW_HEIGHT) - y) / glutGet(GLUT_WINDOW_HEIGHT) * SCREEN_H;
    if (globalGameState == GAMEOVER && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float bx = SCREEN_W/2 - 60, by = SCREEN_H/2 - 30, bw = 120, bh = 30;
        if (worldX >= bx && worldX <= bx+bw && worldY >= by && worldY <= by+bh) {
            lives = 3; score = 0;
            currentStage = 1;
            initStage(1);
            globalGameState = PLAYING;
        }
        return;
    }
    if (currentStage == 2) Stage2::mouse(button, state, x, y);
    if (currentStage == 4) Stage4::mouse(button, state, x, y);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    displayStage(currentStage);
    if (currentStage >= 1 && globalGameState == GAMEOVER) drawGlobalGameOverOverlay();
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(SCREEN_W, SCREEN_H);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Heart and Gold");
    glClearColor(0.53f, 0.81f, 0.98f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, SCREEN_W, 0, SCREEN_H);
    glMatrixMode(GL_MODELVIEW);

    glutDisplayFunc(display);
    glutKeyboardFunc(dispatchKeyboard);
    glutKeyboardUpFunc(dispatchKeyboardUp);
    glutSpecialFunc(dispatchSpecial);
    glutSpecialUpFunc(dispatchSpecialUp);
    glutMouseFunc(dispatchMouse);
    glutTimerFunc(16, updateStage, 0);

    glutMainLoop();
    return 0;
}
