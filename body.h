#pragma once

#include "apple.h"



class CBody {
private:
    list <CBodySegment> segments;
    CRect geom;
    CApple apple;

public:
    friend class CSnakeGame;

    CBody(CRect Geom) : geom(Geom), apple(Geom){
        CPoint start;
        start.x = geom.topleft.x + geom.size.x / 2;
        start.y = geom.topleft.y + geom.size.y / 1.5;
        segments.push_back(CBodySegment(start));
        for (short i = 0;i<2;i++){
            start.y--;
            add_segment_front(start);
        }
    }


    void add_segment_back(const CPoint &pos) {
        segments.push_back(CBodySegment(pos));
    }
    void add_segment_front(const CPoint &pos) {
        segments.push_front(CBodySegment(pos));
    }
    bool check_for_collision(CPoint new_placement){
        bool first_done = false;
        for (const auto& segment: segments){
            if (!first_done){
                first_done = true;
                continue;
            }
            if ((segment.position.x == new_placement.x) && (segment.position.y == new_placement.y)){
                return true;
            }
        }
        return false;
    }

    bool move_snake(int delta_x, int delta_y) {
        if (!segments.empty()) {
            auto first_elem = segments.front();
            CPoint new_placement(first_elem.position.x + delta_x, first_elem.position.y + delta_y);
            if (check_for_collision(new_placement)){
                return false;
            }
            add_segment_front(new_placement);
            if (!check_for_apple_collision()) {
                segments.pop_back();
            }
            else{
                apple.found_apple(segments);
            }
        }
        else{
            return false;
        }
        return true;
    }

    bool check_for_apple_collision() {
        return (apple.position.x == segments.front().position.x) && (apple.position.y == segments.front().position.y);
    }


    int handle_movement(int dir) {
        auto &last_elem = segments.front();
        int delta_x = 0;
        int delta_y = 0;
        switch (dir) {
            case KEY_UP:
                if (last_elem.position.y == geom.topleft.y + 1) {
                    delta_y += geom.size.y - 3;
                } else {
                    delta_y--;
                }
                break;
            case KEY_DOWN:
                if (last_elem.position.y == geom.topleft.y + geom.size.y - 2) {
                    delta_y += geom.topleft.y - last_elem.position.y + 1;
                } else {
                    delta_y++;
                }
                break;
            case KEY_RIGHT:
                if (last_elem.position.x == geom.topleft.x + geom.size.x - 2) {
                    delta_x += geom.topleft.x - last_elem.position.x + 1;
                } else {
                    delta_x++;
                }
                break;
            case KEY_LEFT:
                if (last_elem.position.x == geom.topleft.x + 1) {
                    delta_x += geom.size.x - 3;
                } else {
                    delta_x--;
                }
                break;
            default:
                return 1;
        }
        if (move_snake(delta_x, delta_y)){
            return 0;
        }
        return -1;
    }

    bool move_objects_alongside_board(int key) {
        int delta_x = 0;
        int delta_y = 0;
        switch (key) {
            case KEY_UP:
                delta_y--;
                break;
            case KEY_DOWN:
                delta_y++;
                break;
            case KEY_RIGHT:
                delta_x++;
                break;
            case KEY_LEFT:
                delta_x--;
                break;
            default:
                return false;
        }
        geom.topleft.x += delta_x;
        apple.position.x += delta_x;
        for (auto &segment: segments){
            segment.position.x += delta_x;
            segment.position.y += delta_y;
        }
        geom.topleft.y += delta_y;
        apple.geom.topleft.x =geom.topleft.x;
        apple.geom.topleft.y =geom.topleft.y;
        apple.position.y += delta_y;
        return true;
    }

    void paint() {
        bool head_drawn = false;
        for (const auto &segment: segments) {
            gotoyx(segment.position.y, segment.position.x);
            if (!head_drawn){
                printw("*");
                head_drawn = true;
                continue;
            }
            printw("+");
        }
        apple.paint();
    }
};