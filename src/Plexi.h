//
//  Plexi.h
//  Jeugdfilmfestival
//
//  Created by The Distillery on 23/02/14.
//
//

#ifndef PLEXI
#define PLEXI

#include <iostream>



class Plexi {
    
public:
    Plexi(int x, int y, int size);
    
    void update();
    void draw();
    
    int size;
    int x;
    int y;
    
    void init();
    void light(string p, int v, int s);
    void hit();
    void died();
    void reset();
    
    int value;
    //int score;
    
    bool isHit;
    bool isLit;
    bool isDead;
    
    ofImage image;

    
private:
    float hit_ts;
    float lit_ts;
    float dead_ts;
    
    bool isMatch;
};



#endif


