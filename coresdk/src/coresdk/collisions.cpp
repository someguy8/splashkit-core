//
//  collisions.cpp
//  splashkit
//
//  Created by Andrew Cain on 25/08/2016.
//  Copyright © 2016 Andrew Cain. All rights reserved.
//

#include "collisions.h"
#include "physics.h"
#include "sprites.h"
#include "utility_functions.h"

#include <cmath>
#include <functional>

#include "graphics.h"
#include "utils.h"

using std::function;

namespace splashkit_lib
{
    //#define DEBUG_STEP

    // Step over pixels in the two areas based on the supplied matrix
    //
    // See http://www.austincc.edu/cchrist1/GAME1343/TransformedCollision/TransformedCollision.htm
    bool _step_through_pixels(
        double w1, double h1,
        const matrix_2d &matrix1,
        double w2, double h2,
        const matrix_2d &matrix2,
        function<bool(int, int, int, int)> end_fn)
    {
        bool a_is_1;
        double h_a, w_a;
        double h_b, w_b;
        matrix_2d transform_a_to_b;

        // Determine the smaller area to step through.
        if (w1 * h1 <= w2 * h2) // use bitmap 1 as the one to scan
        {
            a_is_1 = true;
            h_a = h1;
            w_a = w1;

            h_b = h2;
            w_b = w2;

            // Calculate a matrix which transforms from 1's local space into
            // world space and then into 2's local space
            transform_a_to_b = matrix_multiply(matrix1, matrix_inverse(matrix2));
        }
        else // use bitmap 2
        {
            a_is_1 = false;
            h_a = h2;
            w_a = w2;

            h_b = h1;
            w_b = w1;

            // Calculate a matrix which transforms from 1's local space into
            // world space and then into 2's local space
            transform_a_to_b = matrix_multiply(matrix2, matrix_inverse(matrix1));
        }

        vector_2d step_x, step_y, y_pos_in_b, pos_in_b;

        // Calculate the top left corner of A in B's local space
        // This variable will be reused to keep track of the start of each row
        y_pos_in_b = matrix_multiply(transform_a_to_b, vector_to(0, 0));

        // When a point moves in A's local space, it moves in B's local space with a
        // fixed direction and distance proportional to the movement in A.
        // This algorithm steps through A one pixel at a time along A's X and Y axes
        // Calculate the analogous steps in B:
        step_x = vector_subtract(matrix_multiply(transform_a_to_b, vector_to(1, 0)), y_pos_in_b);
        step_y = vector_subtract(matrix_multiply(transform_a_to_b, vector_to(0, 1)), y_pos_in_b); // y inverted for drawing

        int x_b, y_b;

        // have to check all pixels of one bitmap
        // For each row of pixels in A (the smaller)
        for (int y_a = 0; y_a < h_a; y_a++)
        {
            pos_in_b = y_pos_in_b;

            // For each pixel in this row
            for (int x_a = 0; x_a < w_a; x_a++)
            {
                // Calculate this pixel's location in B
                // positionInB = transform_a_to_b * vector_to(xA, yA);

                // Round to the nearest pixel
                x_b = trunc(pos_in_b.x);
                y_b = trunc(pos_in_b.y);

                // If the pixel lies within the bounds of B
                if ((0 <= x_b) and (x_b < w_b) and (0 <= y_b) and (y_b < h_b))
                {
                    if ((a_is_1 and end_fn(x_a, y_a, x_b, y_b)) or ((not a_is_1) and end_fn(x_b, y_b, x_a, y_a)))
                    {
                        return true;
                    }
                }

                // Move to the next pixel in the row
                pos_in_b = vector_add(pos_in_b, step_x);
            }

            // Move to the next row
            y_pos_in_b = vector_add(y_pos_in_b, step_y);
        }

        // No intersection found
        return false;
    }

    bool _collision_within_bitmap_images_with_translation(bitmap bmp1, int c1, const matrix_2d &matrix1, bitmap bmp2, int c2, const matrix_2d &matrix2)
    {
        return _step_through_pixels(bitmap_cell_width(bmp1), bitmap_cell_height(bmp1), matrix1,
                                    bitmap_cell_width(bmp2), bitmap_cell_height(bmp2), matrix2,
                                    [&](int ax, int ay, int bx, int by)
                                    {
#if DEBUG_STEP
                                        point_2d apt, bpt;
                                        apt = matrix_multiply(matrix1, point_at(ax, ay));
                                        if (pixel_drawn_at_point(bmp1, c1, ax, ay))
                                            draw_circle(COLOR_RED, apt.x, apt.y, 3);
                                        bpt = matrix_multiply(matrix2, point_at(bx, by));
                                        if (pixel_drawn_at_point(bmp2, c2, bx, by))
                                            draw_circle(COLOR_PINK, bpt.x, bpt.y, 2);

                                        if (pixel_drawn_at_point(bmp1, c1, ax, ay) and pixel_drawn_at_point(bmp2, c2, bx, by))
                                        {
                                            fill_circle(COLOR_GREEN, apt.x, apt.y, 1);
                                            fill_circle(COLOR_YELLOW, bpt.x, bpt.y, 3);
                                        }
#endif
                                        return pixel_drawn_at_point(bmp1, c1, ax, ay) and pixel_drawn_at_point(bmp2, c2, bx, by);
                                    });
    }

    bool bitmap_point_collision(bitmap bmp, int cell, const matrix_2d &translation, const point_2d &pt)
    {
        if (INVALID_PTR(bmp, BITMAP_PTR))
        {
            return false;
        }

        if (not point_in_quad(pt, quad_from(bitmap_cell_rectangle(bmp), translation)))
        {
            return false;
        }

        return _step_through_pixels(1, 1, translation_matrix(pt.x, pt.y), bmp->cell_w, bmp->cell_h, translation, [&](int ax, int ay, int bx, int by)
                                    {
#if DEBUG_STEP
                                        point_2d bpt;
                                        bpt = matrix_multiply(translation, point_at(bx, by));
                                        if (pixel_drawn_at_point(bmp, cell, bx, by))
                                            fill_rectangle(COLOR_PINK, bpt.x, bpt.y, translation.elements[0][0], translation.elements[1][1]);
#endif
                                        return pixel_drawn_at_point(bmp, cell, bx, by);
                                    });
    }

    bool bitmap_point_collision(bitmap bmp, const matrix_2d &translation, const point_2d &pt)
    {
        return bitmap_point_collision(bmp, 0, translation, pt);
    }

    bool bitmap_point_collision(bitmap bmp, const point_2d &bmp_pt, const point_2d &pt)
    {
        return bitmap_point_collision(bmp, translation_matrix(bmp_pt), pt);
    }

    bool bitmap_point_collision(bitmap bmp, double bmp_x, double bmp_y, double x, double y)
    {
        return bitmap_point_collision(bmp, 0, translation_matrix(bmp_x, bmp_y), point_at(x, y));
    }

    bool bitmap_point_collision(bitmap bmp, int cell, const point_2d &bmp_pt, const point_2d &pt)
    {
        return bitmap_point_collision(bmp, cell, translation_matrix(bmp_pt), pt);
    }

    bool bitmap_point_collision(bitmap bmp, int cell, double bmp_x, double bmp_y, double x, double y)
    {
        return bitmap_point_collision(bmp, cell, translation_matrix(bmp_x, bmp_y), point_at(x, y));
    }

    bool bitmap_rectangle_collision(bitmap bmp, int cell, const matrix_2d &translation, const rectangle &rect)
    {
        if (INVALID_PTR(bmp, BITMAP_PTR))
        {
            return false;
        }

        quad q1, q2;

        q1 = quad_from(bitmap_cell_rectangle(bmp), translation);
        q2 = quad_from(rect);

        if (not quads_intersect(q1, q2))
            return false;

        return _step_through_pixels(rect.width, rect.height, translation_matrix(rect.x, rect.y), bmp->cell_w, bmp->cell_h, translation, [&](int ax, int ay, int bx, int by)
                                    { return pixel_drawn_at_point(bmp, cell, bx, by); });
    }

    bool bitmap_rectangle_collision(bitmap bmp, int cell, const point_2d &pt, const rectangle &rect)
    {
        return bitmap_rectangle_collision(bmp, cell, translation_matrix(pt), rect);
    }

    bool bitmap_rectangle_collision(bitmap bmp, const point_2d &pt, const rectangle &rect)
    {
        return bitmap_rectangle_collision(bmp, 0, translation_matrix(pt), rect);
    }

    bool bitmap_rectangle_collision(bitmap bmp, double x, double y, const rectangle &rect)
    {
        return bitmap_rectangle_collision(bmp, 0, translation_matrix(x, y), rect);
    }

    bool bitmap_rectangle_collision(bitmap bmp, int cell, double x, double y, const rectangle &rect)
    {
        return bitmap_rectangle_collision(bmp, cell, translation_matrix(x, y), rect);
    }

    bool bitmap_circle_collision(bitmap bmp, int cell, const matrix_2d &translation, const circle &circ)
    {
        if (INVALID_PTR(bmp, BITMAP_PTR))
        {
            return false;
        }

        quad q1, q2;

        q1 = quad_from(bitmap_cell_rectangle(bmp), translation);
        rectangle rect = rectangle_around(circ);
        q2 = quad_from(rect);

        if (not quads_intersect(q1, q2))
            return false;

        return _step_through_pixels(rect.width, rect.height, translation_matrix(rect.x, rect.y), bmp->cell_w, bmp->cell_h, translation, [&](int ax, int ay, int bx, int by)
                                    { return pixel_drawn_at_point(bmp, cell, bx, by) && point_in_circle(point_at(rect.x + ax, rect.y + ay), circ); });
    }

    bool bitmap_circle_collision(bitmap bmp, int cell, const point_2d &pt, const circle &circ)
    {
        return bitmap_circle_collision(bmp, cell, translation_matrix(pt), circ);
    }

    bool bitmap_circle_collision(bitmap bmp, const point_2d &pt, const circle &circ)
    {
        return bitmap_circle_collision(bmp, 0, translation_matrix(pt), circ);
    }

    bool bitmap_circle_collision(bitmap bmp, double x, double y, const circle &circ)
    {
        return bitmap_circle_collision(bmp, 0, translation_matrix(x, y), circ);
    }

    bool bitmap_circle_collision(bitmap bmp, int cell, double x, double y, const circle &circ)
    {
        return bitmap_circle_collision(bmp, cell, translation_matrix(x, y), circ);
    }

    bool sprite_bitmap_collision(sprite s, bitmap bmp, int cell, double x, double y)
    {
        if (!rectangles_intersect(sprite_collision_rectangle(s), bitmap_cell_rectangle(bmp, point_at(x, y))))
        {
            return false;
        }

        if (sprite_collision_kind(s) == AABB_COLLISIONS)
        {
            return bitmap_rectangle_collision(bmp, cell, point_at(x, y), sprite_collision_rectangle(s));
        }

        return _collision_within_bitmap_images_with_translation(
            sprite_collision_bitmap(s), sprite_current_cell(s),
            sprite_location_matrix(s),
            bmp,
            cell,
            translation_matrix(x, y));
    }

    bool sprite_bitmap_collision(sprite s, bitmap bmp, int cell, const point_2d &pt)
    {
        return sprite_bitmap_collision(s, bmp, cell, pt.x, pt.y);
    }

    bool sprite_bitmap_collision(sprite s, bitmap bmp, double x, double y)
    {
        return sprite_bitmap_collision(s, bmp, 0, x, y);
    }

    bool sprite_point_collision(sprite s, const point_2d &pt)
    {
        if (!point_in_circle(pt, sprite_collision_circle(s)))
        {
            return false;
        }
        else if (bitmap_cell_count(sprite_collision_bitmap(s)) > 1)
        {
            return bitmap_point_collision(sprite_collision_bitmap(s), sprite_current_cell(s), sprite_location_matrix(s), pt);
        }
        else
        {
            return bitmap_point_collision(sprite_collision_bitmap(s), sprite_location_matrix(s), pt);
        }
    }

    bool sprite_rectangle_collision(sprite s, const rectangle &rect)
    {
        if (!rectangles_intersect(sprite_collision_rectangle(s), rect))
        {
            return false;
        }

        return bitmap_rectangle_collision(sprite_collision_bitmap(s), sprite_current_cell(s), sprite_location_matrix(s), rect);
    }

    bool sprite_collision(sprite s1, sprite s2)
    {
        if (!rectangles_intersect(sprite_collision_rectangle(s1), sprite_collision_rectangle(s2)))
        {
            return false;
        }

        if (sprite_collision_kind(s1) == AABB_COLLISIONS)
        {
            return sprite_rectangle_collision(s2, sprite_collision_rectangle(s1));
        }

        if (sprite_collision_kind(s2) == AABB_COLLISIONS)
        {
            return sprite_rectangle_collision(s1, sprite_collision_rectangle(s2));
        }

        return _collision_within_bitmap_images_with_translation(
            sprite_collision_bitmap(s1), sprite_current_cell(s1), sprite_location_matrix(s1),
            sprite_collision_bitmap(s2), sprite_current_cell(s2), sprite_location_matrix(s2));
    }

    bool bitmap_collision(bitmap bmp1, int cell1, const matrix_2d &matrix1, bitmap bmp2, int cell2, const matrix_2d &matrix2)
    {
        quad q1 = quad_from(bitmap_cell_rectangle(bmp1), matrix1);
        quad q2 = quad_from(bitmap_cell_rectangle(bmp2), matrix2);

        if (not quads_intersect(q1, q2))
        {
            return false;
        }

        return _collision_within_bitmap_images_with_translation(bmp1, cell1, matrix1,
                                                                bmp2, cell2, matrix2);
    }

    bool bitmap_collision(bitmap bmp1, int cell1, const point_2d &pt1, bitmap bmp2, int cell2, const point_2d &pt2)
    {
        return bitmap_collision(bmp1, cell1, translation_matrix(pt1), bmp2, cell2, translation_matrix(pt2));
    }

    bool bitmap_collision(bitmap bmp1, int cell1, double x1, double y1, bitmap bmp2, int cell2, double x2, double y2)
    {
        return bitmap_collision(bmp1, cell1, translation_matrix(x1, y1), bmp2, cell2, translation_matrix(x2, y2));
    }

    bool bitmap_collision(bitmap bmp1, const point_2d &pt1, bitmap bmp2, const point_2d &pt2)
    {
        return bitmap_collision(bmp1, 0, pt1.x, pt1.y, bmp2, 0, pt2.x, pt2.y);
    }

    bool bitmap_collision(bitmap bmp1, double x1, double y1, bitmap bmp2, double x2, double y2)
    {
        return bitmap_collision(bmp1, 0, translation_matrix(x1, y1), bmp2, 0, translation_matrix(x2, y2));
    }

    // Add sprite gravite functions to sprite. User can enter Sprite mass and falling speed
    void sprite_gravity(sprite s, vector_2d falling, double mass)
    {
        sprite_set_velocity(s, falling);
        sprite_set_mass(s, mass);
    }

    void sprite_push(sprite s1, sprite s2, double mass1, double mass2)
    {
        if (mass1 > mass2)
        {
            sprite_set_velocity(s2, vector_from_angle(angle_between(sprite_velocity(s2), sprite_velocity(s1)), mass1 - mass2));
        }
        else if (mass1 = mass2 || mass1 < mass2)
        {
            sprite_set_velocity(s2, vector_from_angle(angle_between(sprite_velocity(s2), sprite_velocity(s1)), 0));
            sprite_set_velocity(s1, vector_from_angle(-angle_between(sprite_velocity(s2), sprite_velocity(s1)), 0.1));
        }
    }

    // Insert physics formula into sprite collision, make sprite collsion more realistic
    void Physical_sprites_collision(sprite sprite_1, sprite sprite_2, double mass1, double mass2, double speed1, double speed2)
    {
        double sprite1_kinetic_energy = 0.5 * mass1 * speed1 * speed1;
        double sprite2_kinetic_energy = 0.5 * mass2 * speed2 * speed2;
        double sprite_kinetic_energy = sprite1_kinetic_energy - sprite2_kinetic_energy;
        if (sprite_collision(sprite_1, sprite_2))
        {
            if (sprite_rotation(sprite_1) >= 0 && sprite_rotation(sprite_1) <= 45)
            {
                move_sprite_to(sprite_2, sprite_x(sprite_2) + sprite_kinetic_energy, sprite_y(sprite_2));
                move_sprite_to(sprite_1, sprite_x(sprite_1) - sprite_kinetic_energy, sprite_y(sprite_1));
            }
            if (sprite_rotation(sprite_1) > 45 && sprite_rotation(sprite_1) <= 135)
            {
                move_sprite_to(sprite_2, sprite_x(sprite_2), sprite_y(sprite_2) + sprite_kinetic_energy);
                move_sprite_to(sprite_1, sprite_x(sprite_1), sprite_y(sprite_1) - sprite_kinetic_energy);
            }
            if (sprite_rotation(sprite_1) > 135 && sprite_rotation(sprite_1) <= 225)
            {
                move_sprite_to(sprite_2, sprite_x(sprite_2) - sprite_kinetic_energy, sprite_y(sprite_2));
                move_sprite_to(sprite_1, sprite_x(sprite_1) + sprite_kinetic_energy, sprite_y(sprite_1));
            }
            if (sprite_rotation(sprite_1) > 225 && sprite_rotation(sprite_1) < 315)
            {
                move_sprite_to(sprite_2, sprite_x(sprite_2), sprite_y(sprite_2) - sprite_kinetic_energy);
                move_sprite_to(sprite_1, sprite_x(sprite_1), sprite_y(sprite_1) + sprite_kinetic_energy);
            }
            if (sprite_rotation(sprite_1) >= 315 && sprite_rotation(sprite_1) <= 360)
            {
                move_sprite_to(sprite_2, sprite_x(sprite_2) + sprite_kinetic_energy, sprite_y(sprite_2));
                move_sprite_to(sprite_1, sprite_x(sprite_1) - sprite_kinetic_energy, sprite_y(sprite_1));
            }
        }
    }

    // Function used for calculating kinetic energy
    float KINETIC_ENERGY(sprite sprite_1)
    {
        float KINETIC;
        // KINETIC_ENERGY = 1/2 m*v*v
        KINETIC = 0.5 * sprite_mass(sprite_1) * sprite_speed(sprite_1) * sprite_speed(sprite_1);
        return KINETIC;
    }

    float velocity_after_collied(sprite sprite_1, sprite sprite_2)
    {
        float velocity;
        /* According to the law of energy conservation, the Obstacle will gain all kinetic energy from
        * player, and ENERGY = 1/2 m*v*v, we have already know the mass of obstacle, so
        * v = root of (2*ENERGY/m)
        */
        velocity = sqrt(2 * KINETIC_ENERGY(sprite_1) / sprite_mass(sprite_2));
        return velocity;
    }

    // Partial collision with angles function
    void collision_angle(sprite sp1, sprite sp2)
    {
        float velocity;
        velocity = velocity_after_collied(sp1, sp2);

        if (sprite_collision(sp1, sp2))
        {
            // Assume when sprite collied with obstacle, sprite will transfer its all kinetic energy to obstacle, so sprite will stop moving.
            sprite_set_dx(sp1, 0);

            if (sprite_rotation(sp1) >= 0 && sprite_rotation(sp1) <= 45)
            {
                move_sprite_to(sp2, sprite_x(sp2) + velocity, sprite_y(sp2));
                sprite_set_dx(sp2, velocity);
            }
            if (sprite_rotation(sp1) > 45 && sprite_rotation(sp1) <= 135)
            {
                move_sprite_to(sp2, sprite_x(sp2), sprite_y(sp2) + velocity);
                sprite_set_dy(sp2, velocity);
            }
            if (sprite_rotation(sp1) > 135 && sprite_rotation(sp1) <= 225)
            {
                move_sprite_to(sp2, sprite_x(sp2) - velocity, sprite_y(sp2));
                sprite_set_dx(sp2, -velocity);
            }
            if (sprite_rotation(sp1) > 225 && sprite_rotation(sp1) < 315)
            {
                move_sprite_to(sp2, sprite_x(sp2), sprite_y(sp2) - velocity);
                sprite_set_dy(sp2, -velocity);
            }
            if (sprite_rotation(sp1) >= 315 && sprite_rotation(sp1) <= 360)
            {
                move_sprite_to(sp2, sprite_x(sp2) + velocity, sprite_y(sp2));
                sprite_set_dx(sp2, velocity);
            }
        }
        if (GetKeyState('S'))
        {
            sprite_set_dx(sp2, 0);
            sprite_set_dy(sp2, 0);
        }
    }

    // Shape kinetic energy and velocity
    float KINETIC_ENERGY(float c1_mass, float C1_X_speed, float C1_Y_speed)
    {
        float speed;
        float KINETIC;
        speed = sqrt(C1_X_speed * C1_X_speed + C1_Y_speed * C1_Y_speed);
        // KINETIC_ENERGY = 1/2 m*v*v
        KINETIC = 0.5 * c1_mass * speed * speed;
        return KINETIC;
    }

    float velocity_after_collied(float c1_mass, float c2_mass, float C1_X_speed, float C1_Y_speed)
    {
        float velocity;
        //According to the law of energy conservation, the Obstacle will gain all kinetic energy from
        //player, and ENERGY = 1/2 m*v*v, we have already know the mass of obstacle, so
        //v = root of (2*ENERGY/m)
        velocity = sqrt(2 * KINETIC_ENERGY(c1_mass, C1_X_speed, C1_Y_speed) / c2_mass);
        return velocity;
    }

    // Circle collision perform
    void collision(circle c1, circle c2, float c1_mass, float c2_mass, float C1_X_speed, float C1_Y_speed)
    {
        float Obstacle_velocity;
        float circle_X_speed;
        float circle_Y_speed;
        float obstacle_X_speed;
        float obstacle_Y_speed;

        Obstacle_velocity = velocity_after_collied(c1_mass, c2_mass, C1_X_speed, C1_Y_speed);

        

        if (circles_intersect(c1, c2))
        {
            circle_X_speed = 0;
            circle_Y_speed = 0;
            if (Obstacle_velocity < 0.1)
            {
                Obstacle_velocity = 0;
            }

            if (circle_x(c1) < circle_x(c2))
            {
                obstacle_X_speed += Obstacle_velocity;
            }
            if (circle_x(c1) > circle_x(c2))
            {
                obstacle_X_speed -= Obstacle_velocity;
            }
            if (circle_y(c1) > circle_y(c2))
            {
                obstacle_Y_speed -= Obstacle_velocity;
            }
            if (circle_y(c1) < circle_y(c2))
            {
                obstacle_Y_speed += Obstacle_velocity;
            }
        }
    }

    // Rectangle collsion
    void rect_collision(rectangle c1, rectangle c2, float c1_mass, float c2_mass, float C1_X_speed, float C1_Y_speed)
    {
        float Obstacle_velocity;
        float circle_X_speed;
        float circle_Y_speed;
        float obstacle_X_speed;
        float obstacle_Y_speed;

        Obstacle_velocity = velocity_after_collied(c1_mass, c2_mass, C1_X_speed, C1_Y_speed);

        if (rectangles_intersect(c1, c2))
        {
            circle_X_speed = 0;
            circle_Y_speed = 0;
            if (Obstacle_velocity < 0.1)
            {
                Obstacle_velocity = 0;
            }

            if (rectangle_left(c1) < rectangle_left(c2))
            {
                obstacle_X_speed += Obstacle_velocity;
            }
            else if (rectangle_left(c1) > rectangle_left(c2))
            {
                obstacle_X_speed -= Obstacle_velocity;
            }
            else if (rectangle_left(c1) > rectangle_left(c2))
            {
                obstacle_Y_speed -= Obstacle_velocity;
            }
            else if (rectangle_left(c1) < rectangle_left(c2))
            {
                obstacle_Y_speed += Obstacle_velocity;
            }
        }
    }

    // Calculate free falling velocity
    float Free_Falling(float ratio, sprite c1, float Gravity)
    {
        float acceleration;
        float falling_velocity;
        // Gravity
        acceleration = Gravity * ratio;
        falling_velocity = acceleration + sprite_dy(c1);

        return falling_velocity;
    }

    // Calculate density
    float density(sprite c1, float volume)
    {
        float density;
        density = sprite_mass(c1) / volume;

        return density;
    }

    // Calculate sinking velocity.
    float sinking_velocity(sprite c1, float buoyancy, float volume, float Gravity)
    {
        float velocity;
        float F = sprite_mass(c1) * sprite_speed(c1);
        float density = sprite_mass(c1) / volume;
        float F_after_water = sqrt((F - buoyancy * density) * (F - buoyancy * density));

        velocity = F_after_water / sprite_mass(c1);

        return velocity;
    }

    // Simulating sinking effects.
    void sinking(sprite c1, float Height_of_water, float buoyancy, float Gravity, float volume, float density_of_water)
    {
        if (sprite_y(c1) > Height_of_water && density(c1, volume) > density_of_water)
        {
            sprite_set_dy(c1, sinking_velocity(c1, buoyancy, volume, Gravity));
        }
        if (sprite_y(c1) > Height_of_water && density(c1, volume) < density_of_water)
        {
            sprite_set_dy(c1, 0);
        }
    }

    // Calculate the momentum of a moving sprite.
    float sprite_momentum_x(sprite spr)
    {
        return sprite_mass(spr) * sprite_velocity(spr).x;
    }
    float sprite_momentum_y(sprite spr)
    {
        return sprite_mass(spr) * sprite_velocity(spr).y;
    }

    // Formula to calculate and change the final velocity of both objects experiencing an inelastic collision.
    void sprite_inelastic_collision(sprite spr_a, sprite spr_b)
    {
        float mass_a, mass_b, v_a, v_b, v_final;

        // The mass of two sprites
        mass_a = sprite_mass(spr_a);
        mass_b = sprite_mass(spr_b);

        // The velocities of two sprites
        v_a = sprite_velocity(spr_a).x;
        v_b = sprite_velocity(spr_b).x;

        // Formula
        v_final = ((mass_a * v_a) + (mass_b * v_b)) / (mass_a + mass_b);

        // Set the new velocities of the sprites after the calculations
        if (sprite_collision(spr_a, spr_b))
        {
            sprite_set_dx(spr_a, v_final);
            sprite_set_dx(spr_b, v_final);
        }
    }

    // Formula to calculate and change the final velocity of both objects experiencing an elastic collision.
    void sprite_elastic_collision(sprite spr_a, sprite spr_b)
    {
        float mass_a, mass_b, v_a, v_b, v_a_final, v_b_final;

        // The mass of two sprites
        mass_a = sprite_mass(spr_a);
        mass_b = sprite_mass(spr_b);

        // The velocities of two sprites
        v_a = sprite_velocity(spr_a).x;
        v_b = sprite_velocity(spr_b).x;

        // Formula
        v_a_final = (((mass_a - mass_b) / (mass_a + mass_b)) * v_a + ((2 * mass_b) / (mass_a + mass_b)) * v_b);
        v_b_final = (((2 * mass_a) / (mass_a + mass_b)) * v_a + ((mass_b - mass_a) / (mass_a + mass_b)) * v_b);

        // Set the new velocities of the sprite after the calculations
        if (sprite_collision(spr_a, spr_b))
        {
            sprite_set_dx(spr_a, v_a_final);
            sprite_set_dx(spr_b, v_b_final);
        }
    }

    // This function will confine the sprite within the screen.
    void sprite_confine_screen(sprite spr, string screen_name)
    {
        // These series of if statements prevent the sprite from moving away from the screen.
        if ((sprite_x(spr) + sprite_width(spr)) > window_width(screen_name))
        {
            sprite_set_x(spr, window_width(screen_name) - sprite_width(spr));
        }
        if (sprite_x(spr) < 0)
        {
            sprite_set_x(spr, 0);
        }
        if (sprite_y(spr) < 0)
        {
            sprite_set_y(spr, 0);
        }
        if ((sprite_y(spr) + sprite_height(spr)) > window_height(screen_name))
        {
            sprite_set_y(spr, window_height(screen_name) - sprite_height(spr));
        }
    }

    // This function will return the sprite's impulse from the x-axis.
    double sprite_impulse_x(sprite spr)
    {
        // i = f*t = m*v
        double impulse = sprite_mass(spr) * sprite_velocity(spr).x;
        return impulse;
    }

    // This function will return the sprite's impulse from the y-axis.
    double sprite_impulse_y(sprite spr)
    {
        // i = f*t = m*v
        double impulse = sprite_mass(spr) * sprite_velocity(spr).y;
        return impulse;
    }

    // This function will set the sprite's impulse in the x-axis.
    void sprite_set_impulse_x(sprite spr, double impulse)
    {
        // v = i / m
        double final_velocity_x = impulse / sprite_mass(spr);
        sprite_set_dx(spr, final_velocity_x);
    }

    // This function will set the sprite's impulse in the y-axis.
    void sprite_set_impulse_y(sprite spr, double impulse)
    {
        // v = i / m
        double final_velocity_y = impulse / sprite_mass(spr);
        sprite_set_dy(spr, final_velocity_y);
    }

}
