#ifndef FREEWAY_RUN_OBSTACLE
#define FREEWAY_RUN_OBSTACLE

#include "splashkit.h"
#include "player.h"

using namespace std;

#define OBSTACLE_SPEED 60

//Data on an obstacle
struct obstacle_data
{
  bool active;            //Determines whether the puddle is in effect
  sprite obstacle_sprite; //To perform movement on the obstacle
};

//Create a new obstacle up in a specific lane in the game
obstacle_data new_obstacle(double lane);

//Perform changes to the obstacle up e.g. change speed relative to the player's speed
void update_obstacle(obstacle_data &obstacle, const player_data &player);

//Draw an obstacle in the game
void draw_obstacle(const obstacle_data &obstacle);

//Read user input and update the obstacle (experimental)
void obstacle_handle_input(obstacle_data &obstacle);

#endif