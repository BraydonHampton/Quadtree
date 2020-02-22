#ifndef TRNODE_H
#define TRNODE_H

#include <iostream>
#include <list>
#include <vector>
#include "Constants.h"
#include "dot.h"
#include "Surface.h"

class TRNode
{
public:
    
TRNode(std::vector< Dot > & points)
    : x_(0), y_(0), w_(W), h_(H), depth_(0),
        child1_(NULL), child2_(NULL), child3_(NULL), child4_(NULL)
    {
        for (int i = 0; i < points.size(); ++i)
        {
            this->ppoints_.push_back(&points[i]);
        }
//        std::cout << "building root" << '\n';
    }
    
TRNode(int x, int y, int w, int h, int depth)
    : x_(x), y_(y), w_(w), h_(h), depth_(depth), child1_(NULL),  child2_(NULL), child3_(NULL), child4_(NULL)
    {}

    ~TRNode()
    {
        if (this == NULL) return;
        
        this->child1_->deleteT();
        this->child2_->deleteT();
        this->child3_->deleteT();
        this->child4_->deleteT();
        
        
        for (int i = this->ppoints_.size(); i < 0; --i)
        {
            this->ppoints_[i] = NULL;
            this->ppoints_.pop_back();
        }
        
        this->ppoints_.clear();
        
//        std::cout << "deleting root" << std::endl;

        delete this;
    }

    int getX() { return x_; }
    int getY() { return y_; }
    int getW() { return w_; }
    int getH() { return h_; }
    int getDepth() { return depth_; }
    std::vector< Dot* > const & getPpoints() { return ppoints_; }

    TRNode* & getChild1() { return child1_; }
    TRNode* & getChild2() { return child2_; }
    TRNode* & getChild3() { return child3_; }
    TRNode* & getChild4() { return child4_; }


    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // split() takes the current node and splits it into 4 parts
    // as well as dividing out the dots it points to dependent on
    // their locations
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    void split()
    {
        for (int i = 0; i < this->ppoints_.size(); ++i)
        {
            if (this->ppoints_[i]->getY() < this->h_ / 2 + this->y_)
            {
               
                if (this->ppoints_[i]->getX() < this->w_ / 2 + this->x_) // go to c1
                {
                    getChild1()->ppoints_.push_back(this->ppoints_[i]);
                }
                else                                  // go to c2
                {
                    getChild2()->ppoints_.push_back(this->ppoints_[i]);
                }
            }
            else
            {
                if (this->ppoints_[i]->getX() < this->w_ / 2 + this->x_) // go to c3
                {
                    getChild3()->ppoints_.push_back(this->ppoints_[i]);
                }
                else                                  // go to c4
                {
                    getChild4()->ppoints_.push_back(this->ppoints_[i]);
                }
             
            }
        }
    }

    void build()
    {
        if(this->ppoints_.size() > THRESH && this->depth_ < MAXDEPTH)
        {
            getChild1() = new TRNode(x_, y_, w_/2, h_/2, this->depth_ +1 );
            getChild2() = new TRNode(x_ + w_/2, y_, w_/2, h_/2, this->depth_ +1);
            getChild3() = new TRNode(x_, y_ + h_/2, w_/2, h_/2, this->depth_ +1);
            getChild4() = new TRNode(x_ + w_/2, y_ + h_/2, w_/2, h_/2, this->depth_ +1);

            this->split();
            
            getChild1()->build();
            getChild2()->build();
            getChild3()->build();
            getChild4()->build();
        }
    }

    void draw(Surface & surface)
    {
        
        surface.put_line(getX(), getY(), getX(), getY() + getH() - 1, WHITE);
        surface.put_line(getX(), getY(), getX() + getW() - 1, getY(), WHITE); 
        
        /*
        surface.put_rect(x_, y_, 1, h_ - 1, WHITE); // top left -> bot left
        surface.put_rect(x_, y_, w_ - 1, 1, WHITE); // top left -> 
        surface.put_rect(x_, h_ - 1, w_ - 1, 1, WHITE);
        surface.put_rect(w_ - 1, y_, 1, h_ - 1, WHITE);
        */

        if (getChild1() != NULL)
        {
//            std::cout << "here";
            getChild1()->draw(surface);
            getChild2()->draw(surface);
            getChild3()->draw(surface);
            getChild4()->draw(surface);
        }
    }

    void deleteT()
    {
        if (this == NULL) return;
        
        this->child1_->deleteT();
        this->child2_->deleteT();
        this->child3_->deleteT();
        this->child4_->deleteT();
        

        for (int i = this->ppoints_.size(); i < 0; --i)
        {
            this->ppoints_[i] = NULL;
            this->ppoints_.pop_back();
        }
        
        this->ppoints_.clear();

//        std::cout << "deleting root" << std::endl;
//        (*this) = NULL;
        delete this;
    }
    
private:
// make list of circles
    int x_;
    int y_;
    int w_;
    int h_;
    int depth_;
//TRNode * parent_;
    TRNode * child1_;
    TRNode * child2_;
    TRNode * child3_;
    TRNode * child4_;
    std::vector< Dot* > ppoints_;
};

std::ostream & operator<<(std::ostream & cout, TRNode & node)
{
    cout << "<NODE: " << &node << '\n'
         << " child1 = " << node.getChild1()
         << " child2 = " << node.getChild2()
         << " child3 = " << node.getChild3()
         << " child4 = " << node.getChild4()
         << ">\n";
}

#endif
