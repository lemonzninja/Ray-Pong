#include "../../../src/raylib.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

const int screenWidth = 800;
const int screenHeight = 450;

// variable to hold the mouse position
Vector2 mousePosition = { 0, 0 };

// Make a rectangle variable for the button
Rectangle button = { 0, 0, 0, 0 };

int buttonState = 0; // 0 = not clicked, 1 = clicked
bool buttonClicked = false;

// Make a rectangle Struct for the box
typedef struct Box
{
    Vector2 position;
    Vector2 size;
    Color color;
} Box;

Box box1 = { 0 };


// Local Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void);          // Update and draw one frame
void StartGame();                           // A function to start the game when the button is clicked

int main() 
{
    // Initialization
    //--------------------------------------------------------------------------------------
    
    InitWindow(screenWidth, screenHeight, "raylib");

    // Set the button size
    button.width = 100;
    button.height = 50;
    // Set the button position
    button.x = screenWidth / 2 - button.width / 2;
    button.y = screenHeight / 2 - button.height / 2;
    
    // Set the box size
    box1.size.x = 100;
    box1.size.y = 150;
    // Set the box position
    box1.position.x = screenWidth / 2 - box1.size.x / 2;
    box1.position.y = screenHeight / 2 - box1.size.y / 2;
    // Set the box color
    box1.color = RED;

    //--------------------------------------------------------------------------------------

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();                  // Close window and OpenGL context
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
        }   

        box1.color = BLUE;
    }
    else
    {
        box1.color = RED;
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

        DrawRectangleV(box1.position, box1.size, box1.color);
    EndDrawing();
    //----------------------------------------------------------------------------------
}

// A function to start the game when the button is clicked
void StartGame()
{
    // Start the game
    DrawText("Game Started", 10, 10, 20, BLACK);

}