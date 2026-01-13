#include "game_ball.h" // include game_ball.h to access the GAME_BALL_OBJ class

/*
NOTICE HOW WE'RE DEFINING THE FUNCTIONS THAT ARE STORED WITHIN THE GAME_BALL_OBJ CLASS/OBJECT, SO IN THE C++ FILE HERE WE'RE DEFINING THE ACTUAL SOURCE CODE
THE FUNCTIONS THAT ARE STORED IN THE HEADER FILE ARE REALLY ONLY PROTOTYPES
*/


// define constructor of GAME_BALL_OBJ when there are no arguments/parameters specified; within the constructor member initalizer list, we create an IN_GAME_OBJECT with no constructor (remember our GAME_BALL_OBJ is derived from IN_GAME_OBJ),
// our ball_radius data member is set to a value of 12.5f, and our ball_stuck data member is set to true
// we use a constructor member initalizer list because it allows us to set the values of data members prior to the body of the constructor executing
GAME_BALL_OBJ::GAME_BALL_OBJ() : IN_GAME_OBJ(), ball_radius(12.5f), ball_stuck(true)
{

}

/* 
	define constructor of GAME_BALL_OBJ when there ARE arguments / parameters specified; within the constructor member initalizer list, we take the IN_GAME_OBJECT and fill it with the relevant arguments / parameters,
	position_of_ball_argument, a 2-value glm vector that takes its x and y coordinates as radius_of_ball_argument * 2.0f which represents the game_object_scale_size, ball_sprite_argument as the sprite_texture, a 
	3-value glm vector with all rgb values set to 1.0f (white) for the game_object_color, and physics_velocity_of_ball_argument for the game_object_physics_velocity

	additionally, within the GAME_BALL_OBJ data members, we set ball_radius as radius_of_ball_argument, and ball_stuck set to true 

*/
// we use a constructor member initalizer list because it allows us to set the values of data members prior to the body of the constructor executing
GAME_BALL_OBJ::GAME_BALL_OBJ(glm::vec2 position_of_ball_argument, float radius_of_ball_argument, glm::vec2 physics_velocity_of_ball_argument, TEXTURE_2D_OBJ ball_sprite_argument) : IN_GAME_OBJ(position_of_ball_argument, glm::vec2(radius_of_ball_argument * 2.0f, radius_of_ball_argument * 2.0f), ball_sprite_argument, glm::vec3(1.0f), physics_velocity_of_ball_argument), ball_radius(radius_of_ball_argument), ball_stuck(true)
{

}

// define the Ball_Movement public member function
glm::vec2 GAME_BALL_OBJ::Ball_Movement(float delta_time, unsigned int width_of_window_argument)
{
	// check if the GAME_BALL_OBJ is not stuck to the player with the ball_stuck data member (aka is false)
	if (!this->ball_stuck)
	{
		// move the ball around the game screen by using an addition compound assignment opperator in which the product of the ball's velocity * delta time is the value being addition compound assigned to the ball's position
		this->game_object_position += this->game_object_physics_velocity * delta_time;

		// if statment that checks if the ball position's x coordinate is less than or equal to the left most side of the screen (0.0), flip the velocity and restore it to the correct position (essentially making the ball bounce back preventing the ball from going out of bounds)
		if (this->game_object_position.x <= 0.0f)
		{
			// negate the velocity's x coordinate value to make the ball go in the opposite direction (in other words, bounce)
			this->game_object_physics_velocity.x = -this->game_object_physics_velocity.x;

			// set the x coordinate of the ball's position to 0.0f thus making it in bounds at the left most side of the screen (0.0)
			this->game_object_position.x = 0.0f;
		}
		// else if statment that checks if the sum of the ball's position and scale_size x coordinate (need to get the right side of the player to do the comparision which is why we do a sum) 
		// is greater than or equal to the right most side of the screen (width_of_window), if it is, flip the velocity and restore it to the correct position (essentially making the ball bounce back preventing the ball from going out of bounds)
		else if (this->game_object_position.x + this->game_object_scale_size.x >= width_of_window_argument)
		{
			// negate the velocity's x coordinate value to make the ball go in the opposite direction (in other words, bounce)
			this->game_object_physics_velocity.x = -this->game_object_physics_velocity.x;

			// set the x coordinate of the ball's position to the difference of the width_of_window and the scale_size's x coordinate to keep the ball in bounds at the right most side of the screen (width_of_window)
			// I THINK WE USE THE SCALE_SIZE TO CHECK IF IT IS IN BOUNDS BECAUSE THIS IS WHAT THE BALL/OBJECT IS SCALED TO WITHIN THE TRANSFORMATION MATRIX WHICH ARE THE ACTUAL DIMENSIONS; RATHER THAN THE POSITION
			// THAT THE BALL/OBJECT IS ON SCREEN
			this->game_object_position.x = width_of_window_argument - this->game_object_scale_size.x;
		}

		// if statement that checks if the ball position's y coordiante is less than or equal to the top most side of the screen (0.0), flip the velocity and restore it to the correct position (essentially makingg the ball bounce back preventing the ball from going out of bounds)
		if (this->game_object_position.y <= 0.0f)
		{
			// negate the velocity's y coordiante to make the ball go in the opposite direction (in other words, bounce)
			this->game_object_physics_velocity.y = -this->game_object_physics_velocity.y;
			// set the y coordinate of the ball's position to 0.0f thus making it in bounds at the top most side of the screen (0.0)
			this->game_object_position.y = 0.0f;
		}
	}
	// return the changes to the ball's position within this function
	return this->game_object_position;
}


// define the Ball_Reset public member function
void GAME_BALL_OBJ::Ball_Reset(glm::vec2 position_of_ball_argument, glm::vec2 physics_velocity_of_ball_argument)
{
	// if ball is outside the screen, set the ball's position and velocity data members to the specified argument/parameters within this function; as well as set the ball_stuck data member to true (reseting the ball to its original state)
	this->game_object_position = position_of_ball_argument;
	this->game_object_physics_velocity = physics_velocity_of_ball_argument;
	this->ball_stuck = true;
}