#pragma once
#include "winsys.h"
#include <cstdlib>
struct CBodySegment {
    CPoint position;

    CBodySegment(CPoint Position) : position(Position) {}
};

class CApple{
private:
    CRect geom;
    short apples_eaten;


public:
    friend class CBody;
    friend class CSnakeGame;
    CPoint position;
    CApple(CRect Geom): geom(Geom) {
        position.x = geom.topleft.x+geom.size.x/1.2;
        position.y = geom.topleft.y+geom.size.y/8;
        apples_eaten = 3;
    }
    void handle_level_number(){
        gotoyx(geom.topleft.y-1, geom.topleft.x);
        printw("| LEVEL %d |",apples_eaten/3);
    }
    void paint(){
        gotoyx(position.y, position.x);
        printw("0");
        handle_level_number();
    }
    void random_apple_placement(const list <CBodySegment>& segments){
        srand((unsigned) time(NULL));
        bool no_overlap;
        while(true) {
            no_overlap = true;
            this->position.x = geom.topleft.x+1 + rand() % (geom.size.x-2);
            this->position.y = geom.topleft.y+1 + rand() % (geom.size.y-2);
            for (const auto &segment: segments) {
                if ((this->position.x == segment.position.x) && (this->position.y == segment.position.y)) {
                    no_overlap = false;
                    break;
                }
            }
            if (no_overlap){
                break;
            }
        }
    }
    void found_apple(const list <CBodySegment>& segments){
        apples_eaten++;
        random_apple_placement(segments);
        if (apples_eaten%3==0){
            wtimeout(stdscr,150 - 2*apples_eaten);
        }
    }
};