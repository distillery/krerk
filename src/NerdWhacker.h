//
//  NerdWhacker.h
//  Jeugdfilmfestival
//
//  Created by The Distillery on 23/02/14.
//
//

#ifndef NERDWHACKER
#define NERDWHACKER

#include <iostream>
#include "ofMain.h"
#include "Game.h"

#define NUM_FRAMES 24
#define NUM_BYTES_IN 3
#define NUM_BYTES_OUT 10



class NerdWhacker : public ofBaseApp {
    
public:
    void setup();
    void update();
    void draw();
    void keyReleased(int key);

private:

    // ARDUINO
    bool bSendSerialMessage;
    unsigned char bytesReturned[NUM_BYTES_IN];
    int countCycles;
    ofSerial serial;
    
    // GAME
    Game* game;
    
    // TEXT
    ofTrueTypeFont msg;
};



#endif
