#pragma once
#include "body.h"

class CSnakeGame : public CFramedWindow
{
private:
    bool paused;
    bool help_on;
    int last_key;
    bool game_over;
    CBody body;
    bool check_for_180(int key1,int key2){
        return((key1==KEY_DOWN && key2==KEY_UP) || (key2==KEY_DOWN && key1==KEY_UP) || (key1==KEY_LEFT && key2==KEY_RIGHT) || (key1==KEY_RIGHT && key2==KEY_LEFT));
    }
    void reset_game(){
        paused = true;
        help_on = true;
        last_key = 0;
        game_over = false;
        body = CBody(geom);
    }

public:
    CSnakeGame(CRect r, char _c = ' ') : CFramedWindow(r, _c),paused(true),help_on(true),last_key(0),game_over(false), body(geom){};
    bool handleEvent(int key){
        if (key == 'h' || key == 'H'){
            help_on = !help_on;
            if (game_over){
                return true;
            }
            if (help_on){
                paused = true;
            }
            else{
                paused = false;
            }
            return true;
        }
        if (key=='p' || key == 'P'){
            if (game_over) {
                return false;
            }
            if (help_on) {
                return false;
            }
            paused = !paused;
            return true;
        }
        if (key == 'r' || key=='R'){
            reset_game();
            return true;
        }
        if (key == '\t'){
            if (!children.empty()) {
                children.push_front(children.back());
                children.pop_back();
            }
            paused = true;
            return true;
        }
        if (!paused) {
            if (game_over){
                return false;
            }
            if (key==KEY_DOWN || key==KEY_UP || key==KEY_RIGHT || key==KEY_LEFT){
                if (!check_for_180(last_key,key)){
                    last_key = key;
                }
            }
            int code_handle_movement = body.handle_movement(last_key);
            switch (code_handle_movement){
                case -1:
                    game_over= true;
                    paused = true;
                    return true;
                case 0:
                    return true;
                case 1:
                    return false;
            }
            return false;
        }
        else if(paused || game_over){
            CWindow::handleEvent(key);
            return body.move_objects_alongside_board(key);
        }
        return false;
    }
    void paint()
    {
        CFramedWindow::paint();
        body.paint();
        if (help_on){
            paint_help();
        }
        if (game_over){
            paint_game_over(body.apple.apples_eaten);
            return;
        }
    }
};
