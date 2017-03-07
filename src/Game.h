//
//  Game.h
//  Jeugdfilmfestival
//
//  Created by The Distillery on 23/02/14.
//
//

#ifndef GAME
#define GAME

#include <iostream>
#include "Plexi.h"

#define NUM_PLEXIS 12
#define NUM_GRAPHICS 8
#define COUNTDOWN_TIME 4



class Game {

public:
    Game();
    
    void update();
    void draw();
    
    void start();

    void selectPlexi(int plexi);
    void movePlexi(string dir, int val);
    void resizePlexi(int val);
    
    void hitPlexi(int plexi);
    void hitMultiPlayer();
    void hitSinglePlayer();
    
    bool setup;
    int resize_plexi;

    Plexi *plexi[NUM_PLEXIS];
    
    string action;
    
    void saveSettings();

    string image[NUM_GRAPHICS];
    int value[NUM_GRAPHICS];

    bool playing;
    bool countdown;
    
    int score;
    int lives;
    int rank;
    int topscore;
    
    vector<int> ranking;
    
private:
    
    float countdown_ts;
    
    float speed;
    
    bool exit;

};



#endif


