#include "bn_core.h"

// for examples
#include "info.h"

#include "bn_assert.h"

// bn headers
// #include "bn_math.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_sprite_ptr.h"
// #include "bn_bg_palettes.h"
#include "bn_regular_bg_ptr.h"

// camera
#include "bn_blending.h"
#include "bn_optional.h"
#include "bn_rect_window.h"
#include "bn_camera_actions.h"

// gfx
#include "bn_bg_palettes.h"
#include "bn_bgs_mosaic_actions.h"
#include "bn_mosaic_attributes.h"
#include "bn_mosaic_attributes_hblank_effect_ptr.h"
#include "bn_sprites_mosaic_actions.h"

// text generator
#include "bn_sprite_text_generator.h"

// fonts
#include "fixed_8x8_sprite_font.h"
#include "fixed_8x16_sprite_font.h"
#include "variable_8x8_sprite_font.h"
#include "variable_8x16_sprite_font.h"

// sprites
#include "bn_sprite_items_blonde.h"
#include "bn_sprite_items_dino.h"

// bgs
#include "bn_regular_bg_items_land.h"
#include "bn_regular_bg_items_clouds.h"

void button_test(bn::sprite_text_generator& text_generator){

    int offsetY = -80;
    int space = 10;

    bn::vector<bn::sprite_ptr, 32> text_sprites;
    text_generator.set_center_alignment();
    // text_generator.generate(0, 0, "Button test, press any key except START button", text_sprites);

    while(! bn::keypad::start_pressed())
    {
        text_generator.generate(0, 60, "press any key except START button", text_sprites);

        if(bn::keypad::left_held())
        {
            text_generator.generate(0, offsetY + space * 1, "left", text_sprites);
        }
        
        if(bn::keypad::right_held())
        {
            text_generator.generate(0, offsetY + space * 2, "right", text_sprites);
        }

        if(bn::keypad::down_held())
        {
            text_generator.generate(0, offsetY + space * 3, "down", text_sprites);
        }
        
        if(bn::keypad::up_held())
        {
            text_generator.generate(0, offsetY + space * 4, "up", text_sprites);
        }

        if (bn::keypad::start_held())
        {
            text_generator.generate(0, offsetY + space * 5, "start", text_sprites);
        }
        
        if (bn::keypad::select_held())
        {
            text_generator.generate(0, offsetY + space * 6, "SELECT", text_sprites);
        }
        
        if (bn::keypad::a_held())
        {
            text_generator.generate(0, offsetY + space * 7, "A", text_sprites);
        }
        
        if (bn::keypad::b_held())
        {
            text_generator.generate(0, offsetY + space * 8, "B", text_sprites);
        }

        if (bn::keypad::l_held())
        {
            text_generator.generate(0, offsetY + space * 9, "L", text_sprites);
        }
        
        if (bn::keypad::r_held())
        {
            text_generator.generate(0, offsetY + space * 10, "R", text_sprites);
        }

        bn::core::update();

        // clear sprites, or will be explode!
        text_sprites.clear();
    }
}

void sprite_move_test(bn::sprite_text_generator& text_generator){
    constexpr const bn::string_view info_text_lines[] = {
        "LEFT & RIGHT: move x",
        "UP & DOWN: move y",
        "L & R: change speed",
        "START: go to next scene",
    };

    info info("Sprites movement", info_text_lines, text_generator);

    bn::sprite_ptr blonde_sprite = bn::sprite_items::blonde.create_sprite(0, 0);

    int speed = 1;
    float modifier = 1.0;

    // left-up is (0, 0)
    while(! bn::keypad::start_pressed())
    {
        if(bn::keypad::left_held())
        {
            blonde_sprite.set_x(blonde_sprite.x() - speed * modifier);
        }
        else if(bn::keypad::right_held())
        {
            blonde_sprite.set_x(blonde_sprite.x() + speed * modifier);
        }

        if(bn::keypad::down_held())
        {
            blonde_sprite.set_y(blonde_sprite.y() + speed * modifier);
        }
        else if(bn::keypad::up_held())
        {
            blonde_sprite.set_y(blonde_sprite.y() - speed * modifier);
        }

        if (bn::keypad::l_pressed())
        {
            modifier *= 0.8;
        }

        if (bn::keypad::r_pressed())
        {
            modifier *= 1.2;
        }

        info.update();
        bn::core::update();
    }

    bn::sprites_mosaic::set_stretch(0);
}

void sprite_mosaic_test(bn::sprite_text_generator& text_generator){
    constexpr const bn::string_view info_text_lines[] = {
        "LEFT: decrease horizontal stretch",
        "RIGHT: increase horizontal stretch",
        "DOWN: decrease vertical stretch",
        "UP: increase vertical stretch",
        "",
        "START: go to next scene",
    };

    info info("Sprites mosaic", info_text_lines, text_generator);

    bn::sprite_ptr blonde_sprite = bn::sprite_items::blonde.create_sprite(0, 0);
    blonde_sprite.set_mosaic_enabled(true);

    bn::sprites_mosaic::set_stretch(0.1);

    while(! bn::keypad::start_pressed())
    {
        bn::fixed horizontal_stretch = bn::sprites_mosaic::horizontal_stretch();
        bn::fixed vertical_stretch = bn::sprites_mosaic::vertical_stretch();

        if(bn::keypad::left_held())
        {
            bn::sprites_mosaic::set_horizontal_stretch(bn::max(horizontal_stretch - 0.01, bn::fixed(0)));
        }
        else if(bn::keypad::right_held())
        {
            bn::sprites_mosaic::set_horizontal_stretch(bn::min(horizontal_stretch + 0.01, bn::fixed(1)));
        }

        if(bn::keypad::down_held())
        {
            bn::sprites_mosaic::set_vertical_stretch(bn::max(vertical_stretch - 0.01, bn::fixed(0)));
        }
        else if(bn::keypad::up_held())
        {
            bn::sprites_mosaic::set_vertical_stretch(bn::min(vertical_stretch + 0.01, bn::fixed(1)));
        }

        info.update();
        bn::core::update();
    }

    bn::sprites_mosaic::set_stretch(0);
}

void camera_scene(bn::camera_ptr& camera, bn::sprite_text_generator& text_generator)
{
    constexpr const bn::string_view info_text_lines[] = {
        "PAD: move camera",
        "",
        "START: go to next scene",
    };

    info info("Camera position", info_text_lines, text_generator);

    while(! bn::keypad::start_pressed())
    {
        if(bn::keypad::left_held())
        {
            camera.set_x(camera.x() - 1);
        }
        else if(bn::keypad::right_held())
        {
            camera.set_x(camera.x() + 1);
        }

        if(bn::keypad::up_held())
        {
            camera.set_y(camera.y() - 1);
        }
        else if(bn::keypad::down_held())
        {
            camera.set_y(camera.y() + 1);
        }

        info.update();
        bn::core::update();
    }

    camera.set_position(0, 0);
}

class CameraScene{
    public:
        bn::regular_bg_ptr      land_bg;
        bn::regular_bg_ptr      clouds_bg;
        bn::sprite_ptr          dino_sprite;
        bn::window              outside_window;
        bn::rect_window         internal_window;
        bn::camera_ptr          camera;

        void InitScene(){
            land_bg = bn::regular_bg_items::land.create_bg(0, 0);
            clouds_bg = bn::regular_bg_items::clouds.create_bg(0, 0);
            bn::blending::set_transparency_alpha(0.5);
            clouds_bg.set_blending_enabled(true);

            dino_sprite = bn::sprite_items::dino.create_sprite(-60, 0);
            outside_window = bn::window::outside();
            outside_window.set_show_bg(clouds_bg, false);

            internal_window = bn::rect_window::internal();
            internal_window.set_boundaries(-48, 8, 48, 104);

            camera = bn::camera_ptr::create(0, 0);
            land_bg.set_camera(camera);
            dino_sprite.set_camera(camera);
            internal_window.set_camera(camera);
        }

        void ExitScene(){
            land_bg.;
            clouds_bg.;
            dino_sprite.;
            outside_window
            internal_window
            camera
        }

    private:
};

// void init_camera_scene(){
//     bn::regular_bg_ptr land_bg = bn::regular_bg_items::land.create_bg(0, 0);

//     bn::regular_bg_ptr clouds_bg = bn::regular_bg_items::clouds.create_bg(0, 0);
//     bn::blending::set_transparency_alpha(0.5);
//     clouds_bg.set_blending_enabled(true);

//     bn::sprite_ptr dino_sprite = bn::sprite_items::dino.create_sprite(-60, 0);

//     bn::window outside_window = bn::window::outside();
//     outside_window.set_show_bg(clouds_bg, false);

//     bn::rect_window internal_window = bn::rect_window::internal();
//     internal_window.set_boundaries(-48, 8, 48, 104);

//     bn::camera_ptr camera = bn::camera_ptr::create(0, 0);
//     land_bg.set_camera(camera);
//     dino_sprite.set_camera(camera);
//     internal_window.set_camera(camera);
// }

// void exit_camera_scene(){

// }

int main()
{
    bn::core::init();
    
    bn::sprite_text_generator text_generator(variable_8x16_sprite_font);
    bn::bg_palettes::set_transparent_color(bn::color(16,16,16));

    // camera
    // bn::regular_bg_ptr land_bg = bn::regular_bg_items::land.create_bg(0, 0);

    // bn::regular_bg_ptr clouds_bg = bn::regular_bg_items::clouds.create_bg(0, 0);
    // bn::blending::set_transparency_alpha(0.5);
    // clouds_bg.set_blending_enabled(true);

    // bn::sprite_ptr dino_sprite = bn::sprite_items::dino.create_sprite(-60, 0);

    // bn::window outside_window = bn::window::outside();
    // outside_window.set_show_bg(clouds_bg, false);

    // bn::rect_window internal_window = bn::rect_window::internal();
    // internal_window.set_boundaries(-48, 8, 48, 104);

    // bn::camera_ptr camera = bn::camera_ptr::create(0, 0);
    // land_bg.set_camera(camera);
    // dino_sprite.set_camera(camera);
    // internal_window.set_camera(camera);

    CameraScene cameraScene = CameraScene();

    while(true)
    {
        button_test(text_generator);
        bn::core::update();

        sprite_mosaic_test(text_generator);
        bn::core::update();

        sprite_move_test(text_generator);
        bn::core::update();

        // camera_scene(camera, text_generator);
        // bn::core::update();

        cameraScene.InitScene();
        camera_scene(cameraScene.camera, text_generator);
        bn::core::update();
        cameraScene.ExitScene();
    }
}