//
//  Disorder.hpp
//  dreamSimDebug
//
//  Created by Jake on 13/04/2023.
//

#ifndef Disorder_hpp
#define Disorder_hpp

#include <stdio.h>

class Disorder
{
public:
    Disorder(const int times[16], const float types[16], const float durations[16]); //constructor, passes the dream level values
    ~Disorder(); //destructor
    
    void run(int seconds, float& sleepOldVal, float& sleepNewVal, float& durationOldVal, float& durationNewVal);
    
    private:
    
        int data[16];
        float sleepType[16];
        float eventDuration[16];
};

#endif /* Disorder_hpp */
