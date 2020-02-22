#ifndef DOT_H
#define DOT_H

#include "Constants.h"
#include <cmath>

class Dot
{
public:
    Dot()
    {
        x_ = rand() % W;
        y_ = rand() % H;
        rad_ = 3;
        color_ = 1;
        dir_ = rand() % 4 + 1;
        speed_ = 2;
    }
    
    // getters
    int getX() { return x_; }
    int getY() { return y_; }
    int getRad() { return rad_; }
    int getColor() { return color_; }
    int getDir() { return dir_; }
    int getSpeed() { return speed_; }

    // setters
    void setX(int x) { x_ = x; }
    void setY(int y) { y_ = y; }
    void setRad(int rad) { rad_ = rad; }
    void setColor(int color) { color_ = color; }
    void setDir(int dir) { dir_ = dir; }

    void flipColor()
    {
        color_ = (color_ == 1 ? 0 : 1);
    }

    void move()
    {
        switch(dir_)
        {

        case 1: // nw
            x_ -= getSpeed();
            y_ -= getSpeed();

            if (x_ <= 0 + rad_)
            {
                //std::cout << 'l';
                setDir(2);
            }
            if (y_ <= 0 + rad_) setDir(3);
            break;
            
        case 2: // ne
            x_ += getSpeed();
            y_ -= getSpeed();

            if (x_ >= W - rad_) setDir(1);
            if (y_ <= 0 + rad_) setDir(4);
            break;
            
        case 3: // sw
            x_ -= getSpeed();
            y_ += getSpeed();

            if (x_ <= 0 + rad_) setDir(4);
            if (y_ >= H - rad_) setDir(1);
            break;
            
        case 4: // se
            x_ += getSpeed();
            y_ += getSpeed();

            if (x_ >= W - rad_) setDir(3);
            if (y_ >= H - rad_) setDir(2);
            break;
            
// deciding whether to implement
            
        case 5: // north
            y_ -= 1;
            break;
        case 6: // south
            y_ += 1;
            break;
        case 7: // east
            x_ += 1;
            break;
        case 8: // west
            x_ -= 1;
            break;
        }
    }

    bool collision(Dot d) // PROVE
    {
        double distanceX = d.getX() - getX();
        double distanceY = d.getY() - getY();
        double distance = sqrt(distanceX * distanceX + distanceY * distanceY);
        
        if (distance <= d.getRad() + getRad()) return true;
        return false;
    }
                            
private:
    int x_;
    int y_;
    int rad_;
    int color_;
    int dir_;
    int speed_;
    
};

#endif
