#include "bn_sprite_builder.h"
#include <vector>

class Particle{
private:
    float x = 0;
    float y = 0;
    float dx = 0;
    float dy = 0;
    float life = 0;
    float currentLife = 0;

    float gravity = 0;
    float dx = 0;
    float dx = 0;

    float scale = 0;
    float dScale = 0;
    // animation as sprite array?

    // std::list< bn::sprite_ptr > particleSprites = nullptr;

    void update(){
        x += dx;
        y += dy;
        dy += gravity;
        scale *= dScale;
        life -= 0.02f;

        if (life < 0){
            delete this;
        }

    }

public:
    // Particle(){
    // }

    // ~Particle(){
    // }
};