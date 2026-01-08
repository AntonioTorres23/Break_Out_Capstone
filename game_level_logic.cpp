#include "game_level_logic.h" // include game_level_logic to access the GAME_LEVEL_OBJ class

#include <fstream> // include fstream to open and read the buffers of external files

#include <sstream> // include sstream to access string streams

/*
	NOTICE HOW WE'RE DEFINING THE FUNCTIONS THAT ARE STORED WITHIN THE GAME_LEVEL_OBJ CLASS/OBJECT, SO IN THE C++ FILE HERE WE'RE DEFINING THE ACTUAL SOURCE CODE
	THE FUNCTIONS THAT ARE STORED IN THE HEADER FILE ARE REALLY ONLY PROTOTYPES
*/


// define the Level_Load member function 

void GAME_LEVEL_OBJ::Level_Load(const char* level_file_location, unsigned int width_of_level, unsigned int height_of_level)
{
	// get rid of the old data within the game_object_bricks data member by using its built-in method that comes with std::vectors
	this->game_object_bricks.clear();

}

