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
	this->Render_Text_Shader = RESOURCE_MANAGER::Shader_Load("Resources/Shaders/text.vert", "Resources/Shaders/text.frag", nullptr, "text_shader");
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
	// create a for loop with character as our iterator and assign it with the first character within the text_to_be_rendered_on_screen argument/parameter
	// then with the character not being equal to the last character within the argument/parameter, iterate through the characters
	for (character = text_to_be_rendered_on_screen.begin(); character != text_to_be_rendered_on_screen.end(); character++)
	{
		// create a single chracter object that is equal to the data that is located within whatever character is found within our iterator
		Single_Character character_in_text = Multiple_Characters[*character];

		// get the x-coordinate position of the character_in_text object by taking its bearing x coordinate value multiplied by the text_scale_size argument/parameter
		// then take the value of that product and add it by the text_x_position argument/parameter
		float character_in_text_x_position =  text_x_position + character_in_text.Glyph_Bearing.x * text_scale_size;
		/* 		
			for the y - coordiante position of the character_intext object, its a bit different; first, because we are using an orthographic projection matrix, all of 
			our y coordiantes are going from top to bottom with its origin point 0.0 representing the top of the screen 

			we need to get the top of the actual character which free type does not offer, however there are a few characters that do reach this top space are H, T, or X
			if we take the difference between one of these character's bearing y coordinate value and our current character_in_text's bearing y value, we can confine the
			top of the character_in_text's top edge

			we also scale this difference by the text_scale_size argument/parameter

			same idea applies where we take the product and add it by the text_y_position argument/parameter
		*/
		float character_in_text_y_position = text_y_position + (this->Multiple_Characters['H'].Glyph_Bearing.y - character_in_text.Glyph_Bearing.y) * text_scale_size;

		// now to calculate the character_in_text's width we take the x coordinate glyph size of the character and mulutiply it by the text_scale_size argument/parameter
		float character_in_text_width = character_in_text.Glyph_Size.x * text_scale_size;
		// same idea applies as prior but this time we use the character_in_text's y coordinate glyph size 
		float character_in_text_height = character_in_text.Glyph_Size.y * text_scale_size;

		// now create a 2D array with our newly calculated vertex data for this singular character which will be sent to the vertex buffer object to represent the 2D quad of our character
		// we also take the texture coordinates for our character texture from the bitmap
		// notice how we are applying some of our width and height values to certain vectors here
		float character_in_text_vertex_data[6][4] =
		{
			// 1st triangle                                                                                                   top-left texture coordinate
			{character_in_text_x_position, character_in_text_y_position + character_in_text_height,                           0.0f, 1.0f},
			//                                                                                                                bottom-right texture coordinate
			{character_in_text_x_position + character_in_text_width, character_in_text_y_position,                            1.0f, 0.0f},
			//                                                                                                                bottom-left texture coordiante
			{character_in_text_x_position, character_in_text_y_position,                                                      0.0f, 0.0f}, 
			// 2nd triangle                                                                                                   top-left texture coordinate
			{character_in_text_x_position, character_in_text_y_position + character_in_text_height,                           0.0f, 1.0f},
			//																						                          top-right texture coordinate			                               
			{character_in_text_x_position + character_in_text_width, character_in_text_y_position + character_in_text_height, 1.0f, 1.0f},
			//                                                                                                                bottom-right texture coordinate
			{character_in_text_x_position + character_in_text_width, character_in_text_y_position,                            1.0f, 0.0f}

		};


		// bind the texture of the character from the bit map on top of the 2D quad
		glBindTexture(GL_TEXTURE_2D, character_in_text.Glyph_Texture_ID);
		// update the Vertex Buffer Object buffer data with our character_in_text_vertex_data
		glBindBuffer(GL_ARRAY_BUFFER, this->Vertex_Buffer_Object);
		// use glBufferSubData to substitute the null data that we had configured at first prior
		// we also take the size of the character_in_text_vertex_data 2D array
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(character_in_text_vertex_data), character_in_text_vertex_data);
		// unbind the text vertex buffer object
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// draw the character quad on screen
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// use bitshifting to advance the cursors for the next character
		// use addition compound assignment on the character_in_text_x_position for this as well as multiply the bitshift value by the text_scale_size argument/parameter
		// bitshiting by 6 gets the value in pixels; 1/64th * 2 to the power of 6 equals 64
		text_x_position += (character_in_text.Glyph_Advance >> 6) * text_scale_size;
	}
	// after the for loop, unbind the vertex array and any currently bound textures
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

}