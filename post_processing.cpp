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
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
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

// define Start_Render function
void POST_PROCESSING_OBJ::Start_Render()
{
	// bind the multi sampled frame buffer object public data member
	glBindFramebuffer(GL_FRAMEBUFFER, this->multi_sampled_framebuffer_object);
	// set the clear color buffer to a black (0.0, 0.0, 0.0) with full opacity (alpha value of 1.0)
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT);
}

// define Finish_Render function
void POST_PROCESSING_OBJ::Finish_Render()
{
	// bind the multi-sampled framebuffer object as our read framebuffer because we want to read all the data that is within this framebuffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->multi_sampled_framebuffer_object);
	// bind the regular framebuffer object as our draw framebuffer because we want this framebuffer's data to be the one displayed on the screen-filled quad
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->framebuffer_object);
	// now blit the two framebuffers together, so read the information stored in the multi-sampled framebuffer and write that information to the regular framebuffer
	// the extra parameters here relate to the dimensions of each framebuffer with the first 4 parameters being the source framebuffer (multi-sampled framebuffer) dimensions and the next 4 being
	// the destination framebuffer (regular framebuffer) with the last two being the buffer that we want to store which is the color buffer and the filtering method which we want GL_NEAREST
	// the zeros represent the beginning coordinates of the framebuffer which we want to be zero, so its zero to the dimensions of the screen
	glBlitFramebuffer(0, 0, this->Post_Processing_Width, this->Post_Processing_Height, 0, 0, this->Post_Processing_Width, this->Post_Processing_Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	// unbind the framebuffer to bind both the read and write framebuffers to the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// define the Render_Post_Processing function
void POST_PROCESSING_OBJ::Render_Post_Processing(float post_processing_time)
{
	// activate the post processing shader program
	this->Post_Processing_Shader_Object.Activate();
	// set the post_processing_time uniform variable in the fragment shader to the value stored in the argument/parameter post_processing_time
	this->Post_Processing_Shader_Object.uniform_float("post_processing_time", post_processing_time);
	// set the post_processing_effect_confuse uniform boolean variable within both the vertex and fragment shaders to the value stored within the Confuse_Effect public data member
	this->Post_Processing_Shader_Object.uniform_integer("post_processing_effect_confuse", this->Confuse_Effect, true);
	// set the post_processing_effect_chaos uniform boolean variable within both the vertex and fragment shaders to the value stored within the Chaos_Effect public data member
	this->Post_Processing_Shader_Object.uniform_integer("post_processing_effect_chaos", this->Chaos_Effect, true);
	// set the post_processing_effect_screen_shake uniform boolean variable within both the vertex and fragment shaders to the value stored within the Screen_Shake_Effect public data member
	this->Post_Processing_Shader_Object.uniform_integer("post_processing_effect_screen_shake", this->Screen_Shake_Effect);
	// activate the GL_TEXTURE0 to display the framebuffer object texture on the screen-filled quad
	glActiveTexture(GL_TEXTURE0);
	// bind the TEXTURE_2D_OBJ data member that is currently storing the framebuffer color buffer data within it
	this->Post_Processing_Texture_Object.Bind_Texture();
	// bind the vertex_array_object private data member
	glBindVertexArray(this->post_processing_vertex_array_object);

	glDisable(GL_DEPTH_TEST);
	
	// draw the screen-filled quad
	glDrawArrays(GL_TRIANGLES, 0, 6);
	// unbind the vertex array object
	glBindVertexArray(0);
}

// define the vertex_data_initialze private member function
void POST_PROCESSING_OBJ::vertex_data_initialze()
{
	/*
	 create a vertex buffer object for our screen-filled quad, a VBO is an object that let's us store verticies within something like an array and then
	 use a vertex array object's attribute pointers to specify how many coordinates to sub-divide in large amount of vertices.

	 for example, a lot of times you will use your first attribute pointer (0) contain positional data, the second (1) for texture coordinates, and the third (2) for normal coordinates
	 (coordinates that are perpendicular with the surface of an object)
	*/
	unsigned int post_processing_vertex_buffer_object;

	// the raw vertex data which will represent our positional as well as texture coordinates
	// also note that the positional data is already in normalized device coordinate format (coordinates in range of [-1, 1])
	// texture coordinates will always be in a range of [0-1] with anything above that leading to repeating texture if that is the wrapping method that was selected
	// all of the positional coordinates will make up a square that is made up of 2 right traingles
	// notice how these coordiantes use the full range of NDC to take up the whole screen
	float screen_filled_quad_vertex_data[] =
	{
		// positional coordinates		// texture coordiantes
		// upper right triangle; see how texture coordiantes match up with positional coordiantes
	   -1.0f, -1.0f,					 0.0f, 0.0f,
		1.0f,  1.0f,                     1.0f, 1.0f,
	   -1.0f,  1.0f,                     0.0f, 1.0f,

		// lower right triangle
	   -1.0f, -1.0f,					0.0f, 0.0f,
		1.0f, -1.0f,                    1.0f, 0.0f,
		1.0f, 1.0f,                     1.0f, 1.0f
	};

	// generate a vertex array object with the OpenGL function glGenVertexArrays and the private data member
	glGenVertexArrays(1, &this->post_processing_vertex_array_object);

	// generate a vertex buffer object with the prior variable we defined and glGenBuffers
	glGenBuffers(1, &post_processing_vertex_buffer_object);

	// here we bind the vertex buffer object to configure the vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, post_processing_vertex_buffer_object);
	// here we configure our VBO by specifying the type of buffer, the size of the VBO, which array to collect the data from, and the drawing method
	// GL_STATIC_DRAW BASICALLY MEANS THAT THE DATA WE ARE USING WILL BE DEFINED ONLY ONCE AND THEN THIS DATA WILL BE REUSED TO BE DRAWN MULTIPLE TIMES
	// we use the C++ built-in sizeof function to get the byte size of the sprite_vertex_data array
	glBufferData(GL_ARRAY_BUFFER, sizeof(screen_filled_quad_vertex_data), screen_filled_quad_vertex_data, GL_STATIC_DRAW);

	// here we also bind the vertex array object private data member for configuration
	glBindVertexArray(this->post_processing_vertex_array_object);

	// enable our first vertex (and only) array attribute index
	glEnableVertexAttribArray(0);
	// configure vertex arribute array 0 with the OpenGL function glVertexArribPointer
	/*
	   here we specify which attribute array index we want to configure, how many coordinates / float values to expect, what data type to expect which is float,
	   if we want the values to be normalized (clamped to range of -1 to 1 for signed values and 0 to 1 for unsigned values) or not in which we don't want,
	   the stride which is the byte offset per value will always be the amount of values per attribute times the sizeof a singular data type (which
	   in this case is float), and the offset (if this is the first attribute pointer we want our offset to be this void pointer function at zero)
	   */
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	// now we unbind our vertex buffor object by calling 0 in glBindBuffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// same concept applies to vertex array objects
	glBindVertexArray(0);
}
