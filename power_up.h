#ifndef POWER_UP_HEADER
#define POWER_UP_HEADER

#include <glm/glm.hpp> // include glm, a library that allows for us to create OpenGL vectors, matrices, etc.
#include <string> // include string library to help identify the different powerups

#include "in_game_obj.h" // include the IN_GAME_OBJ header file to access its class

// define a constant glm vec-2 variable that will store the size of our powerup object
const glm::vec2 POWER_UP_SIZE(60.0f, 20.0f);
// define a constant glm vec-2 variable that will store the velocity at which the power up will move towards the bottom of the screen
// we only want it to move on the y-axis so we don't want an x-coordinate value other than 0
const glm::vec2 POWER_UP_VELOCITY(0.0f, 150.0f);

// POWER_UP_OBJ is a class which represents objects in game that once a tile/block/brick is broken, it will randomly drop one of these
// objects that will fall towards the bottom of the screen. If a player has a collision with a POWER_UP_OBJ depending on the power up
// it will give the player an advantage or disadvantage

// make POWER_UP_OBJ a subclass of IN_GAME_OBJ; meaning that GAME_BALL_OBJ will import/inherit all of the data/function members of IN_GAME_OBJ
// this makes IN_GAME_OBJ a BASE CLASS and POWER_UP_OBJ a DERIVED CLASS; this saves us time having to rewrite data/function members within POWER_UP_OBJ
class POWER_UP_OBJ : public IN_GAME_OBJ
{
public: // all (newly defined) public data/function members that are within POWER_UP_OBJ; all previously data/function members inherited from IN_GAME_OBJ don't need to be redefined here
	// string public data member that will deduce the type of power up
	std::string Power_Up_Type; 
	// float public data member that will store the duration that the power up will effect the player
	float Power_Up_Duration;
	// boolean public data member that tells if the power up has been activated or not
	bool Power_Up_Activated;
	// constructor that takes a string for the power up type, a 3-value glm vector that represents the color of the power up, a float value for the power up duration
	// a 2-value glm vector that represents the position of the power up, and a TEXTURE_2D_OBJ that has the texture of a particular power up as its arguments/parameters
	
	/*
		we use a contstructor member initalizer list because it allows us to set the values of data members prior to the body of the constructor executing
		within the constructor member initializer list we call the IN_GAME_OBJ constructor with the argument/parameter power up position, the POWER_UP_SIZE
		constant 2-value glm vector variable, the TEXTURE_2D_OBJ argument/parameter, the 3-value glm vector argument/parameter, and the POWER_UP_VELOCITY
		constant 2-value glm vector variable

		in addition, we also take our previous arguments/parameters in this constructor and assign the values within those arguments/parameters to the related
		public data members

		the bool public data member is initialzed but does not have a value assigned to it
	*/

	POWER_UP_OBJ(std::string power_up_type_argument, glm::vec3 power_up_color_argument, float power_up_duration_argument, glm::vec2 power_up_position_argument, TEXTURE_2D_OBJ power_up_texture_argument) : IN_GAME_OBJ(power_up_position_argument, POWER_UP_SIZE, power_up_texture_argument, power_up_color_argument, POWER_UP_VELOCITY), Power_Up_Type(power_up_type_argument), Power_Up_Duration(power_up_duration_argument), Power_Up_Activated()
	{

	}
};

#endif // !POWER_UP_HEADER

