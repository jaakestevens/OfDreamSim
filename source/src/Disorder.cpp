//
//  Disorder.cpp
//  dreamSimDebug
//
//  Created by Jake on 13/04/2023.
//

#include "Disorder.hpp"

Disorder::Disorder(const int times[16], const float type[16], const float duration[16])
{


    for(int i = 0; i < 15; i++)
    {
        data[i] = times[i];
    }

    for(int i = 0; i < 15; i++)
    {
        sleepType[i] = type[i];
    }

    //event duration

    for(int i = 0; i < 15; i++)
    {
        eventDuration[i] = duration[i];

    }

}
Disorder::~Disorder()
{

}
void Disorder::run(int seconds, float& sleepOldVal, float& sleepNewVal, float& durationOldVal, float& durationNewVal)
{
        for(int i = 0; i < 15; i++)
        {
            if(data[i] == seconds) //if the time in seconds array matches with the progress of the dream
            {
                sleepNewVal = sleepType[i]; //the new value becomes this value and the previous is scaled up to this value

                if(i > 0)
                {
                    sleepOldVal = sleepType[i-1];
                }

                durationNewVal = eventDuration[i];

                if(i > 0)
                {
                    durationOldVal = eventDuration[i-1];
                }
            }
        }
}
