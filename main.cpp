#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <stdlib.h>
#include <iostream>
#include <string>
#include <random>
#include <stack>

int bombasEnvolta(int x, int y);
int foraArray(int x, int y);
int randomNumber(){
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0,9); // distribution in range [1, 6]
    
    int nmb = dist6(rng);
    return nmb;
}

bool gameOver = false;

void revelar(int x, int y);

const int WIDTH = 9;
const int HEIGHT = 9;

int board[WIDTH][HEIGHT];
int revealed[WIDTH][HEIGHT];

class CampoMinado : public olc::PixelGameEngine
{
public:
    CampoMinado()
    {
        sAppName = "Campo Minado";
    }
public:
    int qtd_bombas_inicial = 10;
public:
    bool OnUserCreate() override
    {
        for(int y = 0; y < HEIGHT; y++){
            for(int x = 0; x < WIDTH; x++){
                board[y][x] = 0;
                revealed[y][x] = 0;
            }
        }
        int qtd_bombas = qtd_bombas_inicial;
        while(qtd_bombas > 0){
            int y = randomNumber();
            int x = randomNumber();
            if(board[y][x] == -1){
                continue;
            }else{
                board[y][x] = -1;
                qtd_bombas--;
            }
        }
        for(int y = 0; y < WIDTH; y++){
            for(int x = 0; x < HEIGHT; x++){
                if(board[y][x] != -1)
                    board[y][x] = bombasEnvolta(x,y);
                }
            }
        return true;
    }
    bool OnUserUpdate(float fElapsedTime) override
    {
         FillRect(olc::vi2d(0,0), olc::vi2d(ScreenWidth(), ScreenHeight()), olc::WHITE);
        std::string printString;
        olc::Pixel px = olc::BLACK;
        if(gameOver){
            printString = std::string("Game Over");
            px = olc::RED;
        }else{
            printString = std::string("Quantidade de bombas: " + std::to_string(qtd_bombas_inicial));
        }
        DrawString(olc::vi2d(10, ScreenHeight() - 40), printString, px, 2U);
        const int SQUARE_SIZE = ScreenWidth()/WIDTH;
        int step_size_width = ScreenWidth()/WIDTH;
        int step_size_height = (ScreenHeight() - 70)/HEIGHT;

        //CONTROLES
        if(!gameOver){
            if(GetMouse(olc::Mouse::LEFT).bPressed){
                olc::vi2d mousePos = GetMousePos();
                int mousePosX = mousePos.x/SQUARE_SIZE;
                int mousePosY = (mousePos.y)/SQUARE_SIZE;
                if(board[mousePosY][mousePosX] == -1){
                    gameOver = true;
                }
                if(board[mousePosY][mousePosX] == 0){
                    revelar(mousePosX, mousePosY);
                }
                if(board[mousePosY][mousePosX] > 0){
                    revealed[mousePosY][mousePosX] = 1;
                }
            }
            

        }
        int array_y = 0;
        for(int y = 0; y < (((int) ScreenHeight()) - 70); y+=step_size_height){
            int array_x = 0;
            for(int x = 0; x < ScreenWidth(); x+= step_size_width){
                olc::Pixel px = olc:: GREY;
                bool write = false;
                if(revealed[array_y][array_x] == 1){
                    write = true;
                switch(board[array_y][array_x]){
                case -1:
                    px = olc::RED;      break;
                case 0:
                    px = olc::WHITE;    break;
                default:
                    px = olc::BLACK;    break;
                }
                }
                FillRect(olc::vi2d(x,y),olc::vi2d(SQUARE_SIZE,SQUARE_SIZE), px);
                if(write)
                    DrawString(olc::vi2d(x,y), std::to_string(board[array_y][array_x]), olc::BLUE);
                array_x++;
            }
            array_y++;
        }
        for(int y = 0; y <= HEIGHT; y ++){
            DrawLine(olc::vi2d(0,y*SQUARE_SIZE), olc::vi2d(ScreenWidth(), y*SQUARE_SIZE),olc::WHITE);
        }
        for(int x = 0; x <= WIDTH; x++){
            DrawLine(olc::vi2d(x*SQUARE_SIZE,0),
                        olc::vi2d(x*SQUARE_SIZE,
                        ScreenHeight()-70),
                        olc::DARK_GREY);
        }
        return true;
        }
};

int main() {   
    CampoMinado cm;
    if(cm.Construct(405,475,1,1))
        cm.Start();
    return 0;
}


int bombasEnvolta(int x, int y){
    int count = 0;
    count += foraArray(x+1,y);
    count += foraArray(x-1,y);
    count += foraArray(x,y+1);
    count += foraArray(x,y-1);
    count += foraArray(x+1,y+1);
    count += foraArray(x-1,y-1);
    count += foraArray(x+1,y-1);
    count += foraArray(x-1,y+1);
    return count;
}
int foraArray(int x, int y){
    if(x > WIDTH || x < 0){
        return 0;
    }
    if(y > HEIGHT || y < 0){
        return 0;
    }
    else{
        if(board[y][x] == -1){
            return 1;
        }
        return 0;
    }
}

int foraArrayFunc(int x, int y){
    if(x > WIDTH || x < 0){
        return -1;
    }
    if(y > HEIGHT || y < 0){
        return -1;
    }
     return board[y][x];
}

int foraRevealed(int x, int y){
    if(x > WIDTH || x < 0){
        return -1;
    }
    if(y > HEIGHT || y < 0){
        return -1;
    }
    return revealed[y][x];
}

void revelar(int x, int y){
    std::stack<olc::vi2d> stack;
    olc::vi2d inicio = olc::vi2d(x,y);
    stack.push(inicio);
    while(!stack.empty()){
        olc::vi2d atual = stack.top();
        revealed[atual.y][atual.x] = 1;
        std::cout << "atual x: " << atual.x << " " << "atual y: " << atual.y << std::endl;
        stack.pop();
        if(foraArrayFunc(atual.x + 1, atual.y) == 0 && foraRevealed(atual.x + 1, atual.y) == 0){
            stack.push(olc::vi2d(atual.x + 1, atual.y));
        }
        if(foraArrayFunc(atual.x, atual.y+1) == 0 && foraRevealed(atual.x, atual.y+1) == 0){
            stack.push(olc::vi2d(atual.x, atual.y+1));
        }
        if(foraArrayFunc(atual.x - 1, atual.y) == 0 && foraRevealed(atual.x - 1, atual.y) == 0){
            stack.push(olc::vi2d(atual.x - 1, atual.y));
        }
        if(foraArrayFunc(atual.x, atual.y-1) == 0 && foraRevealed(atual.x, atual.y-1) == 0){
            stack.push(olc::vi2d(atual.x, atual.y-1));
        }
        

        if(foraArrayFunc(atual.x + 1, atual.y) > 0 && foraRevealed(atual.x + 1, atual.y) == 0){
            revealed[atual.y][atual.x + 1] = 1;
        }
        if(foraArrayFunc(atual.x, atual.y+1) > 0 && foraRevealed(atual.x, atual.y+1) == 0){
            revealed[atual.y+1][atual.x] = 1;
        }
        if(foraArrayFunc(atual.x - 1, atual.y) > 0 && foraRevealed(atual.x - 1, atual.y) == 0){
            revealed[atual.y][atual.x - 1] = 1;
        }
        if(foraArrayFunc(atual.x, atual.y-1) > 0 && foraRevealed(atual.x, atual.y-1) == 0){
            revealed[atual.y-1][atual.x] = 1;
        }
    }
}