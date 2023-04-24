#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofResetElapsedTimeCounter();

    dreaming = false;
    typeSelected = false;
    uiPoints = true;
    timerEnd = false;
    
    
    uiPos1 = 276; //Values for where the mesh is located
    uiPos2 = -408;
    uiPos3 = -246;
    uiPos4 = 228;
    
    uiPos5 = 192; //Will be changed
    uiPos6 = -96;
    
    uiAmount = 1; //amount of "stars"
    uiDistance = 60; //closeness of starts to form a connection
    
    
    //Timer logic
    startTime = ofGetElapsedTimeMillis();
    
    //visuals
    ofSetBackgroundColor(0, 0, 0);
    ofSetFrameRate(60);
    mesh.setMode(OF_PRIMITIVE_LINES);
    
    //Data values being declared
    sleepVal = 0.1f;
    durationVal = 0.1f;
    sleepOldVal = 0.f;
    sleepNewVal = 0.f;
    durationOldVal = 0.f;
    durationNewVal = 0.f;
    
    //Picture Seeds
    oceanImg.load("Ocean.jpeg");
    cityImg.load("City.jpeg");
    snowImg.load("Snow.jpeg");
    greenImg.load("Green.jpeg");
    //Sets the first image as the ocean(will be changed when the user drags their own seed
    dreamChange(oceanImg);
}

//--------------------------------------------------------------
void ofApp::update(){
    //Scales sleep values from one to the next to create one rising or falling number
        
    sleepVal += (sleepNewVal - sleepVal) * (stepAmount);
    durationVal += (durationNewVal - durationVal) * (stepAmount);


    //Mesh
    if(sleepVal < 0.4) //before the stage "a2"(slightly awake still)
    {
        ofSeedRandom(durationVal); //slows things down and creates a stuttering effect
    }else
    {
        ofSeedRandom(60); //sets a random seed
    }
    mesh.clear();

    for(int i = 0; i < uiAmount; i++) //for loop running through all the particles
    {
        float speedAmount = ofMap(durationVal, 0.f, 30.f, 0.98, 1.04); //Maps speed of particles

        ofVec3f position = ofVec3f( //Position of particles
                            ofMap(ofNoise(ofRandom(600 * speedAmount), ofGetElapsedTimef() * 0.006), 0, 1, uiPos1, uiPos2), //X
                            ofMap(ofNoise(ofRandom(600 * speedAmount), ofGetElapsedTimef() * 0.006), 0, 1, uiPos3, uiPos4), //Y
                                   ofMap(ofNoise(ofRandom(600 * speedAmount), ofGetElapsedTimef() * 0.006), 0, 1, uiPos5, uiPos6)); //Z

        mesh.addVertex(position);

        int indexRandomness = ofMap(sleepVal, 0.f, 2.f, 10, 1); //as the sleepValue increases, more of the photo weill be processed

        int randomIndex = ofRandom(colours.size() /indexRandomness); //assigns a random pixel for each star


        int scaledIndex = ofMap(sleepVal, 1.8, 1.925, 0, colours.size()); //Will analyse more of the picture as the dream increases

        int remAlpha = ofRandom(ofMap(durationVal, 15.f, 30.f, 0, 255)); //maps duration to the alpha of new vertexs created when in REM SLEEP

        int pixelIndex; //this will be placed to assign a colour to the mesh
        int pixelAlpha;

        //choreographed changes to the mesh logic at certain levels of sleep

        if(sleepVal >= 1.8 && sleepVal <= 1.81) //rem transition
        {
            remTransition = true;
            pixelIndex = scaledIndex;
            mesh.addColor(ofColor(colours[pixelIndex][0],colours[pixelIndex][1],colours[pixelIndex][2], pixelAlpha));
        }
        else
        {
            remTransition = false;
        }


        if(sleepVal >= 1.81) //In rem sleep
        {
            
            pixelIndex = randomIndex;

            if((sleepVal > 1.86 && sleepVal < 1.863) || (sleepVal > 1.92 && sleepVal < 1.923))//increments
            {
                pixelIndex = scaledIndex; //creates a flashing effect
            }

            uiPos5 -= timeAsleep / ofGetFrameRate(); //will zoom in
            pixelAlpha = remAlpha;

        }
        else
        {
            pixelIndex = randomIndex;
            pixelAlpha = colours[pixelIndex][3];
            uiPos5 = 192; //zooms out when not in rem sleep
            uiPos6 = -96;
        }

        mesh.addColor(ofColor(colours[pixelIndex][0],colours[pixelIndex][1],colours[pixelIndex][2], pixelAlpha));
    }

    //end of for loop in UI amount


    for(int i = 0; i < mesh.getVertices().size(); i++)
    {
        auto position = mesh.getVertex(i);

        for(int j = 0; j < mesh.getVertices().size(); j++)
        {
            auto jdistance = mesh.getVertex(j);
            auto distance = glm::distance(position, jdistance); //checking distance from position and jdistance

            if(distance < uiDistance) //if they are close to each other
            {
                mesh.addIndex(i); //adds a line between two vertices
                mesh.addIndex(j);

            }
        }
    }

    //Deciding what illness function to run through
    
    if(dreaming)
    {
        if(sleepType == 1)
        {
            brux.run(seconds,sleepOldVal,sleepNewVal,durationOldVal,durationNewVal);
            timeAsleep = 2.5;
        }
        if(sleepType == 2)
        {
            insom.run(seconds,sleepOldVal,sleepNewVal, durationVal, durationNewVal);
            timeAsleep = 2.5;
        }
        if(sleepType == 3)
        {
            control.run(seconds,sleepOldVal,sleepNewVal,durationOldVal,durationNewVal);
            timeAsleep = 1.1;
        }
        if(sleepType == 4)
        {
            narc.run(seconds,sleepOldVal,sleepNewVal, durationVal, durationNewVal);
            timeAsleep = 6.15;
        }
        if(sleepType == 5)
        {
            rbd.run(seconds,sleepOldVal,sleepNewVal,durationOldVal,durationNewVal);
            timeAsleep = 1.56;
        }
    }
    
    if(dreaming == false)
    {
        sleepVal = 0.f;
        durationVal = 0.f;
    }

    if(seconds >= 300)
        {
            setup(); //reset at the end of the dream
        }

}

//--------------------------------------------------------------
void ofApp::draw(){
    //FIRST MENU
    if(dreaming == false)
    {
        if(typeSelected == false) //Loading Screen
        {
            ofDrawBitmapString(" The aim of this project is to provide a visual representation \n of the sleep process, created from data provided by the \n CAP Sleep Database. By selecting a participant the mesh onscreen will \n grow, shrink and move in relation to the stage of sleep the \n participant is in. The colour of the mesh is determined by the 'dream seed' \n where, the program will select random colours from a portion of the \n image provided. As the subject falls deeper asleep, the portion \n of the image that is analysed will increase. Enjoy!", 10, 25);
            
            ofDrawBitmapString("Sleep stages \nfrom low to high \ninclude:", ofGetWidth() * 0.8, 85);
            ofDrawBitmapString("A1", ofGetWidth() * 0.9, 130);
            ofDrawBitmapString("A2", ofGetWidth() * 0.9, 145);
            ofDrawBitmapString("A3", ofGetWidth() * 0.9, 160);
            ofDrawBitmapString("S2", ofGetWidth() * 0.9, 175);
            ofDrawBitmapString("S3", ofGetWidth() * 0.9, 190);
            ofDrawBitmapString("S4", ofGetWidth() * 0.9, 205);
            ofDrawBitmapString("REM", ofGetWidth() * 0.9,225);

            ofDrawBitmapString("Participant Selcted : ", ofGetWidth() * 0.45, ofGetHeight() * 0.6);
            ofDrawBitmapString(sleepType, ofGetWidth() * 0.63, ofGetHeight() * 0.6);

            ofDrawBitmapString("Welcome to the dream Simulator!", ofGetWidth() * 0.4, ofGetHeight() * 0.4);

            ofSetColor(230, 230, 255);
            ofDrawBitmapString("CONTAINS FLASHING LIGHTS", ofGetWidth() * 0.8, ofGetHeight() * 0.05);

            ofSetColor(255);
            ofDrawBitmapString("Please select the participant you wish to dream using keys 1-5 \n and then pressing 'r'", ofGetWidth() * 0.25, ofGetHeight() * 0.5);

            ofDrawBitmapString("1. Bruxism", ofGetWidth() * 0.05, ofGetHeight() * 0.75);
            ofDrawBitmapString("involuntary habitual \n grinding of \n the teeth \n (MID LEVEL SLEEP)", ofGetWidth() * 0.05, ofGetHeight() * 0.8);

            ofDrawBitmapString("2. Insomnia", ofGetWidth() * 0.25, ofGetHeight() * 0.75);
            ofDrawBitmapString("habitual sleeplesness \n inability to \n sleep \n (LOW LEVEL / NO SLEEP", ofGetWidth() * 0.25, ofGetHeight() * 0.8);

            ofDrawBitmapString("3. N/A", ofGetWidth() * 0.45, ofGetHeight() * 0.75);
            ofDrawBitmapString("control subject \n with no known \n disorder \n (DEEP SLEEP)", ofGetWidth() * 0.45, ofGetHeight() * 0.8);

            ofDrawBitmapString("4. Narcolepsy", ofGetWidth() * 0.65, ofGetHeight() * 0.75);
            ofDrawBitmapString("affects the brain's \n ability \n to control \n  sleep-wake \n cycles \n (LOW LEVEL SLEEP)", ofGetWidth() * 0.65, ofGetHeight() * 0.8);

            ofDrawBitmapString("5. Rem Behavioural \n Disorder", ofGetWidth() * 0.85, ofGetHeight() * 0.75);
            ofDrawBitmapString("increased physical \n movements during \n sleep \n (REM SLEEP)", ofGetWidth() * 0.85, ofGetHeight() * 0.8);
        }

        else if(typeSelected == true) //When a participant has been chosen
        {
            ofDrawBitmapString("Thanks for choosing!", ofGetWidth() * 0.4, ofGetHeight() * 0.4);

            ofDrawBitmapString("Now drag a jpeg of your choice onto the screen to begin the dream", ofGetWidth() * 0.25, ofGetHeight() * 0.45);
        }
    }

    if(dreaming == true)
        {
            
            
            //Timer
            time = ofGetElapsedTimeMillis() - startTime;
            seconds = (int)time / 1000;

            
            //Image selection
            ofDrawBitmapString("Press '1'", ofGetWidth() * 0.85, ofGetHeight() * 0.5 - 100);
            drawSeedImage(oceanImg, 0, -100);

            ofDrawBitmapString("Press '2'", ofGetWidth() * 0.85, ofGetHeight() * 0.5);
            drawSeedImage(cityImg, 0,0);

            ofDrawBitmapString("Press '3'", ofGetWidth() * 0.85, ofGetHeight() * 0.5 + 100);
            drawSeedImage(snowImg, 0,100);

            ofDrawBitmapString("Press '4'", ofGetWidth() * 0.85, ofGetHeight() * 0.5 + 200);
            drawSeedImage(greenImg, 0,200);

            ofDrawBitmapString("Press '5'", ofGetWidth() * 0.85, ofGetHeight() * 0.5 + 300);
            ofDrawBitmapString("(Your Seed)'", ofGetWidth() * 0.85, ofGetHeight() * 0.5 + 330);

            if(dragImage.size() > 0)
            {
                for(int i = 0; i < dragImage.size(); i++)
                {
                    drawSeedImage(dragImage[i], 0, 300);
                }

            }

            ofDrawBitmapString("Press the Spacebar to choose a new participant", ofGetWidth() * 0.45, ofGetHeight() * 0.95);

            
            ofSetColor(255);
            ofDrawBitmapString(seconds, ofGetWidth() * 0.52, 25);
            ofDrawBitmapString("/ 300", ofGetWidth() * 0.55, 25);
            ofDrawBitmapString("seconds", ofGetWidth() * 0.6, 25);

            ofDrawBitmapString(sleepVal, ofGetWidth() * 0.9, ofGetHeight() * 0.1);
            ofDrawBitmapString("Sleep Value", ofGetWidth() * 0.8, ofGetHeight() * 0.1);


            ofDrawBitmapString(durationVal, ofGetWidth() * 0.94, ofGetHeight() * 0.2);
            ofDrawBitmapString("Duration Value", ofGetWidth() * 0.8, ofGetHeight() * 0.2);

            sleepStage(); //function that draws the stage of sleep
            
            sleepStyle(); //draws the participants disorder

            if(remTransition)
            {
                ofSetColor(255, 0, 0);
                ofDrawBitmapString("REM Transition", ofGetWidth() * 0.1, 25);
            }
            if(sleepVal >= 1.81)
            {
                ofSetColor(255, 0, 0);
                ofDrawBitmapString("REM DREAMING", ofGetWidth() * 0.1, 25);
            }

            ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
            ofSetColor(255);

            float sizeVal = ofMap(durationVal,0,30,4,3);

                if(uiPoints == true) //if true
                {
                    for(int i = 0; i < mesh.getVertices().size(); i++)
                    {
                        ofDrawSphere(mesh.getVertex(i),sizeVal); //puts a sphere with width and height 3
                    }
                }

            mesh.draw();

            //Mapping
            uiDistance = sleepVal * 100;
            uiAmount = sleepVal * 100;
            uiPos5 = ofMap(sleepVal, 2.f, 0.f, 300, 0);
            uiPos6 = ofMap(sleepVal,2.f, 0.f, -200, 400);


            if(sleepVal < 1.8)
            {
                uiPoints = true;
            }

            else if(sleepVal >= 1.8)
            {
                uiPoints = false; //starts on the side of the mesh
            }

        }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == ' ')
    {
        setup();
        
    }
        if(dreaming == false)
        {
            
            //Sleeptype selection
            
            if(key == '1')
            {
                sleepType = 1; //brux
            }
            if(key == '2')
            {
                sleepType = 2; //insom
            }
            if(key == '3')
            {
                sleepType = 3; //control
            }
            if(key == '4')
            {
                sleepType = 4; //narc
            }
            if(key == '5')
            {
                sleepType = 5; //rbd
            }
            
            if(key == 'r' && sleepType > 0)//confirm selection
            {
                
                typeSelected = true;
                std::cout<< typeSelected <<std::endl;
            }
        }
        if(dreaming)
        {
            //seed seletion
            if(key == '1')
            {
                dreamChange(oceanImg);
            }
            if(key == '2')
            {
                dreamChange(cityImg);
            }
            if(key == '3')
            {
                dreamChange(snowImg);
            }
            if(key == '4')
            {
                dreamChange(greenImg);
            }
        }
        
        //Own image
        if(key == '5' && dragImage.size() > 0)
        {
            for(int i = 0; i < dragImage.size(); i++)
            {
                dreamChange(dragImage[i]);
            }
        }
}

void ofApp::dreamChange(ofImage dreamSeed)
{
    
    colours.clear(); //clears vector
        
        ofPixels pixels = dreamSeed.getPixels();
        int w = dreamSeed.getWidth();
        int h = dreamSeed.getHeight();
    
    
        for (int i = 0; i < h; i++)
        {
            for(int j = 0; j < w; j++)
            {
                ofColor color = pixels.getColor(i, j);
                colours.push_back(color); //fills vector with the colours
            }
        }
}
void ofApp::drawSeedImage(ofImage seed, int xOffset, int yOffset) //draws seed images on the side of the screen
{
    //Max width and height
    int maxWidth = 100;
    int maxHeight = 100;
    
    //calculate new width and height while maintaining aspect ratio
    float imageRatio = (float)seed.getWidth() / (float)seed.getHeight();
    float newWidth, newHeight;
    if(imageRatio > 1.0f)
    {
        newWidth = maxWidth;
        newHeight = maxWidth / imageRatio;
    } else{
        newWidth = maxHeight * imageRatio;
        newHeight = maxHeight;
    }
    
    seed.resize(newWidth, newHeight);
    
    int x = ofGetWidth() * 0.95;
    int y = ofGetHeight() * 0.5;
    
    seed.draw(x + xOffset,y + yOffset);
}

void ofApp::sleepStage()
{
   
    ofDrawBitmapString("Sleep Stage: ", 5, 25);
    if(sleepVal < 0.1)
    {
        ofDrawBitmapString("Awake", ofGetWidth() * 0.15, 25);
    }
    if(sleepVal > 0.1 && sleepVal < 0.4)
    {
        ofDrawBitmapString("A1", ofGetWidth() * 0.15, 25);
    }
    if(sleepVal >= 0.4 && sleepVal < 0.7)
    {
        ofDrawBitmapString("A2", ofGetWidth() * 0.15, 25);
    }
    if(sleepVal >= 0.7 && sleepVal < 1.1)
    {
        ofDrawBitmapString("A3", ofGetWidth() * 0.15, 25);
    }
    if(sleepVal >= 1.1 && sleepVal < 1.3)
    {
        ofDrawBitmapString("S2", ofGetWidth() * 0.15, 25);
    }
    if(sleepVal >= 1.3 && sleepVal < 1.5)
    {
        ofDrawBitmapString("S3", ofGetWidth() * 0.15, 25);
    }
    if(sleepVal >= 1.5 && sleepVal < 1.8)
    {
        ofDrawBitmapString("S4", ofGetWidth() * 0.15, 25);
    }

};

void ofApp::sleepStyle()
{
    if(sleepType == 1)
    {
        ofDrawBitmapString("Bruxism", ofGetWidth() * 0.05, ofGetHeight() * 0.95);
    }
    if(sleepType == 2)
    {
        ofDrawBitmapString("Insomnia", ofGetWidth() * 0.05, ofGetHeight() * 0.95);
    }
    if(sleepType == 3)
    {
        ofDrawBitmapString("No Disorder", ofGetWidth() * 0.05, ofGetHeight() * 0.95);
    }
    if(sleepType == 4)
    {
        ofDrawBitmapString("Narcolepsy", ofGetWidth() * 0.05, ofGetHeight() * 0.95);
    }
    if(sleepType == 5)
    {
        ofDrawBitmapString("RBD", ofGetWidth() * 0.05, ofGetHeight() * 0.95);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    if(dragInfo.files.size() > 0 && typeSelected == true)
    {
        dragImage.assign(dragInfo.files.size(), ofImage());
        for(int i = 0; i < dragInfo.files.size(); i++)
        {
            dragImage[i].load(dragInfo.files[i]); //Loads your own dream seed
            
            if(dreaming == false)
            {
                ofResetElapsedTimeCounter();
                dreaming = true;
            }
        }
    }
}
