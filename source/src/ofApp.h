#pragma once

#include "ofMain.h"
#include "Disorder.hpp"


class ofApp : public ofBaseApp{

    public:
        void setup();
        void update();
        void draw();

        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void mouseEntered(int x, int y);
        void mouseExited(int x, int y);
        void windowResized(int w, int h);
        void gotMessage(ofMessage msg);
        
        void dragEvent(ofDragInfo dragInfo); //Drag seed
        void dreamChange(ofImage dreamseed); //changing the image
        void drawSeedImage(ofImage seed, int xOffset, int yOffset); //draw images on the side of the screen
        void dreamSelection(); //stage of dream draw
        void sleepStage();
        void sleepStyle();
    
    
        //Mesh
        ofMesh mesh;
    
        ofParameter <float> uiAmount; //amount on screen 3-100
        ofParameter <float> uiPos1;
        ofParameter <float> uiPos2;
        ofParameter <float> uiPos3;
        ofParameter <float> uiPos4;
        ofParameter <float> uiPos5;
        ofParameter <float> uiPos6;
        ofParameter<float> uiDistance;
        ofParameter<bool> uiPoints;
        ofParameter<bool> dreaming; //whether the participant is dreaming
        ofParameter<int> sleepType = 0; //participant
        ofParameter<bool> typeSelected; //whether the particiant has been selected
    
    //Image
        ofImage oceanImg;
        ofImage cityImg;
        ofImage greenImg;
        ofImage snowImg;
    
        vector<ofImage>dragImage;
        vector<ofColor>colours;

        int dream;
        int w, h;
        
 
    
    //Timer logic
        float startTime;
        bool timerEnd;
        ofParameter<float> endTime;
        unsigned long elapsedDreamTime = 0;
        float time;
        int seconds;
    
    //Current Values
        float sleepVal;
        float durationVal;
        
    //Speed of increase
        float stepAmount = 0.001;
        
    //Values used for scaling
        float sleepOldVal;
        float sleepNewVal;
        float durationOldVal;
        float durationNewVal;
        float timeAsleep;
        bool remTransition; //as the subject enters REM sleep
    
     //Sleep values in accordance to neurocognitive theory
        float S2 = 1.1f; //S = deeper sleep
        float S3 = 1.3f;
        float S4 = 1.5;
        float A1 = 0.1; // A = lighter sleep
        float A2 = 0.4;
        float A3 = 0.7;
        float REM = 2.0f; // REM sleep
    
   
    const int bruxTimes[16] = {0,46, 47, 86, 107, 137, 148, 167, 197, 208, 227, 245, 257, 287, 317, 327};
    const float bruxLevels[16] = {S2,S2,A3,A3,S2,S2,A2,S2,S2,A1,S2,S2,S2,A1,A1,S2};
    const float bruxDurations[16] = {30,30,13,15,30,30,11,30,30,11,30,7,30,30,30,30};
    
    const int insomTimes[16] = {0,40, 90, 128, 140, 170, 178, 200, 230, 260, 290, 320, 350, 380, 410, 460};
    const float insomLevels[16] = {A2,S3,S3,A3,S2,S2,A2,S2,S2,A3,0,0,0,0,0,0};
    const float insomDurations[16] = {17,30,30,21,30,30,22,30,30,30,30,30,30,30,30,30};
    
    const int controlTimes[16] = {0, 12, 23, 42, 72, 102, 132, 156, 162, 174, 192, 199, 222, 265, 272, 277};
    const float controlLevels[16] = {A1,S4,A1,S4,S4,S4,S4,A1,S4,A1,S4,A1,S4,A1,S4,A2};
    const float controlDurations[16] = {6,30,11,30,30,30,30,8,30,9,30,4,30,30,7,30};
    
    const int narcTimes[16] = {0, 5, 13, 43, 46, 73, 103, 132, 133, 163, 166, 180, 193, 223, 253, 276};
    const float narcLevels[16] = {S2,A3,S2,S2,A2,S2,S2,A1,S2,S3,A1,A1,S3,S3,S3,A1};
    const float narcDurations[16] = {30,20,30,30,13,30,30,8,30,30,5,10,30,30,30,11};
    
    const int remTimes[16] = {0, 9, 39, 69, 99, 129, 159, 189, 219, 249, 265, 279, 309, 332, 339, 353};
    const float remLevels[16] = {REM,REM,REM,REM,REM,REM,REM,S2,REM,S2,A1,S2,S2,A1,S2,A1};
    const float remDurations[16] = {30,30,30,30,30,30,30,30,30,30,6,30,30,6,30,6};
    
        //Participants
    class Disorder brux{bruxTimes, bruxLevels,bruxDurations};
    class Disorder insom{insomTimes, insomLevels,insomDurations};
    class Disorder control{controlTimes, controlLevels,controlDurations};
    class Disorder narc{narcTimes, narcLevels,narcDurations};
    class Disorder rbd{remTimes, remLevels,remDurations};
//
//    class Insomnia insom{s2,s3,s4,a1,a2,a3,rem};
//
//    class Control control{s2,s3,s4,a1,a2,a3,rem};
//
//    class Narcolepsy narc{s2,s3,s4,a1,a2,a3,rem};
//
//    class RBD rbd{s2,s3,s4,a1,a2,a3,rem};
    
    
//    class Illness brux{s2,s3,s4,a1,a2,a3,rem};
//    class Illness insom{s2,s3,s4,a1,a2,a3,rem};
//    class Illness control{s2,s3,s4,a1,a2,a3,rem};
//    class Illness narc{s2,s3,s4,a1,a2,a3,rem};
//    class Illness rbd{s2,s3,s4,a1,a2,a3,rem};
    
    
};
