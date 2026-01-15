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

	// create a unsigned int variable that will store our tile data from an external file
	// EACH LEVEL FILE STORES MULTIPLE UNSIGNED INTEGERS THAT REPRESENT A DIFFERENT KIND OF BRICK
	// 0: EMPTY SPACE WITHIN A LEVEL
	// 1: A BRICK THAT IS SOLID AND THERFORE CANNOT BE DESTROYED
	// any number greater than 1: a brick that can be destroyed, where every integer represents a different color
	unsigned int tile_code_from_file; 

	// create a game_level_obj
	GAME_LEVEL_OBJ level_object; 

	// create a string variable that will store a line from each file
	std::string line_of_code_from_tile_code_file;

	// create a ifstream variable that will open file
	std::ifstream tile_code_file_open(level_file_location);

	// create a 2D vector that will store the data from the external file but within this vector
	std::vector<std::vector<unsigned int>> tile_data_2D_vector;

	// if tile_code_file_open could open the file, then parse through the file and grab data
	if (tile_code_file_open)
	{
		// while parsing and grabbing a line from the "file buffer" and putting storing the data within line_of_code_from_tile_code_file using the standard library function std::getline
		while (std::getline(tile_code_file_open, line_of_code_from_tile_code_file))
		{
			// create a string stream object and store the data grabbed from line_of_code_from_tile_code_file into here
			std::istringstream tile_code_string_stream(line_of_code_from_tile_code_file);
			// create a standard library vector of the type unsigned int and all grab all of the individual data stored witin the data file, then send it to the row_within_tile_code vector, remember the data within a level file looks similar to a matrix with unsigned integers seperated by spaces
			// a row stores multiple unsigned int values within it
			std::vector<unsigned int> row_within_tile_code;
			// while tile_code_string_stream is sending all the read data from every line to the tile_code_from_file variable (it reads every value separated by a space), push this data to the row_within_tile_code vector
			while (tile_code_string_stream >> tile_code_from_file)
			{
				// send all of the individual data gathered from title_code_from_file to the row_within_tile_code 
				row_within_tile_code.push_back(tile_code_from_file);
			}
			// after sending all of the individual data to the row_within_tile_code vector, now push all of those rows into the "columns" or vector that stores the rows (tile_dataa_2D_vector), thus creating the 2D vector
			tile_data_2D_vector.push_back(row_within_tile_code);
		}
		// if the size of tile_data_2D_vector is greater than the value of 0 (thus not empty), begin intalizing the tile data
		if (tile_data_2D_vector.size() > 0)
		{
			// call the private memmber function initalize_level with the new tile_data_2D_vector, the width_of_level parameter/argument, and the height_of_level parameter/argument
			this->initalize_level(tile_data_2D_vector, width_of_level, height_of_level);
		}
	}
	
}


// define the initalize_level privaate member function

void GAME_LEVEL_OBJ::initalize_level(std::vector<std::vector<unsigned int>> data_of_tiles, unsigned int width_of_level, unsigned int height_of_level)
{

	// to get the height of all the tiles/bricks we take the entire size of the data_of_tiles 2D vector
	unsigned int height_of_all_tiles = data_of_tiles.size();
	// to get the width of all the tiles/bricks we take size of the first row (I think) since we only use one index
	unsigned int width_of_all_tiles = data_of_tiles[0].size();
	// to get the width of each individual tile, divide the width of the entire level by the width_of_all_tiles
	// we have statically cast the width_of_all_tiles to a float value which I think is because we had to index that variable
	float width_of_individual_tile = width_of_level / static_cast<float>(width_of_all_tiles);
	// same concept applies to getting the height of an individual tile but this time we don't need to preform a static cast on the height_of_all_tiles variable
	float height_of_individual_tile = height_of_level / height_of_all_tiles;
	// initalize all of the tiles based on 2D vector data_of_tiles
	// create a for loop in which the y coordinate is related to the height_of_all_tiles (aka the columns within the 2D vector), that when the y_coordinate is lesss than the height_of_all_tiles, increment the y coordinate by 1
	for (unsigned int y_coordinate = 0; y_coordinate < height_of_all_tiles; ++y_coordinate)
	{
		// create another for loop in which the x coordinate is related to the width_of_all_tiles (aka the rows within the 2D vector), that when the y_coordinate is less than the width_of_all_tiles, increment the x coordinate by 1
		for (unsigned int x_coordinate = 0; x_coordinate < width_of_all_tiles; ++x_coordinate)
		{
			// check the type of tile/block that is stored within the data_of_tiles 2D vector, with y being the "column" (height) index, and x being the "row" (width) index
			// if the value is equal to 1, it is considered a solid block/tile/brick
			if (data_of_tiles[y_coordinate][x_coordinate] == 1)
			{
				// within a 2-value GLM vector, take the width_of_individual_tile and multiply it by the for loop argument x_coordinate, same applies for the y_coorrdinate and height_of_individual_tile
				// so my best guess on the logic of this is that you are multiplying the inividual sizes of x and y coordinates by thier respective width and height of a single tile to keep them in 
				// range of the dimensions of a singular tile
				glm::vec2 position_of_tile(width_of_individual_tile * x_coordinate, height_of_individual_tile * y_coordinate);
				// in another 2-value GLM vector, simply take the width_of_individual_tile as its x coordinte and height_of_individual_tile as its y coordiante to represent the scale size of a tile/block/brick
				glm::vec2 scale_size_of_tile(width_of_individual_tile, height_of_individual_tile);
				// create an IN_GAME_OBJ that takes in our prior position and scale size variables as parameters/arguments, in addition to a solid block texture and a 3-value color vector that produces a gray-ish color
				IN_GAME_OBJ solid_tile(position_of_tile, scale_size_of_tile, RESOURCE_MANAGER::Texture_Get("solid_block"), glm::vec3(0.8f, 0.8f, 0.7f));
				// set the solid_tile IN_GAME_OBJ's public data member game_object_solid to true
				solid_tile.game_object_solid = true;
				// push this solid_tile IN_GAME_OBJ into the public data member game_object_bricks standard library vector that stores IN_GAME_OBJs within the GAME_LEVEL_OBJ
				this->game_object_bricks.push_back(solid_tile);
			}

			// else if statement if the value within data_of_tiles 2D vector is greater than 1, remember anything greater than one is a destroyable tile/block/brick with each value coorelating to a color
			else if (data_of_tiles[y_coordinate][x_coordinate] > 1)
			{
				// if value within data_of_tiles 2D vector is equal to 1, make the destroyable tile/block/brick white using a 3-value GLM vector
				glm::vec3 color_of_tile = glm::vec3(1.0f);

				// if value within data_of_tiles 2D vector is equal to 2, make the color_of_tile a purple-ish color
				if (data_of_tiles[y_coordinate][x_coordinate] == 2)
					color_of_tile = glm::vec3(0.2f, 0.6f, 1.0f);
				// if value within data_of_tiles 2D vector is equal to 3, make the color_of_tile a green-ish color
				else if (data_of_tiles[y_coordinate][x_coordinate] == 3)
					color_of_tile = glm::vec3(0.0f, 0.7f, 0.0f);
				// if value within data_of_tiles 2D vector is equal to 4, make the color_of_tile a yellow-ish color
				else if (data_of_tiles[y_coordinate][x_coordinate] == 4)
					color_of_tile = glm::vec3(0.8f, 0.8f, 0.4f);
				// if value within data_of_tiles 2D vector is equal to 5, make the color_of_tile a red-ish color
				else if (data_of_tiles[y_coordinate][x_coordinate] == 5)
					color_of_tile = glm::vec3(1.0f, 0.5f, 0.0f);

				// now similar to the solid tile/block/brick, multiply the x_coordinate and y_coordinate by thier respective width_of_individual_tile and height_of_individual_tile within a 2-value GLM vector that will be our position_of_tile variable
				glm::vec2 position_of_tile(width_of_individual_tile * x_coordinate, height_of_individual_tile * y_coordinate);

				// same thing with the scale_size_of_tile variable 
				glm::vec2 scale_size_of_tile(width_of_individual_tile, height_of_individual_tile);

				// now push all these results into the IN_GAME_OBJ standard library vector called game_object_bricks data member within the GAME_LEVEL_OBJ
				// also include a regular block texture within the parameters/arguments
				this->game_object_bricks.push_back(IN_GAME_OBJ(position_of_tile, scale_size_of_tile, RESOURCE_MANAGER::Texture_Get("block_destroyable"), color_of_tile));

			}

		}
	}

}

// define Level_Render_And_Draw member function

void GAME_LEVEL_OBJ::Level_Render_and_Draw(RENDER_SPRITE_OBJ &render_sprite_argument)
{
	// create a for loop that iterates through all provided IN_GAME_OBJ stored in the game_object_bricks standard lib vector within GAME_LEVEL_OBJ
	// if the state of the IN_GAME_OBJ is not destroyed, then render the IN_GAME_OBJECT witin the game_object_bricks standard lib vector
	// 
	// I believe that since we are not using a predefined standerd lib vector and the iterator is using adresses locations, we don't need to use a ; since we are not initalizing anything
	// but just parsing through something that is already pre-existing
	for (IN_GAME_OBJ &tiles_in_game_objects_bricks : this->game_object_bricks)
		if (!tiles_in_game_objects_bricks.game_object_destroyed)
			// render tile/block/brick with the address of a RENDER_SPRITE_OBJ specified within the parameter/argument 
			tiles_in_game_objects_bricks.Game_Object_Draw(render_sprite_argument);
}

// define Level_Complete member functinon

bool GAME_LEVEL_OBJ::Level_Complete()
{
	// create a for loop that iterates through all provided IN_GAME_OBJ stored in the game_object_bricks standard lib vector within GAME_LEVEL_OBJ
	// if the state of the IN_GAME_OBJ is not destroyed, and is not a solid tile/block/brick, then return false. Meaning that the level is not complete yet
	// else, return true meaning that the level is complete
	// 
	// I believe that since we are not using a predefined standerd lib vector and the iterator is using adresses locations, we don't need to use a ; since we are not initalizing anything
	// but just parsing through something that is already pre-existing

	for (IN_GAME_OBJ& tiles_in_game_objects_bricks : this->game_object_bricks)
	{
		if (!tiles_in_game_objects_bricks.game_object_destroyed && !tiles_in_game_objects_bricks.game_object_solid)
			return false;
		return true;
	}
}