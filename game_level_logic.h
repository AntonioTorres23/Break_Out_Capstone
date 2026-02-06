#ifndef GAME_LEVEL_HEADER
#define GAME_LEVEL_HEADER

#include <vector> // don't get this confused with a glm vector, this is a standard library that allows us to organize and store variables/objects in a more efficient way; it has methods that make parsing through this data much easier than something like an array

#include <glad/glad.h> //include GLAD; a lib that loads the addresses of OpenGL function pointers
#include <glm/glm.hpp> // include glm for uniform vector/matrix sets

// include in_game_obj header file to access the IN_GAME_OBJ class
#include "in_game_obj.h"

// include the render_sprite header file to access the RENDER_SPRITE_OBJ class 
#include "render_sprite.h"

// include power_up header file to access the POST_PROCESSING_OBJ class
#include "power_up.h"

// include resource_manager header file to access the RESOURCE_MANAGER class specificly its static functions
#include "resource_manager.h"

// GAME_LEVEL_OBJ class that stores our tiles within a break out level and can load levels from a HDD file location on the OS
class GAME_LEVEL_OBJ
{
public: // all public data/function members
	// a standard library vector data member that stores all of the bricks in our game level as a IN_GAME_OBJ
	std::vector<IN_GAME_OBJ> game_object_bricks;
	// a standard library vector data member that stores all of the powerups in our game level as a POWER_UP_OBJ
	std::vector<POWER_UP_OBJ> power_up_objects;
	// empty constructor of the GAME_LEVEL_OBJ class
	GAME_LEVEL_OBJ()
	{

	}

	// void member function that will load a level from an external file
	void Level_Load(const char* level_file_location, unsigned int width_of_level, unsigned int height_of_level);

	// void member function that will render and draw a level
	void Level_Render_and_Draw(RENDER_SPRITE_OBJ& render_sprite_argument);
	
	// void member function that will generate a random power up
	void Power_Up_Generate(IN_GAME_OBJ& brick_within_game);

	// void member function that will update the power ups in game
	void Power_Up_Update(float delta_time);

	// boolean member function that checks if the level is completed and all non-solid tiles become destroyed
	bool Level_Complete();

private: // all private data/function members

	// void private member function that will intalize a level from the provided tile data, takes in a 2D vector (think of it like a nested vector, or rows and columns) indexed by two locations; i.e. [i][j] where i stores multiple unsigned int values [j]
	// think of it as [i] as a container/box and [j] as the items that are stored inside
	void initalize_level(std::vector<std::vector<unsigned int>> data_of_tiles, unsigned int width_of_level, unsigned int height_of_level);

};

#endif // !GAME_LEVEL_HEADER
