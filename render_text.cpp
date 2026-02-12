#include <iostream> // include iostream to send default output

#include <glm/gtc/type_ptr.hpp> // enable type pointers for glm vectors/matrices and such
#include <ft2build.h> // include this header file to initialize and setup freetype library; think of it as an entry way to be able to use the freetype library
#include FT_FREETYPE_H // include this preprocessor macro/variable to access the actual library you want to use within freetype, which in this case is the freetype header


#include "render_text.h" // include render_text header file that contains class template

#include "resource_manager.h" // include resource_manager header file to access the static functions within the render_text c++ file


// RENDER_TEXT_OBJ constructor definition
RENDER_TEXT_OBJ::RENDER_TEXT_OBJ(unsigned int width_of_screen_argument, unsigned int height_of_screen_argument)
{
	// load the shader within the constructor with the resource manager static function and providing the related shader files
	// this will be stored within the Render_Text_Shader public data member
	this->Render_Text_Shader = RESOURCE_MANAGER::Shader_Load("text.vert", "text.frag", nullptr, "text_shader");
	// set a orthographic projection matrix to the dimensions of the screen from our related arguments statically casted to a float value
	// notice how we don't provide near and far data for this projection matrix and I think this is because we want the text to appear in the foreground of the game
	this->Render_Text_Shader.uniform_matrix_4("text_orthographic_projection_matrix", glm::ortho(0.0f, static_cast<float>(width_of_screen_argument), static_cast<float>(height_of_screen_argument), 0.0f));
	// set the texture value for our text image within the fragment shader
	this->Render_Text_Shader.uniform_integer("texture_image", 0);
	// generate a vertex array object with our related private data member
	glGenVertexArrays(1, &this->Vertex_Array_Object);
	// generate a vertex buffer object with our related private data member
	glGenBuffers(1, &this->Vertex_Buffer_Object);
	// bind the vertex array object so that we can configure its attribute array pointers
	glBindVertexArray(this->Vertex_Array_Object);
	// bind the vertex buffer object so that we can configure it
	// we specify that it is a GL_ARRAY_BUFFER within the first parameter
	glBindBuffer(GL_ARRAY_BUFFER, this->Vertex_Buffer_Object);
	// here we configure our VBO by specifying the type of buffer, the size of the VBO, and the drawing method
	// GL_DYNAMIC_DRAW BASICALLY MEANS THAT THE DATA WE ARE USING WILL BE CHANGED MULTIPLE TIMES AND USED MANY TIMES
	// we use the C++ built-in sizeof function to get the byte size of our text data; here we don't specify an actual
	// array where the vertex data is stored but 6 times the size of float (a single float data value is 4 bytes) times 4 which equals 96 bytes
	// in addition we do not specify a location for where our vertex data is so we just leave it as a null value
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	// enable our first vertex (and only) array attribute index
	glEnableVertexAttribArray(0);
	// configure vertex arribute array 0 with the OpenGL function glVertexArribPointer
	/*
	   here we specify which attribute array index we want to configure, how many coordinates / float values to expect, what data type to expect which is float,
	   if we want the values to be normalized (clamped to range of -1 to 1 for signed values and 0 to 1 for unsigned values) or not in which we don't want,
	   the stride which is the byte offset per value will always be the amount of values per attribute times the sizeof a singular data type (which
	   in this case is float), and the offset which this time doesn't need the void pointer function seen in other classes
	 */
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	// unbind the vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// unbind the vertex array object
	glBindVertexArray(0);
}

// define the Text_Generate function
void RENDER_TEXT_OBJ::Text_Generate(std::string type_of_font, unsigned int size_of_font)
{
	// clear all of the characters that were stored in the Multiple_Characters public data member
	this->Multiple_Characters.clear();
	// create a free type library object to access its features
	FT_Library free_type_library_object;
	// if the FT_Init_FreeType function returns false or anything other than 0 (binary value for true) then send an error message to default output 
	if (FT_Init_FreeType(&free_type_library_object))
	{
		std::cout << "ERROR::FREE_TYPE::FAILED_TO_INITIALIZE_FREE_TYPE_LIBRARY" << std::endl; 
	}
	// load a font as a face object
	FT_Face font_object; 
	// if the font cannot be loaded with the FT_New_Face function then send an error message to default output
	// we use the type_of_font argument/parameter converted to a c string to deduce the type of font we want
	if (FT_New_Face(free_type_library_object, type_of_font.c_str(), 0, &font_object))
	{
		std::cout << "ERROR::FREE_TYPE::FAILED_TO_LOAD_FREE_TYPE_FONT" << std::endl;
	}

	// set the size of the characters with this function and the provided argument/parameter
	// we keep the pixel width set at 0
	FT_Set_Pixel_Sizes(font_object, 0, size_of_font);

	// we disable byte alignment which means we disable the constraint within OpenGL that limits textures
	// to a size within multiples of 4

	// this function disables that constaint 
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);




}