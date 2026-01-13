#ifndef GAME_BALL_HEADER
#define GAME_BALL_HEADER

#include <glad/glad.h>  // include GLAD; a lib that loads the addresses of OpenGL function pointers
#include <glm/glm.hpp> // include glm, a library that allows for us to create OpenGL vectors, matrices, etc. 

#include "in_game_obj.h" // include in_game_object header file because a ball is a type of in game object so we need to access its class capabilites
#include "process_texture.h" // include process_texture header file to access the class

// GAME_BALL_OBJ is a class that will be used to create the ball in our breakout game; this comes with data/function members that use collisions to bounce around the screen and many other functionalities

// make GAME_BALL_OBJ a subclass of IN_GAME_OBJ; meaning that GAME_BALL_OBJ will import/inherit all of the data/function members of IN_GAME_OBJ
// this makes IN_GAME_OBJ a BASE CLASS and GAME_BALL_OBJ a DERIVED CLASS; this saves us time having to rewrite data/function members within GAME_BALL_OBJ 
class GAME_BALL_OBJ : public IN_GAME_OBJ
{
public: // all (newly defined) public data/function members that are within GAME_BALL_OBJ; all previously data/function members inherited from IN_GAME_OBJ don't need to be redefined here

	// float data member that stores the radius of the ball object
	float ball_radius;

	// boolean data member that checks if the ball object is currently stuck or not to the player
	bool ball_stuck;

	// constructor of GAME_BALL_OBJ with no parameters/arguments
	GAME_BALL_OBJ();

	// constructor of GAME_BALL_OBJ with 4 parameters/arguments, 1: a 2-value glm vector which is the position of the ball, 2: a float value which is the radius of the ball, 
	// 3: a 2-value glm vector which is the physics_velocity of the ball, 4: a TEXTURE_2D_OBJ to represent the ball's sprite
	GAME_BALL_OBJ(glm::vec2 position_of_ball_argument, float radius_of_ball_argument, glm::vec2 physics_velocity_of_ball_argument, TEXTURE_2D_OBJ ball_sprite_argument);

	// member function that processes the ball's movemnt within the game; takes delta time and the width of the game screen as arguments/parameters
	glm::vec2 Ball_Movement(float delta_time, unsigned int width_of_window_argument);
	
	// member function that resets the GAME_BALL_OBJ; takes the position of the ball as well as the ball's physics_velocity as arguments/parameters
	void Ball_Reset(glm::vec2 position_of_ball_argument, glm::vec2 physics_velocity_of_ball_argument);
};

#endif // !GAME_BALL_HEADER
