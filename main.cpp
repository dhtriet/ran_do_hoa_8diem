#include <bits/stdc++.h>
#include <raylib.h>
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
// 0: Basic Map, 1: Maze Map
int selectedMap = 0;
int selectedDifficulty = 1; // 0 = Dễ, 1 = Bình thường, 2 = Khó
float snakeSpeed = 0.1f;
const int menuItems = 5;
string menuText[menuItems] = {"NEW GAME", "QUIT GAME", "CREDIT", "ABOUT","SETTING"};
int selectedItem = 0;
int currentScreen = 0; // 0: Menu, 1: Credit, 2: New Game, 3: Ranking, 4: About

// ==================== SNAKE GAME CLASSES ====================
static bool allowMove = false;
double lastUpdateTime = 0;

Vector2 Vector2Add(Vector2 v1, Vector2 v2)
{
    return Vector2{v1.x + v2.x, v1.y + v2.y};
}
bool Vector2Equals(Vector2 v1, Vector2 v2)
{
    return (v1.x == v2.x) && (v1.y == v2.y);
}
// ==================== MAZE MAP (LABYRINTH STYLE) ====================
vector<Vector2> mazeWalls =
{
    {1,1},{2,1},{3,1},{4,1},{5,1},
    {1,2},{1,3},{1,4},{1,5},{1,6},
    {5,2},{5,3},{5,4},{5,5},{5,6},
    {2,6},{3,6},{4,6},{1,7},{1,8},{2,8},{3,8},{4,8},{5,8},{5,7},{4,7},{3,7},{2,7},{2,2},{2,3},{2,4},{3,2},{3,3},{3,4},{4,2},{4,3},{4,4},{5,2},{5,3},{5,4},{6,1},{6,2},{6,3},{6,4},{6,5},{6,6},{6,7},{6,8},{7,8},{8,8},{9,8},{4,5},{3,5},{2,5},
    {7,1},{8,1},{9,1},{7,2},{8,2},{9,2},{7,3},{8,3},{9,3},{9,5},{9,6},{8,5},{8,6},{10,1},{11,1},{12,1},{13,1},{14,1},{15,1},{16,1},{17,1},{18,1},{10,6},{10,5},{11,6},{11,5},{12,6},{12,5},{13,6},{13,5},{13,4},{13,3},{12,4},{12,3},{11,4},{11,3},{13,7},{12,7},{11,7},{14,7},{15,7},{16,7},{11,8},{11,9},{11,10}
    ,{2,10},{3,10},{4,10},{5,10},{6,10},{7,10},{8,10},{9,10},{10,10},{0,11},{1,11},{2,11},{3,11},{4,11},{5,11},{0,12},{1,12},{2,12},{3,12},{4,12},{5,12},{0,19},{1,19},{2,19},{3,19},{4,19},{5,19},{6,19},{7,19},{8,19},{9,19},{13,19},{14,19},{15,19},{16,19},{17,19},{18,19},{19,19},{13,18},{13,17},{13,16},{13,15},{14,18},{14,17},{14,16},{14,15},{15,18},{15,17},{15,16},{15,15}
    ,{11,18},{11,17},{11,16},{11,15},{11,14},{11,13},{11,12},{10,15},{10,14},{10,13},{10,12},{9,12},{9,13},{9,14},{9,15},{8,12},{8,13},{8,14},{8,15},{7,12},{7,13},{7,14},{7,15},{1,17},{2,17},{3,17},{4,17},{5,17},{1,16},{2,16},{3,16},{4,16},{5,16},{1,15},{2,15},{3,15},{4,15},{5,15},{1,14},{2,14},{3,14},{4,14},{5,14},{6,14},{6,15}
    ,{9,18},{9,17},{8,18},{7,18},{18,2},{17,2},{16,2},{15,2},{18,3},{17,3},{16,3},{15,3},{18,4},{17,4},{16,4},{15,4},{18,5},{17,5},{16,5},{15,5},{18,6},{18,7},{18,8},{18,9},{18,10},{17,10},{16,10},{15,10},{14,10},{13,10},{13,9},{14,9},{15,9},{16,9},{17,9},{18,17}
    ,{18,16},{18,15},{18,14},{18,13},{17,13},{17,14},{17,15},{17,17},{17,16},{16,13},{15,13},{14,13},{13,13},{12,13},{12,12},{13,12},{14,12},{13,11},{14,11}

};
bool EventTriggered(double interval)
{
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval)
    {
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
                DrawRectangle(75 + x * cellSize, 75 + y * cellSize, cellSize, cellSize, green);
            else
                DrawRectangle(75 + x * cellSize, 75 + y * cellSize, cellSize, cellSize, lightGreen);
        }
    }
}
//mecung
void DrawMazeWalls()
{
    for (auto wall : mazeWalls)
    {
        DrawRectangle(
            75 + wall.x * cellSize,
            75 + wall.y * cellSize,
            cellSize,
            cellSize,
            darkGreen
        );
        DrawRectangleLines(
            75 + wall.x * cellSize,
            75 + wall.y * cellSize,
            cellSize,
            cellSize,
            BLACK
        );
    }
}
// ==================== STRUCT ====================
struct Segment
{
    Vector2 pos;
};

class Snake
{
public:
    Segment body[400];
    int length = 3;
    Vector2 direction = {1, 0};
    bool addSegment = false;

    Snake()
    {
        body[0].pos = {6, 9};
        body[1].pos = {5, 9};
        body[2].pos = {4, 9};
    }

    void Draw()
    {
        for (int i = 0; i < length; i++)
        {
            Rectangle r = {75 + body[i].pos.x * cellSize, 75 + body[i].pos.y * cellSize, (float)cellSize, (float)cellSize};
            if (i == 0)
                DrawRectangleRounded(r, 0.5, 6, RED);   // đầu rắn màu đỏ
            else
                DrawRectangleRounded(r, 0.5, 6, darkGreen); // thân rắn màu xanh đậm
        }
    }

    void Update()
    {
        // Dịch mảng từ cuối về đầu
        for (int i = length; i > 0; i--)
            body[i].pos = body[i - 1].pos;

        // Cập nhật đầu
        body[0].pos = Vector2Add(body[0].pos, direction);

        if (!addSegment)
        {
            // nếu không thêm đoạn thì giữ length (chúng ta đã dịch và sẽ effectively bỏ phần cuối bằng không tăng length)
            // nothing extra needed because we wrote into body[length] but didn't increment length
        }
        else
        {
            // nếu thêm đoạn thì tăng length để giữ phần mới
            length++;
            addSegment = false;
        }
    }

    void Reset()
    {
        length = 3;
        body[0].pos = {6, 9};
        body[1].pos = {5, 9};
        body[2].pos = {4, 9};
        direction = {1, 0};
    }
};

class Food
{
public:
    Vector2 position;
    Texture2D texture;

    Food()
    {
        Image image = LoadImage("Graphics/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomCell(); // tạm khởi tạo
    }

    ~Food()
    {
        UnloadTexture(texture);
    }

    void Draw()
    {
        DrawTexture(texture, 75 + position.x * cellSize, 75 + position.y * cellSize, WHITE);
    }

    Vector2 GenerateRandomCell()
{
    int x = GetRandomValue(0, cellCount - 1);
    int y = GetRandomValue(0, cellCount - 1);
    return Vector2{(float)x, (float)y};
}

    // đổi nhận Snake & kiểm tra mảng body
    /* Vector2 GenerateRandomPos(Snake &snake) {
         Vector2 pos = GenerateRandomCell();
         bool conflict = false;
         while (true) {
             conflict = false;
             for (int i = 0; i < snake.length; i++) {
                 if (Vector2Equals(pos, snake.body[i].pos)) {
                     conflict = true;
                     break;
                 }
             }
             if (!conflict) break;
             pos = GenerateRandomCell();
         }
         return pos;
     }*/
//    Vector2 GenerateRandomPos(Snake &snake, bool walls[cellCount][cellCount])
//    {
//        Vector2 pos = GenerateRandomCell();
//        bool conflict = false;
//
//        while (true)
//        {
//            conflict = false;
//
//            // Không được trùng với thân rắn
//            for (int i = 0; i < snake.length; i++)
//            {
//                if (Vector2Equals(pos, snake.body[i].pos))
//                {
//                    conflict = true;
//                    break;
//                }
//            }
//
//            // Không được nằm trong tường
//            if (walls[(int)pos.y][(int)pos.x])
//            {
//                conflict = true;
//            }
//
//            if (!conflict) break; // Nếu vị trí hợp lệ thì dừng
//            pos = GenerateRandomCell(); // Nếu không hợp lệ thì sinh lại
//        }
//
//        return pos;
//    }
Vector2 GenerateRandomPos(Snake &snake, bool walls[cellCount][cellCount])
{
    Vector2 pos;
    bool conflict;
    do {
        conflict = false;
        pos = GenerateRandomCell();

        // Kiểm tra tường
        if (walls[(int)pos.y][(int)pos.x]) conflict = true;

        // Kiểm tra thân rắn
        for (int i = 0; i < snake.length; i++) {
            if (Vector2Equals(pos, snake.body[i].pos)) {
                conflict = true;
                break;
            }
        }

    } while (conflict);
    return pos;
}
};
bool walls[cellCount][cellCount] = {false};

void InitWalls()
{
    for (auto wall : mazeWalls)
    {
        walls[(int)wall.y][(int)wall.x] = true;
    }
}
class Game
{
public:
    Snake snake;
    Food food;
    bool running = true;
    int score = 0;
    int highScore = 0;
    Sound eatSound, wallSound;
    Texture2D appleIcon;
    Texture2D recordIcon;
    Game()
    {
        InitAudioDevice();
        eatSound = LoadSound("Sounds/eat.mp3");
        wallSound = LoadSound("Sounds/wall.mp3");
        Image appleImg = LoadImage("Graphics/tao.png");
        appleIcon = LoadTextureFromImage(appleImg);
        UnloadImage(appleImg);

        Image recordImg = LoadImage("Graphics/cup.png");
        recordIcon = LoadTextureFromImage(recordImg);
        UnloadImage(recordImg);
        // Sinh food lần đầu tránh trùng với rắn
     food.position = food.GenerateRandomPos(snake, walls);
    }

    ~Game()
    {
        UnloadSound(eatSound);
        UnloadSound(wallSound);
        UnloadTexture(appleIcon);
        UnloadTexture(recordIcon);
        CloseAudioDevice();
    }

    void Draw()
    {
        food.Draw();
        snake.Draw();
        DrawRectangleLinesEx(Rectangle{(float)75 - 5, (float)75 - 5,
                                       (float)cellSize * cellCount + 10,
                                       (float)cellSize * cellCount + 10},
                             5, darkGreen);
        DrawText("Retro Snake", 75 - 5, 20, 40, darkGreen);

// Vẽ biểu tượng quả táo
        DrawTexture(appleIcon, 65, 715, WHITE);
        DrawText(TextFormat("%i", score), 100, 725, 30, darkGreen);

// Vẽ biểu tượng kỷ lục
        DrawTexture(recordIcon, 60, 675, WHITE);
        DrawText(TextFormat("%i", highScore), 100, 687, 30, darkGreen);
    }

    void Update()
    {
        if (running)
        {
            snake.Update();
            CheckCollisionWithFood();
            CheckCollisionWithEdges();
            CheckCollisionWithTail();
            //mecung
            if(selectedMap==1)
            CheckCollisionWithMaze();
        }
    }

    void CheckCollisionWithFood()
    {
        if (Vector2Equals(snake.body[0].pos, food.position))
        {
            food.position = food.GenerateRandomPos(snake,walls);
            snake.addSegment = true;
            score++;
            if (score > highScore) highScore = score; // cập nhật kỷ lục
            PlaySound(eatSound);
        }
    }

    void CheckCollisionWithEdges()
    {
        if (snake.body[0].pos.x >= cellCount || snake.body[0].pos.x < 0 ||
                snake.body[0].pos.y >= cellCount || snake.body[0].pos.y < 0)
            GameOver();
    }

    void CheckCollisionWithTail()
    {
        for (int i = 1; i < snake.length; i++)
        {
            if (Vector2Equals(snake.body[0].pos, snake.body[i].pos))
            {
                GameOver();
                break;
            }
        }
    }

    void GameOver()
    {
        snake.Reset();
        food.position = food.GenerateRandomPos(snake,walls);
        running = false;
        score = 0;
        PlaySound(wallSound);
    }
    //mecung
    void CheckCollisionWithMaze()
    {
        for (auto wall : mazeWalls)
        {
            if (Vector2Equals(snake.body[0].pos, wall))
            {
                GameOver();
                break;
            }
        }
    }
};

// Function to draw menu with effects
void DrawMenu()
{
    ClearBackground(green);

    // Khung ngoài
    DrawRectangleLinesEx({50, 50, (float)(screenWidth - 100), (float)(screenHeight - 100)}, 5, Fade(darkGreen, 0.7f + 0.3f * sin(GetTime() * 2)));
    DrawRectangleLinesEx({55, 55, (float)(screenWidth - 110), (float)(screenHeight - 110)}, 3, Fade(lightGreen, 0.7f + 0.3f * cos(GetTime() * 2)));

    // Tiêu đề
    const char* title = "SWARM_CAMP";
    int titleWidth = MeasureText(title, 60);
    float pulse = 0.5f + 0.5f * sin(GetTime() * 3);

    DrawText(title, screenWidth / 2 - titleWidth / 2 + 3, 103, 60, Fade(BLACK, 0.3f));
    DrawText(title, screenWidth / 2 - titleWidth / 2, 100, 60, Color{43, 51, 24, (unsigned char)(255 * pulse)});

    // Lấy vị trí chuột
    Vector2 mousePos = GetMousePosition();

    // Vẽ từng mục menu
    for (int i = 0; i < menuItems; i++)
    {
        int textWidth = MeasureText(menuText[i].c_str(), 40);
        int xPos = screenWidth / 2 - textWidth / 2;
        int yPos = 250 + i * 70;
        Rectangle box = {(float)xPos - 25, (float)yPos - 10, (float)textWidth + 50, 60};

        Color textColor = darkGreen;

        // Nếu chuột đang trỏ vào ô
        if (CheckCollisionPointRec(mousePos, box))
        {
            textColor = Fade(red, 0.8f);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                selectedItem = i; // Chọn mục menu
            }
        }

        // Hiệu ứng khi được chọn
        if (i == selectedItem)
        {
            float animOffset = 5 * sin(GetTime() * 5);
            DrawText(">", xPos - 40 + animOffset, yPos, 40, gold);
            DrawText("<", xPos + textWidth + 20 - animOffset, yPos, 40, gold);
            DrawRectangleRounded(box, 0.5f, 10, Fade(lightGreen, 0.3f));
            textColor = red;
        }

        DrawText(menuText[i].c_str(), xPos, yPos, 40, textColor);
    }

    // Hướng dẫn
    float fade = 0.5f + 0.5f * sin(GetTime() * 2);
    DrawText("Click to select or use UP/DOWN", screenWidth / 2 - MeasureText("Click to select or use UP/DOWN", 20) / 2, 600, 20, Fade(darkGreen, fade));
    DrawText("Press ENTER to confirm", screenWidth / 2 - MeasureText("Press ENTER to confirm", 20) / 2, 630, 20, Fade(darkGreen, fade));
}

void DrawNewGame()
{
    // Nền với hiệu ứng gradient
    for (int y = 0; y < screenHeight; y++)
    {
        float ratio = (float)y / screenHeight;
        Color rowColor = Color
        {
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
        Color rowColor = Color
        {
            (unsigned char)(173 * (1 - ratio) + 100 * ratio),
            (unsigned char)(204 * (1 - ratio) + 150 * ratio),
            (unsigned char)(96 * (1 - ratio) + 100 * ratio),
            255
        };
        DrawRectangle(0, y, screenWidth, 1, rowColor);
    }

    // Vẽ khung viền trang trí
    DrawRectangleRounded((Rectangle)
    {
        50, 50, screenWidth - 100, screenHeight - 100
    }, 0.05f, 10, Fade(lightGreen, 0.3f));
    DrawRectangleRoundedLines((Rectangle)
    {
        50, 50, screenWidth - 100, screenHeight - 100
    }, 0.05f, 10, darkGreen);
    // Vẽ tiêu đề với shadow effect
    const char* title = "CREDIT";
    int titleWidth = MeasureText(title, 60);
    DrawText(title, screenWidth/2 - titleWidth/2 + 3, 83, 60, Fade(BLACK, 0.3f));
    DrawText(title, screenWidth/2 - titleWidth/2, 80, 60, gold);

    // Vẽ thông tin credit với background cho dễ đọc
    const char* lines[] =
    {
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
        DrawRectangleRounded((Rectangle)
        {
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
    DrawRectangleRounded((Rectangle)
    {
        (float)(screenWidth/2 - returnWidth/2 - 15),
        (float)450,
        (float)(returnWidth + 30),
        50
    }, 0.5f, 10, Fade(lightGreen, 0.8f));

    // Viền nút
    DrawRectangleRoundedLines((Rectangle)
    {
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
        Color rowColor = Color
        {
            (unsigned char)(173 * (1 - ratio) + 100 * ratio),
            (unsigned char)(204 * (1 - ratio) + 150 * ratio),
            (unsigned char)(96 * (1 - ratio) + 100 * ratio),
            255
        };
        DrawRectangle(0, y, screenWidth, 1, rowColor);
    }

    // Vẽ khung viền trang trí
    DrawRectangleRounded((Rectangle)
    {
        50, 50, screenWidth - 100, screenHeight - 100
    }, 0.05f, 10, Fade(lightGreen, 0.3f));
    DrawRectangleRoundedLines((Rectangle)
    {
        50, 50, screenWidth - 100, screenHeight - 100
    }, 0.05f, 10, darkGreen);

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
/*void DrawSetting()
{
    // Nền với hiệu ứng gradient
    for (int y = 0; y < screenHeight; y++)
    {
        float ratio = (float)y / screenHeight;
        Color rowColor = Color
        {
            (unsigned char)(173 * (1 - ratio) + 100 * ratio),
            (unsigned char)(204 * (1 - ratio) + 150 * ratio),
            (unsigned char)(96 * (1 - ratio) + 100 * ratio),
            255
        };
        DrawRectangle(0, y, screenWidth, 1, rowColor);
    }

    DrawRectangleRounded((Rectangle)
    {
        50, 50, screenWidth - 100, screenHeight - 100
    }, 0.05f, 10, Fade(lightGreen, 0.3f));
    DrawRectangleRoundedLines((Rectangle)
    {
        50, 50, screenWidth - 100, screenHeight - 100
    }, 0.05f, 10, darkGreen);

    const char* title = "SETTING";
    int titleWidth = MeasureText(title, 60);
    DrawText(title, screenWidth/2 - titleWidth/2 + 3, 83, 60, Fade(BLACK, 0.3f));
    DrawText(title, screenWidth/2 - titleWidth/2, 80, 60, gold);

    // Vẽ lựa chọn bản đồ
    DrawText("CHOOSE MAP:", 220, 200, 35, darkGreen);
    const char* mapNames[2] = {"BASIC MAP", "MAZE MAP"};

    for (int i = 0; i < 2; i++)
    {
        int textWidth = MeasureText(mapNames[i], 30);
        int yPos = 260 + i * 60;
        Color textColor = (i == selectedMap) ? red : darkGreen;

        if (i == selectedMap)
        {
            DrawRectangleRounded((Rectangle)
            {
                (float)(screenWidth/2 - textWidth/2 - 25), (float)(yPos - 5), (float)(textWidth + 50), 45
            }, 0.5f, 10, Fade(lightGreen, 0.4f));
            DrawText(">", screenWidth/2 - textWidth/2 - 50 + 10 * sin(GetTime() * 4), yPos, 30, gold);
        }

        DrawText(mapNames[i], screenWidth/2 - textWidth/2, yPos, 30, textColor);
    }

    // Hướng dẫn
    DrawText("Press UP/DOWN to change map", screenWidth/2 - MeasureText("Press UP/DOWN to change map", 20)/2, 460, 20, darkGreen);
    DrawText("Press 5 to return to main menu", screenWidth/2 - MeasureText("Press 5 to return to main menu", 20)/2, 500, 20, darkGreen);
}*/
void DrawSetting()
{
    // Nền gradient
    for (int y = 0; y < screenHeight; y++)
    {
        float ratio = (float)y / screenHeight;
        Color rowColor = {
            (unsigned char)(173 * (1 - ratio) + 100 * ratio),
            (unsigned char)(204 * (1 - ratio) + 150 * ratio),
            (unsigned char)(96 * (1 - ratio) + 100 * ratio),
            255
        };
        DrawRectangle(0, y, screenWidth, 1, rowColor);
    }

    // Khung chính
    DrawRectangleRounded({50, 50, (float)(screenWidth - 100), (float)(screenHeight - 100)}, 0.05f, 10, Fade(lightGreen, 0.3f));
    DrawRectangleRoundedLines({50, 50, (float)(screenWidth - 100), (float)(screenHeight - 100)}, 0.05f, 10, darkGreen);

    // Tiêu đề
    const char* title = "SETTING";
    int titleWidth = MeasureText(title, 60);
    DrawText(title, screenWidth / 2 - titleWidth / 2 + 3, 83, 60, Fade(BLACK, 0.3f));
    DrawText(title, screenWidth / 2 - titleWidth / 2, 80, 60, gold);

    Vector2 mousePos = GetMousePosition();

    // ========== CHỌN BẢN ĐỒ ==========
    DrawText("CHOOSE MAP (1/2):", 220, 200, 35, darkGreen);
    const char* mapNames[2] = {"BASIC MAP", "MAZE MAP"};

    for (int i = 0; i < 2; i++)
    {
        int textWidth = MeasureText(mapNames[i], 30);
        int yPos = 260 + i * 60;
        Rectangle box = {(float)(screenWidth / 2 - textWidth / 2 - 25), (float)(yPos - 5), (float)(textWidth + 50), 45};
        Color textColor = darkGreen;

        // Kiểm tra hover chuột
        if (CheckCollisionPointRec(mousePos, box))
        {
            textColor = Fade(red, 0.8f);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                selectedMap = i;
        }

        // Highlight map được chọn + hiệu ứng mũi tên động
        if (i == selectedMap)
        {
            DrawRectangleRounded(box, 0.5f, 10, Fade(lightGreen, 0.4f));
            DrawText(">", screenWidth / 2 - textWidth / 2 - 50 + 10 * sin(GetTime() * 4), yPos, 30, gold);
            textColor = red;
        }

        DrawText(mapNames[i], screenWidth / 2 - textWidth / 2, yPos, 30, textColor);
    }

    // ========== CHỌN ĐỘ KHÓ ==========
    DrawText("CHOOSE DIFFICULTY (7/8/9):", 150, 420, 35, darkGreen);
    const char* difficultyNames[3] = {"EASY", "MEDIUM", "HARD"};

    for (int i = 0; i < 3; i++)
    {
        int textWidth = MeasureText(difficultyNames[i], 30);
        int yPos = 480 + i * 50;
        Rectangle box = {(float)(screenWidth / 2 - textWidth / 2 - 25), (float)(yPos - 5), (float)(textWidth + 50), 40};
        Color textColor = darkGreen;

        // Kiểm tra hover chuột
        if (CheckCollisionPointRec(mousePos, box))
        {
            textColor = Fade(red, 0.8f);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                selectedDifficulty = i;
        }

        // Highlight difficulty được chọn + hiệu ứng mũi tên động
        if (i == selectedDifficulty)
        {
            DrawRectangleRounded(box, 0.5f, 10, Fade(lightGreen, 0.4f));
            DrawText(">", screenWidth / 2 - textWidth / 2 - 50 + 10 * sin(GetTime() * 4), yPos, 30, gold);
            textColor = red;
        }

        DrawText(difficultyNames[i], screenWidth / 2 - textWidth / 2, yPos, 30, textColor);
    }

    // Hướng dẫn nhập số và quay về menu
    DrawText("Press 1/2 to choose map", screenWidth / 2 - MeasureText("Press 1/2 to choose map", 20)/2, 650, 20, darkGreen);
  DrawText("Press 7/8/9 to choose difficulty", screenWidth / 2 - MeasureText("Press 7/8/9 to choose difficulty", 20)/2 , 670, 20, darkGreen);
    DrawText("Press 5 to return to main menu", screenWidth / 2 - MeasureText("Press 5 to return to main menu", 20)/2, 690, 20, darkGreen);
}

// ==================== DRAW NEW GAME (RUN SNAKE) ====================
void DrawNewGame(Game &game)
{
    ClearBackground(green);
    if (selectedMap == 0)
    {
        DrawCheckerBackground(); // Map cơ bản
    }
    else
    {
        DrawCheckerBackground();
        DrawMazeWalls(); // Map mê cung
    }
     double snakeSpeed = 0.1;

if (selectedDifficulty == 0) {
    snakeSpeed = 0.5; // Dễ
}
else if (selectedDifficulty == 1) {
    snakeSpeed = 0.25; // Trung bình
}
else {
    snakeSpeed = 0.1; // Khó
}
    if (EventTriggered(snakeSpeed))
    {
        allowMove = true;
        game.Update();
    }

    if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && game.snake.direction.y != 1 && allowMove)
    {
        game.snake.direction = {0, -1};
        game.running = true;
        allowMove = false;
    }
    if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) && game.snake.direction.y != -1 && allowMove)
    {
        game.snake.direction = {0, 1};
        game.running = true;
        allowMove = false;
    }
    if ((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) && game.snake.direction.x != 1 && allowMove)
    {
        game.snake.direction = {-1, 0};
        game.running = true;
        allowMove = false;
    }
    if ((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) && game.snake.direction.x != -1 && allowMove)
    {
        game.snake.direction = {1, 0};
        game.running = true;
        allowMove = false;
    }

    game.Draw();

}

int main()
{
    InitWindow(screenWidth, screenHeight, "Snake Gameeee");
    InitWalls();
    SetExitKey(KEY_NULL);
    SetTargetFPS(0);

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
                else if (selectedItem == 4)
                {
                    currentScreen = 4; // SETTING
                }
            }
        }

        // ================= Quay lại menu =================
        if (currentScreen == 1 && IsKeyPressed(KEY_THREE))
            currentScreen = 0; // CREDIT → MENU
        if (currentScreen == 3 && IsKeyPressed(KEY_FOUR))
            currentScreen = 0; // ABOUT → MENU
        if (currentScreen == 4 && IsKeyPressed(KEY_FIVE))
            currentScreen = 0; // SETTING → MENU

        if (currentScreen == 2 && IsKeyPressed(KEY_ESCAPE))
        {
            currentScreen = 0;      // Quay lại menu
            game.running = false;   // Dừng cập nhật game
            game.snake.Reset();     // Reset vị trí rắn
            allowMove = false;      // Ngăn di chuyển khi vừa vào lại
        }
        if (currentScreen == 4)
        {
        // Xử lý input bằng số
    if (IsKeyPressed(KEY_ONE)) selectedMap = 0;   // 1 → BASIC MAP
    if (IsKeyPressed(KEY_TWO)) selectedMap = 1;   // 2 → MAZE MAP

    if (IsKeyPressed(KEY_SEVEN)) selectedDifficulty = 0; // 7 → EASY
    if (IsKeyPressed(KEY_EIGHT)) selectedDifficulty = 1; // 8 → MEDIUM
    if (IsKeyPressed(KEY_NINE)) selectedDifficulty = 2;  // 9 → HARD

    // Quay về menu chính
    if (IsKeyPressed(KEY_FIVE)) currentScreen = 0;
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
        else if (currentScreen == 4)
        {
            DrawSetting();
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
