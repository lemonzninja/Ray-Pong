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

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

const int screenWidth = 800;
const int screenHeight = 450;

// variable to hold the mouse position
Vector2 mousePosition = {0, 0};

// Title Variable
char title[20] = "Ray Pong";

// Start Button Variables and Struct
Rectangle StartButtonRectangle = {0, 0, 0, 0}; // Button rectangle
bool StartButtonClicked = false;      // True if the StartButtonRectangle is clicked

// info button variables
Rectangle InfoButtonRectangle = {0, 0, 0, 0}; // Button rectangle
bool InfoButtonClicked = false;      // True if the StartButtonRectangle is clicked

typedef struct ButtonBox // A struct to hold the box data
{
    Vector2 position;
    Vector2 size;
    Color color;
} ButtonBox;
ButtonBox StartButtonBox = {0};
//----------------------------------------------------------------------------------


ButtonBox InfoButtonBox = {0};

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

// Local Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void); // Update and draw one frame
void StartGame();                  // A function to start the game when the StartButtonRectangle is clicked
void DrawGame();                   // A function to draw the game
void MoveBall();                   // A function to move the ball`

void DrawStart(); // Draw Start on the start StartButtonRectangle

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "raylib");

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
        The ball variables
        ball is the struct that holds the ball data
    */
    // Set the ball velocity
    ballVelocity = 5;
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
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

// Update and draw game frame
static void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------

    // Get the mouse position
    mousePosition = GetMousePosition();

    // Check if the mouse is over the StartButtonRectangle
    if (CheckCollisionPointRec(mousePosition, StartButtonRectangle) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        StartButtonClicked = true;
        DrawText("Clicked", 10, 10, 20, BLACK);
    }
    
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

    // Check if the mouse is over the InfoButtonRectangle
    if (CheckCollisionPointRec(mousePosition, InfoButtonRectangle) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        InfoButtonClicked = true;
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
    


    // If the StartButtonRectangle is clicked, start the game
    if (StartButtonClicked)
    {
        StartGame();
    }

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    // if game is started,hide the StartButtonRectangle and show the game
    if (!StartButtonClicked)
    {

        // Draw the Start Menu
        DrawStart();
                       
    }
    else
    {
        // Draw the game

        DrawGame();

        // Hide the start StartButtonRectangle
        StartButtonRectangle.x = -1000;
        StartButtonRectangle.y = -1000;
    }

    EndDrawing();
    //----------------------------------------------------------------------------------
}

// A function to start the game when the StartButtonRectangle is clicked
void StartGame()
{
    MoveBall();
}

void DrawGame()
{
    // Draw the ball
    DrawCircleV(ball.position, ball.radius, ball.color);
}

// Draw the start StartButtonRectangle
void DrawStart()
{
    // Draw the title text size 100 in the center of the screen above the StartButtonRectangle
    DrawText(title, screenWidth / 2 - MeasureText(title, 100) / 2, screenHeight / 2 - 200, 100, BLACK);

    DrawRectangleRec(StartButtonRectangle, StartButtonBox.color);
    
    // Draw the InfoButtonRectangle
    DrawRectangleRec(InfoButtonRectangle, InfoButtonBox.color);

    // Draw the text "Start" in the center of the StartButtonRectangle
    DrawText("Start", screenWidth / 2 - MeasureText("Start", 20) / 2, screenHeight / 2 - 10, 20, BLACK);
}

void MoveBall()
{
    // Move the ball
    ball.position.x += ball.velocity.x;
    ball.position.y += ball.velocity.y;

    // Check for collision with the walls
    if (ball.position.x >= screenWidth - ball.radius)
    {
        ball.velocity.x *= -1;
    }
    else if (ball.position.x <= 0 + ball.radius)
    {
        ball.velocity.x *= -1;
    }
    else if (ball.position.y >= screenHeight - ball.radius)
    {
        ball.velocity.y *= -1;
    }
    else if (ball.position.y <= 0 + ball.radius)
    {
        ball.velocity.y *= -1;
    }
}
