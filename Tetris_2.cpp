#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <vector>
#include <thread> // для использования std::this_thread::sleep_for
#include <chrono>
//рефакторинг сделать 
using namespace std;

void gotoxy(int x,int y) {
    COORD p = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}
int sleepTime = 200;
const int PI = 3.1415;
const int WIDTH  = 10 + 2;
const int HEIGHT = 20 + 1;

const int EMPTY  = 0;
const int BORDER = 1;
const int BLOCK  = 2;

const int BLOCK_IN_LINE = 10;

const int SLEEP_DELETE = 50;

struct Point {
    int x;
    int y;
};
class Figure {
    vector <Point> vec;
    vector <Point> bvec;

public:
    //vector <Point> vec;
    int getX(int i) {
        return vec[i].x;
    }
    int getY(int i) {
        return vec[i].y;
    }
    int getVecSize() {
        return vec.size();
    }
    void save() {
        
        bvec = vec;
    }
    void undo() {

        vec = bvec;
    }
    void push(int x, int y) {
        Point temp;
        temp.x = x;
        temp.y = y;
        vec.push_back(temp);
    }
     //это для "анимации" падения(доделать)
    void hide() {
        for (int i = 0; i < vec.size(); i++) {
            gotoxy(vec[i].x, vec[i].y);
            cout << " ";
        }
        
    }

    void show() {
       hide();
       
     for (int i = 0; i < vec.size(); i++) {
        gotoxy(vec[i].x, vec[i].y);
        cout << (char)178;
    }       
}        
    
    

   
    
    //падение фигуры(не прям вау)
    void fall() {
        for (int i = 0; i < vec.size(); i++)
            vec[i].y++;
    }
    void up() {
        for (int i = 0; i < vec.size(); i++)
            vec[i].y--;
    }

    void left() {
        for (int i = 0; i < vec.size(); i++)
            vec[i].x--;
    }
    void right() {
        for (int i = 0; i < vec.size(); i++)
            vec[i].x++;
    }
                               
    //спавним новую фигуру (на том же месте) но можно сделать и на рандом но нужно ограничение на спавн
    void spawn() {
        vec.clear();
        int r = rand() % 6;

        switch (r) {
        case 0:
            this->push(5, 0);
            this->push(6, 0);
            this->push(5, 1);
            this->push(6, 1);
        break;
        case 1:
            this->push(5, 0);
            this->push(6, 0);
            this->push(7, 0);
            this->push(8, 0);
        break;
        case 2:
            this->push(5, 0);
            this->push(6, 0);
            this->push(7, 0);
            this->push(7, 1);
            break;
        case 3:
            this->push(5, 0);
            this->push(6, 0);
            this->push(6, 1);
            this->push(7, 0);
            break;
        case 4:
            this->push(5, 0);
            this->push(6, 0);
            this->push(6, 1);
            break;
        case 5:
            this->push(5, 0);
            this->push(6, 0);
            break;    
        
        
            
            
        }
        
        
    }
    void rotateR() {
        for (int i = 0; i < vec.size(); i++) {
            int x = -(vec[i].y - vec[1].y) + vec[1].x;
            int y = (vec[i].x - vec[1].x) + vec[1].y;
            vec[i].x = x;
            vec[i].y = y;
        }
    }
    void rotatel() {
        for (int i = 0; i < vec.size(); i++) {
            int x = (vec[i].y - vec[1].y) + vec[1].x;
            int y = -(vec[i].x - vec[1].x) + vec[1].y;
            vec[i].x = x;
            vec[i].y = y;
        }
    }
};    
        

class GameMap {
    
public:
vector <vector<int>> vec;
    void init( int width, int height) {
        vector<int> temp;

        temp.push_back(BORDER);
        for (int i = 1; i < width - 1; i++)
            temp.push_back(EMPTY);

        temp.push_back(BORDER);

        for (int i = 0; i < height; i++)
            vec.push_back(temp);

        temp.clear();

        for (int i = 0; i < width; i++)
            temp.push_back(BORDER);

        vec.push_back(temp);
    }

    void show() {
        //поле с нулями единицами и двойками справа от основного-временное 

       /* for (int i = 0; i < vec.size(); i++) {
            gotoxy(19, i);
            for (int j = 0; j < vec[i].size(); j++) {
                cout << vec[i][j];
            }
            cout << endl;
        }*/
        gotoxy(0, 0);
        for (int i = 0; i < vec.size(); i++) {
            for (int j = 0; j < vec[i].size(); j++) {
                if (vec[i][j] == BORDER) {
                    cout << (char)219;
                }
                if (vec[i][j] == BLOCK) {
                    cout << (char)178;
                }
                if (vec[i][j] == EMPTY) {
                    cout << " ";
                }
            }
            cout << endl;
        }
    }
    void animateLines(vector<int> lines) {
        if (lines.empty() ){
            return;
        }
        for (int i = 1; i <= BLOCK_IN_LINE; i++) {
            for (int j = 0; j < lines.size(); j++) {
                gotoxy(i, lines[j]);
                cout << " ";

            }
            Sleep(SLEEP_DELETE);
        }

    }


    int deleteLine(int width) {
        // Определение линий, которые нужно удалить
        vector<int> lines;
        for (int i = vec.size() - 2; i >= 0; i--) {
            int d = 0;
            for (int j = 1; j < vec[i].size() - 1; j++) {
                if (vec[i][j] == BLOCK) {
                    d++;
                }
            }
            if (d == BLOCK_IN_LINE) {
                lines.push_back(i);
            }
        }

        // Удаление рядов
        for (int i = 0; i < lines.size(); i++) {
            vec.erase(vec.begin() + lines[i]);
        }

        // Добавление пустых рядов в игровое поле вместо удаленных
        vector<int> temp;
        temp.push_back(BORDER);
        for (int i = 1; i < width - 1; i++)
            temp.push_back(EMPTY);
        temp.push_back(BORDER);

        for (int i = 0; i < lines.size(); i++) {
            vec.insert(vec.begin(), temp);
        }
        animateLines(lines);
        return lines.size();
    }
   

    bool isCollision( Figure& fig) {
        for (int i = 0; i < fig.getVecSize(); i++) {

            if (fig.getY(i) < 0) {
                return true;
            }
            if (fig.getX(i) <= 0) {
                return true;
            }
            if (fig.getX(i) >= WIDTH-1) {
                return true;
            }

            if (vec[fig.getY(i)][fig.getX(i)] == BORDER) {//стоп на границе 
                return true;
            }
            if (vec[fig.getY(i)][fig.getX(i)] == BLOCK) {//стоп с фигуркой (неплохо работает но нужны константы а не цифры )
                return true;
            }
        }
        return false;
    }
    //фиксация фигуры
    void fixed(Figure &fig) {
        for (int i = 0; i < fig.getVecSize(); i++) {
            vec[fig.getY(i)][fig.getX(i)] = BLOCK;


        }
    }
    

};          
 
class Game {
private:
    int score = 0;
    int level = 1;
public:

    Game() {

    }
    void gameOver() {
        gotoxy(21, 8);
        cout << "GAME OVER";
    }
    void Score(int lines) {
        switch (lines) {
        case 1:
            score += 10;
            break;
        case 2:
            score += 12;
            break;
        case 3:
            score += 20;
            break;

        case 4:
            score += 35;
            break;
        }
        if (score >= 100) {
            levelUp();
            score = 0;
        }
    }
    void showTitle() {
        gotoxy(21, 1);
        cout << "TETRIS";
    }
   

    
    void showScore() {
        gotoxy(21, 6);
        cout << "SCORE:"<<score;
        
    }
    void levelUp() {
        level++;
        
        if (level <= 10) {
            sleepTime -= 20; 
        }
    }
        
    void showlevel() {
            gotoxy(21, 3);
            cout << "LEVEL:" << level;
        }
    
int run() {
       GameMap map;
       map.init(WIDTH, HEIGHT);
       map.show();
       

    //рисуем фигурыв 
    Figure fig;
    fig.spawn();
   
    
    

    //кнопки тык тык 
    int ch = 0;
    int i = 0;
    int x = 5, y = 5;
        while (true) {

            while (!_kbhit()) {
                showTitle();
                showlevel();
                showScore();
                fig.hide();
                fig.fall();
                fig.show();
               
                   if (map.isCollision(fig)) {

                   fig.up();
                   map.fixed(fig);
                   fig.show();
                   int lines = map.deleteLine(WIDTH);
                   Score(lines);
                   showScore();
                   
                   
                    map.show();
                   fig.spawn();
                   if (map.isCollision(fig)) {
                       fig.show();
                       this->gameOver();
                       return 0;
                   }
               }
               Sleep(sleepTime);
              
            }
            fig.show();
            ch = _getch();
                switch (ch) {
                    case 77://right
                        fig.hide();
                        fig.right();
                        if (map.isCollision(fig)) {
                            fig.left();
                        }
                    break;
                    case 80://down
                        fig.hide();
                        fig.fall();
                        if (map.isCollision(fig)) {
                            fig.up();
                        }
                    break;
                    case 75://left
                        fig.hide();
                        fig.left();
                        if (map.isCollision(fig)) {
                            fig.right();
                        }
                    break;
                    case 72://rotate right 
                        
                        fig.hide();
                        fig.save();
                        fig.rotateR();
                        if (map.isCollision(fig)) {
                            fig.left();
                        }
                        if (map.isCollision(fig)) {
                            fig.undo();
                            fig.rotatel();
                        }
                    break;
                    case 32://rotate left
                        
                        fig.hide();
                        fig.save();
                        fig.rotatel();
                        if (map.isCollision(fig)) {
                            fig.right();
                        }
                        if (map.isCollision(fig)) {
                            fig.undo();
                            fig.rotateR();
                        }
                        break;
                   
                }
        }
return 0;
}
   

   
};

 


//так себе главная функция
int main()
{
    void* handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO structCursorInfo;
    GetConsoleCursorInfo(handle, &structCursorInfo);
    structCursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(handle, &structCursorInfo);
    

    
    Game game;
    game.run();
    //_getch();
    
}

