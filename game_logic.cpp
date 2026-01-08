// game_logic c++ file

#include "game_logic.h" // include game_logic header file that contains class template
#include "resource_manager.h" // include resource_manager header file to access the static functions within the game_logic c++ file
#include "render_sprite.h" // include render_sprite header file to allow for RENDER_SPRITE_OBJ to be created in the game_logic c++ file

/*
NOTICE HOW WE'RE DEFINING THE FUNCTIONS THAT ARE STORED WITHIN THE GAME_OBJ CLASS/OBJECT, SO IN THE C++ FILE HERE WE'RE DEFINING THE ACTUAL SOURCE CODE
THE FUNCTIONS THAT ARE STORED IN THE HEADER FILE ARE REALLY ONLY PROTOTYPES
*/

// create a RENDER_SPRITE_OBJ non-contructed pointer object; this is just a place holder since we did not specify construction parameters to begin running the body of the constructor  
RENDER_SPRITE_OBJ *Sprite_Render; 

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

	// load a texture which will represent our sprite with the resource manager static function Texture_Load
	RESOURCE_MANAGER::Texture_Load("Resources/Textures/dog_smile2.png", true, "dog_smile2");


}

// game update of player movement and ball location function definition
void GAME_OBJ::Update_Game(float delta_time)
{

}

// process player input function definition
void GAME_OBJ::Process_User_Input(float delta_time)
{

}

// render game onto player screen function definition
void GAME_OBJ::Render_Game()
{
	// create a temporary texture object that stores the same texture but within an l-value object
	TEXTURE_2D_OBJ texture_object_for_sprite_rendering;

	// now get that same texture object via the static function Texture_Get and store it in the texture object we just created  
	texture_object_for_sprite_rendering = RESOURCE_MANAGER::Texture_Get("dog_smile2");

	// within the Render_and_Draw_Sprite method function, we specify our texture image created earlier with the texture_object_for_sprite_rendering

	/*

	  the reason why we have to do this has to do with the compiler and that the RENDER_SPRITE_OBJ take a non constant reference to the shader. Our Shader_Get static function from the resource manager returns the shader by value
	  via returning the mapped string associated with the shader object, which is a temporary object (aka an r value) that only builds the shader object then destroys it which does not let us keep a reference to it. Which is why
	  we need to create another l-value object that is stores the shader program in memory and so the contructor can grab the address.
	*/

	// Draw and Render our sprite on screen via the function Render_and_Draw_Spirte function
	Sprite_Render->Render_and_Draw_Spirte(texture_object_for_sprite_rendering, glm::vec2(350.0f,500.0f), glm::vec2(100.0f, 100.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
}