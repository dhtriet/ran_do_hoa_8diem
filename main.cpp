#include <bits/stdc++.h>
#include <raylib.h>
#include <deque>
using namespace std;

// ==================== COLORS & MENU ====================
Color red = {255, 0, 0, 255};
Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};
Color lightGreen = {200, 220, 150, 255};
Color gold = {255, 215, 0, 255};

const int screenWidth = 800;
const int screenHeight = 800;
const int cellSize = 30;
const int cellCount = 20;
const int offset = 75;

const int menuItems = 5;
string menuText[menuItems] = {"NEW GAME", "QUIT GAME", "CREDIT", "ABOUT"};
int selectedItem = 0;
int currentScreen = 0; // 0: Menu, 1: Credit, 2: New Game, 3: Ranking, 4: About

// ==================== SNAKE GAME CLASSES ====================
static bool allowMove = false;
double lastUpdateTime = 0;

Vector2 Vector2Add(Vector2 v1, Vector2 v2) {
    return Vector2{v1.x + v2.x, v1.y + v2.y};
}
bool Vector2Equals(Vector2 v1, Vector2 v2) {
    return (v1.x == v2.x) && (v1.y == v2.y);
}

bool ElementInDeque(Vector2 element, deque<Vector2> deque) {
    for (auto &v : deque)
        if (Vector2Equals(v, element))
            return true;
    return false;
}
bool EventTriggered(double interval) {
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}
void DrawCheckerBackground()
{
    for (int y = 0; y < cellCount; y++)
    {
        for (int x = 0; x < cellCount; x++)
        {
            // Tạo hiệu ứng caro xen kẽ
            if ((x + y) % 2 == 0)
                DrawRectangle(offset + x * cellSize, offset + y * cellSize, cellSize, cellSize, green);
            else
                DrawRectangle(offset + x * cellSize, offset + y * cellSize, cellSize, cellSize, lightGreen);
        }
    }
}
class Snake {
public:
    deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    Vector2 direction = {1, 0};
    bool addSegment = false;

    void Draw() {
        for (auto &segment : body) {
            Rectangle r = {offset + segment.x * cellSize, offset + segment.y * cellSize, (float)cellSize, (float)cellSize};
            DrawRectangleRounded(r, 0.5, 6, darkGreen);
        }
    }

    void Update() {
        body.push_front(Vector2Add(body[0], direction));
        if (!addSegment) body.pop_back();
        else addSegment = false;
    }

    void Reset() {
        body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
        direction = {1, 0};
    }
};

class Food {
public:
    Vector2 position;
    Texture2D texture;

    Food(deque<Vector2> snakeBody) {
        Image image = LoadImage("Graphics/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPos(snakeBody);
    }

    ~Food() { UnloadTexture(texture); }

    void Draw() {
        DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, WHITE);
    }

    Vector2 GenerateRandomCell() {
        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);
        return Vector2{x, y};
    }

    Vector2 GenerateRandomPos(deque<Vector2> snakeBody) {
        Vector2 pos = GenerateRandomCell();
        while (ElementInDeque(pos, snakeBody))
            pos = GenerateRandomCell();
        return pos;
    }
};

class Game {
public:
    Snake snake;
    Food food = Food(snake.body);
    bool running = true;
    int score = 0;
    Sound eatSound, wallSound;

    Game() {
        InitAudioDevice();
        eatSound = LoadSound("Sounds/eat.mp3");
        wallSound = LoadSound("Sounds/wall.mp3");
    }

    ~Game() {
        UnloadSound(eatSound);
        UnloadSound(wallSound);
        CloseAudioDevice();
    }

    void Draw() {
        food.Draw();
        snake.Draw();
        DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5,
                                       (float)cellSize * cellCount + 10,
                                       (float)cellSize * cellCount + 10},
                             5, darkGreen);
        DrawText("Retro Snake", offset - 5, 20, 40, darkGreen);
        DrawText(TextFormat("%i", score), offset - 5, offset + cellSize * cellCount + 10, 40, darkGreen);
    }

    void Update() {
        if (running) {
            snake.Update();
            CheckCollisionWithFood();
            CheckCollisionWithEdges();
            CheckCollisionWithTail();
        }
    }

    void CheckCollisionWithFood() {
        if (Vector2Equals(snake.body[0], food.position)) {
            food.position = food.GenerateRandomPos(snake.body);
            snake.addSegment = true;
            score++;
            PlaySound(eatSound);
        }
    }

    void CheckCollisionWithEdges() {
        if (snake.body[0].x >= cellCount || snake.body[0].x < 0 ||
            snake.body[0].y >= cellCount || snake.body[0].y < 0)
            GameOver();
    }

    void CheckCollisionWithTail() {
        deque<Vector2> headless = snake.body;
        headless.pop_front();
        if (ElementInDeque(snake.body[0], headless)) GameOver();
    }

    void GameOver() {
        snake.Reset();
        food.position = food.GenerateRandomPos(snake.body);
        running = false;
        score = 0;
        PlaySound(wallSound);
    }
};

// Function to draw menu with effects
void DrawMenu()
{
    // Draw background
    ClearBackground(green);

    // vẽ khung
    DrawRectangleLinesEx((Rectangle)
    {
        50, 50, screenWidth - 100, screenHeight - 100
    }, 5, Fade(darkGreen, 0.7f + 0.3f * sin(GetTime()*2)));
    DrawRectangleLinesEx((Rectangle)
    {
        55, 55, screenWidth - 110, screenHeight - 110
    }, 3, Fade(lightGreen, 0.7f + 0.3f * cos(GetTime()*2)));

    // Draw title with shadow and pulse effect
    const char* title = "SWARM_CAMP";
    int titleWidth = MeasureText(title, 60);
    float pulse = 0.5f + 0.5f * sin(GetTime() * 3);

    DrawText(title, screenWidth/2 - titleWidth/2 + 3, 103, 60, Fade(BLACK, 0.3f));
    DrawText(title, screenWidth/2 - titleWidth/2, 100, 60, Color{43, 51, 24, (unsigned char)(255 * pulse)});

    // Draw menu items with selection effects
    for (int i = 0; i < menuItems; i++)
    {
        Color textColor;
        if (i == selectedItem)
        {
            textColor = red;
        }
        else
        {
            textColor = darkGreen;
        }

        int textWidth = MeasureText(menuText[i].c_str(), 40);
        int xPos = screenWidth/2 - textWidth/2;
        int yPos = 250 + i * 70;

        // Draw selection indicators
        if (i == selectedItem)
        {
            float animOffset = 5 * sin(GetTime() * 5);
            DrawText(">", xPos - 40 + animOffset, yPos, 40, gold);
            DrawText("<", xPos + textWidth + 20 - animOffset, yPos, 40, gold);

            // Draw highlight background
            DrawRectangleRounded((Rectangle)
            {
                (float)xPos - 20, (float)yPos - 5, (float)textWidth + 40, 50
            }, 0.5f, 10, Fade(lightGreen, 0.3f));
        }

        // Draw menu text
        DrawText(menuText[i].c_str(), xPos, yPos, 40, textColor);
    }

    // Draw instructions with fading effect
    float fade = 0.5f + 0.5f * sin(GetTime() * 2);
    DrawText("Use UP/DOWN arrows to navigate", screenWidth/2 - MeasureText("Use UP/DOWN arrows to navigate", 20)/2, 600, 20, Fade(darkGreen, fade));
    DrawText("Press ENTER to select", screenWidth/2 - MeasureText("Press ENTER to select", 20)/2, 630, 20, Fade(darkGreen, fade));

}
void DrawNewGame()
{
    // Nền với hiệu ứng gradient
    for (int y = 0; y < screenHeight; y++)
    {
        float ratio = (float)y / screenHeight;
        Color rowColor = Color{
            (unsigned char)(173 * (1 - ratio) + 100 * ratio),
            (unsigned char)(204 * (1 - ratio) + 150 * ratio),
            (unsigned char)(96 * (1 - ratio) + 100 * ratio),
            255
        };
        DrawRectangle(0, y, screenWidth, 1, rowColor);
    }


}
void DrawCredit()
{
    // Nền với hiệu ứng gradient
    for (int y = 0; y < screenHeight; y++)
    {
        float ratio = (float)y / screenHeight;
        Color rowColor = Color{
            (unsigned char)(173 * (1 - ratio) + 100 * ratio),
            (unsigned char)(204 * (1 - ratio) + 150 * ratio),
            (unsigned char)(96 * (1 - ratio) + 100 * ratio),
            255
        };
        DrawRectangle(0, y, screenWidth, 1, rowColor);
    }

    // Vẽ khung viền trang trí
    DrawRectangleRounded((Rectangle){50, 50, screenWidth - 100, screenHeight - 100}, 0.05f, 10, Fade(lightGreen, 0.3f));
 DrawRectangleRoundedLines((Rectangle){50, 50, screenWidth - 100, screenHeight - 100}, 0.05f, 10, darkGreen);
    // Vẽ tiêu đề với shadow effect
    const char* title = "CREDIT";
    int titleWidth = MeasureText(title, 60);
    DrawText(title, screenWidth/2 - titleWidth/2 + 3, 83, 60, Fade(BLACK, 0.3f));
    DrawText(title, screenWidth/2 - titleWidth/2, 80, 60, gold);

    // Vẽ thông tin credit với background cho dễ đọc
    const char* lines[] = {
        "HOC VIEN HOANG GIA PI TI AI TI",
        "TRO CHOI GIUN SAN MOI",
        "DUONG HONG TRIET B24DCAT267",
        "TRAN DUC CUONG B24DCAT037",
        "NGUYEN HAI MINH B24DCAT187"
    };

    for (int i = 0; i < 5; i++)
    {
        int textWidth = MeasureText(lines[i], 28);
        int yPos = 180 + i * 50;

        // Vẽ background cho chữ
        DrawRectangleRounded((Rectangle){
            (float)(screenWidth/2 - textWidth/2 - 10),
            (float)(yPos - 5),
            (float)(textWidth + 20),
            40
        }, 0.3f, 10, Fade(WHITE, 0.7f));

        // Vẽ chữ
        DrawText(lines[i], screenWidth/2 - textWidth/2, yPos, 28, darkGreen);
    }

    // Vẽ nút RETURN với hiệu ứng nổi bật
    const char* returnText = "3. RETURN TO MENU";
    int returnWidth = MeasureText(returnText, 35);

    // Background nút
    DrawRectangleRounded((Rectangle){
        (float)(screenWidth/2 - returnWidth/2 - 15),
        (float)450,
        (float)(returnWidth + 30),
        50
    }, 0.5f, 10, Fade(lightGreen, 0.8f));

    // Viền nút
 DrawRectangleRoundedLines((Rectangle){
    (screenWidth/2 - returnWidth/2 - 15),
    450,
    (returnWidth + 30),
    50
}, 0.5f, 10, darkGreen);

    // Chữ nút
    DrawText(returnText, screenWidth/2 - returnWidth/2, 460, 35, red);

    // Vẽ hướng dẫn
    DrawText("Press 3 to return to main menu",
             screenWidth/2 - MeasureText("Press 3 to return to main menu", 20)/2,
             550, 20, Fade(darkGreen, 0.8f));
}
void DrawAbout()
{
    // Nền với hiệu ứng gradient
    for (int y = 0; y < screenHeight; y++)
    {
        float ratio = (float)y / screenHeight;
        Color rowColor = Color{
            (unsigned char)(173 * (1 - ratio) + 100 * ratio),
            (unsigned char)(204 * (1 - ratio) + 150 * ratio),
            (unsigned char)(96 * (1 - ratio) + 100 * ratio),
            255
        };
        DrawRectangle(0, y, screenWidth, 1, rowColor);
    }

    // Vẽ khung viền trang trí
    DrawRectangleRounded((Rectangle){50, 50, screenWidth - 100, screenHeight - 100}, 0.05f, 10, Fade(lightGreen, 0.3f));
    DrawRectangleRoundedLines((Rectangle){50, 50, screenWidth - 100, screenHeight - 100}, 0.05f, 10, darkGreen);

    // Tiêu đề
    const char* title = "ABOUT";
    int titleWidth = MeasureText(title, 60);
    DrawText(title, screenWidth/2 - titleWidth/2 + 3, 83, 60, Fade(BLACK, 0.3f));
    DrawText(title, screenWidth/2 - titleWidth/2, 80, 60, gold);

    // Nội dung (có thể thêm danh sách xếp hạng sau)
    DrawText("Coming Soon...", screenWidth/2 - MeasureText("Coming Soon...", 40)/2,
             screenHeight/2, 40, darkGreen);

    // Nút return
    DrawText("Press 4 to return to main menu",
             screenWidth/2 - MeasureText("Press 4 to return to main menu", 20)/2,
             screenHeight - 100, 20, Fade(darkGreen, 0.8f));
}

// ==================== DRAW NEW GAME (RUN SNAKE) ====================
void DrawNewGame(Game &game) {
    ClearBackground(green);
   DrawCheckerBackground();
    if (EventTriggered(0.2)) {
        allowMove = true;
        game.Update();
    }

    if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && game.snake.direction.y != 1 && allowMove) {
        game.snake.direction = {0, -1}; game.running = true; allowMove = false;
    }
    if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) && game.snake.direction.y != -1 && allowMove) {
        game.snake.direction = {0, 1}; game.running = true; allowMove = false;
    }
    if ((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) && game.snake.direction.x != 1 && allowMove) {
        game.snake.direction = {-1, 0}; game.running = true; allowMove = false;
    }
    if ((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) && game.snake.direction.x != -1 && allowMove) {
        game.snake.direction = {1, 0}; game.running = true; allowMove = false;
    }

    game.Draw();

}

int main()
{
    InitWindow(screenWidth, screenHeight, "Snake Gameeee");
    SetExitKey(KEY_NULL);
    SetTargetFPS(120);

    Game game; // Tạo game snake

    while (!WindowShouldClose())
    {
        // ================= Xử lý menu =================
        if (currentScreen == 0)
        {
            if (IsKeyPressed(KEY_UP))
            {
                selectedItem--;
                if (selectedItem < 0) selectedItem = menuItems - 1;
            }
            else if (IsKeyPressed(KEY_DOWN))
            {
                selectedItem++;
                if (selectedItem >= menuItems) selectedItem = 0;
            }

            if (IsKeyPressed(KEY_ENTER))
            {
                if (selectedItem == 0)
                {
                    currentScreen = 2; // NEW GAME
                }
                else if (selectedItem == 1)
                {
                    // QUIT GAME
                    CloseWindow();
                    return 0;
                }
                else if (selectedItem == 2)
                {
                    currentScreen = 1; // CREDIT
                }
                else if (selectedItem == 3)
                {
                    currentScreen = 3; // ABOUT
                }
            }
        }

        // ================= Quay lại menu =================
        if (currentScreen == 1 && IsKeyPressed(KEY_THREE))
            currentScreen = 0; // CREDIT → MENU
        if (currentScreen == 3 && IsKeyPressed(KEY_FOUR))
            currentScreen = 0; // ABOUT → MENU

if (currentScreen == 2 && IsKeyPressed(KEY_ESCAPE))
{
    currentScreen = 0;      // Quay lại menu
    game.running = false;   // Dừng cập nhật game
    game.snake.Reset();     // Reset vị trí rắn
    allowMove = false;      // Ngăn di chuyển khi vừa vào lại
}
        // ================= Vẽ =================
        BeginDrawing();

        if (currentScreen == 0)
        {
            DrawMenu();
        }
        else if (currentScreen == 1)
        {
            DrawCredit();
        }
        else if (currentScreen == 2)
        {
            DrawNewGame(game);
        }
        else if (currentScreen == 3)
        {
            DrawAbout();
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
