#include <bits/stdc++.h>
#include <raylib.h>
using namespace std;

// ==================== COLORS & MENU ====================
Color mau_do = {255, 0, 0, 255};
Color mau_xanh = {173, 204, 96, 255};
Color xanh_dam = {43, 51, 24, 255};
Color xanh_nhat = {200, 220, 150, 255};
Color mau_ran = {60, 100, 40, 255};
Color mau_vang = {255, 215, 0, 255};

const int chieu_rong_man_hinh = 800;
const int chieu_cao_man_hinh = 800;
const int kich_thuoc_o = 30;
const int so_o = 20;
// 0: Basic Map, 1: Maze Map
int ban_do_duoc_chon = 0;
int do_kho_duoc_chon = 1; // 0 = Dễ, 1 = Bình thường, 2 = Khó
float toc_do_ran = 0.1f;
const int so_item_menu = 5;
string text_menu[so_item_menu] = {"NEW GAME", "QUIT GAME", "CREDIT", "ABOUT","SETTING"};
int item_duoc_chon = 0;
int man_hinh_hien_tai = 0; // 0: Menu, 1: Credit, 2: New tro_choi, 3: Ranking, 4: About

// ==================== SNAKE GAME CLASSES ====================
bool cho_phep_di_chuyen = false;
double thoi_gian_cap_nhat_cuoi = 0;

Vector2 cong_vector2(Vector2 v1, Vector2 v2)
{
    return Vector2{v1.x + v2.x, v1.y + v2.y};
}
bool so_sanh_vector2(Vector2 v1, Vector2 v2)
{
    return (v1.x == v2.x) && (v1.y == v2.y);
}
// ==================== MAZE MAP (LABYRINTH STYLE) ====================
vector<Vector2> tuong_me_cung =
{
    {1,1},{2,1},{3,1},{4,1},{5,1},
    {1,2},{1,3},{1,4},{1,5},{1,6},
    {5,2},{5,3},{5,4},{5,5},{5,6},
    {2,6},{3,6},{4,6},{1,7},{1,8},
    {2,8},{3,8},{4,8},{5,8},{5,7},
    {4,7},{3,7},{2,7},{2,2},{2,3},
    {2,4},{3,2},{3,3},{3,4},{4,2},
    {4,3},{4,4},{5,2},{5,3},{5,4},
    {6,1},{6,2},{6,3},{6,4},{6,5},
    {6,6},{6,7},{6,8},{7,8},{8,8},
    {9,8},{4,5},{3,5},{2,5},{7,1},
    {8,1},{9,1},{7,2},{8,2},{9,2},
    {7,3},{8,3},{9,3},{9,5},{9,6},
    {8,5},{8,6},{10,1},{11,1},{12,1},
    {13,1},{14,1},{15,1},{16,1},{17,1},
    {18,1},{10,6},{10,5},{11,6},{11,5},
    {12,6},{12,5},{13,6},{13,5},{13,4},
    {13,3},{12,4},{12,3},{11,4},{11,3},
    {13,7},{12,7},{11,7},{14,7},{15,7},
    {16,7},{11,8},{11,9},{11,10},{2,10},
    {3,10},{4,10},{5,10},{6,10},{7,10},
    {8,10},{9,10},{10,10},{0,11},{1,11},
    {2,11},{3,11},{4,11},{5,11},{0,12},
    {1,12},{2,12},{3,12},{4,12},{5,12},
    {0,19},{1,19},{2,19},{3,19},{4,19},
    {5,19},{6,19},{7,19},{8,19},{9,19},
    {13,19},{14,19},{15,19},{16,19},{17,19},
    {18,19},{19,19},{13,18},{13,17},{13,16},
    {13,15},{14,18},{14,17},{14,16},{14,15},
    {15,18},{15,17},{15,16},{15,15},{11,18},
    {11,17},{11,16},{11,15},{11,14},{11,13},
    {11,12},{10,15},{10,14},{10,13},{10,12},
    {9,12},{9,13},{9,14},{9,15},{8,12},{8,13},
    {8,14},{8,15},{7,12},{7,13},{7,14},{7,15},
    {1,17},{2,17},{3,17},{4,17},{5,17},{1,16},
    {2,16},{3,16},{4,16},{5,16},{1,15},{2,15},
    {3,15},{4,15},{5,15},{1,14},{2,14},{3,14},
    {4,14},{5,14},{6,14},{6,15},{9,18},{9,17},
    {8,18},{7,18},{18,2},{17,2},{16,2},{15,2},
    {18,3},{17,3},{16,3},{15,3},{18,4},{17,4},
    {16,4},{15,4},{18,5},{17,5},{16,5},{15,5},
    {18,6},{18,7},{18,8},{18,9},{18,10},{17,10},
    {16,10},{15,10},{14,10},{13,10},{13,9},{14,9},
    {15,9},{16,9},{17,9},{18,17},{18,16},{18,15},
    {18,14},{18,13},{17,13},{17,14},{17,15},
    {17,17},{17,16},{16,13},{15,13},{14,13},
    {13,13},{12,13},{12,12},{13,12},{14,12},{13,11},{14,11}
};
bool su_kien_duoc_kich_hoat(double interval)
{
    double currentTime = GetTime();
    if (currentTime - thoi_gian_cap_nhat_cuoi >= interval)
    {
        thoi_gian_cap_nhat_cuoi = currentTime;
        return true;
    }
    return false;
}
void ve_nen_caro()
{
    for (int y = 0; y < so_o; y++)
    {
        for (int x = 0; x < so_o; x++)
        {
            // Tạo hiệu ứng caro xen kẽ
            if ((x + y) % 2 == 0)
                DrawRectangle(75 + x * kich_thuoc_o, 75 + y * kich_thuoc_o, kich_thuoc_o, kich_thuoc_o, mau_xanh);
            else
                DrawRectangle(75 + x * kich_thuoc_o, 75 + y * kich_thuoc_o, kich_thuoc_o, kich_thuoc_o, xanh_nhat);
        }
    }
}
//mecung
void ve_tuong_me_cung()
{
    for (auto wall : tuong_me_cung)
    {
        DrawRectangle(
            75 + wall.x * kich_thuoc_o,
            75 + wall.y * kich_thuoc_o,
            kich_thuoc_o,
            kich_thuoc_o,
            xanh_dam
        );
        DrawRectangleLines(
            75 + wall.x * kich_thuoc_o,
            75 + wall.y * kich_thuoc_o,
            kich_thuoc_o,
            kich_thuoc_o,
            BLACK
        );
    }
}
// ==================== STRUCT ====================
struct doan
{
    Vector2 vi_tri;
};

class ran
{
public:
    doan than[400];
    int do_dai = 3;
    Vector2 huong = {1, 0};
    bool them_doan = false;

    Texture2D texHead;
    Texture2D texBody;

    ran()
    {
        // Khởi tạo vị trí ban đầu
        than[0].vi_tri = {6, 9};
        than[1].vi_tri = {5, 9};
        than[2].vi_tri = {4, 9};

        // Nạp ảnh đầu và thân
        texHead = LoadTexture("Graphics/Head.png");
        texBody = LoadTexture("Graphics/Body.png");
    }

    ~ran()
    {
        UnloadTexture(texHead);
        UnloadTexture(texBody);
    }

    void Draw()
    {
        for (int i = 0; i < do_dai; i++)
        {
            Vector2 pos = than[i].vi_tri;
           Rectangle destRec = {
    75 + pos.x * kich_thuoc_o + kich_thuoc_o / 2.0f,
    75 + pos.y * kich_thuoc_o + kich_thuoc_o / 2.0f,
    (float)kich_thuoc_o,
    (float)kich_thuoc_o
};
            Rectangle srcRec = {0, 0, (float)texBody.width, (float)texBody.height};
            float rotation = 0.0f;

            if (i == 0)
            {
                // Đầu rắn
                srcRec.width = texHead.width;
                srcRec.height = texHead.height;

              if (huong.x == 1) rotation = 90;    // đi sang phải → xoay -90
else if (huong.x == -1) rotation = -90; // đi sang trái → xoay +90
else if (huong.y == -1) rotation = 0;  // đi lên → đúng hướng ảnh
else if (huong.y == 1) rotation = 180; // đi xuống → xoay 180

                DrawTexturePro(texHead, srcRec, destRec,
                               Vector2{kich_thuoc_o / 2.0f, kich_thuoc_o / 2.0f},
                               rotation, WHITE);
            }
            else
            {
                // Thân rắn
                DrawTexturePro(texBody, srcRec, destRec,
                               Vector2{kich_thuoc_o / 2.0f, kich_thuoc_o / 2.0f},
                               0, WHITE);
            }
        }
    }

    void Update()
    {
        for (int i = do_dai; i > 0; i--)
            than[i].vi_tri = than[i - 1].vi_tri;

        than[0].vi_tri = cong_vector2(than[0].vi_tri, huong);

        if (them_doan)
        {
            do_dai++;
            them_doan = false;
        }
    }

    void Reset()
    {
        do_dai = 3;
        than[0].vi_tri = {6, 9};
        than[1].vi_tri = {5, 9};
        than[2].vi_tri = {4, 9};
        huong = {1, 0};
    }
};

class thuc_an
{
public:
    Vector2 vi_tri_thuc_an;
    Texture2D hinh_texture;

    thuc_an()
    {
        Image image = LoadImage("Graphics/food.png");
        hinh_texture = LoadTextureFromImage(image);
        UnloadImage(image);
        vi_tri_thuc_an = sinh_o_ngau_nhien(); // tạm khởi tạo
    }

    ~thuc_an()
    {
        UnloadTexture(hinh_texture);
    }

    void Draw()
    {
        DrawTexture(hinh_texture, 75 + vi_tri_thuc_an.x * kich_thuoc_o, 75 + vi_tri_thuc_an.y * kich_thuoc_o, WHITE);
    }

    Vector2 sinh_o_ngau_nhien()
{
    int x = GetRandomValue(0, so_o - 1);
    int y = GetRandomValue(0, so_o - 1);
    return Vector2{(float)x, (float)y};
}

    // đổi nhận ran & kiểm tra mảng than
    /* Vector2 sinh_vi_tri_ngau_nhien(ran &snake) {
         Vector2 vi_tri = sinh_o_ngau_nhien();
         bool conflict = false;
         while (true) {
             conflict = false;
             for (int i = 0; i < snake.do_dai; i++) {
                 if (so_sanh_vector2(vi_tri, snake.than[i].vi_tri)) {
                     conflict = true;
                     break;
                 }
             }
             if (!conflict) break;
             vi_tri = sinh_o_ngau_nhien();
         }
         return vi_tri;
     }*/
//    Vector2 sinh_vi_tri_ngau_nhien(ran &snake, bool mang_tuong[so_o][so_o])
//    {
//        Vector2 vi_tri = sinh_o_ngau_nhien();
//        bool conflict = false;
//
//        while (true)
//        {
//            conflict = false;
//
//            // Không được trùng với thân rắn
//            for (int i = 0; i < snake.do_dai; i++)
//            {
//                if (so_sanh_vector2(vi_tri, snake.than[i].vi_tri))
//                {
//                    conflict = true;
//                    break;
//                }
//            }
//
//            // Không được nằm trong tường
//            if (mang_tuong[(int)vi_tri.y][(int)vi_tri.x])
//            {
//                conflict = true;
//            }
//
//            if (!conflict) break; // Nếu vị trí hợp lệ thì dừng
//            vi_tri = sinh_o_ngau_nhien(); // Nếu không hợp lệ thì sinh lại
//        }
//
//        return vi_tri;
//    }
Vector2 sinh_vi_tri_ngau_nhien(ran &snake, bool mang_tuong[so_o][so_o])
{
    Vector2 vi_tri;
    bool conflict;
    do {
        conflict = false;
        vi_tri = sinh_o_ngau_nhien();

        // Kiểm tra tường
        if (mang_tuong[(int)vi_tri.y][(int)vi_tri.x]) conflict = true;

        // Kiểm tra thân rắn
        for (int i = 0; i < snake.do_dai; i++) {
            if (so_sanh_vector2(vi_tri, snake.than[i].vi_tri)) {
                conflict = true;
                break;
            }
        }

    } while (conflict);
    return vi_tri;
}
};
bool mang_tuong[so_o][so_o] = {false};

void khoi_tao_tuong()
{
    for (auto wall : tuong_me_cung)
    {
        mang_tuong[(int)wall.y][(int)wall.x] = true;
    }
}
class tro_choi
{
public:
    ran snake;
    thuc_an food;
    bool dang_choi = true;
    int diem = 0;
    int diem_cao = 0;
    Sound am_thanh_an, am_thanh_va_tuong;
    Texture2D icon_tao;
    Texture2D icon_cup;
    tro_choi()
    {
        InitAudioDevice();
        am_thanh_an = LoadSound("Sounds/eat.mp3");
        am_thanh_va_tuong = LoadSound("Sounds/wall.mp3");
        Image appleImg = LoadImage("Graphics/tao.png");
        icon_tao = LoadTextureFromImage(appleImg);
        UnloadImage(appleImg);

        Image recordImg = LoadImage("Graphics/cup.png");
        icon_cup = LoadTextureFromImage(recordImg);
        UnloadImage(recordImg);
        // Sinh food lần đầu tránh trùng với rắn
     food.vi_tri_thuc_an = food.sinh_vi_tri_ngau_nhien(snake, mang_tuong);
    }

    ~tro_choi()
    {
        UnloadSound(am_thanh_an);
        UnloadSound(am_thanh_va_tuong);
        UnloadTexture(icon_tao);
        UnloadTexture(icon_cup);
        CloseAudioDevice();
    }

    void Draw()
    {
        food.Draw();
        snake.Draw();
        DrawRectangleLinesEx(Rectangle{(float)75 - 5, (float)75 - 5,
                                       (float)kich_thuoc_o * so_o + 10,
                                       (float)kich_thuoc_o * so_o + 10},
                             5, xanh_dam);
        DrawText("Retro ran", 75 - 5, 20, 40, xanh_dam);

// Vẽ biểu tượng quả táo
        DrawTexture(icon_tao, 65, 715, WHITE);
        DrawText(TextFormat("%i", diem), 100, 725, 30, xanh_dam);

// Vẽ biểu tượng kỷ lục
        DrawTexture(icon_cup, 60, 675, WHITE);
        DrawText(TextFormat("%i", diem_cao), 100, 687, 30, xanh_dam);
    }

    void Update()
    {
        if (dang_choi)
        {
            snake.Update();
            kiem_tra_va_cham_thuc_an();
            kiem_tra_va_cham_bien();
            kiem_tra_va_cham_duoi();
            //mecung
            if(ban_do_duoc_chon==1)
            kiem_tra_va_cham_me_cung();
        }
    }

    void kiem_tra_va_cham_thuc_an()
    {
        if (so_sanh_vector2(snake.than[0].vi_tri, food.vi_tri_thuc_an))
        {
            food.vi_tri_thuc_an = food.sinh_vi_tri_ngau_nhien(snake,mang_tuong);
            snake.them_doan = true;
            diem++;
            if (diem > diem_cao) diem_cao = diem; // cập nhật kỷ lục
            PlaySound(am_thanh_an);
        }
    }

    void kiem_tra_va_cham_bien()
    {
        if (snake.than[0].vi_tri.x >= so_o || snake.than[0].vi_tri.x < 0 ||
                snake.than[0].vi_tri.y >= so_o || snake.than[0].vi_tri.y < 0)
            ket_thuc_game();
    }

    void kiem_tra_va_cham_duoi()
    {
        for (int i = 1; i < snake.do_dai; i++)
        {
            if (so_sanh_vector2(snake.than[0].vi_tri, snake.than[i].vi_tri))
            {
                ket_thuc_game();
                break;
            }
        }
    }

    void ket_thuc_game()
    {
        snake.Reset();
        food.vi_tri_thuc_an = food.sinh_vi_tri_ngau_nhien(snake,mang_tuong);
        dang_choi = false;
        diem = 0;
        PlaySound(am_thanh_va_tuong);
    }
    //mecung
    void kiem_tra_va_cham_me_cung()
    {
        for (auto wall : tuong_me_cung)
        {
            if (so_sanh_vector2(snake.than[0].vi_tri, wall))
            {
                ket_thuc_game();
                break;
            }
        }
    }
};

// Function to draw menu with effects
void ve_menu()
{
    ClearBackground(mau_xanh);

    // Khung ngoài
    DrawRectangleLinesEx({50, 50, (float)(chieu_rong_man_hinh - 100), (float)(chieu_cao_man_hinh - 100)}, 5, Fade(xanh_dam, 0.7f + 0.3f * sin(GetTime() * 2)));
    DrawRectangleLinesEx({55, 55, (float)(chieu_rong_man_hinh - 110), (float)(chieu_cao_man_hinh - 110)}, 3, Fade(xanh_nhat, 0.7f + 0.3f * cos(GetTime() * 2)));

    // Tiêu đề
    const char* title = "SWARM_CAMP";
    int titleWidth = MeasureText(title, 60);
    float pulse = 0.5f + 0.5f * sin(GetTime() * 3);

    DrawText(title, chieu_rong_man_hinh / 2 - titleWidth / 2 + 3, 103, 60, Fade(BLACK, 0.3f));
    DrawText(title, chieu_rong_man_hinh / 2 - titleWidth / 2, 100, 60, Color{43, 51, 24, (unsigned char)(255 * pulse)});

    // Lấy vị trí chuột
    Vector2 mousePos = GetMousePosition();

    // Vẽ từng mục menu
    for (int i = 0; i < so_item_menu; i++)
    {
        int textWidth = MeasureText(text_menu[i].c_str(), 40);
        int xPos = chieu_rong_man_hinh / 2 - textWidth / 2;
        int yPos = 250 + i * 70;
        Rectangle box = {(float)xPos - 25, (float)yPos - 10, (float)textWidth + 50, 60};

        Color textColor = xanh_dam;

        // Nếu chuột đang trỏ vào ô
        if (CheckCollisionPointRec(mousePos, box))
        {
            textColor = Fade(mau_do, 0.8f);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                item_duoc_chon = i; // Chọn mục menu
            }
        }

        // Hiệu ứng khi được chọn
        if (i == item_duoc_chon)
        {
            float animOffset = 5 * sin(GetTime() * 5);
            DrawText(">", xPos - 40 + animOffset, yPos, 40, mau_vang);
            DrawText("<", xPos + textWidth + 20 - animOffset, yPos, 40, mau_vang);
            DrawRectangleRounded(box, 0.5f, 10, Fade(xanh_nhat, 0.3f));
            textColor = mau_do;
        }

        DrawText(text_menu[i].c_str(), xPos, yPos, 40, textColor);
    }

    // Hướng dẫn
    float fade = 0.5f + 0.5f * sin(GetTime() * 2);
    DrawText("Click to select or use UP/DOWN", chieu_rong_man_hinh / 2 - MeasureText("Click to select or use UP/DOWN", 20) / 2, 600, 20, Fade(xanh_dam, fade));
    DrawText("Press ENTER to confirm", chieu_rong_man_hinh / 2 - MeasureText("Press ENTER to confirm", 20) / 2, 630, 20, Fade(xanh_dam, fade));
}

void ve_choi_moi()
{
    // Nền với hiệu ứng gradient
    for (int y = 0; y < chieu_cao_man_hinh; y++)
    {
        float ratio = (float)y / chieu_cao_man_hinh;
        Color rowColor = Color
        {
            (unsigned char)(173 * (1 - ratio) + 100 * ratio),
            (unsigned char)(204 * (1 - ratio) + 150 * ratio),
            (unsigned char)(96 * (1 - ratio) + 100 * ratio),
            255
        };
        DrawRectangle(0, y, chieu_rong_man_hinh, 1, rowColor);
    }


}
void ve_credit()
{
    // Nền với hiệu ứng gradient
    for (int y = 0; y < chieu_cao_man_hinh; y++)
    {
        float ratio = (float)y / chieu_cao_man_hinh;
        Color rowColor = Color
        {
            (unsigned char)(173 * (1 - ratio) + 100 * ratio),
            (unsigned char)(204 * (1 - ratio) + 150 * ratio),
            (unsigned char)(96 * (1 - ratio) + 100 * ratio),
            255
        };
        DrawRectangle(0, y, chieu_rong_man_hinh, 1, rowColor);
    }

    // Vẽ khung viền trang trí
    DrawRectangleRounded((Rectangle)
    {
        50, 50, chieu_rong_man_hinh - 100, chieu_cao_man_hinh - 100
    }, 0.05f, 10, Fade(xanh_nhat, 0.3f));
    DrawRectangleRoundedLines((Rectangle)
    {
        50, 50, chieu_rong_man_hinh - 100, chieu_cao_man_hinh - 100
    }, 0.05f, 10, xanh_dam);
    // Vẽ tiêu đề với shadow effect
    const char* title = "CREDIT";
    int titleWidth = MeasureText(title, 60);
    DrawText(title, chieu_rong_man_hinh/2 - titleWidth/2 + 3, 83, 60, Fade(BLACK, 0.3f));
    DrawText(title, chieu_rong_man_hinh/2 - titleWidth/2, 80, 60, mau_vang);

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
            (float)(chieu_rong_man_hinh/2 - textWidth/2 - 10),
            (float)(yPos - 5),
            (float)(textWidth + 20),
            40
        }, 0.3f, 10, Fade(WHITE, 0.7f));

        // Vẽ chữ
        DrawText(lines[i], chieu_rong_man_hinh/2 - textWidth/2, yPos, 28, xanh_dam);
    }

    // Vẽ nút RETURN với hiệu ứng nổi bật
    const char* returnText = "3. RETURN TO MENU";
    int returnWidth = MeasureText(returnText, 35);

    // Background nút
    DrawRectangleRounded((Rectangle)
    {
        (float)(chieu_rong_man_hinh/2 - returnWidth/2 - 15),
        (float)450,
        (float)(returnWidth + 30),
        50
    }, 0.5f, 10, Fade(xanh_nhat, 0.8f));

    // Viền nút
    DrawRectangleRoundedLines((Rectangle)
    {
        (chieu_rong_man_hinh/2 - returnWidth/2 - 15),
        450,
        (returnWidth + 30),
        50
    }, 0.5f, 10, xanh_dam);

    // Chữ nút
    DrawText(returnText, chieu_rong_man_hinh/2 - returnWidth/2, 460, 35, mau_do);

    // Vẽ hướng dẫn
    DrawText("Press 3 to return to main menu",
             chieu_rong_man_hinh/2 - MeasureText("Press 3 to return to main menu", 20)/2,
             550, 20, Fade(xanh_dam, 0.8f));
}
void ve_about()
{
    // ===== NỀN GRADIENT =====
    for (int y = 0; y < chieu_cao_man_hinh; y++)
    {
        float ratio = (float)y / chieu_cao_man_hinh;
        Color rowColor = {
            (unsigned char)(173 * (1 - ratio) + 120 * ratio),
            (unsigned char)(204 * (1 - ratio) + 180 * ratio),
            (unsigned char)(96 * (1 - ratio) + 110 * ratio),
            255
        };
        DrawRectangle(0, y, chieu_rong_man_hinh, 1, rowColor);
    }

    // ===== KHUNG =====
    float margin = 60;
    Rectangle khung = {margin, margin, (float)(chieu_rong_man_hinh - margin * 2), (float)(chieu_cao_man_hinh - margin * 2)};
    DrawRectangleRounded(khung, 0.05f, 10, Fade(xanh_nhat, 0.25f));
    DrawRectangleRoundedLines(khung, 0.05f, 10, xanh_dam);

    // ===== TIÊU ĐỀ =====
    const char *title = "TUTORIAL";
    int titleSize = 64;
    int titleWidth = MeasureText(title, titleSize);
    int titleX = chieu_rong_man_hinh / 2 - titleWidth / 2;
    DrawText(title, titleX + 3, 83, titleSize, Fade(BLACK, 0.4f));
    DrawText(title, titleX, 80, titleSize, mau_vang);

    // ===== NỘI DUNG =====
    int left = khung.x + 120;
    int top = 200;
    int fontSize = 28;
    int lineSpace = 38;
    int maxWidth = chieu_rong_man_hinh - 300; // độ rộng vùng text
    Color textColor = xanh_dam;

    const char *noi_dung[] = {
        "You can move up, left, down, or right using the arrow keys or W A S D.",
        "There are two simple rules you must follow when playing: do not hit a wall and do not bite your own tail.",
        "Crashing into a wall or your tail will end the game immediately."
    };

    int so_dong = sizeof(noi_dung) / sizeof(noi_dung[0]);

    // Hàm tạm: ngắt dòng khi câu vượt maxWidth
    auto drawWrappedText = [&](const char *text, int x, int &y, int fontSize, int lineSpace, int maxWidth, Color color)
    {
        const char *start = text;
        char line[256] = "";
        int textWidth = 0;
        while (*start)
        {
            int len = 0;
            line[0] = '\0';
            const char *lastSpace = nullptr;

            while (*start && textWidth < maxWidth)
            {
                line[len++] = *start;
                line[len] = '\0';
                textWidth = MeasureText(line, fontSize);
                if (*start == ' ') lastSpace = start;
                start++;
                if (textWidth >= maxWidth && lastSpace)
                {
                    start = lastSpace + 1;
                    line[lastSpace - text + 1] = '\0';
                    break;
                }
            }
            DrawText(line, x, y, fontSize, color);
            y += lineSpace;
            textWidth = 0;
        }
    };

    int y = top;
    for (int i = 0; i < so_dong; i++)
    {
        drawWrappedText(noi_dung[i], left, y, fontSize, lineSpace, maxWidth, textColor);
        y += 10; // khoảng cách giữa các đoạn
    }

    // ===== NÚT QUAY LẠI =====
    const char *back = "Press 4 to return to main menu";
    int backWidth = MeasureText(back, 22);
    DrawText(back,
             chieu_rong_man_hinh / 2 - backWidth / 2,
             chieu_cao_man_hinh - 90, 22, Fade(xanh_dam, 0.9f));
}

void ve_cai_dat()
{
    // Nền gradient
    for (int y = 0; y < chieu_cao_man_hinh; y++)
    {
        float ratio = (float)y / chieu_cao_man_hinh;
        Color rowColor = {
            (unsigned char)(173 * (1 - ratio) + 100 * ratio),
            (unsigned char)(204 * (1 - ratio) + 150 * ratio),
            (unsigned char)(96 * (1 - ratio) + 100 * ratio),
            255
        };
        DrawRectangle(0, y, chieu_rong_man_hinh, 1, rowColor);
    }

    // Khung chính
    DrawRectangleRounded({50, 50, (float)(chieu_rong_man_hinh - 100), (float)(chieu_cao_man_hinh - 100)}, 0.05f, 10, Fade(xanh_nhat, 0.3f));
    DrawRectangleRoundedLines({50, 50, (float)(chieu_rong_man_hinh - 100), (float)(chieu_cao_man_hinh - 100)}, 0.05f, 10, xanh_dam);

    // Tiêu đề
    const char* title = "SETTING";
    int titleWidth = MeasureText(title, 60);
    DrawText(title, chieu_rong_man_hinh / 2 - titleWidth / 2 + 3, 83, 60, Fade(BLACK, 0.3f));
    DrawText(title, chieu_rong_man_hinh / 2 - titleWidth / 2, 80, 60, mau_vang);

    Vector2 mousePos = GetMousePosition();

    // ========== CHỌN BẢN ĐỒ ==========
    DrawText("CHOOSE MAP (1/2):", 220, 200, 35, xanh_dam);
    const char* mapNames[2] = {"BASIC MAP", "MAZE MAP"};

    for (int i = 0; i < 2; i++)
    {
        int textWidth = MeasureText(mapNames[i], 30);
        int yPos = 260 + i * 60;
        Rectangle box = {(float)(chieu_rong_man_hinh / 2 - textWidth / 2 - 25), (float)(yPos - 5), (float)(textWidth + 50), 45};
        Color textColor = xanh_dam;

        // Kiểm tra hover chuột
        if (CheckCollisionPointRec(mousePos, box))
        {
            textColor = Fade(mau_do, 0.8f);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                ban_do_duoc_chon = i;
        }

        // Highlight map được chọn + hiệu ứng mũi tên động
        if (i == ban_do_duoc_chon)
        {
            DrawRectangleRounded(box, 0.5f, 10, Fade(xanh_nhat, 0.4f));
            DrawText(">", chieu_rong_man_hinh / 2 - textWidth / 2 - 50 + 10 * sin(GetTime() * 4), yPos, 30, mau_vang);
            textColor = mau_do;
        }

        DrawText(mapNames[i], chieu_rong_man_hinh / 2 - textWidth / 2, yPos, 30, textColor);
    }

    // ========== CHỌN ĐỘ KHÓ ==========
    DrawText("CHOOSE DIFFICULTY (7/8/9):", 150, 420, 35, xanh_dam);
    const char* difficultyNames[3] = {"EASY", "MEDIUM", "HARD"};

    for (int i = 0; i < 3; i++)
    {
        int textWidth = MeasureText(difficultyNames[i], 30);
        int yPos = 480 + i * 50;
        Rectangle box = {(float)(chieu_rong_man_hinh / 2 - textWidth / 2 - 25), (float)(yPos - 5), (float)(textWidth + 50), 40};
        Color textColor = xanh_dam;

        // Kiểm tra hover chuột
        if (CheckCollisionPointRec(mousePos, box))
        {
            textColor = Fade(mau_do, 0.8f);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                do_kho_duoc_chon = i;
        }

        // Highlight difficulty được chọn + hiệu ứng mũi tên động
        if (i == do_kho_duoc_chon)
        {
            DrawRectangleRounded(box, 0.5f, 10, Fade(xanh_nhat, 0.4f));
            DrawText(">", chieu_rong_man_hinh / 2 - textWidth / 2 - 50 + 10 * sin(GetTime() * 4), yPos, 30, mau_vang);
            textColor = mau_do;
        }

        DrawText(difficultyNames[i], chieu_rong_man_hinh / 2 - textWidth / 2, yPos, 30, textColor);
    }

    // Hướng dẫn nhập số và quay về menu
    DrawText("Press 1/2 to choose map", chieu_rong_man_hinh / 2 - MeasureText("Press 1/2 to choose map", 20)/2, 650, 20, xanh_dam);
  DrawText("Press 7/8/9 to choose difficulty", chieu_rong_man_hinh / 2 - MeasureText("Press 7/8/9 to choose difficulty", 20)/2 , 670, 20, xanh_dam);
    DrawText("Press 5 to return to main menu", chieu_rong_man_hinh / 2 - MeasureText("Press 5 to return to main menu", 20)/2, 690, 20, xanh_dam);
}

// ==================== DRAW NEW GAME (RUN SNAKE) ====================
void ve_choi_moi(tro_choi &game)
{
    ClearBackground(mau_xanh);
    if (ban_do_duoc_chon == 0)
    {
        ve_nen_caro(); // Map cơ bản
    }
    else
    {
        ve_nen_caro();
        ve_tuong_me_cung(); // Map mê cung
    }
     double toc_do_ran = 0.1;

if (do_kho_duoc_chon == 0) {
    toc_do_ran = 0.5; // Dễ
}
else if (do_kho_duoc_chon == 1) {
    toc_do_ran = 0.25; // Trung bình
}
else {
    toc_do_ran = 0.1; // Khó
}
    if (su_kien_duoc_kich_hoat(toc_do_ran))
    {
        cho_phep_di_chuyen = true;
        game.Update();
    }

    if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && game.snake.huong.y != 1 && cho_phep_di_chuyen)
    {
        game.snake.huong = {0, -1};
        game.dang_choi = true;
        cho_phep_di_chuyen = false;
    }
    if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) && game.snake.huong.y != -1 && cho_phep_di_chuyen)
    {
        game.snake.huong = {0, 1};
        game.dang_choi = true;
        cho_phep_di_chuyen = false;
    }
    if ((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) && game.snake.huong.x != 1 && cho_phep_di_chuyen)
    {
        game.snake.huong = {-1, 0};
        game.dang_choi = true;
        cho_phep_di_chuyen = false;
    }
    if ((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) && game.snake.huong.x != -1 && cho_phep_di_chuyen)
    {
        game.snake.huong = {1, 0};
        game.dang_choi = true;
        cho_phep_di_chuyen = false;
    }

    game.Draw();

}

int main()
{
    InitWindow(chieu_rong_man_hinh, chieu_cao_man_hinh, "ran Gameeee");
    khoi_tao_tuong();
    SetExitKey(KEY_NULL);
    SetTargetFPS(0);

    tro_choi game; // Tạo game snake

    while (!WindowShouldClose())
    {
        // ================= Xử lý menu =================
        if (man_hinh_hien_tai == 0)
        {
            if (IsKeyPressed(KEY_UP))
            {
                item_duoc_chon--;
                if (item_duoc_chon < 0) item_duoc_chon = so_item_menu - 1;
            }
            else if (IsKeyPressed(KEY_DOWN))
            {
                item_duoc_chon++;
                if (item_duoc_chon >= so_item_menu) item_duoc_chon = 0;
            }

            if (IsKeyPressed(KEY_ENTER))
            {
                if (item_duoc_chon == 0)
                {
                    man_hinh_hien_tai = 2; // NEW GAME
                }
                else if (item_duoc_chon == 1)
                {
                    // QUIT GAME
                    CloseWindow();
                    return 0;
                }
                else if (item_duoc_chon == 2)
                {
                    man_hinh_hien_tai = 1; // CREDIT
                }
                else if (item_duoc_chon == 3)
                {
                    man_hinh_hien_tai = 3; // ABOUT
                }
                else if (item_duoc_chon == 4)
                {
                    man_hinh_hien_tai = 4; // SETTING
                }
            }
        }

        // ================= Quay lại menu =================
        if (man_hinh_hien_tai == 1 && IsKeyPressed(KEY_THREE))
            man_hinh_hien_tai = 0; // CREDIT → MENU
        if (man_hinh_hien_tai == 3 && IsKeyPressed(KEY_FOUR))
            man_hinh_hien_tai = 0; // ABOUT → MENU
        if (man_hinh_hien_tai == 4 && IsKeyPressed(KEY_FIVE))
            man_hinh_hien_tai = 0; // SETTING → MENU

        if (man_hinh_hien_tai == 2 && IsKeyPressed(KEY_ESCAPE))
        {
            man_hinh_hien_tai = 0;      // Quay lại menu
            game.dang_choi = false;   // Dừng cập nhật game
            game.snake.Reset();     // Reset vị trí rắn
            cho_phep_di_chuyen = false;      // Ngăn di chuyển khi vừa vào lại
        }
        if (man_hinh_hien_tai == 4)
        {
        // Xử lý input bằng số
    if (IsKeyPressed(KEY_ONE)) ban_do_duoc_chon = 0;   // 1 → BASIC MAP
    if (IsKeyPressed(KEY_TWO)) ban_do_duoc_chon = 1;   // 2 → MAZE MAP

    if (IsKeyPressed(KEY_SEVEN)) do_kho_duoc_chon = 0; // 7 → EASY
    if (IsKeyPressed(KEY_EIGHT)) do_kho_duoc_chon = 1; // 8 → MEDIUM
    if (IsKeyPressed(KEY_NINE)) do_kho_duoc_chon = 2;  // 9 → HARD

    // Quay về menu chính
    if (IsKeyPressed(KEY_FIVE)) man_hinh_hien_tai = 0;
        }
        // ================= Vẽ =================
        BeginDrawing();

        if (man_hinh_hien_tai == 0)
        {
            ve_menu();
        }
        else if (man_hinh_hien_tai == 1)
        {
            ve_credit();
        }
        else if (man_hinh_hien_tai == 2)
        {
            ve_choi_moi(game);
        }
        else if (man_hinh_hien_tai == 3)
        {
            ve_about();
        }
        else if (man_hinh_hien_tai == 4)
        {
            ve_cai_dat();
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
