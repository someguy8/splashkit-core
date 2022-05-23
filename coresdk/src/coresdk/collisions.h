/**
 * @header  collisions
 * @author  Andrew Cain
 * @brief   SplashKit Collisions library allow you to perform tests between
 *          bitmaps, sprites and shapes to determin if a collision has occured.
 *
 * @attribute group  physics
 * @attribute static collisions
 */

#ifndef collisions_h
#define collisions_h

#include "images.h"
#include "matrix_2d.h"
#include "sprites.h"
#include "types.h"
namespace splashkit_lib
{
    /**
     * Tests if a bitmap drawn using the passed in translation matrix would draw a pixel
     * at the passed in point. Use to check collisions between a point and a bitmap.
     *
     * @param bmp           The bitmap to test.
     * @param translation   The translation matrix that represents the position, scale,
     *                      and rotation of the bitmap when it is drawn.
     * @param pt            The point in the resulting drawing that is being tested.
     * @returns             True if drawing the bitmap, using the passed in translation,
     *                      will draw a pixel at the indicated point.
     *
     * @attribute suffix    with_translation
     *
     * @attribute class bitmap
     * @attribute method point_collision
     */
    bool bitmap_point_collision(bitmap bmp, const matrix_2d& translation, const point_2d& pt);

    /**
     * Tests if a bitmap drawn at the `bmp_pt` point would draw a pixel
     * at point `pt`. Use to check collisions between a point and a bitmap.
     *
     * @param bmp           The bitmap to test.
     * @param bmp_pt        The point where the bitmap is drawn.
     * @param pt            The point in the resulting drawing that is being tested.
     * @returns             True if drawing the bitmap, at `bmp_pt`,
     *                      will draw a pixel at the indicated point (`pt`).
     *
     * @attribute suffix    at_point
     *
     * @attribute class bitmap
     * @attribute method point_collision
     */
    bool bitmap_point_collision(bitmap bmp, const point_2d &bmp_pt, const point_2d& pt);

    /**
     * Tests if a bitmap drawn at x,y would draw a pixel at the passed in point. Use 
     * to check collisions between a point and a bitmap.
     *
     * @param bmp   The bitmap to test.
     * @param bmp_x The x location where the bitmap is drawn
     * @param bmp_y The y location where the bitmap is drawn
     * @param  x    The x location of the point to test
     * @param  y    The y location of the point to test
     * @returns     True if drawing the bitmap, at the indicated location,
     *              will draw a pixel at the indicated point.
     *
     * @attribute class bitmap
     * @attribute method point_collision
     */
    bool bitmap_point_collision(bitmap bmp, double bmp_x, double bmp_y, double x, double y);

    /**
     * Tests if a cell of a bitmap drawn at x,y would draw a pixel at the passed in point. Use
     * to check collisions between a point and a bitmap.
     *
     * @param bmp           The bitmap to test
     * @param cell          The cell of the bitmap to test
     * @param bmp_pt        The point where the bitmap is drawn.
     * @param pt            The point in the resulting drawing that is being tested.
     * @returns             True if drawing the bitmap cell, at the indicated location,
     *                      will draw a pixel at the indicated point.
     *
     * @attribute class bitmap
     * @attribute method point_collision
     *
     * @attribute suffix    for_cell_at_point
     */
    bool bitmap_point_collision(bitmap bmp, int cell, const point_2d &bmp_pt, const point_2d& pt);

    /**
     * Tests if a cell of a bitmap drawn at x,y would draw a pixel at the passed in point. Use
     * to check collisions between a point and a bitmap.
     *
     * @param bmp   The bitmap to test
     * @param cell  The cell of the bitmap to test
     * @param bmp_x The x location where the bitmap is drawn
     * @param bmp_y The y location where the bitmap is drawn
     * @param  x    The x location of the point to test
     * @param  y    The y location of the point to test
     * @returns     True if drawing the bitmap cell, at the indicated location,
     *              will draw a pixel at the indicated point.
     *
     * @attribute class bitmap
     * @attribute method point_collision
     *
     * @attribute suffix    for_cell
     */
    bool bitmap_point_collision(bitmap bmp, int cell, double bmp_x, double bmp_y, double x, double y);
    
    /**
     * Tests if a bitmap cell drawn using the passed in translation matrix would draw a pixel
     * at the passed in point.
     *
     * @param bmp           The bitmap to test.
     * @param cell          The cell of the bitmap to check.
     * @param translation   The translation matrix that represents the position, scale,
     *                      and rotation of the bitmap when it is drawn.
     * @param pt            The point in the resulting drawing that is being tested.
     * @returns             True if drawing the bitmap, using the passed in translation,
     *                      will draw a pixel at the indicated point.
     *
     * @attribute suffix    for_cell_with_translation
     *
     * @attribute class bitmap
     * @attribute method point_collision
     */
    bool bitmap_point_collision(bitmap bmp, int cell, const matrix_2d& translation, const point_2d& pt);

    /**
     * Tests if a bitmap cell drawn using a passed in translation, will
     * intersect with a rectangle. You can use this to detect collisions between
     * bitmaps and rectangles.
     *
     * @param  bmp         The bitmap to test
     * @param  cell        The cell of the bitmap to check
     * @param  translation The matrix used to transfrom the bitmap when drawing
     * @param  rect        The rectangle to test
     * @return             True if a drawn pixel in the cell of the bitmap will
     *                     intersect with `rect` when drawn.
     *
     * @attribute suffix    for_cell_with_translation
     *
     * @attribute class bitmap
     * @attribute method rectangle_collision
     */
    bool bitmap_rectangle_collision(bitmap bmp, int cell, const matrix_2d& translation, const rectangle& rect);

    /**
     * Tests if a bitmap cell drawn at `pt` would intersect with a rectangle.
     *
     * @param  bmp  The bitmap to test
     * @param  cell The cell of the bitmap to check
     * @param  pt   The location where the bitmap is drawn
     * @param  rect The rectangle to test
     * @return      True if a drawn pixel in the cell of the bitmap will
     *              intersect with `rect` when drawn.
     *
     * @attribute suffix    for_cell_at_point
     *
     * @attribute class bitmap
     * @attribute method rectangle_collision
     */
    bool bitmap_rectangle_collision(bitmap bmp, int cell, const point_2d& pt, const rectangle& rect);

    /**
     * Tests if a cell of the bitmap drawn at `x`, `y` would intersect with a rectangle.
     *
     * @param  bmp  The bitmap to test
     * @param  cell The cell of the bitmap to check
     * @param  x    The x location where the bitmap is drawn
     * @param  y    The y location where the bitmap is drawn
     * @param  rect The rectangle to test
     * @return      True if a drawn pixel in the bitmap will
     *              intersect with `rect` when drawn.
     *
     * @attribute suffix    for_cell
     *
     * @attribute class bitmap
     * @attribute method rectangle_collision
     */
    bool bitmap_rectangle_collision(bitmap bmp, int cell, double x, double y, const rectangle& rect);
    
    /**
     * Tests if a bitmap drawn at `x`, `y` would intersect with a rectangle.
     *
     * @param  bmp  The bitmap to test
     * @param  x    The x location where the bitmap is drawn
     * @param  y    The y location where the bitmap is drawn
     * @param  rect The rectangle to test
     * @return      True if a drawn pixel in the bitmap will
     *              intersect with `rect` when drawn.
     *
     * @attribute class bitmap
     * @attribute method rectangle_collision
     */
    bool bitmap_rectangle_collision(bitmap bmp, double x, double y, const rectangle& rect);
    
    /**
     * Tests if a bitmap drawn at `pt` would intersect with a rectangle.
     *
     * @param  bmp  The bitmap to test
     * @param  pt   The location where the bitmap is drawn
     * @param  rect The rectangle to test
     * @return      True if a drawn pixel in the cell of the bitmap will
     *              intersect with `rect` when drawn.
     *
     * @attribute suffix    at_point
     *
     * @attribute class bitmap
     * @attribute method rectangle_collision
     */
    bool bitmap_rectangle_collision(bitmap bmp, const point_2d& pt, const rectangle& rect);
    
    
    /**
     * Tests if a bitmap cell drawn using a passed in translation, will
     * intersect with a circle. You can use this to detect collisions between
     * bitmaps and circles.
     *
     * @param  bmp         The bitmap to test
     * @param  cell        The cell of the bitmap to check
     * @param  translation The matrix used to transfrom the bitmap when drawing
     * @param  circ        The circle to test
     * @return             True if a drawn pixel in the cell of the bitmap will
     *                     intersect with `circ` when drawn.
     *
     * @attribute suffix    for_cell_with_translation
     *
     * @attribute class bitmap
     * @attribute method circle_collision
     */
    bool bitmap_circle_collision(bitmap bmp, int cell, const matrix_2d& translation, const circle& circ);
    
    /**
     * Tests if a bitmap cell drawn at `pt` would intersect with a circle.
     *
     * @param  bmp  The bitmap to test
     * @param  cell The cell of the bitmap to check
     * @param  pt   The location where the bitmap is drawn
     * @param  circ The circle to test
     * @return      True if a drawn pixel in the cell of the bitmap will
     *              intersect with `circ` when drawn.
     *
     * @attribute suffix    for_cell_at_point
     *
     * @attribute class bitmap
     * @attribute method circle_collision
     */
    bool bitmap_circle_collision(bitmap bmp, int cell, const point_2d& pt, const circle& circ);
    
    /**
     * Tests if a bitmap cell drawn at `x`, `y` would intersect with a circle.
     *
     * @param  bmp  The bitmap to test
     * @param  cell The cell of the bitmap to check
     * @param  x    The x location where the bitmap is drawn
     * @param  y    The y location where the bitmap is drawn
     * @param  circ The circle to test
     * @return      True if a drawn pixel in the bitmap will
     *              intersect with `circ` when drawn.
     *
     * @attribute suffix    for_cell
     *
     * @attribute class bitmap
     * @attribute method circle_collision
     */
    bool bitmap_circle_collision(bitmap bmp, int cell, double x, double y, const circle& circ);
    
    /**
     * Tests if a bitmap drawn at `x`, `y` would intersect with a circle.
     *
     * @param  bmp  The bitmap to test
     * @param  x    The x location where the bitmap is drawn
     * @param  y    The y location where the bitmap is drawn
     * @param  circ The circle to test
     * @return      True if a drawn pixel in the bitmap will
     *              intersect with `circ` when drawn.
     *
     * @attribute class bitmap
     * @attribute method circle_collision
     */
    bool bitmap_circle_collision(bitmap bmp, double x, double y, const circle& circ);
    
    /**
     * Tests if a bitmap drawn at `pt` would intersect with a circle.
     *
     * @param  bmp  The bitmap to test
     * @param  pt   The location where the bitmap is drawn
     * @param  circ The circle to test
     * @return      True if a drawn pixel in the cell of the bitmap will
     *              intersect with `circ` when drawn.
     *
     * @attribute suffix    at_point
     *
     * @attribute class bitmap
     * @attribute method circle_collision
     */
    bool bitmap_circle_collision(bitmap bmp, const point_2d& pt, const circle& circ);
    
    /**
     * Tests if a sprite will collide with a bitmap drawn at the indicated
     * location.
     *
     * @param  s    The sprite to test
     * @param  bmp  The bitmap to test
     * @param  cell The cell of the bitmap to check
     * @param  x    The x location where the bitmap is drawn
     * @param  y    The y location where the bitmap is drawn
     * @return      True if the sprite collides with the bitmap cell when drawn
     *              at the indicated location.
     *
     * @attribute suffix    with_cell
     *
     * @attribute class sprite
     * @attribute method bitmap_collision
     */
    bool sprite_bitmap_collision(sprite s, bitmap bmp, int cell, double x, double y);

    /**
     * Tests if a sprite will collide with a bitmap drawn at the indicated
     * location.
     *
     * @param  s    The sprite to test
     * @param  bmp  The bitmap to test
     * @param  cell The cell of the bitmap to check
     * @param  pt   The point where the bitmap is drawn
     * @return      True if the sprite collides with the bitmap cell when drawn
     *              at the indicated location.
     *
     * @attribute suffix    with_cell_at_point
     *
     * @attribute class sprite
     * @attribute method bitmap_collision
     */
    bool sprite_bitmap_collision(sprite s, bitmap bmp, int cell, const point_2d &pt);

    /**
     * Tests if a sprite will collide with a bitmap drawn at the indicated
     * location.
     *
     * @param  s    The sprite to test
     * @param  bmp  The bitmap to test
     * @param  x    The x location where the bitmap is drawn
     * @param  y    The y location where the bitmap is drawn
     * @return      True if the sprite collides with the bitmap when drawn
     *              at the indicated location.
     *
     * @attribute class sprite
     * @attribute method bitmap_collision
     */
    bool sprite_bitmap_collision(sprite s, bitmap bmp, double x, double y);

    /**
     * Tests if a sprite is drawn at a given point.
     *
     * @param  s  The sprite to test
     * @param  pt The point to check
     * @return    True if the sprite it drawn at the indicated point
     *
     * @attribute class sprite
     * @attribute method point_collision
     */
    bool sprite_point_collision(sprite s, const point_2d& pt);

    /**
     * Tests if a sprite is drawn within an given area (rectangle).
     *
     * @param  s    The sprite to test
     * @param  rect The rectangle to check
     * @return      True if the sprite it drawn in the rectangle area
     *
     * @attribute class sprite
     * @attribute method rectangle_collision
     */
    bool sprite_rectangle_collision(sprite s, const rectangle& rect);

    /**
     * Tests if two given sprites `s1` and `s2` are collided
     * @param  s1 the first `sprite` to test
     * @param  s2 the second `sprite` to test
     *
     * @returns return `true` if both `s1` and `s2` are colliding, false otherwise.
     *
     * @attribute class   sprite
     * @attribute method  sprite_collision
     * @attribute self    s1
     */
    bool sprite_collision(sprite s1, sprite s2);

    /**
     * Check if two bitmaps collide, based on their cell and transform
     * matricies.
     *
     * @param  bmp1    The first bitmap
     * @param  cell1   The cell of the first bitmap.
     * @param  matrix1 The transformation for the first matrix
     * @param  bmp2    The second bitmap
     * @param  cell2   The cell of the second bitmap.
     * @param  matrix2 The transformation for the second matrix
     * @return         True if pixels in the two bitmaps would collide/overlap
     *
     * @attribute suffix    for_cells_with_translations
     *
     * @attribute class bitmap
     * @attribute method bitmap_collision
     */
    bool bitmap_collision(bitmap bmp1, int cell1, const matrix_2d &matrix1, bitmap bmp2, int cell2, const matrix_2d &matrix2);

    /**
     * Check if two bitmaps collide.
     *
     * @param  bmp1   The first bitmap
     * @param  cell1  The cell of the first bitmap.
     * @param  pt1    The point where bitmap 1 is drawn
     * @param  bmp2   The second bitmap
     * @param  cell2  The cell of the second bitmap.
     * @param  pt2    The point where bitmap 2 is drawn
     * @return        True if pixels in the two bitmaps would collide/overlap
     *
     * @attribute suffix    for_cells_at_points
     *
     * @attribute class bitmap
     * @attribute method bitmap_collision
     */
    bool bitmap_collision(bitmap bmp1, int cell1, const point_2d &pt1, bitmap bmp2, int cell2, const point_2d &pt2);

    /**
     * Check if two bitmaps collide.
     *
     * @param  bmp1   The first bitmap
     * @param  cell1  The cell of the first bitmap.
     * @param  x1     The x location where the first bitmap is drawn
     * @param  y1     The y location where the first bitmap is drawn
     * @param  bmp2   The second bitmap
     * @param  cell2  The cell of the second bitmap.
     * @param  x2     The x location where the second bitmap is drawn
     * @param  y2     The y location where the second bitmap is drawn
     * @return        True if pixels in the two bitmaps would collide/overlap
     *
     * @attribute suffix    for_cells
     *
     * @attribute class bitmap
     * @attribute method bitmap_collision
     */
    bool bitmap_collision(bitmap bmp1, int cell1, double x1, double y1, bitmap bmp2, int cell2, double x2, double y2);

    /**
     * Check if two bitmaps collide.
     *
     * @param  bmp1   The first bitmap
     * @param  pt1    The point where bitmap 1 is drawn
     * @param  bmp2   The second bitmap
     * @param  pt2    The point where bitmap 2 is drawn
     * @return        True if pixels in the two bitmaps would collide/overlap
     *
     * @attribute suffix    at_points
     *
     * @attribute class bitmap
     * @attribute method bitmap_collision
     */
    bool bitmap_collision(bitmap bmp1, const point_2d &pt1, bitmap bmp2, const point_2d &pt2);

    /**
     * Check if two bitmaps collide.
     *
     * @param  bmp1   The first bitmap
     * @param  x1     The x location where the first bitmap is drawn
     * @param  y1     The y location where the first bitmap is drawn
     * @param  bmp2   The second bitmap
     * @param  x2     The x location where the second bitmap is drawn
     * @param  y2     The y location where the second bitmap is drawn
     * @return        True if pixels in the two bitmaps would collide/overlap
     *
     * @attribute class bitmap
     * @attribute method bitmap_collision
     */
    bool bitmap_collision(bitmap bmp1, double x1, double y1, bitmap bmp2, double x2, double y2);

    /**
     * Add new variable mass to Sprite
     * 
     * @param s  The sprite used for collision
     * @param falling used for set velocity
     * @param mass The mass of sprite
     */
  
    void sprite_gravity(sprite s, vector_2d falling, double mass);

    /**
     * Push sprites when collision exist  
     * 
     * @param s1 The first sprite
     * @param s2 The second sprite
     * @param mass1 The mass of first sprite
     * @param mass2 The mass of second sprite
     */

    void sprite_push(sprite s1, sprite s2, double mass1, double mass2);

    /**
     * add physics formula including sprite kinetic energy, interaction force and elastic collision into sprite collision and make it more realistic
     *  @param sprite_1 The first sprite
     *  @param sprite_2 The second sprite
     *  @param mass1 The mass of first sprite used for calculate the kinetic energy of first sprite
     *  @param mass2 The mass of second sprite used for calculate the kinetic energy of second sprite
     *  @param speed1 The speed/velocity of first sprite used for calculate the kinetic energy of first sprite
     *  @param speed2 The speed/velocity of second sprite used for calculate the kinetic energy of second sprite
     */

    void Physical_sprites_collision(sprite sprite_1, sprite sprite_2, double mass1, double mass2, double speed1, double speed2);

    /**
     * add physics collision function with angles
     *  @param sprite_1 The first sprite
     *  @param sprite_2 The second sprite
     */
    void collision_angle(sprite sp1,sprite sp2);

    /**
     * add function for calculating kinetic energy
     *  @param sprite_1 The first sprite
     */
    float KINETIC_ENERGY(sprite sprite_1);

    /**
     * add physics collision function with angles
     *  @param sprite_1 The first sprite
     *  @param sprite_2 The second sprite
     */
    float velocity_after_collied(sprite sprite_1, sprite sprite_2);



     /**
     * add physics kinetic function for shapes
     *  @param c1_mass        shape mass
     *  @param C1_X_speed     shape speed in x d  
     *  @param C1_Y_speed     shape speed in y d  
     */
    float KINETIC_ENERGY(float c1_mass, float C1_X_speed, float C1_Y_speed);


     /**
     * add velocity function for shapes
     *  @param c1_mass        shape1 mass
     *  @param c2_mass        shape2 mass
     *  @param C1_X_speed     shape1 speed in x d  
     *  @param C1_Y_speed     shape1 speed in y d  
     */
    float velocity_after_collied(float c1_mass,float c2_mass,float C1_X_speed, float C1_Y_speed);

     /**
     * add collision function for circles
     *  @param c1       circle1
     *  @param c2       circle2
     *  @param c1_mass    shape1 speed in x d  
     *  @param c2_mass     shape1 speed in y d  
     *  @param c1_x_speed    shape1 speed in x d  
     *  @param c2_y_speed     shape1 speed in y d       
     */
    void collision(circle c1, circle c2, float c1_mass, float c2_mass, float C1_X_speed, float C1_Y_speed);

     /**
     * add collision function for circles
     *  @param c1       cirlce1
     *  @param c2       circle22
     *  @param c1_mass    shape1 speed in x d  
     *  @param c2_mass     shape1 speed in y d  
     *  @param c1_x_speed    shape1 speed in x d  
     *  @param c2_y_speed     shape1 speed in y d       
     */
    void rect_collision(rectangle c1, rectangle c2, float c1_mass, float c2_mass, float C1_X_speed, float C1_Y_speed);
     /**
     * add collision function for circles
     *  @param c1       rectangle1
     *  @param c2       rectangle2
     *  @param c1_mass    shape1 speed in x d  
     *  @param c2_mass     shape1 speed in y d  
     *  @param c1_x_speed    shape1 speed in x d  
     *  @param c2_y_speed     shape1 speed in y d       
     */

    float Free_Falling(float ratio, sprite c1, float Gravity);
    /**
     * add function for calculating falling velocity
     *  @param ratio       ratio to calculate falling acceleration
     *  @param c1          sprite 1 
     *  @param Gravity     The gravity   
     */
    float density(sprite c1, float volume);
    /**
     * add function for calculating density
     *  @param c1       sprite1
     *  @param volume
     */
    float sinking_velocity(sprite c1, float buoyancy, float volume, float Gravity);
    /**
     * add function for calculating sinking velocity
     *  @param c1       
     *  @param buoyancy          
     *  @param volume
     *  @param gravity
     */
    void sinking(sprite c1,float Height_of_water, float buoyancy, float Gravity, float volume, float density_of_water);
    /**
     * add function for calculating sinking velocity
     *  @param c1         sprite 1
     *  @param  Height_of_water        
     *  @param  buoyancy
     *  @param  Gravity
     *  @param  volume
     *  @param  density_of_water
     */
    float sprite_momentum_x(sprite spr);
    /**
     * Calculates the momentum of a moving sprite in the x-axis
     * @param spr   Sprite object to calculate
     * @return      Returns the momentum of sprite in the x-axis
     */
    float sprite_momentum_y(sprite spr);
    /**
     * Calculates the momentum of a moving sprite in the y-axis
     * @param spr   Sprite object to calulcate
     * @return      Returns the momentum of sprite in the x-axis
     */
    void sprite_inelastic_collision(sprite spr_a, sprite spr_b);
    /**
     * Calculate, change velocity, and detect collision between two sprites when in inelastic collision
     * @param spr_a Sprite A
     * @param spr_b Sprite B
     */
    void sprite_elastic_collision(sprite spr_a, sprite spr_b);
    /**
     * Calculates, change velocities, and detect collision between two sprites when in elastic collision
     * @param spr_a Sprite A
     * @param spr_b Sprite B
     */
    void sprite_confine_screen(sprite spr, string screen_name);
    /**
     * Confines the sprite within the playable screen only
     * @param spr         Sprite to confine
     * @param screen_name The name of the window
     */
    double sprite_impulse_x(sprite spr);
    /**
     * Calculates and returns the impulse of a sprite from the x-axis.
     * @param spr  Sprite to calculate impulse
     */
    double sprite_impulse_y(sprite spr);
    /**
     * Calculates and returns the impulse of a sprite from the x-axis.
     * @param spr  Sprite to calculate impulse
     */
    void sprite_set_impulse_x(sprite spr, double impulse);
    /**
     * Sets the sprite's impulse in the x-axis.
     * @param spr       Sprite to set impulse
     * @param impulse   Value to set the sprite's impulse
     */
    void sprite_set_impulse_y(sprite spr, double impulse);
    /**
     * Sets the sprite's impulse in the y-axis.
     * @param spr       Sprite to set impulse
     * @param impulse   Value to set the sprite's impulse
     */ 
}
#endif /* collisions_h */
