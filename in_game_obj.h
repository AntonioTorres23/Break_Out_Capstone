#ifndef IN_GAME_OBJ_HEADER
#define IN_GAME_OBJ_HEADER

#include <glad/glad.h> //include GLAD; a lib that loads the addresses of OpenGL function pointers
#include <glm/glm.hpp> // include glm for uniform vector/matrix sets

// include proccess_texture header to access the TEXTURE_2D_OBJ class 
#include "process_texture.h"
// include render_sprite header to access the RENDER_SPRITE_OBJ class
#include "render_sprite.h"

// a container object that holds all data members necessary for a singular in game object
class IN_GAME_OBJ
{
public: // all of our public data/function members
	
	// a 2-value vector that stores the position, scale size, and physics velocity
	glm::vec2 game_object_position, game_object_scale_size, game_object_physics_velocity;

	// a 3-value vector that contains the color of our sprite
	glm::vec3 game_object_color; 

	// a float value that contains the rotation of the object in degrees
	float game_object_rotation;

	// boolean value to see if a game object is solid or not
	bool game_object_solid; 

	// a boolean value to see if a game object is destroyed or not
	bool game_object_destroyed;

	// a TEXTURE_2D_OBJ which holds the texture of the sprite
	TEXTURE_2D_OBJ sprite_texture; 

	// constructor value with no parameters/arguments
	IN_GAME_OBJ();

	// constructor value that requires a game_object_position 3-value vector, a 2-value game_object_scale_size vector, a TEXTURE_2D_OBJ object, a 3-value game_object_color vector, and a 2-value game_object_physics_velocity vector
	// we can see that these required parameters relate to the public data members stored within IN_GAME_OBJ
	// the game_object_color parameter if not filled, will take all 3 coordinates as 1.0f (aka a white color)
	// same idea applies to the game_object_physics_velocity parameter if not filled, it will 2 coordinates as 0.0f
	IN_GAME_OBJ(glm::vec2 game_object_position_argument, glm::vec2 game_object_scale_size_argument, TEXTURE_2D_OBJ sprite_texture_argument, glm::vec3 game_object_color_argument = glm::vec3(1.0f), glm::vec2 game_object_physics_velocity_argument = glm::vec2(0.0f, 0.0f));

	// a virtual void member function that draws the in game object with a RENDER_SPRITE_OBJ reference
	// A VIRTUAL FUNCTION IS A MEMBER FUNCTION THAT WITHIN A BASE CLASS CAN BE OVERRIDDEN WITHIN DERIVED CLASSES; MEANING THAT WHEN ANOTHER CLASS IMPORTS THE DATA/FUNCTION MEMBERS OF 
	// THE BASE CLASS, IT CAN TAKE THE SAME MEMBER FUNCTION AND CHANGE ITS BEHAVIOR WITHIN THE NEW CLASS
	
	// THIS PRIMARILY APPLIES WITH THINGS LIKE POINTERS; FOR EXAMPLE, YOU HAVE A POINTER OBJECT OF THE ORIGINAL BASE CLASS THAT IS POINTING TO THE ADDRESS OF AN OBJECT OF THE DERIVED CLASS
	// WITHIN THIS DERIVED CLASS, YOU CHANGE A MEMBER FUNCTION OF THE SAME NAME FROM THE BASE CLASS TO DO SOMETHING DIFFERENT. WHEN YOU USE THAT POINTER AND CALL THAT FUNCTION, IT WILL
	// NOT GIVE YOU THE CHANGED RESULT THAT WAS DONE WITHIN THE MEMBER FUNCTION OF THE DERIVED CLASS. THIS IS BECAUSE THE POINTER OBJECT IS STILL USING THE BASE OBJECT AS A CLASS TEMPLATE
	// SO IT WILL USE THE BASE CLASS'S VERSION OF THE MEMBER FUNCTION. HOWEVER, WHEN USING THE VIRTUAL KEYWORD IN THE BASE CLASSES'S MEMBER FUNCTION, TRY THE CONCEPT DESCRIBED PRIOR AND YOU
	// WILL SEE THAT THE POINTER OBJECT OF THE ORIGINAL BASE CLASS NOW DISPLAYS THE CHANGED MEMBER FUNCTION WITHIN THE DERIVED CLASS

	// ALSO NOTE WHENEVER YOU ARE CHANGING THE SOURCE CODE OF A MEMBER FUNCTION WITHIN THE DERIVED CLASS, TO USE THE OVERRIDE KEYWORD TO CORRECTLY DENOTE THAT THERE HAS BEEN A CHANGE TO THE MEMBER FUNCTION

	virtual void Game_Object_Draw(RENDER_SPRITE_OBJ& render_sprite_obj_argument);
};

#endif // !IN_GAME_OBJ_HEADER

