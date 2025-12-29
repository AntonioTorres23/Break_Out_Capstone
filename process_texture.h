#ifndef PROCESS_TEXTURE_HEADER
#define PROCESS_TEXTURE_HEADER

#include <glad/glad.h> // include glad for OpenGL pointer functions 

//  can store and set parameters of a 2D Texture; Along with function for utility
class TEXTURE_2D_OBJ
{
public: // where all publicly accessible methods/functions go
	
	// method that stores texture ID 
	unsigned int texture_ID; 
	// stores the dimensions of the 2D texture; width and height
	unsigned int width_of_texture, height_of_texture;
	// format of texture internally (i.e. GL_RED, GL_RG, GL_RGB)
	unsigned int texture_format_internally;
	// format of the already loaded image within OpenGL
	unsigned int texture_format_loaded; 
	// texture parameters
	// methods for storing texture's s and t (x and y) parameters
	unsigned int texture_wrap_s; 
	unsigned int texture_wrap_t;
	// methods for storing texture's min and mag filter parameters
	unsigned int texture_min_filter; 
	unsigned int texture_mag_filter;
	// constructor for this class/object which sets default texture modes
	TEXTURE_2D_OBJ();
	// prototype function definition that generates a texture from the image data provided
	void Create_Texture(unsigned int texture_w, unsigned int texture_h, const char *texture_data);
	// binds current texture as an active GL_TEXTURE_2D object
	void Bind_Texture() const; // defined as a const function to deny it permission to change the values of data value member within the texture class
};

#endif // !PROCESS_TEXTURE_HEADER
