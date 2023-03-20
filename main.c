#include "../../../src/raylib.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

const int screenWidth = 800;
const int screenHeight = 450;

// variable to hold the mouse position
Vector2 mousePosition = {0, 0};

// Start Button Variables and Struct
Rectangle button = {0, 0, 0, 0}; // Button rectangle
int buttonState = 0;             // 0 = not clicked, 1 = clicked
bool buttonClicked = false;      // True if the button is clicked

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
void StartGame();                  // A function to start the game when the button is clicked
void DrawGame();                   // A function to draw the game
void MoveBall();                   // A function to move the ball`

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "raylib");

    // The button variables
    // Set the button size
    button.width = 50;
    button.height = 50;
    // Set the button position
    button.x = screenWidth / 2 - button.width / 2;
    button.y = screenHeight / 2 - button.height / 2;
    // Set the box size
    StartButtonBox.size.x = 50;
    StartButtonBox.size.y = 50;
    // Set the box position
    StartButtonBox.position.x = screenWidth / 2 - StartButtonBox.size.x / 2;
    StartButtonBox.position.y = screenHeight / 2 - StartButtonBox.size.y / 2;
    // Set the box color
    StartButtonBox.color = RED;
    //--------------------------------------------------------------------------------------

    // The ball variables
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
    while (!WindowShouldClose()) // Detect window close button or ESC key
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
    mousePosition = GetMousePosition();

    if (CheckCollisionPointRec(mousePosition, button))
    {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            buttonState = 1;
        }
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            buttonState = 0;
            buttonClicked = true;

            // pick a random direction for the ball
            ball.velocity.x = GetRandomValue(-ballVelocity, ballVelocity);
            ball.velocity.y = GetRandomValue(-ballVelocity, ballVelocity);
        }

        StartButtonBox.color = LIGHTGRAY;
    }
    else
    {
        StartButtonBox.color = DARKGRAY;
    }

    if (buttonClicked)
    {
        StartGame();
    }

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText("This is a raylib example", 10, 40, 20, DARKGRAY);

    // if game is started,hide the button and show the game
    if (!buttonClicked)
    {
        DrawRectangleRec(button, StartButtonBox.color);
    }
    else
    {
        DrawGame();
    }

    EndDrawing();
    //----------------------------------------------------------------------------------
}

// A function to start the game when the button is clicked
void StartGame()
{
    MoveBall();
}

void DrawGame()
{
    // Draw the ball
    DrawCircleV(ball.position, ball.radius, ball.color);
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
