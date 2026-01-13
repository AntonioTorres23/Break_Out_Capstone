// game_logic c++ file

#include "game_logic.h" // include game_logic header file that contains class template
#include "resource_manager.h" // include resource_manager header file to access the static functions within the game_logic c++ file
#include "render_sprite.h" // include render_sprite header file to allow for RENDER_SPRITE_OBJ to be created in the game_logic c++ file
#include "in_game_obj.h"   // include in_game_obj header file to allow for IN_GAME_OBJ to be created in the game_logic c++ file
#include "game_ball.h"    // include game_ball header file to allow for GAME_BALL_OBJ to be created in the game_logic c++ file

/*
NOTICE HOW WE'RE DEFINING THE FUNCTIONS THAT ARE STORED WITHIN THE GAME_OBJ CLASS/OBJECT, SO IN THE C++ FILE HERE WE'RE DEFINING THE ACTUAL SOURCE CODE
THE FUNCTIONS THAT ARE STORED IN THE HEADER FILE ARE REALLY ONLY PROTOTYPES
*/

// create a RENDER_SPRITE_OBJ non-contructed pointer object; this is just a place holder since we did not specify construction parameters to begin running the body of the constructor  
RENDER_SPRITE_OBJ *Sprite_Render; 
// create a IN_GAME_OBJ non-constructed pointer object that will represent the position of the player/user
IN_GAME_OBJ *Player_Object; 
// create a GAME_BALL_OBJ non-contructed pointer object that will represent the breakout ball
GAME_BALL_OBJ *Game_Ball;

// use the game object contructor that takes as arguments the width and height that the game window should be
// we also use a contructor member initalizer list with the Game_State ENUM, Key_Pressed_Buffer, and Height and Width of the screen data members. We set the Height and Width of the screen to their respective constructor arguments
// we use a contstructor member initalizer list because it allows us to set the values of data members prior to the body of the constructor executing
GAME_OBJ::GAME_OBJ(unsigned int width_of_window, unsigned int height_of_window)
	: Game_State(ACTIVE_GAME), Key_Pressed_Buffer(), Width_Of_Screen(width_of_window), Height_Of_Screen(height_of_window)
{

}

// deconstructor for the game object
GAME_OBJ::~GAME_OBJ()
{
	// within the deconstructor delete any objects that are dynamically allocated using the new keyword
	// the delete keyword is used to destroy pointer objects that are dynamically created via the new keyword; therefore this deallocates memory from the heap that these dynamically allocated objects use
	delete Sprite_Render;
	delete Player_Object;
}

// game initazlier function definition
void GAME_OBJ::Initalize_Game()
{
	// load the shaders provided with the static function shader_load from the resource_manager header file
	RESOURCE_MANAGER::Shader_Load("Resources/Shaders/sprite_test.vert", "Resources/Shaders/sprite_test.frag", nullptr, "sprite_test");
	// set a orthographic projection matrix to the dimensions of the screen from our related public data members statically casted to a float value and a near distance of -1 and a far distance of 1 which are Normalized Device Coordinates
	glm::mat4 sprite_orthographic_projection_matrix = glm::ortho(0.0f, static_cast<float>(this->Width_Of_Screen), static_cast<float>(this->Height_Of_Screen), 0.0f, -1.0f, 0.0f);
	// get the shader via the mapped_shader name with the resource manager static function Shader_Get and both activate the shader/set the sampler2D uniform variable to GL_ACTIVETEXTURE0
	RESOURCE_MANAGER::Shader_Get("sprite_test").Activate().uniform_integer("texture_image", 0);
	// now set the sprite_orthographic_projection_matrix we defined here to be sent to the fragment shader uniform variable of the same name
	RESOURCE_MANAGER::Shader_Get("sprite_test").uniform_matrix_4("sprite_orthographic_projection_matrix", sprite_orthographic_projection_matrix);
	
	// create a temporary shader object that stores the same sprite test shader but within an l-value object
	SHADER_OBJ dynamic_memory_sprite_test_shader_object;

	// now get that same shader program via the static function Shader_Get and store it in the shader object we just created  
	dynamic_memory_sprite_test_shader_object = RESOURCE_MANAGER::Shader_Get("sprite_test");

	// now with our sprite_render pointer object we created earlier, dynamically allocate memory from the heap with the new keyword to return an address of the rendered sprite constructor object to the sprite_render pointer object
	// within the constructor we specify our sprite_test shader program created earlier with the dynamic_memory_sprite_test_shader_object
	
	/*
	
	  the reason why we have to do this has to do with the compiler and that the RENDER_SPRITE_OBJ take a non constant reference to the shader. Our Shader_Get static function from the resource manager returns the shader by value
	  via returning the mapped string associated with the shader object, which is a temporary object (aka an r value) that only builds the shader object then destroys it which does not let us keep a reference to it. Which is why
	  we need to create another l-value object that is stores the shader program in memory and so the contructor can grab the address. 
	*/ 
	Sprite_Render = new RENDER_SPRITE_OBJ(dynamic_memory_sprite_test_shader_object);

	// load texture(s) which will represent our sprite(s) with the resource manager static function Texture_Load
	RESOURCE_MANAGER::Texture_Load("Resources/Textures/background.jpg", false, "game_background");
	RESOURCE_MANAGER::Texture_Load("Resources/Textures/awesomeface.png", true, "game_ball");
	RESOURCE_MANAGER::Texture_Load("Resources/Textures/block.png", false, "block_destroyable");
	RESOURCE_MANAGER::Texture_Load("Resources/Textures/block_solid.png", false, "solid_block");
	RESOURCE_MANAGER::Texture_Load("Resources/Textures/paddle.png", true, "player_object");

	// create the GAME_LEVEL_OBJ(s) and load the level(s) via the corresponding member function Level_Load method within a GAME_LVL_OBJ
	GAME_LEVEL_OBJ level_one; 
	// notice how we half our height of window/screen dimensions for our corresponding Level_Load parameter/argument because we want the tiles/blocks/bricks to be further up towards the top of the screen (at least I think that's why)
	level_one.Level_Load("Resources/Levels/standard_level.level", this->Width_Of_Screen, this->Height_Of_Screen / 2);

	GAME_LEVEL_OBJ level_two; 

	level_two.Level_Load("Resources/Levels/gaps_level.level", this->Width_Of_Screen, this->Height_Of_Screen / 2);

	GAME_LEVEL_OBJ level_three;

	level_three.Level_Load("Resources/Levels/space_invader_level.level", this->Width_Of_Screen, this->Height_Of_Screen / 2);

	GAME_LEVEL_OBJ level_four;

	level_four.Level_Load("Resources/Levels/bounce_level.level", this->Width_Of_Screen, this->Height_Of_Screen / 2);

	// now push all level(s) that where previously defined and loaded into the Game_Levels standard lib vector data member of the GAME_OBJ
	this->Game_Levels.push_back(level_one);
	this->Game_Levels.push_back(level_two);
	this->Game_Levels.push_back(level_three);
	this->Game_Levels.push_back(level_four);
	// set the Game_Level data member within GAME_OBJ to the value of 0
	// this is simply a way we can index the GAME_LEVEL_OBJs that are stored within the Game_Levels standard lib vector to grab a particular level
	// i.e. Index 0: level_one aka standard_level Index 1: level_two aka gaps_level
	this->Game_Level = 0; 

	// create a 2-value GLM vector that will store the position of the player object within the screen
	// this will be calculated by taking the GAME_OBJ's data members Width_Of_Screen, Height_Of_Screen and subtracting those values by the respective SCALE_SIZE_OF_PLAYER 2-value GLM vector coordinates
	// with subtracting half of the Width_Of_Screen by half of the SCALE_SIZE_OF_PLAYER.x for the x value and subtracting Height_Of_Screen by SCALE_SIZE_OF_PLAYER.y with no changes to the original values 
	glm::vec2 position_of_player = glm::vec2(this->Width_Of_Screen / 2.0f - SCALE_SIZE_OF_PLAYER.x / 2.0f, this->Height_Of_Screen - SCALE_SIZE_OF_PLAYER.y);

	// now with our player_object pointer object we created earlier, dynamically allocate memory from the heap with the new keyword to return an address of the IN_GAME_OBJ constructor object to the player_object pointer object

	Player_Object = new IN_GAME_OBJ(position_of_player, SCALE_SIZE_OF_PLAYER, RESOURCE_MANAGER::Texture_Get("player_object"));

	// create a 2-value GLM vector that will store the position of the ball object within the screen
	// this will be calculated by adding the position_of player and a 2-value glm vector with SCALE_SIZE_OF_PLAYER.x / 2.0f - BALL_OBJECT_RADIUS for the x coordinate, and -BALL_OBJECT_RADIUS * 2.0f for the y coordinate
	// this places the ball with the player to "tie" the two objects together before the player is ready to start the game by launching the ball
	glm::vec2 position_of_ball = position_of_player + glm::vec2(SCALE_SIZE_OF_PLAYER.x / 2.0f - BALL_OBJECT_RADIUS, -BALL_OBJECT_RADIUS * 2.0f);


	// now with our ball_object pointer object we created earlier, dynamically allocate memory from the heap with the new keyword to return an address of the GAME_BALL_OBJ constructor object to the ball_object pointer object 
	Game_Ball = new GAME_BALL_OBJ(position_of_ball, BALL_OBJECT_RADIUS, BALL_OBJECT_PHYSICS_VELOCITY, RESOURCE_MANAGER::Texture_Get("game_ball"));
}

// game update of player movement and ball location function definition
void GAME_OBJ::Update_Game(float delta_time)
{
	// call the member function Ball_Movement to update the location of the ball and include the delta_time argument/parameter and the Width_Of_Screen data member stored within the GAME_OBJ
	Game_Ball->Ball_Movement(delta_time, this->Width_Of_Screen);
}

// process player input function definition
void GAME_OBJ::Process_User_Input(float delta_time)
{
	// if the state of the game enumeration is considered active, process user input
	if (this->Game_State == ACTIVE_GAME)
	{
		// velocity is calculated by the PLAYER_PHYSICS_VELOCITY variable from the game_logic header file and multiplying it by the delta time, this slows down the object so the computer can "catch up" with rendering the scene
		float player_object_velocity = PLAYER_PHYSICS_VELOCITY * delta_time;
		// if the KEY_PRESSED_BUFFER data member array within GAME_OBJ contains GLFW_KEY_A (if this statment is true)
		if (this->Key_Pressed_Buffer[GLFW_KEY_A])
		{
			// if the position of the player is greater than 0.0f (the furthest left to the screen) use a subtraction compound assignment operator with the player_object_velocity variable to move the Player_Object.game_object_position's x value to the left
			// meaning that everytime the a key is pressed by the player/user, Player_Object.game_object_position's x value will be subtracted (or lowered) by the value of player_object_velocity, but only to the limits of the screen which the furtheset left
			// we can go is the value 0.0f (think of the orthographic projection matrix for the shaders we use which defines the dimensions of the screen, we specify with 0.0f as our left most part of the matrix) 
			if (Player_Object->game_object_position.x >= 0.0f)
				Player_Object->game_object_position.x -= player_object_velocity;
			
			// if the GAME_BALL_OBJ's data member ball_stuck is true, do the same subtraction compound matrix to the ball position's x coordinate using the player_object_velocity value, moving the ball along with the player to the left
			if (Game_Ball->ball_stuck)
			{
				Game_Ball->game_object_position.x -= player_object_velocity;
			}

		}
		// if the KEY_PRESSED_BUFFER data member array within GAME_OBJ contains GLFW_KEY_D (if this statment is true)
		if (this->Key_Pressed_Buffer[GLFW_KEY_D])
		{
			// similar to the previous statement, if the position of the player is less than or equal to the furthest right to the screen, we can move the Player_Object.game_object_position's x value to the right
			// to see if we are in the borders of the screen, we have to take the Width_Of_Screen data member within GAME_OBJ subtracted by the game_object_scale_size within IN_GAME_OBJ
			// this takes the right edge of the Player_Object's scale_size which is the size of the object itself and subtracts it by the entire width of the screen to give us a value of the max amount the Player_Object.game_object_position's x value can go
			// if it is less than this value, then use an addition compound assignment operator to move Player_Object.game_object_position's x value to the right by the value of player_object_velocity
			if (Player_Object->game_object_position.x <= this->Width_Of_Screen - Player_Object->game_object_scale_size.x)
				Player_Object->game_object_position.x += player_object_velocity;

			// if the GAME_BALL_OBJ's data member ball_stuck is true, do the same add compound matrix to the ball position's x coordinate using the player_object_velocity value, moving the ball along with the player to the right
			if (Game_Ball->ball_stuck)
			{
				Game_Ball->game_object_position.x += player_object_velocity;
			}
		}
		// if the KEY_PRESSED_BUFFER data member array within GAME_OBJ contains GLFW_KEY_SPACE (if this statment is true)
		if (this->Key_Pressed_Buffer[GLFW_KEY_SPACE])
		{
			// set the ball_stuck data member within GAME_BALL_OBJ to false which essentially launches the ball and starts the game
			Game_Ball->ball_stuck = false; 
		}

	}
}

// render game onto player screen function definition
void GAME_OBJ::Render_Game()
{

	// create a temporary texture object that stores the same texture but within an l-value object
	TEXTURE_2D_OBJ texture_object_for_sprite_rendering;

	// now get that same texture object via the static function Texture_Get and store it in the texture object we just created  
	texture_object_for_sprite_rendering = RESOURCE_MANAGER::Texture_Get("game_background");

	// within the Render_and_Draw_Sprite method function, we specify our texture image created earlier with the texture_object_for_sprite_rendering

	/*

	  the reason why we have to do this has to do with the compiler and that the RENDER_SPRITE_OBJ take a non constant reference to the shader. Our Shader_Get static function from the resource manager returns the shader by value
	  via returning the mapped string associated with the shader object, which is a temporary object (aka an r value) that only builds the shader object then destroys it which does not let us keep a reference to it. Which is why
	  we need to create another l-value object that is stores the shader program in memory and so the contructor can grab the address.
	*/


	// if the state of the game enumeration is considered active, render the level

	if (this->Game_State == ACTIVE_GAME)
	{
			// DRAW AND RENDER THE BACKGROUND FIRST
			// Draw and Render our sprite on screen via the function Render_and_Draw_Spirte function
			Sprite_Render->Render_and_Draw_Spirte(texture_object_for_sprite_rendering, glm::vec2(0.0f, 0.0f), glm::vec2(this->Width_Of_Screen, this->Height_Of_Screen), 0.0f);

			// NOW DRAW AND RENDER THE TILES/BLOCKS/BRICKS IN THE GAME_LEVELS member standard lib vector
			// the Game_Level member is like an index and stores what level we want to load so in this case index 0 stores level one so we will draw and render level one
			this->Game_Levels[this->Game_Level].Level_Render_and_Draw(*Sprite_Render);

			Player_Object->Game_Object_Draw(*Sprite_Render);

			// NOW DRAW AND RENDER THE BALL
			
			Game_Ball->Game_Object_Draw(*Sprite_Render);
	}


}