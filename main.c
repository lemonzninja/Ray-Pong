/*
    Pong clone made with raylib

    - Start menu with a start button and a title.
    - When game has started A ball that bounces off the walls and the paddles.
    - When game has started Two paddles that move up and down.
    - When game has started Ai that moves the opponent paddle up and down.
    - When game has started A score counter.
    - When game has started A pause button.
    - When game has started A restart button.
    - A quit button.
    - A Info button in the start menu.
    - When game has started A sound effect when the ball hits the paddles.
    - When game has started A sound effect when the ball hits the walls.
    - When game has started A sound effect when the ball hits the score counter.
*/

#include "../../../src/raylib.h"
#include "math.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

const int screenWidth = 800;
const int screenHeight = 450;

// variable to hold the mouse position
Vector2 mousePosition = {0, 0};

typedef enum GameScreen // An enum to hold the screens
{
    START = 0,
    INFO,
    GAME
} GameScreen;

GameScreen currentScreen; // A variable to hold the current screen

// Title Variable
char title[20] = "Ray Pong";

typedef struct ButtonBox // A struct to hold the box data
{
    Vector2 position;
    Vector2 size;
    Color color;

} ButtonBox;

// Start Button Variables and Struct
Rectangle StartButtonRectangle = {0, 0, 0, 0}; // Button rectangle
bool StartButtonClicked = false;               // True if the StartButtonRectangle is clicked
ButtonBox StartButtonBox = {0};

// info button variables
Rectangle InfoButtonRectangle = {0, 0, 0, 0}; // Button rectangle
bool InfoButtonClicked = false;               // True if the StartButtonRectangle is clicked
ButtonBox InfoButtonBox = {0};

// back button variables
Rectangle BackButtonRectangle = {0, 0, 0, 0}; // Button rectangle
bool BackButtonClicked = false;               // True if the StartButtonRectangle is clicked
ButtonBox BackButtonBox = {0};
//----------------------------------------------------------------------------------

// Ball Struct
typedef struct Ball
{
    Vector2 position;
    Vector2 velocity;

    float radius;
    Color color;
} Ball;
Ball ball = {0};
int ballVelocity = 0;
int ballSpeed = 0;
// a struct to hold the paddle data
typedef struct Paddle
{
    Vector2 position;
    Vector2 size;
    Color color;
    Vector2 velocity;
} Paddle;

// Paddle Variables
Paddle playerPaddle = {0};
Paddle enemyPaddle = {0};

// score variables
int playerScore = 0;
int enemyScore = 0;


bool pause = false; // A variable to hold the pause state

bool gameReset = false; // A variable to hold the game reset state
// Local Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void); // Update and draw one frame

void InitButtons();     // A function to initialize the buttons
void UpdateStartMenu(); // Update Start on the start StartButtonRectangle
void DrawStartMenu();   // Draw Start on the start StartButtonRectangle
void UpdateInfoMenu();  // Update Info on the start StartButtonRectangle
void DrawInfoMenu();    // A function to draw the info

void InitPaddles(); // A function to initialize the paddles
void InitBall();    // A function to initialize the ball

void DrawGame();            // A function to draw the game
void UpdateStartGameMenu(); // A function to start the game when the StartButtonRectangle is clicked

void UpdatePaddles(); // A function to update the paddles
void UpdateBall();    // A function to move the ball`

// Update and draw game frame
static void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------

    // Get the mouse position
    mousePosition = GetMousePosition();

    // a SWITCH statement to change the current game state
    switch (currentScreen)
    {
    case START:
    {
        UpdateStartMenu();
    }
    break;
    case INFO:
    { // Draw the info screen
        UpdateInfoMenu();
    }
    break;
    case GAME:
    {
        // IF q is pressed pause the game
        if (IsKeyPressed(KEY_Q))
        {
            pause = !pause;
        }

        // IF the game is not paused update the game
        if (!pause)
        {
            UpdatePaddles();
            UpdateBall();
        }

        // if pause is true draw the pause menu
        if (pause)
        {
            DrawText("PAUSED", screenWidth / 2 - 50, screenHeight / 2 - 50, 20, BLACK);
                         // Draw the back button
            DrawRectangleRec(BackButtonRectangle, BLACK);
            DrawText("Back", BackButtonRectangle.x + 10, BackButtonRectangle.y + 10, 20, WHITE);

            // if the back button is clicked change the current screen to the start screen
            if (CheckCollisionPointRec(mousePosition, BackButtonRectangle))
            {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    currentScreen = START;

                    // reset the game
                    gameReset = true;
                }
            }
        }   
    }
    break;

    default:
        break;
    }

    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    switch (currentScreen)
    {
    case START:
    {
        DrawStartMenu();
    }
    break;
    case INFO:
    { // Draw the info screen
        DrawInfoMenu();
    }
    break;
    case GAME:
    { // Draw the game
        DrawGame();
    }
    default:
        break;
    }

    EndDrawing();
    //----------------------------------------------------------------------------------
}

void InitButtons()
{
    /*
            The StartButton variables
            StartButtonRectangle is the rectangle that will be drawn on the screen
            StartButtonBox is the struct that holds the box data
        */
    StartButtonRectangle.width = 100;
    StartButtonRectangle.height = 50;
    // Set the StartButtonRectangle position
    StartButtonRectangle.x = 350;
    StartButtonRectangle.y = 200;
    // Set the box size
    StartButtonBox.size.x = 50;
    StartButtonBox.size.y = 50;
    // Set the box position
    StartButtonBox.position.x = screenWidth / 2 - StartButtonBox.size.x / 2;
    StartButtonBox.position.y = screenHeight / 2 - StartButtonBox.size.y / 2;
    //--------------------------------------------------------------------------------------

    /*
        The InfoButton variables
        InfoButtonRectangle is the rectangle that will be drawn on the screen
        InfoButtonBox is the struct that holds the box data
        make the InfoButtonRectangle the same size as the StartButtonRectangle
        make the InfoButtonBox just under the StartButtonBox
    */
    InfoButtonRectangle.width = 100;
    InfoButtonRectangle.height = 50;
    // Set the InfoButtonRectangle position
    InfoButtonRectangle.x = 350;
    InfoButtonRectangle.y = 300;
    // Set the box size
    InfoButtonBox.size.x = 50;
    InfoButtonBox.size.y = 50;
    // Set the box position
    InfoButtonBox.position.x = screenWidth / 2 - InfoButtonBox.size.x / 2;
    InfoButtonBox.position.y = screenHeight / 2 - InfoButtonBox.size.y / 2 + 100;
    //--------------------------------------------------------------------------------------

    /*
        The BackButton variables
        BackButtonRectangle is the rectangle that will be drawn on the screen
        BackButtonBox is the struct that holds the box data
        make the BackButtonRectangle the same size as the StartButtonRectangle
        make the BackButtonBox in the top left corner of the screen
    */
    BackButtonRectangle.width = 100;
    BackButtonRectangle.height = 50;
    // Set the BackButtonRectangle position
    BackButtonRectangle.x = 0;
    BackButtonRectangle.y = 0;
    // Set the box size
    BackButtonBox.size.x = 50;
    BackButtonBox.size.y = 50;
    // Set the box position
    BackButtonBox.position.x = 1;
    BackButtonBox.position.y = 1;
    //--------------------------------------------------------------------------------------
}

void UpdateStartMenu()
{
    // Check if the mouse is over the StartButtonRectangle
    if (CheckCollisionPointRec(mousePosition, StartButtonRectangle) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        currentScreen = GAME;
    }

    // Check if the mouse is over the InfoButtonRectangle
    if (CheckCollisionPointRec(mousePosition, InfoButtonRectangle) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        currentScreen = INFO;
    }
}

// Draw the start StartButtonRectangle
void DrawStartMenu()
{
    // Draw the title text size 100 in the center of the screen above the StartButtonRectangle
    DrawText(title, screenWidth / 2 - MeasureText(title, 100) / 2, screenHeight / 2 - 200, 100, BLACK);

    DrawRectangleRec(StartButtonRectangle, StartButtonBox.color);
    // Draw the text "Start" in the center of the StartButtonRectangle
    DrawText("Start", screenWidth / 2 - MeasureText("Start", 20) / 2, screenHeight / 2 - 10, 20, BLACK);

    // Draw the InfoButtonRectangle
    DrawRectangleRec(InfoButtonRectangle, InfoButtonBox.color);
    // Draw the text "Info" in the center of the InfoButtonRectangle
    DrawText("Info", screenWidth / 2 - MeasureText("Info", 20) / 2, screenHeight / 2 + 90, 20, BLACK);

    // Check if the mouse is over the StartButtonRectangle and change the color of the box
    if (CheckCollisionPointRec(mousePosition, StartButtonRectangle))
    {
        // Set the box color to light gray
        StartButtonBox.color = LIGHTGRAY;
    }
    else
    {
        // Set the box color to dark gray
        StartButtonBox.color = DARKGRAY;
    }

    // Check if the mouse is over the InfoButtonRectangle and change the color of the box
    if (CheckCollisionPointRec(mousePosition, InfoButtonRectangle))
    {
        // Set the box color to light gray
        InfoButtonBox.color = LIGHTGRAY;
    }
    else
    {
        // Set the box color to dark gray
        InfoButtonBox.color = DARKGRAY;
    }
}

// A function to start the game when the StartButtonRectangle is clicked
void UpdateStartGameMenu()
{
    UpdateBall();
}

// A function to update the info screen
void UpdateInfoMenu()
{
    // Check if the mouse is over the BackButtonRectangle
    if (CheckCollisionPointRec(mousePosition, BackButtonRectangle) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        currentScreen = START;
    }
}

// Draw the info screen
void DrawInfoMenu()
{
    // Draw the title text size 100 in the center of the screen above the StartButtonRectangle
    DrawText("Abut", screenWidth / 2 - MeasureText(title, 100) / 2, screenHeight / 2 - 200, 100, BLACK);

    // Draw the back button
    DrawRectangleRec(BackButtonRectangle, BackButtonBox.color);
    // Draw the text "Back" in the center of the BackButtonRectangle
    DrawText("Back", BackButtonBox.position.x + 20, BackButtonBox.position.y + 10, 20, BLACK);

    // Check if the mouse is over the BackButtonRectangle and change the color of the box
    if (CheckCollisionPointRec(mousePosition, BackButtonRectangle))
    {
        // Set the box color to light gray
        BackButtonBox.color = LIGHTGRAY;
    }
    else
    {
        // Set the box color to dark gray
        BackButtonBox.color = DARKGRAY;
    }
}

void InitPaddles()
{
    /*
        init the playerPaddle
        -playerPaddle is on the left side of the screen with a little offset
        -enemyPaddle is on the right side of the screen with a little offset
        -set the paddle size
        -set the paddle velocity
        -set the paddle color
    */

    // Set the paddle position to the left side of the screen with a little offset
    playerPaddle.position.x = 40;
    playerPaddle.position.y = screenHeight / 2 - playerPaddle.size.y / 2 + 60;

    // Set the paddle size
    playerPaddle.size.x = 20;
    playerPaddle.size.y = 100;

    // Set the paddle velocity
    playerPaddle.velocity.y = 300;
    // Set the paddle color
    playerPaddle.color = BLUE;

    // Set the paddle position to the right side and center of the screen with a little offset
    enemyPaddle.position.x = screenWidth - 40 - enemyPaddle.size.x;
    enemyPaddle.position.y = screenHeight / 2 - enemyPaddle.size.y / 2 + 60;

    // Set the paddle size
    enemyPaddle.size.x = 20;
    enemyPaddle.size.y = 100;

    // Set the paddle velocity
    enemyPaddle.velocity.y = 200;

    // Set the paddle color
    enemyPaddle.color = RED;
}

void InitBall()
{
    /*
        The ball variables
        ball is the struct that holds the ball data
    */

    ballSpeed = 300;

    // Set the ball velocity
    ballVelocity = ballSpeed;
    // Set the ball size
    ball.radius = 10;
    // Set the ball position
    ball.position.x = screenWidth / 2 - ball.radius / 2;
    ball.position.y = screenHeight / 2 - ball.radius / 2;
    // Set the ball velocity
    ball.velocity.x = ballVelocity;
    ball.velocity.y = ballVelocity;
    // Set the ball color
    ball.color = RED;
}

void DrawGame()
{
    // Draw the score
    DrawText(TextFormat("%i", playerScore), screenWidth / 2 - 50, 50, 40, BLACK);
    DrawText(TextFormat("%i", enemyScore), screenWidth / 2 + 50, 50, 40, BLACK);

    // Draw A line in the middle of the screen
    DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, BLACK);

    // Draw the ball
    DrawCircleV(ball.position, ball.radius, ball.color);

    // Draw the player paddle
    DrawRectangleV(playerPaddle.position, playerPaddle.size, playerPaddle.color);
    // Draw the enemy paddle
    DrawRectangleV(enemyPaddle.position, enemyPaddle.size, enemyPaddle.color);

    // reset the game
    if (gameReset)
    {
        // reset paddles Y position
        playerPaddle.position.y = screenHeight / 2 - playerPaddle.size.y / 2 + 60;
        enemyPaddle.position.y = screenHeight / 2 - enemyPaddle.size.y / 2 + 60;

        // reset ball position
        ball.position.x = screenWidth / 2 - ball.radius / 2;
        ball.position.y = screenHeight / 2 - ball.radius / 2;

        // reset scores
        playerScore = 0;
        enemyScore = 0;

        // reset the gameReset variable
        gameReset = false;
        pause = false;
    }
}

void UpdatePaddles()
{
    // move the player paddle with W and S
    if (IsKeyDown(KEY_W))
    {
        playerPaddle.position.y -= playerPaddle.velocity.y * GetFrameTime();
    }
    else if (IsKeyDown(KEY_S))
    {
        playerPaddle.position.y += playerPaddle.velocity.y * GetFrameTime();
    }
    // Keep the player paddle inside the screen
    if (playerPaddle.position.y <= 0)
    {
        playerPaddle.position.y = 0;
    }
    else if (playerPaddle.position.y >= screenHeight - playerPaddle.size.y)
    {
        playerPaddle.position.y = screenHeight - playerPaddle.size.y;
    }

    // move the enemy paddle with ai logic less actuate than the player paddle  (the enemy paddle is a little bit slower and not jerky)
    if (ball.position.y > enemyPaddle.position.y + enemyPaddle.size.y / 2)
    {
        enemyPaddle.position.y += enemyPaddle.velocity.y * GetFrameTime();
    }
    else if (ball.position.y < enemyPaddle.position.y + enemyPaddle.size.y / 2)
    {
        enemyPaddle.position.y -= enemyPaddle.velocity.y * GetFrameTime();
    }

    // Keep the enemy paddle inside the screen
    if (enemyPaddle.position.y <= 0)
    {
        enemyPaddle.position.y = 0;
    }
    else if (enemyPaddle.position.y >= screenHeight - enemyPaddle.size.y)
    {
        enemyPaddle.position.y = screenHeight - enemyPaddle.size.y;
    }
}

void UpdateBall()
{
    // Move the ball
    ball.position.x += ball.velocity.x * GetFrameTime();
    ball.position.y += ball.velocity.y * GetFrameTime();

    // Check for collision with the walls
    if (ball.position.y >= screenHeight - ball.radius || ball.position.y <= ball.radius)
    {
        // Invert the ball velocity
        ball.velocity.y *= -1;
    }

    // Check if the ball hits the player paddle and bounce it off the paddle and invert the x,y velocity
    if (CheckCollisionCircleRec(ball.position, ball.radius, (Rectangle){playerPaddle.position.x, playerPaddle.position.y, playerPaddle.size.x, playerPaddle.size.y}))
    {
        // bounce the ball off the paddle and invert the x,y velocity in the direction it came from
        ball.velocity.x *= -1;
        ball.velocity.y *= +1;
    }

    // Check if the ball hits the enemy paddle and bounce it off the paddle and invert the x,y velocity
    if (CheckCollisionCircleRec(ball.position, ball.radius, (Rectangle){enemyPaddle.position.x, enemyPaddle.position.y, enemyPaddle.size.x, enemyPaddle.size.y}))
    {
        // bounce the ball off the paddle and invert the x,y velocity in the direction it came from
        ball.velocity.x *= -1;
        ball.velocity.y *= +1;
    }

    // Check if the ball is out of the screen and reset the ball
    if (ball.position.x >= screenWidth + ball.radius)
    {
        // Reset the ball position
        ball.position.x = screenWidth / 2;
        ball.position.y = screenHeight / 2;

        // add a point to the player score
        playerScore++;

        // Reset the ball velocity
        ball.velocity = (Vector2){ballSpeed, ballSpeed};
    }
    else if (ball.position.x <= -ball.radius)
    {
        // Reset the ball position
        ball.position.x = screenWidth / 2;
        ball.position.y = screenHeight / 2;

        // add a point to the enemy score
        enemyScore++;

        // Reset the ball velocity
        ball.velocity = (Vector2){ballSpeed, ballSpeed};

    }
}

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "raylib");

    InitButtons();

    // have the START screen be the first screen
    currentScreen = GAME;

    // Initialize the paddles
    InitPaddles();

    InitBall();

    //--------------------------------------------------------------------------------------

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close StartButtonRectangle or ESC key
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); //` Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
