#include "in_game_obj.h" // import in_game_obj header file to access the IN_GAME_OBJ class template

/*
NOTICE HOW WE'RE DEFINING THE FUNCTIONS THAT ARE STORED WITHIN THE IN_GAME_OBJ CLASS, SO IN THE C++ FILE HERE WE'RE DEFINING THE ACTUAL SOURCE CODE
THE FUNCTIONS THAT ARE STORED IN THE HEADER FILE ARE REALLY ONLY PROTOTYPES
*/

// define the IN_GAME_OBJ constructor that does not contain parameters/arguments
// we use a contstructor member initalizer list because it allows us to set the values of data members prior to the body of the constructor executing
IN_GAME_OBJ::IN_GAME_OBJ() : game_object_position(0.0f, 0.0f), game_object_scale_size(1.0f, 1.0f), game_object_physics_velocity(0.0f), game_object_color(1.0f), game_object_rotation(0.0f), sprite_texture(), game_object_solid(false), game_object_destroyed(false)
{

}

// define the IN_GAME_OBJ constructor that DOES contain parameters/arguments
// we use a contstructor member initalizer list because it allows us to set the values of data members prior to the body of the constructor executing
// NOTICE HOW IN THIS CONSTRUCTOR MEMBER INITALIZER LIST WE ASSIGN THE CORRESPONDING PARAMETERS/ARGUMENTS TO THE ASSOCIATED MEMBERS
IN_GAME_OBJ::IN_GAME_OBJ(glm::vec2 game_object_position_argument, glm::vec2 game_object_scale_size_argument, TEXTURE_2D_OBJ sprite_texture_argument, glm::vec3 game_object_color_argument, glm::vec2 game_object_physics_velocity_argument) : game_object_position(game_object_position_argument), game_object_scale_size(game_object_scale_size_argument), game_object_physics_velocity(game_object_physics_velocity_argument), game_object_color(game_object_color_argument), game_object_rotation(0.0f), sprite_texture(sprite_texture_argument), game_object_solid(false), game_object_destroyed(false)
{

}

// define the Game_Object_Draw public method function
// DON'T NEED TO SPECIFY IT AS A VIRTUAL FUNCTION HERE SINCE WE ARE ONLY DEFINING THE FUNCTION WITHIN THIS C++ FILE
void IN_GAME_OBJ::Game_Object_Draw(RENDER_SPRITE_OBJ& render_sprite_obj_argument)
{
	// call the method function that draws the sprite from the parameter/argument and enter the data methods stored within the IN_GAME_OBJECT to be the parameter/argument for our Render_and_Draw_Sprite method function
	render_sprite_obj_argument.Render_and_Draw_Spirte(this->sprite_texture, this->game_object_position, this->game_object_scale_size, this->game_object_rotation, this->game_object_color);
}