#include <iostream> // include iostream to send default output

#include "process_texture.h" // include header file to access class template

// define constructor with constructor intalizer list containing relevant values to set data members
TEXTURE_2D_OBJ::TEXTURE_2D_OBJ() : width_of_texture(0), height_of_texture(0), texture_format_internally(GL_RGB), texture_format_loaded(GL_RGB), texture_wrap_s(GL_REPEAT), texture_wrap_t(GL_REPEAT), texture_min_filter(GL_LINEAR), texture_mag_filter(GL_LINEAR)
{
	// generate a texture object with the address of the stored texture_ID data member within this object
	glGenTextures(1, &this->texture_ID);
}

// define create_texture member function
void TEXTURE_2D_OBJ::Create_Texture(unsigned int texture_w, unsigned int texture_h, const char* texture_data)
{
	// set the related object data method dimensions equal to the values that are in the arguments
	this->width_of_texture = texture_w; 
	this->height_of_texture = texture_h;

	// create the texture with OpenGL functions
	glBindTexture(GL_TEXTURE_2D, this->texture_ID);
	glTexImage2D(GL_TEXTURE_2D, 0, this->texture_format_internally, texture_w, texture_h, 0, this->texture_format_loaded, GL_UNSIGNED_BYTE, texture_data);
	// set the texture parameters with OpenGL functions
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->texture_wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->texture_wrap_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->texture_min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->texture_mag_filter);
	// unbind texture once done creating texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

// define bind_texture member function; remember we make it constant so that it doesn't effect the data member values within the texture class
void TEXTURE_2D_OBJ::Bind_Texture() const
{
	glBindTexture(GL_TEXTURE_2D, this->texture_ID);
}