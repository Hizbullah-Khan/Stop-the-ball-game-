#include <iostream>
#include <graphics.h>
#include <conio.h>
#include <dos.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

#define ScWidth 1400
#define ScHeight 750
#define OpWidth 35

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 77
#define KEY_RIGHT 75
#define SPACEBAR 32

using namespace std;

class Game {
private:
    int rSize;
    int rx, ry;
    int pSize;
    int stepX;
    int stepY;
    double px, py;
    int score;
    int lives;
    int playerColor;

public:
    Game() {
        rSize = 79;
        rx = ((ScWidth - OpWidth) / 2);
        ry = 350;
        pSize = 140;
        stepX = rand() % 10 + 1; // Random value between 1 and 10
        stepY = rand() % 10 + 1; // Random value between 1 and 10
        px = ((ScWidth - OpWidth) / 2);
        py = 350;
        score = 0;
        lives = 3;
        stepX = 90; // Increased speed in the X direction
        stepY = 90; // Increased speed in the Y direction
        playerColor = WHITE;
    }

    void run() {
        int gd = DETECT, gm, err, tmp;
        initgraph(&gd, &gm, "C:\\tc\\bgi");
        initwindow(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), "Stop the Ball");

        // Display the start screen
        startScreen();

        int activePage = 0;
        int visualPage = 1;

        while (lives > 0) { // Continue playing until lives reach 0
            drawBoard();
            drawReferenceCircle();
            drawPlayer();

            if (kbhit()) {
                char option = getch(); // catch key in option variable
                if (option == SPACEBAR) {
                    int collisionType = circleCollideCircle(px, py, pSize, rx, ry, rSize);
                    if (collisionType == 1) {
                        score += 5; // Increase the score by 5 when player's ball is completely inside the reference circle
                        playerColor = WHITE; // Change the color of the player's ball to blue
                    } else if (collisionType == 2) {
                        score+=5; // Increase the score by 1 when player's ball overlaps the reference circle
                        playerColor = BLUE; // Change the color of the player's ball back to white
                    } else if (collisionType == 3) {
                        score +=3; // Increase the score by 3 when player's ball touches the border of the reference circle
                        playerColor = WHITE; // Change the color of the player's ball back to white
                    } else {
                        lives--;
                        delay(500);
                        if (lives > 0) {
                            cleardevice();
                            drawBoard();
                            drawReferenceCircle();
                            px = ((ScWidth - OpWidth) / 2);
                            py = 350;
                            playerColor = WHITE; // Reset the color of the player's ball back to white
                            drawPlayer();
                        } else {
                            gameOver();
                        }
                    }
                    delay(100);
                }
            }

            // Update player position
            px += stepX;
            py += stepY;

            // Change direction if player goes outside bounds
            if (px > ScWidth - OpWidth || px < 0) {
                stepX = -stepX;
            }
            if (py > ScHeight || py < 0) {
                stepY = -stepY;
            }

            delay(40);
            // Swap active and visual pages
            int temp = activePage;
            activePage = visualPage;
            visualPage = temp;

            // cleardevice();
        }

        getch();
        closegraph();
    }

private:
    void startScreen() {
        cleardevice();
        // Display the start game background image
        readimagefile("startgame.jpg", 0, 0, ScWidth - OpWidth, ScHeight);
        settextstyle(10, 0, 4);
        outtextxy(500, 500, "Press any key to start...");
        getch();
    }

    void gameOver() {
        cleardevice(); // clear screen

        // Display the game over background image
        readimagefile("gameover.jpg", 0, 0, ScWidth - OpWidth, ScHeight);

        setcolor(WHITE);
        settextstyle(10, 0, 3);
        outtextxy(500, 500, "Press any key to restart...");

        // Display the final score
        char scoreText[10];
        sprintf(scoreText, "Score: %d", score);
        settextstyle(4, 0, 5);
        outtextxy(ScWidth - 870, 170, scoreText);

        getch();
        score = 0; // reset score
    }

    void drawBoard() {
        setcolor(WHITE);
        int activePage;
        // Draw the active page
        setactivepage(activePage);

        // Draw the game background image
        readimagefile("hall1.jpg", 0, 0, ScWidth - OpWidth, ScHeight);

        // draw title
        settextstyle(4, 0, 4);
        outtextxy(ScWidth - 375, 50, "Stop the Ball");

        // print score
        // itoa function converts number to character array
        settextstyle(4, 0, 9);
        if (score < 10)
            outtextxy(ScWidth - 270, 250, itoa(score, new char[5], 10));
        else if (score >= 10)
            outtextxy(ScWidth - 320, 250, itoa(score, new char[5], 10));

        // print lives
        settextstyle(4, 0, 7);
        outtextxy(ScWidth - 395, 400, "Lives: ");
        outtextxy(ScWidth - 150, 400, itoa(lives, new char[5], 10));

        settextstyle(4, 0, 1);
        outtextxy(ScWidth - 375, 500, "Controls: ");
        outtextxy(ScWidth - 300, 550, "Spacebar to Stop ");
    }

    void drawReferenceCircle() {
        setcolor(WHITE);
        circle(rx, ry, rSize + 100); // Increase the size of the reference circle
    }

    void drawPlayer() {
        setcolor(WHITE);
        setfillstyle(1, playerColor); // Set the player's ball color
        fillellipse(px, py, pSize, pSize);
    }

    int circleCollideCircle(int x1, int y1, int size1, int x2, int y2, int size2) {
        double distance = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
        if (distance <= size1 + size2) {
            if (distance + size1 <= size2)
                return 1; // Completely inside reference circle
            else if (distance >= size2 - 5)
                return 3; // Touches the border of the reference circle
            else
                return 2; // Overlaps reference circle
        }
        return 0; // Outside reference circle
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}

