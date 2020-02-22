/****************************************************************************
 Yihsiang Liow
 Copyright
 ****************************************************************************/
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <sstream>

#include "Includes.h"
#include "Constants.h"
#include "compgeom.h"
#include "Surface.h"
#include "Event.h"
#include "dot.h"
#include "TRNode.h"

void print(TRNode * node)
{
    if (node == NULL)
    {
        std::cout << "returning\n";
        return;
    }
    
    std::cout << (*node) << "\n\n";

    std::cout << "Printing: " << node << " child 1\n~~~~~~";
    print(node->getChild1());
    std::cout << "Printing: " << node << " child 2\n~~~~~~";
    print(node->getChild2());
    std::cout << "Printing: " << node << " child 3\n~~~~~~";
    print(node->getChild3());
    std::cout << "Printing: " << node << " child 4\n~~~~~~";
    print(node->getChild4());
}

void build(TRNode * node)
{
//        std::cout << "here";
//    std::cout << "Building: " << this << '\n';
    if(node->getPpoints().size() > THRESH && node->getDepth() < MAXDEPTH)
    {
        node->getChild1() = new TRNode(node->getX(), node->getY(), node->getW() / 2, node->getH() / 2, node->getDepth() +1 );
        node->getChild2() = new TRNode(node->getX() + node->getW() / 2, node->getY(), node->getW() / 2, node->getH() / 2, node->getDepth() +1);
        node->getChild3() = new TRNode(node->getX(), node->getY() + node->getH() / 2, node->getW() / 2, node->getH() / 2, node->getDepth() +1);
        node->getChild4() = new TRNode(node->getX() + node->getW() / 2, node->getY() + node->getH() / 2, node->getW()/ 2, node->getH() / 2, node->getDepth() +1);
        
        node->split();
        
        node->getChild1()->build();
        node->getChild2()->build();
        node->getChild3()->build();
        node->getChild4()->build();
    }
}

void baseColl(std::vector<Dot> & points, int numDots)
{
    for (int i = 0; i < numDots - 1; ++i)
    {
        for (int j = i + 1; j < numDots; ++j)
        {
            if (points[i].collision(points[j]))
            {
                points[i].setDir(rand() % 4 + 1);
                points[j].setDir(rand() % 4 + 1);
                
                points[i].flipColor();
                points[j].flipColor();
                break;
            }
        }
    }
}

void QTcoll(TRNode * node)
{   
    if (node->getChild1() == NULL)
    {
        if (node->getPpoints().size() <= 0) return;
        
        
        for (int i = 0; i < node->getPpoints().size() - 1; ++i)
        {
            for (int j = i + 1; j < node->getPpoints().size(); ++j)
            {
                if (node->getPpoints()[i]->collision((*node->getPpoints()[j])))
                {
                    
                    node->getPpoints()[i]->setDir(rand() % 4 + 1);
                    node->getPpoints()[j]->setDir(rand() % 4 + 1);

                    node->getPpoints()[i]->flipColor();
                    node->getPpoints()[j]->flipColor();
                    break;
                    
                }
            }
        }
    }
    else
    {
        QTcoll(node->getChild1());
        QTcoll(node->getChild2());
        QTcoll(node->getChild3());
        QTcoll(node->getChild4());
    }
    return;
}

int main(int argc, char* argv[])
{
    Surface surface(W, H);
    Event event;
    
    int numDots = 1000;
    
    srand((unsigned int) time(NULL));

    std::vector<Dot> points;
    for (int i = 0; i < numDots; ++i)
    {
        Dot dot;
        points.push_back(dot);
    }


    int delay_ = 50;
    int timer = delay_;
    bool qt = false;

    
    while (1)
    {
        TRNode * root = new TRNode(points);
        

        
        if (timer < delay_) ++timer;
        KeyPressed keypressed = get_keypressed();
        // Process event
        if (event.poll() && event.type() == QUIT) break;
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // breakout of program
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        if (keypressed[SPACE]) break;



        
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // add 100 dots
        // ONLY WORKS IF QT IS OFF
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        if (keypressed[RIGHTARROW] && timer == delay_) 
        {
//            std::cout << 'g';
            for (int i = 0; i < 100; ++i)
            {
                Dot dot;
                points.push_back(dot);
            }
            numDots += 100;
            timer = 0;
        }

        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // delete 100 dots
        // ONLY WORKS IF QT IS OFF
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        if (keypressed[LEFTARROW] && timer == delay_)
        {
            for (int i = 0; i < 100; ++i)
            {
                if (!points.empty()) points.pop_back();
            }
            numDots -= 100;
            timer = 0;
        }

        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // toggle quadTree
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~        
        if (keypressed[TAB] && timer == delay_)
        {
            qt = (qt == false ? true : false);
            timer = 0;
        }

        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // move dots
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        for (int i = 0; i < numDots; ++i)
        {
            points[i].move();
        }

        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // build tree
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        if (qt) root->build();

        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // collision check with other dots
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        if (qt)  QTcoll(root);
        else baseColl(points, numDots);
       
        
        
        surface.lock();
        surface.fill(BLACK);
        
        root->draw(surface);

        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // draw dots
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        for (int i = 0; i < numDots; ++i)
        {
            if (points[i].getColor() == 1)
            {
                surface.put_circle(points[i].getX(), points[i].getY(), points[i].getRad(), GREEN);
            }
            else
            {
                surface.put_circle(points[i].getX(), points[i].getY(), points[i].getRad(), RED);
            }
        }
        
        surface.unlock();
        surface.flip();
        
        delay(10);

/*
        std::cout << "STARTING TREE PRINT~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        print(root);
        std::cout << "ENDING TREE PRINT XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n";
*/    
        if (qt)
        {
            root = NULL;
            delete root;
        }
    }

    
    
    return 0;
}
