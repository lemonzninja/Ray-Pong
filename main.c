#include "../../../src/raylib.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

const int screenWidth = 800;
const int screenHeight = 450;

// variable to hold the mouse position
Vector2 mousePosition = {0, 0};

// Start Button Variables and Struct
Rectangle StartButtonRectangle = {0, 0, 0, 0}; // Button rectangle
bool buttonClicked = false;      // True if the StartButtonRectangle is clicked

typedef struct ButtonBox // A struct to hold the box data
{
    Vector2 position;
    Vector2 size;
    Color color;
} ButtonBox;
ButtonBox StartButtonBox = {0};
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
    StartButtonRectangle.width = 50;
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
    // Set the box color
    StartButtonBox.color = RED;
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
        buttonClicked = true;
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

    // If the StartButtonRectangle is clicked, start the game
    if (buttonClicked)
    {
        StartGame();
    }

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    // if game is started,hide the StartButtonRectangle and show the game
    if (!buttonClicked)
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
    DrawRectangleRec(StartButtonRectangle, StartButtonBox.color);
    DrawText("Start", StartButtonRectangle.x + 10, StartButtonRectangle.y + 10, 20, BLACK);
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
