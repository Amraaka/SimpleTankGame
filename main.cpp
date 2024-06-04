#include <raylib.h>
#include <iostream>

Color Green = Color{38, 185, 154, 255};
Color Dark_Green = Color{20, 160, 133, 255};
Color Light_Green = Color{129, 204, 184, 255};
Color Yellow = Color{243, 213, 91, 255};

int player_score = 0;
int cpu_score = 0;

bool two_player_mode = false;

class Ball
{
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw()
    {
        DrawCircle(x, y, radius, RED);
    }

    void Update()
    {
        x += speed_x;
        y += speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0)
        {
            speed_y *= -1;
        }
        // Cpu wins
        if (x + radius >= GetScreenWidth())
        {
            cpu_score++;
            ResetBall();
        }

        if (x - radius <= 0)
        {
            player_score++;
            ResetBall();
        }
    }

    void ResetBall()
    {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = {-1, 1};
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }
};

class Paddle
{
protected:
    void LimitMovement()
    {
        if (y <= 0)
        {
            y = 0;
        }
        if (y + height >= GetScreenHeight())
        {
            y = GetScreenHeight() - height;
        }
    }

public:
    float x, y;
    float width, height;
    int speed;

    void Draw()
    {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
    }

    void Update(int upKey, int downKey)
    {
        if (IsKeyDown(upKey))
        {
            y = y - speed;
        }
        if (IsKeyDown(downKey))
        {
            y = y + speed;
        }
        LimitMovement();
    }
};

class CpuPaddle : public Paddle
{
public:
    void Update(int ball_y)
    {
        if (y + height / 2 > ball_y)
        {
            y = y - speed;
        }
        if (y + height / 2 <= ball_y)
        {
            y = y + speed;
        }
        LimitMovement();
    }
};

Ball ball;
Paddle player;
Paddle player2; // Second player paddle
CpuPaddle cpu;
CpuPaddle extra_cpu;

enum GameState
{
    MENU,
    GAME,
    PAUSE,
    WIN,
    EXIT
};
GameState state = MENU;

void ResetGame(int screen_width, int screen_height, bool extra_cpu_enabled)
{
    player_score = 0;
    cpu_score = 0;
    ball.ResetBall();
    player.x = screen_width - player.width - 10;
    player.y = screen_height / 2 - player.height / 2;
    
    if (two_player_mode) {
        player2.x = 10;
        player2.y = screen_height / 2 - player2.height / 2;
    } else {
        cpu.x = 10;
        cpu.y = screen_height / 2 - cpu.height / 2;
    }

    if (extra_cpu_enabled)
    {
        extra_cpu.x = 10;
        extra_cpu.y = screen_height / 4;
    }
}

void SwitchSides(int screen_width, int screen_height)
{
    float temp_x = player.x;
    player.x = cpu.x;
    cpu.x = temp_x;

    player.y = screen_height / 2 - player.height / 2;
    cpu.y = screen_height / 2 - cpu.height / 2;

    ball.ResetBall();
}

void StartGame(int difficulty, int screen_width, int screen_height)
{
    switch (difficulty)
    {
    case 1: // Easy
        ball.radius = 30;
        ball.speed_x = 5;
        ball.speed_y = 5;
        ResetGame(screen_width, screen_height, false);
        break;
    case 2: // Medium
        ball.radius = 20;
        ball.speed_x = 7;
        ball.speed_y = 7;
        ResetGame(screen_width, screen_height, false);
        break;
    case 3: // Hard
        ball.radius = 15;
        ball.speed_x = 10;
        ball.speed_y = 10;
        ResetGame(screen_width, screen_height, false);
        break;
    case 4: // Hardest
        ball.radius = 10;
        ball.speed_x = 12;
        ball.speed_y = 12;
        ResetGame(screen_width, screen_height, true);
        break;
    }
    state = GAME;
}

int main()
{
    std::cout << "Starting the game" << std::endl;
    const int screen_width = 1280;
    const int screen_height = 800;
    InitWindow(screen_width, screen_height, "My Pong Game!");
    SetTargetFPS(60);

    player.width = 25;
    player.height = 120;
    player.speed = 6;

    player2.width = 25;
    player2.height = 120;
    player2.speed = 6;

    cpu.height = 120;
    cpu.width = 25;
    cpu.speed = 6;

    extra_cpu.height = 120;
    extra_cpu.width = 25;
    extra_cpu.speed = 6;

    while (state != EXIT)
    {
        if (WindowShouldClose())
        {
            state = EXIT;
        }

        BeginDrawing();
        ClearBackground(Dark_Green);

        if (state == MENU)
        {
            DrawText("PONG GAME", screen_width / 2 - MeasureText("PONG GAME", 40) / 2, screen_height / 3, 40, WHITE);
            DrawRectangle(screen_width / 2 - 100, screen_height / 2 - 230, 200, 60, Green);
            DrawRectangle(screen_width / 2 - 100, screen_height / 2 - 160, 200, 60, Green);
            DrawRectangle(screen_width / 2 - 100, screen_height / 2 - 90, 200, 60, Green);
            DrawRectangle(screen_width / 2 - 100, screen_height / 2 - 20, 200, 60, Green);
            DrawRectangle(screen_width / 2 - 100, screen_height / 2 + 50, 200, 60, Green);
            DrawRectangle(screen_width / 2 - 100, screen_height / 2 + 120, 200, 60, Green);
            DrawRectangle(screen_width / 2 - 100, screen_height / 2 + 190, 200, 60, Green);
            DrawText("ONE PLAYER", screen_width / 2 - MeasureText("ONE PLAYER", 30) / 2, screen_height / 2 - 215, 30, DARKGRAY);
            DrawText("TWO PLAYER", screen_width / 2 - MeasureText("TWO PLAYER", 30) / 2, screen_height / 2 - 145, 30, DARKGRAY);
            DrawText("EASY", screen_width / 2 - MeasureText("EASY", 30) / 2, screen_height / 2 - 75, 30, DARKGRAY);
            DrawText("MEDIUM", screen_width / 2 - MeasureText("MEDIUM", 30) / 2, screen_height / 2 - 5, 30, DARKGRAY);
            DrawText("HARD", screen_width / 2 - MeasureText("HARD", 30) / 2, screen_height / 2 + 65, 30, DARKGRAY);
            DrawText("HARDEST", screen_width / 2 - MeasureText("HARDEST", 30) / 2, screen_height / 2 + 135, 30, DARKGRAY);
            DrawText("EXIT", screen_width / 2 - MeasureText("EXIT", 30) / 2, screen_height / 2 + 205, 30, DARKGRAY);

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                Vector2 mouse = GetMousePosition();
                if (CheckCollisionPointRec(mouse, {screen_width / 2 - 100, screen_height / 2 - 230, 200, 60}))
                {
                    two_player_mode = false;
                }
                else if (CheckCollisionPointRec(mouse, {screen_width / 2 - 100, screen_height / 2 - 160, 200, 60}))
                {
                    two_player_mode = true;
                }
                else if (CheckCollisionPointRec(mouse, {screen_width / 2 - 100, screen_height / 2 - 90, 200, 60}))
                {
                    StartGame(1, screen_width, screen_height); // Easy
                }
                else if (CheckCollisionPointRec(mouse, {screen_width / 2 - 100, screen_height / 2 - 20, 200, 60}))
                {
                    StartGame(2, screen_width, screen_height); // Medium
                }
                else if (CheckCollisionPointRec(mouse, {screen_width / 2 - 100, screen_height / 2 + 50, 200, 60}))
                {
                    StartGame(3, screen_width, screen_height); // Hard
                }
                else if (CheckCollisionPointRec(mouse, {screen_width / 2 - 100, screen_height / 2 + 120, 200, 60}))
                {
                    StartGame(4, screen_width, screen_height); // Hardest
                }
                else if (CheckCollisionPointRec(mouse, {screen_width / 2 - 100, screen_height / 2 + 190, 200, 60}))
                {
                    state = EXIT;
                }
            }
        }
        else if (state == GAME)
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                state = PAUSE;
            }

            ball.Update();
            player.Update(KEY_UP, KEY_DOWN);
            if (two_player_mode)
            {
                player2.Update(KEY_W, KEY_S);
            }
            else
            {
                cpu.Update(ball.y);
            }

            if (state == GAME && (ball.x + ball.radius >= GetScreenWidth() || ball.x - ball.radius <= 0))
            {
                if (player_score >= 11 || cpu_score >= 11)
                {
                    state = WIN;
                }
            }

            if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {player.x, player.y, player.width, player.height}))
            {
                ball.speed_x *= -1;
            }

            if (two_player_mode && CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {player2.x, player2.y, player2.width, player2.height}))
            {
                ball.speed_x *= -1;
            }
            else if (!two_player_mode && CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {cpu.x, cpu.y, cpu.width, cpu.height}))
            {
                ball.speed_x *= -1;
            }

            if (state == GAME && player_score >= 11)
            {
                state = WIN;
            }
            else if (state == GAME && cpu_score >= 11)
            {
                state = WIN;
            }

            ClearBackground(Dark_Green);
            DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Green);
            DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Green);
            DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
            ball.Draw();
            player.Draw();
            if (two_player_mode)
            {
                player2.Draw();
            }
            else
            {
                cpu.Draw();
            }

            DrawText(TextFormat("%i", cpu_score), screen_width / 4 - 20, 20, 80, WHITE);
            DrawText(TextFormat("%i", player_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);
        }
        else if (state == PAUSE)
        {
            DrawText("PAUSE", screen_width / 2 - MeasureText("PAUSE", 40) / 2, screen_height / 3, 40, WHITE);
            DrawRectangle(screen_width / 2 - 100, screen_height / 2 - 30, 200, 60, Green);
            DrawText("RESUME", screen_width / 2 - MeasureText("RESUME", 30) / 2, screen_height / 2 - 15, 30, DARKGRAY);

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                Vector2 mouse = GetMousePosition();
                if (CheckCollisionPointRec(mouse, {screen_width / 2 - 100, screen_height / 2 - 30, 200, 60}))
                {
                    state = GAME;
                }
            }

            DrawRectangle(screen_width / 2 - 100, screen_height / 2 + 50, 200, 60, Green);
            DrawText("EXIT TO MENU", screen_width / 2 - MeasureText("EXIT TO MENU", 30) / 2, screen_height / 2 + 65, 30, DARKGRAY);

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                Vector2 mouse = GetMousePosition();
                if (CheckCollisionPointRec(mouse, {screen_width / 2 - 100, screen_height / 2 + 50, 200, 60}))
                {
                    state = MENU;
                }
            }
        }
        else if (state == WIN)
        {
            DrawText("WIN", screen_width / 2 - MeasureText("WIN", 40) / 2, screen_height / 3, 40, WHITE);
            DrawRectangle(screen_width / 2 - 100, screen_height / 2 - 30, 200, 60, Green);
            DrawText("MAIN MENU", screen_width / 2 - MeasureText("MAIN MENU", 30) / 2, screen_height / 2 - 15, 30, DARKGRAY);

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                Vector2 mouse = GetMousePosition();
                if (CheckCollisionPointRec(mouse, {screen_width / 2 - 100, screen_height / 2 - 30, 200, 60}))
                {
                    state = MENU;
                }
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
