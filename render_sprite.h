#ifndef RENDER_SPRITE_HEADER
#define RENDER_SPRITE_HEADER

#include <glad/glad.h> // include glad to access OpenGL pointer functions
#include <glm/glm.hpp> // include glm, a library that allows for us to create OpenGL vectors, matrices, etc. 
#include <glm/gtc/matrix_transform.hpp> // another glm feature that allows us to transform matrices such as rotation, scaling, translation

// include our process_texture header file to use the TEXTURE_2D_OBJ type within this function
#include "process_texture.h"
// include our process_shader header file to use the SHADER_OBJ type within this function
#include "process_shader.h"

// class template for our render sprite object
class RENDER_SPRITE_OBJ
{
public: // all of our public data/function members
	// constructor for RENDER_SPRITE_OBJ which requires an address of a shader object as its parameter 
	RENDER_SPRITE_OBJ(SHADER_OBJ &shader_object_argument);

	// deconstructor for RENDER_SPRITE_OBJ
	~RENDER_SPRITE_OBJ();

	/* 
	void function that draws / renders a sprite on screen, takes a TEXTURE_2D_OBJ's address, a 2-value vector that represents its position in the game environment, 
	a scale size 2-value vector which by default is set to 10.0 on both x and y, a float value that represents the amount of rotation of the sprite in degrees which
	by default is set to 0.0, and a 3-value vector that represents a color to set the sprite as which by default is 1.0 on r, g, and b (white)
	*/
	void Render_and_Draw_Spirte(TEXTURE_2D_OBJ& texture_object_argument, glm::vec2 position_of_sprite_argument, glm::vec2 scale_size_argument = glm::vec2(10.0f, 10.0f), float rotation_degree_argument = 0.0f, glm::vec3 sprite_color_argument = glm::vec3(1.0));


private: // all of our private data/function members
	// private SHADER_OBJ data member
	SHADER_OBJ sprite_shader_object;
	// private vertex array object data member for our sprite
	unsigned int sprite_vertex_array_object;
	// private void function that sets up the sprite's vertex buffer object and vertex pointer attributes
	void vertex_data_intialize();
};

#endif // !RENDER_SPRITE

