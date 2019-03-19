//
//  Game.cpp
//  Jeugdfilmfestival
//
//  Created by The Distillery on 23/02/14.
//
//

#import "ofMain.h"
#include "Game.h"



Game::Game() {
    cout << "game.game" << endl;
    
    ofFile file;
    
    file.open("settings.txt");
    
    if (file.exists()) {
        ofBuffer buffer = file.readToBuffer();
        vector<string> settings = ofSplitString(buffer.getText(), "\n");
        
        for (int p=0; p<sizeof(settings); p++) {
            vector<string> pos = ofSplitString(settings[p], ",");
            plexi[p] = new Plexi(ofToInt(pos[0]), ofToInt(pos[1]), ofToInt(pos[2]));
        }
    }
    else {
        plexi[0] = new Plexi(25, 25, 200);
        plexi[1] = new Plexi(275, 25, 200);
        plexi[2] = new Plexi(525, 25, 200);
        plexi[3] = new Plexi(775, 25, 200);
        plexi[4] = new Plexi(25, 275, 200);
        plexi[5] = new Plexi(275, 275, 200);
        plexi[6] = new Plexi(525, 275, 200);
        plexi[7] = new Plexi(755, 275, 200);
        plexi[8] = new Plexi(25, 525, 200);
        plexi[9] = new Plexi(275, 525, 200);
        plexi[10] = new Plexi(525, 525, 200);
        plexi[11] = new Plexi(775, 525, 200);
    }

    cout << "Reading Arduino: " << endl;
    image[0] = "graphics/sig1.jpg";
    value[0] = 1;
    image[1] = "graphics/sig2.jpg";
    value[1] = 1;
    image[2] = "graphics/sig3.jpg";
    value[2] = 1;
    image[3] = "graphics/sig4.jpg";
    value[3] = 1;
    image[4] = "graphics/sig5.jpg";
    value[4] = 1;
    image[5] = "graphics/sig6.jpg";
    value[5] = 1;
    image[6] = "graphics/sig7.jpg";
    value[6] = 1;
    image[7] = "graphics/beer4.jpg";
    value[7] = -1;

    setup = false;
    resize_plexi = -1;
    
    ranking.clear();
    
    countdown = false;
    playing = false;
    score = 0;
    topscore = 0;
}



void Game::start() {
    
    if (score > 0) {
        ranking.push_back(score);
    }
    
    for (int i=0; i<NUM_GRAPHICS; i++) {
        plexi[i]->init();
    }
    
    playing = false;
    speed = 1000;
    score = 0;
    lives = 18;
    rank = ranking.size();
    
    countdown = true;
    countdown_ts = ofGetElapsedTimef() + COUNTDOWN_TIME;
}



void Game::update() {

    rank = 0;
    for (int i=0; i<ranking.size(); i++) {
        if (score < ranking[i]) rank++;
    }
    
    //cout << "score=" << score << " topscore=" << topscore << " rank=" << rank+1 << endl;
    
    if (countdown) {
        int countdown_rest = floor(countdown_ts - ofGetElapsedTimef());
        //cout << "countdown: " << ofToString(countdown_rest) << endl;
        
        if (countdown_rest > 0) {
            string img;
            switch (countdown_rest) {
                case 3: img = "3"; break;
                case 2: img = "2"; break;
                case 1: img = "1"; break;
            }
            for (int i=0; i<NUM_PLEXIS; i++) {
                plexi[i]->image.loadImage("graphics/"+img+".jpg"); // _flipped
            }
        }
        
        if (countdown_rest<=0) {
            for (int i=0; i<NUM_PLEXIS; i++) {
                plexi[i]->image.clear();
            }
            
            countdown = false;
            playing = true;
        }
        
        action = "game / hitPlexi #" + ofToString(plexi) + " / countdown";
    }
    else if (playing) {
        
        // check if plexi is lit
        bool active = false;
        bool dead = false;
        
        for (int i=0; i<NUM_PLEXIS; i++) {
            
            if (plexi[i]->isLit) {
                active = true;
                
                if (plexi[i]->isHit) {
                    
                    if (plexi[i]->value > 0) {
                        score += plexi[i]->value;
                        if (score>topscore) topscore = score;
                        plexi[i]->value = 0;
                        plexi[i]->image.clear();
                        if (speed>1) speed -= 2;
                        
                        if (lives<=6) lives = 6;
                        else if (lives<=12) lives = 12;
                        else lives = 18;
                    }
                    
                    if (plexi[i]->value < 0) {
                        if (lives<=6) lives = 0;
                        else if (lives<=12) lives = 6;
                        else lives = 12;

                        plexi[i]->value = 0;
                        
                        if (lives==0) {
                            playing = false;
                            dead = true;
                        }
                    }
                }
            }
            
            plexi[i]->update();
        }
        
        if (dead) {
            for (int i=0; i<NUM_PLEXIS; i++) {
                plexi[i]->died();
            }
        }
        else if (!active) {
            lives--;
            if (lives==0) {
                playing = false;
                dead = true;
            }

            int r = ofRandom(NUM_GRAPHICS);
            plexi[int(ofRandom(NUM_PLEXIS))]->light(image[r], value[r], speed);
        }
        
        //cout << ofToString(score) << " | " << ofToString(lives) << " | " << ofToString(speed) << endl;
        action = "game / hitPlexi #" + ofToString(plexi) + " / " + ofToString(score) + "|" + ofToString(lives/2) + "|" + ofToString(speed);
    }
    else {
        
        for (int i=0; i<NUM_PLEXIS; i++) {
            plexi[i]->update();
        }
        
        if (!plexi[0]->isDead) {
            int r = ofRandom(NUM_GRAPHICS);
            plexi[int(ofRandom(NUM_PLEXIS))]->light(image[r], value[r], 500);
        }
    }
}



void Game::draw() {
    for (int p=0; p<NUM_PLEXIS; p++) {
        plexi[p]->draw();
    }
}



// SETUP=FALSE

void Game::hitPlexi(int plexi) {
    action = "game / hitPlexi #" + ofToString(plexi);
    cout << "game / hitPlexi" << endl;

    if (!countdown && !playing) {
            start();
    }
    plexi[this->plexi]->hit();
}

void Game::hitMultiPlayer() {
    action = "game / hitMultiPlayer";
    cout << "game / hitMultiPlayer" << endl;
    
    start();
}

void Game::hitSinglePlayer() {
    action = "game / hitSinglePlayer";
    cout << "game / hitSinglePlayer" << endl;

    start();
}



// SETUP=TRUE

void Game::selectPlexi(int plexi) {
    action = "setup / select plexi #" + ofToString(plexi);
    cout << "setup / select plexi #" << plexi << endl;

    resize_plexi = plexi;
}

void Game::movePlexi(string dir, int val) {
    if (resize_plexi>=0) {
        action = "setup / move plexi #" +  ofToString(resize_plexi) + "." + dir + " > " + ofToString(val);
        cout << "setup / move plexi #" << resize_plexi << " " << dir << " " << val << endl;

        if (dir=="x") {
            plexi[resize_plexi]->x += 1 * val;
        }
        else {
            plexi[resize_plexi]->y += 1 * val;
        }
        
        saveSettings();
    }
}

void Game::resizePlexi(int val) {
    if (resize_plexi>=0) {
        action = "setup / resize plexi #" +  ofToString(resize_plexi) + " > " + ofToString(val);
        cout << "resize plexi #" << resize_plexi << " " << val << endl;

        plexi[resize_plexi]->size += val;
        
        saveSettings();
    }
}

void Game::saveSettings() {
    ofFile file("settings.txt", ofFile::ReadWrite);
    
    if (file.canWrite()) {
        string settings;
        for (int p=0; p<NUM_PLEXIS; p++) {
            settings += ofToString(plexi[p]->x) + "," + ofToString(plexi[p]->y) + "," + ofToString(plexi[p]->size) + "\n";
        }
        
        ofBuffer buffer;
        buffer.set(settings.c_str(), settings.size());
        
        file.writeFromBuffer(buffer);
    }
}


