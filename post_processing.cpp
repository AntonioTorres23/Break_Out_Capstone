#include "post_processing.h" // include post_processing.h to access the POST_PROCESSING_OBJ class

#include <iostream> // include iostream for default output

/*
NOTICE HOW WE'RE DEFINING THE FUNCTIONS THAT ARE STORED WITHIN THE POST_PROCESSING_OBJ CLASS/OBJECT, SO IN THE C++ FILE HERE WE'RE DEFINING THE ACTUAL SOURCE CODE
THE FUNCTIONS THAT ARE STORED IN THE HEADER FILE ARE REALLY ONLY PROTOTYPES
*/

// define the constructor with the required arguments/parameters
// we use a constructor member initalizer list because it allows us to set the values of data members prior to the body of the constructor executing
POST_PROCESSING_OBJ::POST_PROCESSING_OBJ(SHADER_OBJ shader_object_argument, unsigned int width_argument, unsigned int height_argument) : Post_Processing_Shader_Object(shader_object_argument), Post_Processing_Texture_Object(), Post_Processing_Width(width_argument), Post_Processing_Height(height_argument), Confuse_Effect(false), Chaos_Effect(false), Screen_Shake_Effect(false)
{
	// create framebuffer objects with the related private data members within POST_PROCESSING_OBJ
	glGenFramebuffers(1, &this->multi_sampled_framebuffer_object);
	glGenFramebuffers(1, &this->framebuffer_object);
	// same concept as before but this time with the render buffer object private data member within POST_PROCESSING_OBJ
	glGenRenderbuffers(1, &this->renderbuffer_object);
	// begin binding the multi-sampled framebuffer object as the current framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, this->multi_sampled_framebuffer_object);
	// bind the render buffer object as the current renderbuffer
	glBindRenderbuffer(GL_RENDERBUFFER, this->renderbuffer_object);
	// allocate storage for the renderbuffer object with the target being the current renderbuffer object, 4 samples within renderbuffer storage, store GL_RGB to output the color buffer on screen, and the width and height parameters within the arguments in the constructor
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, width_argument, height_argument);
	// attach the current multi-sampled framebuffer object as the target, take in the color buffer as the data we want to attach to the renderbuffer, the currently binded render buffer object, and the actual render buffer object that is stored within the class
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->renderbuffer_object);
	// check if the currently bound multi-sampled framebuffer object initialization was successful
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::POST_PROCESSING::MULTI-SAMPLED_FRAMEBUFFER::FAILED_TO_INITIALIZE_MULTI-SAMPLED_FRAMEBUFFER_OBJECT" << std::endl;
	// now bind the regular framebuffer object private data member as the current framebuffer object
	glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer_object);
	// use the public TEXTURE_2D_OBJ data member and create a texture with the dimensions being the arguments/parameters provided in the constructor and null texture data essentially creating a blank texture
	this->Post_Processing_Texture_Object.Create_Texture(width_argument, height_argument, NULL);
	// attach the data member texture object to the regular framebuffer object as its color attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->Post_Processing_Texture_Object.texture_ID, 0);
	// check if the currently bound regular framebuffer object initialization was successful
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE);
		std::cout << "ERROR::POST_PROCESSING::FRAMEBUFFER::FAILED_TO_INITIALIZE_FRAMEBUFFER_OBJECT" << std::endl;
	// unbind current regular framebuffer object
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


}