// game_logic c++ file

#include "game_logic.h" // include game_logic header file that contains class template
#include "resource_manager.h" // include resource_manager header file to access the static functions within the game_logic c++ file
#include "render_sprite.h" // include render_sprite header file to allow for RENDER_SPRITE_OBJ to be created in the game_logic c++ file
#include "in_game_obj.h"   // include in_game_obj header file to allow for IN_GAME_OBJ to be created in the game_logic c++ file
#include "game_ball.h"    // include game_ball header file to allow for GAME_BALL_OBJ to be created in the game_logic c++ file
#include "generate_particles.h" // include generate_particles header file to allow for GEN_PARTICLES_OBJ to be created in the game_logic c++ file
#include "post_processing.h" // include post_processing header file to allow for POST_PROCESSING_OBJ to be created in the game_logic c++ file

#include <iostream>

// variable only relevant to this c++ file that will store the time that the screen will shake upon a collision with a solid tile/brick/block
float time_screen_will_shake = 0.0f;


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
// create a GEN_PARTICLES_OBJ non-contructed pointer object that will represent the breakout ball
GEN_PARTICLES_OBJ *Generate_Particles;
// create a POST_PROCESSING_OBJ non-contructed pointer object that will represent the post processing object
POST_PROCESSING_OBJ *Post_Processing_Object;

// PROTOTYPE FUNCTION DECLARATIONS for Axis_Aligned_Bounding_Box_Collision_Check function only relevant to this C++ file
bool Axis_Aligned_Bounding_Box_Collision_Check(IN_GAME_OBJ &first_in_game_obj_argument, IN_GAME_OBJ &second_in_game_obj_argument);

// PROTOTYPE FUNCTION DECLARATIONS for Axis_Aligned_Bounding_Box_Collision_Check (OVERLOADED GAME_BALL_OBJ VERSION OF THIS FUNCTION FOR AABB - CIRCLE COLLISION CHECK) function only relevant to this C++ file
BallCollision Axis_Aligned_Bounding_Box_Collision_Check(GAME_BALL_OBJ &game_ball_obj_argument, IN_GAME_OBJ &second_in_game_obj_argument);

// PROTOTYPE FUNCTION DECLARATION for Ball_Bounce_Direction_GLM_Vector function only relevant to this C++ file
BallBounceDirection Ball_Bounce_Direction_GLM_Vector(glm::vec2 glm_2_value_vector_argument);

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
	delete Generate_Particles;
	delete Post_Processing_Object;
}

// game initazlier function definition
void GAME_OBJ::Initalize_Game()
{
	// load the shaders provided with the static function shader_load from the resource_manager header file
	RESOURCE_MANAGER::Shader_Load("Resources/Shaders/sprite_test.vert", "Resources/Shaders/sprite_test.frag", nullptr, "sprite_test");
	RESOURCE_MANAGER::Shader_Load("Resources/Shaders/particle_shader.vert", "Resources/Shaders/particle_shader.frag", nullptr, "particle_shader");
	RESOURCE_MANAGER::Shader_Load("Resources/Shaders/post_processing.vert", "Resources/Shaders/post_processing.frag", nullptr, "post_processing_shader");
	//RESOURCE_MANAGER::Shader_Load("Resources/Shaders/test_fbo.vert", "Resources/Shaders/test_fbo.frag", nullptr, "post_processing_shader");
	// set a orthographic projection matrix to the dimensions of the screen from our related public data members statically casted to a float value and a near distance of -1 and a far distance of 1 which are Normalized Device Coordinates
	glm::mat4 sprite_orthographic_projection_matrix = glm::ortho(0.0f, static_cast<float>(this->Width_Of_Screen), static_cast<float>(this->Height_Of_Screen), 0.0f, -1.0f, 0.0f);
	// get the shader via the mapped_shader name with the resource manager static function Shader_Get and both activate the shader/set the sampler2D uniform variable to GL_ACTIVETEXTURE0
	RESOURCE_MANAGER::Shader_Get("sprite_test").Activate().uniform_integer("texture_image", 0);
	// now set the sprite_orthographic_projection_matrix we defined here to be sent to the fragment shader uniform variable of the same name
	RESOURCE_MANAGER::Shader_Get("sprite_test").uniform_matrix_4("sprite_orthographic_projection_matrix", sprite_orthographic_projection_matrix);
	
	RESOURCE_MANAGER::Shader_Get("particle_shader").Activate().uniform_integer("texture_image", 0);
	RESOURCE_MANAGER::Shader_Get("particle_shader").uniform_matrix_4("sprite_orthographic_projection_matrix", sprite_orthographic_projection_matrix);

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
	RESOURCE_MANAGER::Texture_Load("Resources/Textures/particle.png", true, "particle_object");
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

	// now with our particle_generator pointer object we created earlier, dynamically allocate memory from the heap with the new keyword to return an address of the GEN_PARTICLES_OBJ constructor object to the particle_generator pointer object
	Generate_Particles = new GEN_PARTICLES_OBJ(RESOURCE_MANAGER::Shader_Get("particle_shader"), RESOURCE_MANAGER::Texture_Get("particle_object"), 500);

	// now with our post_processing pointer object we created earlier, dynamically allocate memory from the heap with the new keyword to return an address of the GEN_PARTICLES_OBJ constructor object to the particle_generator pointer object
	Post_Processing_Object = new POST_PROCESSING_OBJ(RESOURCE_MANAGER::Shader_Get("post_processing_shader"), this->Width_Of_Screen, this->Height_Of_Screen);
}

// game update of player movement and ball location function definition
void GAME_OBJ::Update_Game(float delta_time)
{
	// call the member function Ball_Movement to update the location of the ball and include the delta_time argument/parameter and the Width_Of_Screen data member stored within the GAME_OBJ
	Game_Ball->Ball_Movement(delta_time, this->Width_Of_Screen);

	// call the member function Axis_Aligned_Bounding_Box_Collisions to update collisions with the ball and tiles/blocks/bricks in the game; if a collision happens and the block is destroyable, destroy the block
	this->Axis_Aligned_Bounding_Box_Collisions();

	// update the amount of particles on screen and their life, date the delta time, pointer value of the Ball as our game object, the number of new particles to be created, and an offset of half the ball's radius
	Generate_Particles->Particles_Update(delta_time, *Game_Ball, 2, glm::vec2(Game_Ball->ball_radius / 2.0f));

	// if time_screen_will_shake is greater than 0 (meaning that the ball has had a collision with a solid tile/block/brick), then the screen is shaking
	if (time_screen_will_shake > 0.0f)
	{
		// use compound subtraction assignment with delta time to reduce the value within time_screen_will_shake
		// this will reduce the value within time_screen_will_shake until it is less than or equal to 0 (thus not making the screen shake anymore)
		time_screen_will_shake -= delta_time;
		// if time_screen_will_shake is less than or equal to 0, then set the Screen_Shake_Effect public boolean data member to false
		if (time_screen_will_shake <= 0.0f)
			Post_Processing_Object->Screen_Shake_Effect = false;
	}

	// if the ball's y positional value is greater than or equal to the screen's dimensions, the player has lost and restart the level
	if (Game_Ball->game_object_position.y >= this->Height_Of_Screen)
	{
		// call the related member functions that reset the level and player
		this->Level_Reset();
		this->Player_Reset();
	}
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
			// first set up the post processing effects with the related public member function
			Post_Processing_Object->Start_Render();

				// DRAW AND RENDER THE BACKGROUND
				// Draw and Render our sprite on screen via the function Render_and_Draw_Spirte function
				Sprite_Render->Render_and_Draw_Spirte(RESOURCE_MANAGER::Texture_Get("game_background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width_Of_Screen, this->Height_Of_Screen), 0.0f);

				// NOW DRAW AND RENDER THE TILES/BLOCKS/BRICKS IN THE GAME_LEVELS member standard lib vector
				// the Game_Level member is like an index and stores what level we want to load so in this case index 0 stores level one so we will draw and render level one
				this->Game_Levels[this->Game_Level].Level_Render_and_Draw(*Sprite_Render);

				Player_Object->Game_Object_Draw(*Sprite_Render);

				// NOW DRAW AND RENDER PARTICLES BEFORE DRAWING BALL
				Generate_Particles->Render_And_Draw_Particles();

				// NOW DRAW AND RENDER THE BALL
			
				Game_Ball->Game_Object_Draw(*Sprite_Render);

			// finish post processing effects with the related public member function, this sets up the framebuffers and gets the post processing effects ready for rendering
			Post_Processing_Object->Finish_Render();
			// render the screen-filled quad with the framebuffer color buffer texture attachment, include glfwGetTime as the post_processing_time
			Post_Processing_Object->Render_Post_Processing(glfwGetTime());
	}


}

// level reset function definition
void GAME_OBJ::Level_Reset()
{
	// simply load the level that is related to the provided level index using the provided members of GAME_OBJ and if else statements
	if (this->Game_Level == 0)
		this->Game_Levels[this->Game_Level].Level_Load("Resources/Levels/standard_level.level", this->Width_Of_Screen, this->Height_Of_Screen / 2);
	else if (this->Game_Level == 1)
		this->Game_Levels[this->Game_Level].Level_Load("Resources/Levels/gaps_level.level", this->Width_Of_Screen, this->Height_Of_Screen / 2);
	else if (this->Game_Level == 2)
		this->Game_Levels[this->Game_Level].Level_Load("Resources/Levels/space_invader_level.level", this->Width_Of_Screen, this->Height_Of_Screen / 2);
	else if (this->Game_Level == 3)
		this->Game_Levels[this->Game_Level].Level_Load("Resources/Levels/bounce_level.level", this->Width_Of_Screen, this->Height_Of_Screen / 2);
}

// player reset function definition
void GAME_OBJ::Player_Reset()
{
	// set the player's scale size to the original header constant variable SCALE_SIZE_OF_PLAYER like in the constructor
	Player_Object->game_object_scale_size = SCALE_SIZE_OF_PLAYER;

	// now set the position of the player to the same values we used within the constructor as well 
	Player_Object->game_object_position = glm::vec2(this->Width_Of_Screen / 2.0f - SCALE_SIZE_OF_PLAYER.x / 2.0f, this->Height_Of_Screen - SCALE_SIZE_OF_PLAYER.y);

	// lastly reset the ball to its original position which is stuck to the player which is similar to the inital construction of the ball as well
	// however this time we use the player's position stored within its data member as well as the BALL_OBJECT_PHYSICS_VELOCITY constant variable as arguments/parameters
	Game_Ball->Ball_Reset(Player_Object->game_object_position + glm::vec2(SCALE_SIZE_OF_PLAYER.x / 2.0f - BALL_OBJECT_RADIUS, -BALL_OBJECT_RADIUS * 2.0f), BALL_OBJECT_PHYSICS_VELOCITY);
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
			// call and store this function to determine there is a collision detected between the Game_Ball and brick iterator's AABBs, remember to make the game ball have a value pointer to get the actual Ball_Object and not just the address
			BallCollision ball_and_brick_collision_tuple = Axis_Aligned_Bounding_Box_Collision_Check(*Game_Ball, brick_game_object_within_game_level_iterator);
			
			// get first value from ball_and_brick_collision_tuple to see if a collision has occured using the built-in function std::get and then the index position (which in our case is 0 (first value in tuple)
			if (std::get<0>(ball_and_brick_collision_tuple))
			{
				// if the tile/brick/block isn't already destroyed
				//if (!brick_game_object_within_game_level_iterator.game_object_destroyed)
					
				// if tile/brick/block isn't solid, destroy it
				if (!brick_game_object_within_game_level_iterator.game_object_solid)

					brick_game_object_within_game_level_iterator.game_object_destroyed = true;
				// if tile/brick/brick is solid make screen shake	
				else
				{
					// set the time_screen_will_shake variable to a value of 0.05; this will shake the screen for 0.05 seconds once a collision has occured within a solid tile/brick/block
					time_screen_will_shake = 0.05f;
					// set the Screen_Shake_Effect boolean public data member to true
					Post_Processing_Object->Screen_Shake_Effect = true;
				}

				// grab the 2nd value in the ball_and_brick_collision_tuple which is the Ball Bounce Direction enumeration value from the Ball_Bounce_Direction_GLM_Vector within the Axis_Aligned_Bounding_Box_Collision_Check return value
				// again we use std::get and index at position 1 (second value in tuple) 
				BallBounceDirection ball_bounce_direction = std::get<1>(ball_and_brick_collision_tuple);
				// as well as get the ball collision difference 2-value GLM vector which is at position 2 within the tuple index using the same standard lib function
				glm::vec2 ball_collision_difference_vector = std::get<2>(ball_and_brick_collision_tuple);

				// check if there is a horizontal collision; meaning a if statment which if the ball_bounce_direction contains an enumeration value of DIRECTION_LEFT OR DIRECTION_RIGHT
				if (ball_bounce_direction == DIRECTION_LEFT || ball_bounce_direction == DIRECTION_RIGHT)
				{
					// reverse (negate) the ball velocity's x value therfore making it bounce off the tile/block/brick
					Game_Ball->game_object_physics_velocity.x = -Game_Ball->game_object_physics_velocity.x;

					// move the ball to just before it enters the brick using the ball_collision_difference_vector to make it look like the ball "actually bounced" off the tile/block/brick (we call this the penertration value)
					// we do this by taking the ball's raidus value and subtracting it by the absolute value of the ball_collision_difference_vector's x value
					// the standard lib function std::abs gets the absolute value of a variable or int, float, double, etc. data type
					// all an absolute value is the distance of a number from 0, for example |4| = 4 because its 4 to the right from zero, and |-6| = 6 because its 6 to the left from zero
					// basically, an absolute value makes a negative number positive and leaves a positive number uneffected

					float ball_penetration_value = Game_Ball->ball_radius - std::abs(ball_collision_difference_vector.x);
					
					// if the ball_bounce_direction enumeration value is equal to DIRECTION_LEFT (hit the left of the AABB), then we use addition compound assignment on the Ball position's x value with the ball penetration value
					// (move the ball to the right)
					if (ball_bounce_direction == DIRECTION_LEFT)
						Game_Ball->game_object_position.x += ball_penetration_value;
					// if not (ball_bounce direction hit the right of the AABB), then use the same logic as the prior if statment, but instead use subtraction compound assignment
					// (move the ball to the left)
					else
						Game_Ball->game_object_position.x -= ball_penetration_value;
				}
				// if there is no horizontal collision, then it is a vertical collision
				else
				{
					// same concept applies as the vertical collisions, but this time we use y values

					// reverse (negate) the ball velocity's y value therfore making it bounce off the tile/block/brick
					Game_Ball->game_object_physics_velocity.y = -Game_Ball->game_object_physics_velocity.y;

					// move the ball to just before it enters the brick using the ball_collision_difference_vector to make it look like the ball "actually bounced" off the tile/block/brick (we call this the penertration value)
					// we do this by taking the ball's raidus value and subtracting it by the absolute value of the ball_collision_difference_vector's y value

					float ball_penetration_value = Game_Ball->ball_radius - std::abs(ball_collision_difference_vector.y);

					// if the ball_bounce_direction is equal to DIRECTION_UP (hit the top of the AABB), then we use subtraction compound assignment on the Ball position's y value with the ball penetration value
					// I ACTUALLY DON'T KNOW WHY WE DO SUBTRACTION COMPOUND ASSIGNMENT TO MOVE UP; WHEN TO MOVE UP ON A Y AXIS YOU NEED POSITIVE COORDIANTES; BUT MY BEST GUESS IS THAT WHEN YOU USE THE ABSOLUTE VALUE
					// OF THE ball_collision_difference_vector, IF THE VALUE IS A NEGATIVE NUMBER, WHEN APPLIED WITH SUBTRACTION COMPOUND ASSIGNMENT WITH THE BALL POSITION'S Y VALUE IT MAKES A DOUBLE NEGATIVE WITH THE
					// SUBTRACTION COMPOUND ASSIGNMENT OPERATOR THEREFORE MAKING IT POSSITIVE? (JUST A GUESS)

					if (ball_bounce_direction == DIRECTION_UP)
						Game_Ball->game_object_position.y -= ball_penetration_value;

					// else statment means there is no other directions except for DIRECTION_DOWN; meaning the ball hit the bottom of the AABB, so use addition compound assignment on the Ball position's y value with the ball penetration value
					// I ACTUALLY DON'T KNOW WHY WE DO ADDITION COMPOUND ASSIGNMENT TO MOVE DOWN; WHEN TO MOVE DOWN ON A Y AXIS YOU NEED NEGATIVE COORDIANTES; BUT MY BEST GUESS IS THAT WHEN YOU USE THE ABSOLUTE VALUE
					// OF THE ball_collision_difference_vector, IF THE VALUE IS A NEGATIVE NUMBER, WHEN APPLIED WITH ADDITION COMPOUND ASSIGNMENT WITH THE BALL POSITION'S Y VALUE IT JUST LEAVES THE NEGATIVE VALUE AS NORMAL WITH THE
					// ADDITION COMPOUND ASSIGNMENT OPERATOR THEREFORE MAKING IT NEGATIVE? (JUST A GUESS)
					else
					{
						Game_Ball->game_object_position.y += ball_penetration_value;
					}

				}

			}

		}
	}

	// now determine if a collision has occured with the player_object to create the main gameplay aspect

	// call and store this function to determine there is a collision detected between the Game_Ball and Player_Object AABBs, remember to make the game ball and player have a value pointer to get the actual Ball_Object/IN_GAME_Object and not just the addresses
	BallCollision ball_and_player_collision_tuple = Axis_Aligned_Bounding_Box_Collision_Check(*Game_Ball, *Player_Object);

	// if statment that checks if the ball is not stuck to the player and there has been a collision
	// again using the std::get to index the data within ball_and_player_collision_tuple
	if (!Game_Ball->ball_stuck && std::get<0>(ball_and_player_collision_tuple))
	{
		// get the center value of the player's position by taking the sum of the player position x value and the player scale size x value and dividing it by 2 (essentially cutting it in half)
		float center_of_player = Player_Object->game_object_position.x + Player_Object->game_object_scale_size.x / 2.0f;
		// get the distance between the ball and the center of the player by taking the sum of the ball's positon and radius x values and subtracting that from the center of the player value
		float distance_between_ball_and_center_of_player = (Game_Ball->game_object_position.x + Game_Ball->ball_radius) - center_of_player;
		// create a percentage variable that will determine how much velocity will be applied to the ball depending on where the ball is bounced off
		// to do this we take the distance_between_ball_and_center_of_player and dividing that by half the player's x value scale size
		float player_velocity_percentage = distance_between_ball_and_center_of_player / (Player_Object->game_object_scale_size.x / 2.0f);
		// create a constant value that will dilute the amount of impact that the player_velocity_percentage will effect the velocity
		float constant_velocity_strength_value = 2.0f;
		// now store the original velocity values within a 2-value GLM vector to save for later
		glm::vec2 original_velocity_values = Game_Ball->game_object_physics_velocity;
		// now apply the prior variables to the Ball velocity's x value first, by taking the header constant variable BALL_OBJECT_PHYSICS_VELOCITY's x value that we used for the constructor of our ball object and multiplying
		// it by the player_velocity_percentage as well as the constant_velocity_strength_value
		Game_Ball->game_object_physics_velocity.x = BALL_OBJECT_PHYSICS_VELOCITY.x * player_velocity_percentage * constant_velocity_strength_value;

		// to prevent the ball from sticking inside the player, multiply  -1.0f by the absolute value of the ball velocity's y value
		// this makes sure we always return a positive y velocity value so if the ball gets stuck in the player it can escape
		// we can only do this to the player because ball always collides with the top of the player's AABB
		Game_Ball->game_object_physics_velocity.y = -1.0f * std::abs(Game_Ball->game_object_physics_velocity.y);

		// now within both values within the ball's velocity vector, normalize the current velocity vector (make the length/magnitude/homogenous coordinate (coordinate w) to a value of 1) and multiply it by 
		// the length of the original_velocity_values 2-value GLM vector
		Game_Ball->game_object_physics_velocity = glm::normalize(Game_Ball->game_object_physics_velocity) * glm::length(original_velocity_values);

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
BallCollision Axis_Aligned_Bounding_Box_Collision_Check(GAME_BALL_OBJ &game_ball_obj_argument, IN_GAME_OBJ &second_in_game_obj_argument)
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
	glm::vec2 difference_between_closest_point_between_aabb_and_ball_center_point = closest_point_between_aabb_and_ball - ball_center_point;

	// use glm::length (which measures the length/magnitude of a glm vector) to see if this value is less than the GAME_BALL_OBJ's radius this will return a tuple
	// of the boolan value that if true a collision has occurred, if not, a collision has not occured, the enum BallBounce Direction result of the Ball_Bounce_Direction_GLM_Vector function,
	// and the difference_between_closest_point_between_aabb_and_ball_center_point 2-value GLM vector from this function
	if (glm::length(difference_between_closest_point_between_aabb_and_ball_center_point) < game_ball_obj_argument.ball_radius)
		// use the standard built-in function std::make_tuple to return a BallCollision datatype
		return std::make_tuple(true, Ball_Bounce_Direction_GLM_Vector(difference_between_closest_point_between_aabb_and_ball_center_point), difference_between_closest_point_between_aabb_and_ball_center_point);

	// else, return a tuple that has the values false, the UP enumeration value (just a place holder), and a 2-value GLM vector with x and y coorrdinates of 0.0
	else
		return std::make_tuple(false, DIRECTION_UP, glm::vec2(0.0f, 0.0f));
		
}

// define a enumeration function that is only relevant to this C++ file that calculates the direction that the ball hits 
// for example, we want the ball's x velocity coordinate value to be reversed if a collision happens on the sides of the AABB
// as well as we want the ball's y velocity coordinate value to be reversed if a collision happens on the top or bottom of the AABB
/* 

we get this by using the dot product (multiplying two vectors by their respective coordinate (i.e. (x, y, z) * (x, y, z) = (x * x), (y * y), (z * z)) and then adding these results together
to get a float value making the full equation: (x, y, z) * (x, y, z) = (x * x) +  (y * y) + (z * z) = float result)

we use the argument/parameter of this function to get a dot product value between this 2-value GLM vector and all 4 "game (aka world) directional position" vectors that point up, down, left, and right

once that value is generated it is compared by a maxiumum value (degree) and if that value is greater than the maximum value, that dot product value is now the new maximum value and
we return the iterator which indexes the direction enumeration at where the ball bounced. 
*/ 
BallBounceDirection Ball_Bounce_Direction_GLM_Vector(glm::vec2 glm_2_value_vector_argument)
{
	// create a 2-value GLM vector array that stores the "game position directional vectors"
	glm::vec2 game_position_directional_vectors[]
	{
		glm::vec2(0.0f, 1.0f),  // vector pointing up
		glm::vec2(1.0f, 0.0f),  // vector pointing right
		glm::vec2(0.0f, -1.0f), // vector pointing down
		glm::vec2(-1.0f, 0.0f)  // vector pointing right
	};

	// create a float variable of 0.0f
	float maxiumum_value = 0.0f;

	// create an unsigned int variable of the value -1 which is good practice becuase unsigned int variables cannot be negative so this prevents weird from happening with this variable
	// this will represent way for us to index the direction enumeration that bests fits our dot product value

	unsigned int direction_enumeration_that_best_matches_ball_direction_upon_collision_with_aabb = -1;

	// for loop iterator that will loop through all dot products between our function argument/parameter and the game_position_directional_vectors array
	for (unsigned int iterator_for_dot_products_and_direction_enumerations = 0; iterator_for_dot_products_and_direction_enumerations < 4; iterator_for_dot_products_and_direction_enumerations++)
	{
		// calculate our dot products with the GLM function and index our game_position_directional_vectors with the iterator
		// don't forget to normalize the argument/parameter 2-value vector to ensure its length/magnitude/(homogenous value w) is a length of 1
		float value_of_dot_product = glm::dot(glm::normalize(glm_2_value_vector_argument), game_position_directional_vectors[iterator_for_dot_products_and_direction_enumerations]);
	
		// if statment if the dot product value is greater than the maximum_value
		if (value_of_dot_product > maxiumum_value)
		{
			// current dot product value is now the maximum value
			maxiumum_value = maxiumum_value;

			// use the iterator again to index for the enumeration return value which direction the ball is at
			direction_enumeration_that_best_matches_ball_direction_upon_collision_with_aabb = iterator_for_dot_products_and_direction_enumerations;
		}
	}

	// after the for loop is finished, return the BallBounceDirection that best matches where the ball direction is upon collision with the direction_enumeration_that_best_matches_ball_direction_upon_collision_with_aabb as the index value
	return (BallBounceDirection)direction_enumeration_that_best_matches_ball_direction_upon_collision_with_aabb;
}