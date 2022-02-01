#include "Custom.h"
#include "window.h"

using namespace std;

int board[H][W] = {0};

void generate_board(){
    for (int i = 0;i < H;i++)
        for (int j = 0;j < W;j++)
            if (i == 0 || i == H-1 || j == 0 || j == W-1)
                board[i][j] = BLOCK;
}

void display_board(){
    system("CLS");
    
    char scr[H*(W+1)] = {0};
    int p = 0;

    for (int i = 0;i < H;i++){
        for (int j = 0;j < W;j++){
            scr[p++] = (board[i][j] == '\0')?' ':board[i][j];
        }
        scr[p++] = '\n';
    }

    scr[p] = '\0';
    cout << scr << endl;
}

// MOUSE And SNAKE

class Mouse{
    public:
        Mouse(){
            board[x][y] = MOUSE;
        }

        int x = 0;
        int y = 0;

        int gen_pos(int lim){
            srand(time(0));
            return (1 + rand()%(lim-3));
        }

        void change_pos(){
            if (x != 0 && y != 0)
                board[x][y] = Snake_HEAD;

            x = gen_pos(H);
            y = gen_pos(W);

            board[x][y] = MOUSE;
        }
};

class Snake{
    public:
        Snake(){
            intialize(body,0,0);

            change_head(H/2,W/2);
            m.change_pos();

            generate_board();
            display_board();
        }

        Mouse m;

        int score = 0;
        int body[MAX_SCORE][2] = {0};
        int prev_x,prev_y;

        struct Head{
            int x = 1;
            int y = 1;

            int xs = 0;
            int ys = 0;
        };

        Head h;

        void change_head(int x,int y){
            board[h.x][h.y] = 0;

            h.x = x;
            h.y = y;

            body[0][0] = h.x;
            body[0][1] = h.y;

            board[h.x][h.y] = Snake_HEAD;
        }

        void move(char key){

            //MOVEMENT CONTROLS
            switch(key){
                case UP: case 'w':
                    h.xs = -1;
                    h.ys = 0;
                break;

                case DOWN: case 's':
                    h.xs = 1;
                    h.ys = 0;
                break;

                case LEFT: case 'a':
                    h.ys = -1;
                    h.xs = 0;
                break;

                case RIGHT: case 'd':
                    h.ys = +1;
                    h.xs = 0;
                break;

                case ESC: 
                    exit();
                break;
            }
             
            while(h.xs != 0 || h.ys != 0){
                start_motion(h.x,h.y);
            }
        }

        void start_motion(int x,int y){
            if (x == 1   && h.xs == -1 || x == H-2 && h.xs == +1) exit();
            else x += h.xs;

            if (y == 1   && h.ys == -1 || y == W-2 && h.ys == +1) exit();
            else y += h.ys;
            
            if (_kbhit()) move(getch());

            prev_x = h.x;
            prev_y = h.y;

            change_head(x,y);
            body[0][0] = h.x;
            body[0][1] = h.y;

            for (int i = 1;i <= score;i++){
                int tx = body[i][0];
                int ty = body[i][1];

                board[body[i][0]][body[i][1]] = 0;

                body[i][0] = prev_x;
                body[i][1] = prev_y;

                prev_x = tx;
                prev_y = ty;
                
                board[body[i][0]][body[i][1]] = Snake_BODY;
            }

            if (caught()){
                body[score][0] = prev_x;
                body[score][1] = prev_y;

                board[body[score][0]][body[score][1]] = Snake_BODY;

                if (score == MAX_SCORE){
                    cout << "!!! YOU WON !!!" << endl;
                    cout << "Game Exits in ";

                    for (int i = 5;i > 0;i--){
                        cout << i << " ";
                        Sleep(1000);
                    }
                    std::exit(0); 
                }
            }

            for (int i = 0;i <= score;i++){
                for (int j = i+1;j <= score;j++){
                    if (body[i][0] == body[j][0] && body[i][1] == body[j][1]){
                        exit();
                    }
                }
            }

            Sleep(SLEEP_TIME);
            display_board();
            cout << "SCORE: " << score <<endl;
        }


        //SUB-FUNCTIONS
        void intialize(int arr[][2],int x,int y){
            for (int i = 0;i < MAX_SCORE;i++){
                arr[i][0] = x;
                arr[i][1] = y;
            }
        }

        bool caught(){
            if (m.x == h.x && m.y == h.y){
                m.change_pos();
                score++;
                return true;
            }
            return false;
        }

        void exit(){
            cout << "!!! GAME OVER !!!" << endl;
            cout << "Game Exits in ";

            for (int i = 5;i > 0;i--){
                cout << i << " ";
                Sleep(1000);
            }
            std::exit(0); 
        }
};


int main(){
    ShowConsoleCursor(false);
    Snake s;

    char ch;
    while(ch = getch()){
        s.move(ch);
    }
    return 0;
}