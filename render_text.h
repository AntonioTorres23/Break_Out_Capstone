#ifndef RENDER_TEXT_HEADER
#define RENDER_TEXT_HEADER

#include <map> // built-in C++ library that allows us to map two seperate variables to each other, think of it like a dictionary in python

#include <glad/glad.h> // include glad to access OpenGL pointer functions
#include <glm/glm.hpp> // include glm, a library that allows for us to create OpenGL vectors, matrices, etc. 

#include "process_texture.h" // include our process texture header file
#include "process_shader.h"  // include our process shader header file

// a structure that stores various data members for a single character
struct Single_Character
{
	// texture ID of a single character known as a glyph
	unsigned int Glyph_Texture_ID;
	// a integer 2-value glm vector data member that stores the size of a glyph
	glm::ivec2 Glyph_Size;
	// a integer 2-value glm vector data member that sets the bearing of a single glyph
	// a bearing is the offset of the baseline (blank space where the glyph kind of starts) to where
	// the glyph actually is (position where the actual character resides)
	glm::ivec2 Glyph_Bearing; 
	// a unsigned int data member that stores the advance of the glyph
	// this is basically a horizontal (x-axis) offset from the basline to the next glyph/character
	unsigned int Glyph_Advance;
};

// RENDER_TEXT_OBJ is a class that renders text on screen using a library called FreeType
class RENDER_TEXT_OBJ
{
public: // all public data/function members
	
	// a map public data member that will store multiple pre-compiled characters by using a the character data type and Single_Character data structure
	std::map<char, Single_Character> Multiple_Characters; 
	// a SHADER_OBJ public data member that will store the shaders for rendering text on screen
	SHADER_OBJ Render_Text_Shader; 
	// constructor that takes the width and height of our game screen
	RENDER_TEXT_OBJ(unsigned int width_of_screen_argument, unsigned int height_of_screen_argument);
	
	// void public member function that will grab a specific font that is specified within the string argument/parameter and the size of the font within the unsigned int argument/parameter
	// this will generate a list of characters that will be sent to the Multiple_Characters data member which will be ready to render whenever
	void Text_Generate(std::string type_of_font, unsigned int size_of_font);

	// void public member function that will take a string of text to render on string, the x and y coordiantes where then text should be placed, and a 3-value GLM vector that will be set to 1 
	// for all coordiante values (a white color) if no argument is provided
	// this function will take characters within Multiple_Characters, generate the statement provided by the string argument/parameter, and render it on screen as text
	void Text_Render(std::string text_to_be_rendered_on_screen, float text_x_position, float text_y_position, float text_scale_size, glm::vec3 text_color = glm::vec3(1.0));

private: // all private data/function members
	// 2 private data members that represent a Vertex Array Object and a Vertex Buffer Object
	unsigned int Vertex_Array_Object, Vertex_Buffer_Object; 
};


#endif // !RENDER_TEXT_HEADER

