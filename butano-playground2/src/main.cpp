#include "bn_log.h"

#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_blending.h"
#include "bn_bg_palettes.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprites_mosaic.h"
#include "bn_sprite_actions.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_first_attributes.h"
#include "bn_sprite_third_attributes.h"
#include "bn_sprite_affine_second_attributes.h"
#include "bn_sprite_regular_second_attributes.h"
#include "bn_sprite_position_hblank_effect_ptr.h"
#include "bn_sprite_first_attributes_hblank_effect_ptr.h"
#include "bn_sprite_third_attributes_hblank_effect_ptr.h"
#include "bn_sprite_affine_second_attributes_hblank_effect_ptr.h"
#include "bn_sprite_regular_second_attributes_hblank_effect_ptr.h"

#include "info.h"
#include "stats.h"

// sprites
#include "bn_sprite_items_blue_sprite.h"
#include "bn_sprite_items_yellow_sprite.h"
#include "bn_sprite_items_red_sprite.h"
#include "bn_sprite_items_green_sprite.h"
#include "bn_sprite_items_caveman.h"
#include "bn_sprite_items_ninja.h"
#include "bn_sprite_items_arrows2.h"

// bgs
#include "bn_regular_bg_items_land.h"
#include "bn_regular_bg_items_clouds.h"

void draw_sprite_test(){
    bn::sprite_ptr red_sprite = bn::sprite_items::red_sprite.create_sprite(0, 0);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::a_pressed())
            {
                red_sprite.set_visible(! red_sprite.visible());
            }

            // info.update();
            bn::core::update();
        }
}

enum Direction {NONE, UP, DOWN, LEFT, RIGHT};

// topview movement
void topview_movement_test(){

    // 배경 배치
    // 배경-주 배경
    bn::regular_bg_ptr land_bg = bn::regular_bg_items::land.create_bg(0, 0);
    
    // 배경-보조 배경(알파 블렌딩 적용)
    bn::regular_bg_ptr clouds_bg = bn::regular_bg_items::clouds.create_bg(0, 0);
    bn::blending::set_transparency_alpha(0.5);
    clouds_bg.set_blending_enabled(true);

    // 플레이어 정보
    int playerX = 0;
    int playerY = 0;

    // 플레이어 스프라이트
    bn::sprite_ptr ninja_sprite = bn::sprite_items::ninja.create_sprite(0, 0);
    bn::sprite_animate_action<4> action = bn::create_sprite_animate_action_forever(ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 0, 1, 2, 3);

    bool doAnim = false;
    Direction direction = Direction::DOWN;

    bn::fixed_point playerPosition = bn::fixed_point(0, 0);

    // 카메라 배치
    bn::camera_ptr camera = bn::camera_ptr::create(0, 0);

    // 카메라에 배경 및 스프라이트 등록?
    ninja_sprite.set_camera(camera);
    land_bg.set_camera(camera);

    // 이동 및 렌더
    while(! bn::keypad::start_pressed())
    {
        // 4방향 이동
        // 애니메이션
        if(bn::keypad::left_held())
        {
            if (direction != Direction::LEFT)
                action = bn::create_sprite_animate_action_forever(ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 9, 10, 11, 8);
            
            direction = Direction::LEFT;
            doAnim = true;
            // BN_LOG("left");

            ninja_sprite.set_x(ninja_sprite.x() - 1);
        }
        else if(bn::keypad::right_held())
        {
            if (direction != Direction::RIGHT)
                action = bn::create_sprite_animate_action_forever(ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 13, 14, 15, 12);

            direction = Direction::RIGHT;
            doAnim = true;
            // BN_LOG("right");

            ninja_sprite.set_x(ninja_sprite.x() + 1);
        }
        else
        if(bn::keypad::up_held())
        {
            if (direction != Direction::UP)
            action = bn::create_sprite_animate_action_forever(ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 5, 6, 7, 4);

            direction = Direction::UP;
            doAnim = true;
            // BN_LOG("up");

            ninja_sprite.set_y(ninja_sprite.y() - 1);
        }
        else if(bn::keypad::down_held())
        {
            if (direction != Direction::DOWN)
            action = bn::create_sprite_animate_action_forever(ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 1, 2, 3, 0);

            direction = Direction::DOWN;
            doAnim = true;
            // BN_LOG("down");

            ninja_sprite.set_y(ninja_sprite.y() + 1);
        }
        else
        {
            doAnim = false;
        }

        playerPosition.set_x(ninja_sprite.x());
        playerPosition.set_y(ninja_sprite.y());

        // BN_LOG("doAnim: ", doAnim);
        if (doAnim)
        {
            // BN_LOG("move");
            action.update();
        }
        else
        {
            // BN_LOG("stop");
            switch (direction)
            {
            case Direction::LEFT:
                ninja_sprite.set_tiles(bn::sprite_items::ninja.tiles_item().create_tiles(8));
                break;
            case Direction::RIGHT:
                ninja_sprite.set_tiles(bn::sprite_items::ninja.tiles_item().create_tiles(12));
                break;
            case Direction::UP:
                ninja_sprite.set_tiles(bn::sprite_items::ninja.tiles_item().create_tiles(4));
                break;
            case Direction::DOWN:
                ninja_sprite.set_tiles(bn::sprite_items::ninja.tiles_item().create_tiles(0));
                break;
            
            default:
                break;
            }
        }

        // 카메라 위치 업데이트
        // BN_LOG("cameraposition.x: ", camera.x(), " cameraposition.y: ", camera.y());
        // BN_LOG("playerPosition.x: ", playerPosition.x(), " playerPosition.y: ", playerPosition.y());
        // BN_LOG("ninja_sprite.x: ", ninja_sprite.x(), " ninja_sprite.y: ", ninja_sprite.y());
        camera.set_position(playerPosition);

        // info.update();
        bn::core::update();
    }

    // 카메라 제거
    // bn::camera_ptr::_destroy();
    // camera._destroy();
}

void side_movement_test(){

    // TODO: 충돌 처리, 중력 처리

    // 배경-보조 배경(알파 블렌딩 적용)
    bn::regular_bg_ptr clouds_bg = bn::regular_bg_items::clouds.create_bg(0, 0);
    bn::blending::set_transparency_alpha(0.5);
    clouds_bg.set_blending_enabled(true);

    // 플레이어 정보
    int playerX = 0;
    int playerY = 0;

    // 플레이어 스프라이트
    bn::sprite_ptr ninja_sprite = bn::sprite_items::ninja.create_sprite(0, 0);
    bn::sprite_animate_action<4> action = bn::create_sprite_animate_action_forever(ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 0, 1, 2, 3);

    bool doAnim = false;
    Direction direction = Direction::DOWN;

    bn::fixed_point playerPosition = bn::fixed_point(0, 0);

    // 카메라 배치
    bn::camera_ptr camera = bn::camera_ptr::create(0, 0);

    // 카메라에 배경 및 스프라이트 등록?
    ninja_sprite.set_camera(camera);
    // land_bg.set_camera(camera);

    // 이동 및 렌더
    while(! bn::keypad::start_pressed())
    {
        // 4방향 이동
        // 애니메이션
        if(bn::keypad::left_held())
        {
            if (direction != Direction::LEFT)
                action = bn::create_sprite_animate_action_forever(ninja_sprite, 8, bn::sprite_items::ninja.tiles_item(), 9, 10, 11, 8);
            
            direction = Direction::LEFT;
            doAnim = true;
            // BN_LOG("left");

            ninja_sprite.set_x(ninja_sprite.x() - 1);
        }
        else if(bn::keypad::right_held())
        {
            if (direction != Direction::RIGHT)
                action = bn::create_sprite_animate_action_forever(ninja_sprite, 8, bn::sprite_items::ninja.tiles_item(), 13, 14, 15, 12);

            direction = Direction::RIGHT;
            doAnim = true;
            // BN_LOG("right");

            ninja_sprite.set_x(ninja_sprite.x() + 1);
        }
        // else
        // if(bn::keypad::up_held())
        // {
        //     if (direction != Direction::UP)
        //     action = bn::create_sprite_animate_action_forever(ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 5, 6, 7, 4);

        //     direction = Direction::UP;
        //     doAnim = true;
        //     // BN_LOG("up");

        //     ninja_sprite.set_y(ninja_sprite.y() - 1);
        // }
        // else if(bn::keypad::down_held())
        // {
        //     if (direction != Direction::DOWN)
        //     action = bn::create_sprite_animate_action_forever(ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 1, 2, 3, 0);

        //     direction = Direction::DOWN;
        //     doAnim = true;
        //     // BN_LOG("down");

        //     ninja_sprite.set_y(ninja_sprite.y() + 1);
        // }
        else
        {
            doAnim = false;
        }

        playerPosition.set_x(ninja_sprite.x());
        playerPosition.set_y(ninja_sprite.y());

        // BN_LOG("doAnim: ", doAnim);
        if (doAnim)
        {
            // BN_LOG("move");
            action.update();
        }
        else
        {
            // BN_LOG("stop");
            switch (direction)
            {
            case Direction::LEFT:
                ninja_sprite.set_tiles(bn::sprite_items::ninja.tiles_item().create_tiles(8));
                break;
            case Direction::RIGHT:
                ninja_sprite.set_tiles(bn::sprite_items::ninja.tiles_item().create_tiles(12));
                break;
            case Direction::UP:
                ninja_sprite.set_tiles(bn::sprite_items::ninja.tiles_item().create_tiles(4));
                break;
            case Direction::DOWN:
                ninja_sprite.set_tiles(bn::sprite_items::ninja.tiles_item().create_tiles(0));
                break;
            
            default:
                break;
            }
        }

        // 카메라 위치 업데이트
        // BN_LOG("cameraposition.x: ", camera.x(), " cameraposition.y: ", camera.y());
        // BN_LOG("playerPosition.x: ", playerPosition.x(), " playerPosition.y: ", playerPosition.y());
        // BN_LOG("ninja_sprite.x: ", ninja_sprite.x(), " ninja_sprite.y: ", ninja_sprite.y());
        camera.set_position(playerPosition);

        // info.update();
        bn::core::update();
    }

}

void particle_test(){
    
}

void draw_sprite_arrow(){
    bn::sprite_ptr arrow_sprite = bn::sprite_items::arrows2.create_sprite(0, 0);

    bn::sprite_animate_action<2> action = bn::create_sprite_animate_action_forever(arrow_sprite, 16, bn::sprite_items::arrows2.tiles_item(), 0, 1);

    while(! bn::keypad::start_pressed())
    {
        if(bn::keypad::left_pressed())
        {
            arrow_sprite.set_tiles(bn::sprite_items::arrows2.tiles_item().create_tiles(4));
        }
        else if(bn::keypad::right_pressed())
        {
            arrow_sprite.set_tiles(bn::sprite_items::arrows2.tiles_item().create_tiles(6));
        }

        if(bn::keypad::up_pressed())
        {
            arrow_sprite.set_tiles(bn::sprite_items::arrows2.tiles_item().create_tiles(0));
        }
        else if(bn::keypad::down_pressed())
        {
            arrow_sprite.set_tiles(bn::sprite_items::arrows2.tiles_item().create_tiles(2));
        }
        
        // info.update();
        bn::core::update();
    }
}

int main()
{
    bn::core::init();

    while(true)
    {
        // draw_sprite_test();
        // bn::core::update();

        topview_movement_test();
        
        // draw_sprite_arrow();

        
        bn::core::update();
    }
}