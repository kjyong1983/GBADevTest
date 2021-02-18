#include "bn_core.h"

#include "Particle.cpp"
#include "bn_keypad.h"


int main()
{
    bn::core::init();

    float playerX = 0;
    float playerY = 0;


    while(true)
    {
        if(bn::keypad::left_held()){
            playerX -= 1;
        }        

        if(bn::keypad::right_held()){
            playerX += 1;
        }        

        if(bn::keypad::up_held()){
            playerY -= 1;
        }        

        if(bn::keypad::down_held()){
            playerY += 1;
        }        

        if (bn::keypad::a_held())
        {
            // create particles
            
        }
        

        bn::core::update();
    }
}
