//
//  test_collisions.cpp
//  splashkit
//
//  Created by Daniel Haysham on 08/05/2022.
//  Copyright © 2022 Andrew Cain. All rights reserved.
//

#include "window_manager.h"

#include <iostream>
using namespace std;
using namespace splashkit_lib;

#include "input.h"
#include "physics.h"
#include "sprites.h"
#include "collisions.h"
#include "collisions.cpp"

void run_collisions_test()
{
    cout << "Success!";
}

/*
void run_test_collisions2()
{
    window w1 = open_window("Collisions Test", 600, 600)

    float ratio;
    sprite c1;
    float Gravity;

    float volume;


    Free_Falling(ratio, c1, Gravity)
    draw_text(falling_velocity, 14, 18, 340);

    Density(c1, volume)
    draw_text(density, 14, 48, 340)

} 

void run_collisions_test()
{
sprite c1 = create_sprite("rocket_sprt.png");
    sprite_set_move_from_anchor_point(sprt, true);
    sprite_set_x(sprt, 300);
    sprite_set_y(sprt, 300);

float volume = 50;

    drawtext(density(c1, volume), 14, 48, 340);



}*/