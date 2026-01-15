// game_logic c++ file

#include "game_logic.h" // include game_logic header file that contains class template
#include "resource_manager.h" // include resource_manager header file to access the static functions within the game_logic c++ file
#include "render_sprite.h" // include render_sprite header file to allow for RENDER_SPRITE_OBJ to be created in the game_logic c++ file
#include "in_game_obj.h"   // include in_game_obj header file to allow for IN_GAME_OBJ to be created in the game_logic c++ file
#include "game_ball.h"    // include game_ball header file to allow for GAME_BALL_OBJ to be created in the game_logic c++ file


#include <iostream>


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


// PROTOTYPE FUNCTION DECLARATIONS for Axis_Aligned_Bounding_Box_Collision_Check function only relevant to this C++ file
bool Axis_Aligned_Bounding_Box_Collision_Check(IN_GAME_OBJ &first_in_game_obj_argument, IN_GAME_OBJ &second_in_game_obj_argument);

// PROTOTYPE FUNCTION DECLARATIONS for Axis_Aligned_Bounding_Box_Collision_Check (OVERLOADED GAME_BALL_OBJ VERSION OF THIS FUNCTION FOR AABB - CIRCLE COLLISION CHECK) function only relevant to this C++ file
bool Axis_Aligned_Bounding_Box_Collision_Check(GAME_BALL_OBJ &game_ball_obj_argument, IN_GAME_OBJ &second_in_game_obj_argument);


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
	delete Game_Ball;
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
	
	//SHADER_OBJ dynamic_memory_sprite_test_shader_object;

	// now get that same shader program via the static function Shader_Get and store it in the shader object we just created  
	
	//dynamic_memory_sprite_test_shader_object = RESOURCE_MANAGER::Shader_Get("sprite_test");

	// now with our sprite_render pointer object we created earlier, dynamically allocate memory from the heap with the new keyword to return an address of the rendered sprite constructor object to the sprite_render pointer object
	// within the constructor we specify our sprite_test shader program created earlier with the dynamic_memory_sprite_test_shader_object
	
	/*
	
	  the reason why we have to do this has to do with the compiler and that the RENDER_SPRITE_OBJ take a non constant reference to the shader. Our Shader_Get static function from the resource manager returns the shader by value
	  via returning the mapped string associated with the shader object, which is a temporary object (aka an r value) that only builds the shader object then destroys it which does not let us keep a reference to it. Which is why
	  we need to create another l-value object that is stores the shader program in memory and so the contructor can grab the address. 
	*/ 

	// fixed the issue that required the dynamic_memory_sprite_test_shader_object, can't really put it in any better way than the original user that found the solution on LearnOpenGL.com so here is thier statement below this line

	/*
		From User Tuke on the "https://learnopengl.com/In-Practice/2D-Game/Rendering-Sprites" comments:

		#4 is a C++ thing. The SpriteRenderer constructor as it's written takes in a non-const reference to a shader. 
		The ResourceManager::GetShader returns a Shader by value, which is a temporary object, (an rvalue is the term to look up). 
		That returned Shader is built and then destroyed immediately, so the compiler doesn't let you keep a reference to it.
		
		If you make myShader, that becomes an lvalue object allocated on the stack, so you can take a reference to it, which works in Akami's comment. 	
		Imreness's comment works because it changes what's returned from the ResourceManager from a temporary object, to a reference to the RM's Shader, which is also an lvalue.
		
		Another solution is to change the parameter in the SpriteRenderer constructor from a non-const reference to a const ref, i.e. const Shader& Shader. 
		I think the compiler's logic here is this: it knows that the reference passed in is read only, so it will happily just read the data it needs and throw the argument object away when it's done. 
		Passing a non-const ref is used if you want to mutate an object, so the compiler yells at you if it think you're mutating an object that it will just throw away when the constructor is over.
		
		Hopefully that makes sense
	*/

	Sprite_Render = new RENDER_SPRITE_OBJ(RESOURCE_MANAGER::Shader_Get("sprite_test"));

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

	// call the member function Axis_Aligned_Bounding_Box_Collisions to update collisions with the ball and tiles/blocks/bricks in the game; if a collision happens and the block is destroyable, destroy the block
	this->Axis_Aligned_Bounding_Box_Collisions();
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
	
	//TEXTURE_2D_OBJ texture_object_for_sprite_rendering;

	// now get that same texture object via the static function Texture_Get and store it in the texture object we just created  
	
	//texture_object_for_sprite_rendering = RESOURCE_MANAGER::Texture_Get("game_background");

	// within the Render_and_Draw_Sprite method function, we specify our texture image created earlier with the texture_object_for_sprite_rendering

	/*

	  the reason why we have to do this has to do with the compiler and that the RENDER_SPRITE_OBJ take a non constant reference to the shader. Our Shader_Get static function from the resource manager returns the shader by value
	  via returning the mapped string associated with the shader object, which is a temporary object (aka an r value) that only builds the shader object then destroys it which does not let us keep a reference to it. Which is why
	  we need to create another l-value object that is stores the shader program in memory and so the contructor can grab the address.
	*/

	// fixed the issue that required the texture_object_for_sprite_rendering, can't really put it in any better way than the original user that found the solution on LearnOpenGL.com so here is thier statement below this line

	/*
		From User Tuke on the "https://learnopengl.com/In-Practice/2D-Game/Rendering-Sprites" comments:

		#4 is a C++ thing. The SpriteRenderer constructor as it's written takes in a non-const reference to a shader. 
		The ResourceManager::GetShader returns a Shader by value, which is a temporary object, (an rvalue is the term to look up). 
		That returned Shader is built and then destroyed immediately, so the compiler doesn't let you keep a reference to it.
		
		If you make myShader, that becomes an lvalue object allocated on the stack, so you can take a reference to it, which works in Akami's comment. 	
		Imreness's comment works because it changes what's returned from the ResourceManager from a temporary object, to a reference to the RM's Shader, which is also an lvalue.
		
		Another solution is to change the parameter in the SpriteRenderer constructor from a non-const reference to a const ref, i.e. const Shader& Shader. 
		I think the compiler's logic here is this: it knows that the reference passed in is read only, so it will happily just read the data it needs and throw the argument object away when it's done. 
		Passing a non-const ref is used if you want to mutate an object, so the compiler yells at you if it think you're mutating an object that it will just throw away when the constructor is over.
		
		Hopefully that makes sense
	*/

	// if the state of the game enumeration is considered active, render the level

	if (this->Game_State == ACTIVE_GAME)
	{
			// DRAW AND RENDER THE BACKGROUND FIRST
			// Draw and Render our sprite on screen via the function Render_and_Draw_Spirte function
			Sprite_Render->Render_and_Draw_Spirte(RESOURCE_MANAGER::Texture_Get("game_background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width_Of_Screen, this->Height_Of_Screen), 0.0f);

			// NOW DRAW AND RENDER THE TILES/BLOCKS/BRICKS IN THE GAME_LEVELS member standard lib vector
			// the Game_Level member is like an index and stores what level we want to load so in this case index 0 stores level one so we will draw and render level one
			this->Game_Levels[this->Game_Level].Level_Render_and_Draw(*Sprite_Render);

			Player_Object->Game_Object_Draw(*Sprite_Render);

			// NOW DRAW AND RENDER THE BALL
			
			Game_Ball->Game_Object_Draw(*Sprite_Render);
	}


}

// axis aligned bounding box collisions function definition 
void GAME_OBJ::Axis_Aligned_Bounding_Box_Collisions()
{
	// create a for loop that iterates through all provided IN_GAME_OBJ "bricks" stored in Game_Level 1 (0 when specified within the index specifier Game_Level data member) of the GAME_LEVEL_OBJ standard lib vector within GAME_OBJ
	// if the state of the IN_GAME_OBJ is not destroyed, then check if there is a collision between an individual tile/block/brick and the ball object
	//  
	// I believe that since we are not using a predefined standerd lib vector and the iterator is using adresses locations, we don't need to use a ; since we are not initalizing anything
	// but just parsing through something that is already pre-existing
	for (IN_GAME_OBJ &brick_game_object_within_game_level_iterator : this->Game_Levels[this->Game_Level].game_object_bricks)
	{
		if (!brick_game_object_within_game_level_iterator.game_object_destroyed)
		{
			// if there is a collision detected between the Game_Ball and brick iterator's AABBs, remember to make the game ball have a value pointer to get the actual Ball_Object and not just the address
			if (Axis_Aligned_Bounding_Box_Collision_Check(*Game_Ball, brick_game_object_within_game_level_iterator))
			{
				// the collision has occured between the ball and brick iterator's AABBs, and the brick iterator is not a solid tile/block/brick within the game_object_solid data member, then change the game_object_destroyed data member member to true
				if (!brick_game_object_within_game_level_iterator.game_object_solid)
					
					brick_game_object_within_game_level_iterator.game_object_destroyed = true; 

			}

		}
	}

}


// define a boolean function that is only relevant to this C++ file that checks for axis-aligned bounding box (AABB) collisions
// think of this like a hit box for the objects, if the two boxes (objects) overrlap on both axes, we have a collision
// we do this instead of using the actual vertex data of an IN_GAME_OBJ because it simplifies collision detection and improves preformance because some objects can contain many vertices

bool Axis_Aligned_Bounding_Box_Collision_Check(IN_GAME_OBJ &first_in_game_obj_argument, IN_GAME_OBJ &second_in_game_obj_argument)
{
	 
		
	// check if a collision has occured on the x axis of the two provided IN_GAME_OBJs, store this in a boolean variable
	
	/*
		there are 2 things we need to create a bounding box a top-left position and a bottom-right position

		we already have a top-left position within our IN_GAME_OBJ which is game_object_position (I believe this is the case becuase this is simply the location where the object resides)
		additionally, we can retrieve this bottom-right position by taking the sum of prior game_object_position and game_object_scale_size 
		
		I think of the scale size as like the furthest we want the IN_GAME_OBJ to jut out which is always scaled positively within the transformation matrix 
		
		all of this is in "world space" so we translate the vertices into the transformation matrix with scaling being applied first and lastly translations (see render_sprite.cpp for the actual source code)
	
		within this boolean variable, we check using boolean operators to see if the two IN_GAME_OBJ parameters/arguments's AABBs collide with each other
	*/

	// x axis of both IN_GAME_OBJs are greater than or equal to each other, we have an x axis collision
	
	bool axis_aligned_bounding_box_x_axis_collision = first_in_game_obj_argument.game_object_position.x + first_in_game_obj_argument.game_object_scale_size.x >= second_in_game_obj_argument.game_object_position.x && second_in_game_obj_argument.game_object_position.x + second_in_game_obj_argument.game_object_scale_size.x >= first_in_game_obj_argument.game_object_position.x;

	// same concept applies for the y axis but now we use y coordiantes

	bool axis_aligned_bounding_box_y_axis_collision = first_in_game_obj_argument.game_object_position.y + first_in_game_obj_argument.game_object_scale_size.y >= second_in_game_obj_argument.game_object_position.y && second_in_game_obj_argument.game_object_position.y + second_in_game_obj_argument.game_object_scale_size.y >= first_in_game_obj_argument.game_object_position.y;

	// return both axes collisions only if both variables are true (a collision has occured on both axes)
	
	return axis_aligned_bounding_box_x_axis_collision && axis_aligned_bounding_box_y_axis_collision;

}

// OVERLOADED VERSION OF THE PRIOR FUNCTION THAT CHECKS FOR CIRCLE AABB COLLISION DETECTION
// we use a GAME_BALL_OBJ for our our first argument/parameter instead of a IN_GAME_OBJ
bool Axis_Aligned_Bounding_Box_Collision_Check(GAME_BALL_OBJ &game_ball_obj_argument, IN_GAME_OBJ &second_in_game_obj_argument)
{
	// some prior knowledge, 
	// Radius: the distance from the center of a circle to any point on its circumference (the outer parrt of a circle), we defined this earlier within the GAME_BALL_OBJ data member ball_radius 
	// Center: a point exactly in the center of a circle where all radii (plural of radius) meet
	// glm::clamp: glm::clamp will clamp a value (or values for GLM vectors) within a provided given range (i.e. int x = glm::clamp(2, -10, 10) = 2 (the value 2 is in range of -10-10), int y = glm::clamp(20, -10, 10) = 10 (the value 20 is not in range of -10-10 so it gets "clamped" down to that range which would be 10))
	// 
	// get the center point of the GAME_BALL_OBJ by taking the sum of the GAME_BALL_OBJ's position 2-value GLM vector data member and the GAME_BALL_OBJ's float value radius data member; store this result in a 2-value GLM vector
	// both the x and y value of GAME_BALL_OBJ's game_object_position data member are added by the GAME_BALL_OBJ's ball_radius data member value (i.e. (x + ball_radius, y + ball_radius) = ball_center_point glm 2-value vector)
	glm::vec2 ball_center_point(game_ball_obj_argument.game_object_position + game_ball_obj_argument.ball_radius);
	
	// calculate the half-extents of the IN_GAME_OBJ's AABB; this are the distances between the AABB's center as well as its edges
	// to get these results we divide each of the IN_GAME_OBJs game_object_scale_size data member's x and y values by 2
	glm::vec2 axis_aligned_bounding_box_half_extents(second_in_game_obj_argument.game_object_scale_size.x / 2.0, second_in_game_obj_argument.game_object_scale_size.y / 2.0);

	// calculate the IN_GAME_OBJ's AABB's center point (same idea as the GAME_BALL_OBJ's center point) in which we add the relative IN_GAME_OBJ's positional coordinate value with the relative axis_aligned_bounding_box_half_extents's coordinate value
	glm::vec2 axis_aligned_bounding_box_center_point(second_in_game_obj_argument.game_object_position.x + axis_aligned_bounding_box_half_extents.x, second_in_game_obj_argument.game_object_position.y + axis_aligned_bounding_box_half_extents.y);

	// get the difference (result of a subtraction equation) between the ball_center_point and the axis_aligned_bounding_box_center_point
	glm::vec2 difference_between_ball_center_point_and_aabb_center_point = ball_center_point - axis_aligned_bounding_box_center_point;

	// now we clamp that difference variable between the negative axis_aligned_bounding_box_center_point values and axis_aligned_bounding_box_center_point values
	glm::vec2 clamp_difference_between_ball_center_point_and_aabb_center_point_to_half_extents = glm::clamp(difference_between_ball_center_point_and_aabb_center_point, -axis_aligned_bounding_box_half_extents, axis_aligned_bounding_box_half_extents);

	// add this to the axis_aligned_bounding_box_center_point to get the closest point from the AABB and the ball
	glm::vec2 closest_point_between_aabb_and_ball = axis_aligned_bounding_box_center_point + clamp_difference_between_ball_center_point_and_aabb_center_point_to_half_extents;

	// now get the difference between closest_point_between_aabb_and_ball and the ball_center_point to determine if the length of this 2-value GLM vector is less then the GAME_BALL_OBJ's radius (thus a collision happens)
	glm::vec2 difference_between_closest_point_between_aabb_and_ball_and_ball_center_point = closest_point_between_aabb_and_ball - ball_center_point;

	// use glm::length (which measures the length/magnitude of a glm vector) to see if this value is less than the GAME_BALL_OBJ's radius this will return a boolan value that if true a collision has occurred, if not, a collision has not occured
	return glm::length(difference_between_closest_point_between_aabb_and_ball_and_ball_center_point) < game_ball_obj_argument.ball_radius;



}