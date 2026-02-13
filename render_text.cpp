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
	// we also activate the shader with the true value within the 3rd argument/parameter
	this->Render_Text_Shader.uniform_matrix_4("text_orthographic_projection_matrix", glm::ortho(0.0f, static_cast<float>(width_of_screen_argument), static_cast<float>(height_of_screen_argument), 0.0f), true);
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

	// this function disables that constraint 
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// generate a for loop that will generate the standard 128 ascii character set
	// we use a unsigned Graphics Library byte as our iterator data type
	for (GLubyte character = 0; character < 128; character++)
	{
		// load that single ascii character with the FT_Load_Char function
		// check if the character can be loaded and if not, send an error message and continue through the for loop
		// continue means skip this one iteration and move on to the next one
		// THE FT_LOAD_RENDER flag tells FreeType to create a 8-bit grayscale bitmap access via the font_object->glyph->bitmap
		if (FT_Load_Char(font_object, character, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREE_TYPE::FAILED_TO_LOAD_GLYPH" << std::endl;
			continue;
		}

		// create a texture object for our character
		unsigned int individual_character_texture;
		// generate a texture object with glGenTextures
		glGenTextures(1, &individual_character_texture);
		// bind the individual_character_texture as the current texture for configuration
		glBindTexture(GL_TEXTURE_2D, individual_character_texture);
		// create a 2D texture with the OpenGL function glTexImage2D, where we only specify only one color for the format which is GL_RED
		// and the dimensions of the character will be within the bitmap
		// we specify the texture data as the buffer data stored within the bitmap
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, font_object->glyph->bitmap.width, font_object->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, font_object->glyph->bitmap.buffer);
		// set the texture filtering options with OpenGL functions
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// create a Single_Character structure with the data previously defined
		// we use the bitmap data to gather the size of the character, glyph, and advance 
		Single_Character single_character = { individual_character_texture, glm::ivec2(font_object->glyph->bitmap.width, font_object->glyph->bitmap.rows), glm::ivec2(font_object->glyph->bitmap_left, font_object->glyph->bitmap_top), font_object->glyph->advance.x };
		// send this single character to the Multiple Character's public data member via the insert method function
		// we use the standard library function called pair to pair both the unsigned OpenGL byte with the single_character structure
		Multiple_Characters.insert(std::pair<char, Single_Character>(character, single_character));
	}
	// after the for loop undbind any texture objects
	glBindTexture(GL_TEXTURE_2D, 0);
	// destroy all free type objects with the FT_Done_Face and FT_Done_FreeType
	FT_Done_Face(font_object);
	FT_Done_FreeType(free_type_library_object);
}


// define the Text_Render function 
void RENDER_TEXT_OBJ::Text_Render(std::string text_to_be_rendered_on_screen, float text_x_position, float text_y_position, float text_scale_size, glm::vec3 text_color)
{
	// activate the Render_Text_Shader public data member
	this->Render_Text_Shader.Activate();
	// set the color_of_text uniform 3-value vector with the corresponding parameter/argument within the Render_Text_Shader public data member
	this->Render_Text_Shader.uniform_vector_3("color_of_text", text_color);
	// activate GL_TEXTURE0 to diaplay text
	glActiveTexture(GL_TEXTURE0);
	// bind the vertex_array_object private data member
	glBindVertexArray(this->Vertex_Array_Object);

	// create a const_iterator string to loop through all of our 128 ascii characters
	// we use a const_iterator so we don't modify the elements while iterating
	std::string::const_iterator character;
	// create a for loop with character as our iterator and assign it with the beginning of the 
}