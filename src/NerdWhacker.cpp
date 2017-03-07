//
//  NerdWhacker.cpp
//  Jeugdfilmfestival
//
//  Created by The Distillery on 23/02/14.
//
//

#include "NerdWhacker.h"



void NerdWhacker::setup() {

    // GENERAL
    ofSetFrameRate(NUM_FRAMES);
    ofSetWindowTitle("NerdWhacker");
	ofSetVerticalSync(true);
    
    // ARDUINO
    countCycles = 0;
    bSendSerialMessage = true;
    serial.enumerateDevices();
    serial.setup(0, 9600);
    //serial.setup("/dev/ttyACM0", 115200);
    // GAME
    game = new Game();
}



void NerdWhacker::update() {
    
  //  if (serial.isInitialized()==1) {
   //     cout << "serial.setup = ok" << endl;

        // ARDUINO WRITE
        if (bSendSerialMessage) {

            // score_1 + score_2 + score_3
            int score = game->score;
            string score_string = "";
            if (score<100) score_string += "0";
            if (score<10) score_string += "0";
            score_string += ofToString(score);
            const char * score_chars = score_string.c_str();
            char s1 = score_chars[0];
            char s2 = score_chars[1];
            char s3 = score_chars[2];
            
            // lives
            int lives = game->lives / 2;
            string lives_string = ofToString(lives);
            const char * lives_chars = lives_string.c_str();
            char l = lives_chars[0];
            
            // ranking_1 + ranking_2 + ranking_3
            int ranking = game->rank + 1;
            string ranking_string = "";
            if (ranking<100) ranking_string += "0";
            if (ranking<10) ranking_string += "0";
            ranking_string += ofToString(ranking);
            const char * ranking_chars = ranking_string.c_str();
            char r1 = ranking_chars[0];
            char r2 = ranking_chars[1];
            char r3 = ranking_chars[2];
            
            // topscore_1 + topscore_2 + topscore_3
            int topscore = game->topscore;
            string topscore_string = "";
            if (topscore<100) topscore_string += "0";
            if (topscore<10) topscore_string += "0";
            topscore_string += ofToString(topscore);
            const char * topscore_chars = topscore_string.c_str();
            char t1 = topscore_chars[0];
            char t2 = topscore_chars[1];
            char t3 = topscore_chars[2];
            
            // send string
            
            // vervangen door score_string!?
            unsigned char buf[NUM_BYTES_OUT] = {s1, s2, s3, l, t1, t2, t3, r1, r2, r3};
            serial.writeBytes(&buf[0], NUM_BYTES_OUT);
            cout << "Serial buffer: " << s1 << s2 << s3 << l << t1 << t2 << t3 << endl;
            
            bSendSerialMessage = false;
        }
        
        if(++countCycles == NUM_FRAMES) {
            bSendSerialMessage = true;
            countCycles = 0;
        }
        
        // ARDUINO READ
        unsigned char bytesReturned[NUM_BYTES_IN];
        memset(bytesReturned, 0, NUM_BYTES_IN);
        while (serial.readBytes(bytesReturned, NUM_BYTES_IN) > 0) {

        string plx = ofToString(bytesReturned);

        if (plx.length()>0) {
            cout << "input: " << plx << endl;
            
            switch (ofToChar(plx)) {
                case 'm': game->hitMultiPlayer(); break; // m = multiplayer
                case 'n': game->hitSinglePlayer(); break; // n = singleplayer
                case 'p': game->hitSinglePlayer(); break; // p = re/play
                case 'a': game->hitPlexi(0); break; // a = plexi 1
                case 'z': game->hitPlexi(1); break; // z = plexi 2
                case 'e': game->hitPlexi(2); break; // e = plexi 3
                case 'r': game->hitPlexi(3); break; // r = plexi 4
                case 'q': game->hitPlexi(4); break; // q = plexi 5
                case 's': game->hitPlexi(5); break; // s = plexi 6
                case 'd': game->hitPlexi(6); break; // d = plexi 7
                case 'f': game->hitPlexi(7); break; // f = plexi 8
                case 'w': game->hitPlexi(8); break; // w = plexi 9
                case 'x': game->hitPlexi(9); break; // x = plexi 10
                case 'c': game->hitPlexi(10); break; // c = plexi 11
                case 'v': game->hitPlexi(11); break; // v = plexi 12
            }
        }
        
    }
//    }
//    else {
//        cout << "(re)try serial.setup" << endl;
//        serial.setup(0, 115200);
//    }
    
    game->update();
}



void NerdWhacker::draw() {
    
    ofBackground(0, 0, 0);
    
    game->draw();
    
    ofColor(255, 255, 255);
    msg.loadFont("Arial.ttf", 15);
    msg.drawString(game->action, 20, 755);
}



void NerdWhacker::keyReleased(int key) {
    cout << key << endl;
    if (key==10) { // PRESS ENTER FOR CONFIG MODE
        game->setup = !game->setup;
    }
    
    if (game->setup) {
        switch (key) {
            case 'q': game->selectPlexi(0); break; // a = plexi 1
            case 'w': game->selectPlexi(1); break; // z = plexi 2
            case 'e': game->selectPlexi(2); break; // e = plexi 3
            case 'r': game->selectPlexi(3); break; // r = plexi 4
            case 'a': game->selectPlexi(4); break; // q = plexi 5
            case 's': game->selectPlexi(5); break; // s = plexi 6
            case 'd': game->selectPlexi(6); break; // d = plexi 7
            case 'f': game->selectPlexi(7); break; // f = plexi 8
            case 'z': game->selectPlexi(8); break; // w = plexi 9
            case 'x': game->selectPlexi(9); break; // x = plexi 10
            case 'c': game->selectPlexi(10); break; // c = plexi 11
            case 'v': game->selectPlexi(11); break; // v = plexi 12
            
            case OF_KEY_UP: game->movePlexi("y", -1); break;
            case OF_KEY_DOWN: game->movePlexi("y", 1); break;
            case OF_KEY_LEFT: game->movePlexi("x", -1); break;
            case OF_KEY_RIGHT: game->movePlexi("x", 1); break;
            case 111: game->resizePlexi(1); break; // O
            case 108: game->resizePlexi(-1); break;  // L
        }
    }
    else {
        switch (key) {
            case 'q': game->hitPlexi(0); break; // a = plexi 1
            case 'w': game->hitPlexi(1); break; // z = plexi 2
            case 'e': game->hitPlexi(2); break; // e = plexi 3
            case 'r': game->hitPlexi(3); break; // r = plexi 4
            case 'a': game->hitPlexi(4); break; // q = plexi 5
            case 's': game->hitPlexi(5); break; // s = plexi 6
            case 'd': game->hitPlexi(6); break; // d = plexi 7
            case 'f': game->hitPlexi(7); break; // f = plexi 8
            case 'z': game->hitPlexi(8); break; // w = plexi 9
            case 'x': game->hitPlexi(9); break; // x = plexi 10
            case 'c': game->hitPlexi(10); break; // c = plexi 11
            case 'v': game->hitPlexi(11); break; // v = plexi 12

            case 'm': game->hitMultiPlayer(); break; // m = multiplayer
            case 'n': game->hitSinglePlayer(); break; // n = singleplayer
            case 'p': game->hitSinglePlayer(); break; // p = re/play
        }
    }
}


