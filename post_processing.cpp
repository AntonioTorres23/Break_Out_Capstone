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
	// initialize the vertex data for the screen-filled quad with the related private member function
	this->vertex_data_initialze();
	// within the SHADER_OBJ public data member, set the sampler2D uniform to the integer value 0, representing the first GL_TEXTURE (GL_TEXTURE0), we also set a true value to say we want to activate the shader prior to setting the uniform
	this->Post_Processing_Shader_Object.uniform_integer("framebuffer_object_texture", 0, true);
	// now create an offset value by dividing 1 by 300 (this equals 0.00333333333333333333333333333333)
	float framebuffer_texture_coordiante_offset_value = 1.0f / 300.0f;
	// create a 2D array where there are 9 columns and two rows within each column, thus creating something similar to a 2-value glm vector
	// each row represents an offset position within the screen-filled quad's texture coordinates
	float framebuffer_texture_coordiante_offsets[9][2] =
	{
		{-framebuffer_texture_coordiante_offset_value, framebuffer_texture_coordiante_offset_value},  // top left of screen-filled quad
		{0.0f,										   framebuffer_texture_coordiante_offset_value},  // top center of screen-filled quad
		{framebuffer_texture_coordiante_offset_value,  framebuffer_texture_coordiante_offset_value},  // top right of screen-filled quad
		{-framebuffer_texture_coordiante_offset_value, 0.0f},										  // center left of screen-filled quad
		{0.0f,                                         0.0f},                                         // direct center of screen-filled quad	
		{framebuffer_texture_coordiante_offset_value,  0.0f},										  // center right of screen-filled quad
		{-framebuffer_texture_coordiante_offset_value, -framebuffer_texture_coordiante_offset_value}, // bottom left of screen-filled quad
		{0.0f,                                         -framebuffer_texture_coordiante_offset_value}, // bottom center of screen-filled quad
		{framebuffer_texture_coordiante_offset_value,  -framebuffer_texture_coordiante_offset_value}  // bottom right of screen-filled quad
	};

	// now send all of these to the public SHADER_OBJ data member, we have to use the OpenGL function of 2 float matrix-like values because we don't have this data value within our SHADER_OBJ uniform setting member functions
	// as well as due to it being a 2D array we have to call this weird float value pointer within the 2D array to grab the actual row values (2) that are within the columns (9), thus making it like a matrix
	// within the parameters we get the location of the uniform within the shader program, there are 9 columns, and data within those columns represented as rows
	// I THINK THE POINTER VALUE IS CALLING ALL OF THE VALUES THAT ARE STORED WITHIN THE 2D ARRAY ROWS SO ITS ALMOST LIKE A NESTED POINTER VALUE CALL?
	// THINK OF IT AS WE ARE CALLING THE ROW DATA WITHIN THE COLUMN DATA IN WHICH THE COLUMN DATA IS THE FIRST THING THAT IS INDEXED
	glUniform2fv(glGetUniformLocation(this->Post_Processing_Shader_Object.Shader_ID, "framebuffer_texture_coordiante_offsets"), 9, (float*)framebuffer_texture_coordiante_offsets);

	
	// now we create an array of values that will represent the chaos_effect_edge_kernel in which each value will be multiplied by the offset location within the fragment shader
	int chaos_effect_edge_kernel[9] =
	{
		-1, -1, -1,
		-1,  8, -1,
		-1, -1, -1
	};

	// send the edge kernel to the public SHADER_OBJ data member using the 1 float matrix-like value OpenGL function since we don't have this data value within our SHADER_OBJ uniform setting member functions
	glUniform1iv(glGetUniformLocation(this->Post_Processing_Shader_Object.Shader_ID, "chaos_effect_edge_kernel"), 9, chaos_effect_edge_kernel);
	

	// now we create an array of values that will represent the screen_shake_effect_blur_kernel in which each each value will be multiplied by the offset location within the fragment shader
	// each value is a variety of values divided by the value of 16 to create a gaussian blur effect
	float screen_shake_effect_blur_kernel[9] =
	{
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
		2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
	};

	// send the blur kernel to the public SHADER_OBJ data member using the 1 float matrix-like value OpenGL function since we don't have this data value within our SHADER_OBJ uniform setting member functions
	glUniform1fv(glGetUniformLocation(this->Post_Processing_Shader_Object.Shader_ID, "screen_shake_effect_blur_kernel"), 9, screen_shake_effect_blur_kernel);


}