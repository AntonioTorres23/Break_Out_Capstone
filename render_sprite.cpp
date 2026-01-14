#include "render_sprite.h" // include header file to access class template

/*
NOTICE HOW WE'RE DEFINING THE FUNCTIONS THAT ARE STORED WITHIN THE RENDER_SPRITE_OBJ CLASS/OBJECT, SO IN THE C++ FILE HERE WE'RE DEFINING THE ACTUAL SOURCE CODE
THE FUNCTIONS THAT ARE STORED IN THE HEADER FILE ARE REALLY ONLY PROTOTYPES
*/

// define contructor for RENDER_SPRITE_OBJ
RENDER_SPRITE_OBJ::RENDER_SPRITE_OBJ(SHADER_OBJ& shader_object_argument)
{
	// set the private sprite_shader_object data member equal to the shader_object_argument (exact address location) object provided in the constructor
	this->sprite_shader_object = shader_object_argument;
	// begin the process of initalizing our vertex data with the related private function
	this->vertex_data_intialize();
}

// define deconstructor for RENDER_SPRITE_OBJ
RENDER_SPRITE_OBJ::~RENDER_SPRITE_OBJ()
{
	// within the body of the deconstructor, delete the vertex array object private data member that we where using to store our sprite vertex data
	glDeleteVertexArrays(1, &this->sprite_vertex_array_object);
}


// define the void member public function Render_and_Draw_Spirte

void RENDER_SPRITE_OBJ::Render_and_Draw_Spirte(TEXTURE_2D_OBJ& texture_object_argument, glm::vec2 position_of_sprite_argument, glm::vec2 scale_size_argument, float rotation_degree_argument, glm::vec3 sprite_color_argument)
{
	// activate the related shader object for our sprite that is stored in the private data member
	this->sprite_shader_object.Activate();
	// create a basic diagonal transformation matrix for scaling, rotating, and translating the sprites on screen; look up diagonal matrices and how they work well for translating vectors for more info
	glm::mat4 sprite_transformation_matrix = glm::mat4(1.0);
	// translate the entire sprite first with the position_of_sprite_argument; transformations work in this order, scaling, rotation, and then translations (moving the entire sprite around)
	// see how we also include a z coordinate of 0.0 because even though we are working in 2D we still need to provide a z-coordinate which will remain zero
	sprite_transformation_matrix = glm::translate(sprite_transformation_matrix, glm::vec3(position_of_sprite_argument, 0.0f));

	// then we translate the sprite back to its original position using the same scaling size
	sprite_transformation_matrix = glm::translate(sprite_transformation_matrix, glm::vec3(0.5f * scale_size_argument.x, 0.5f * scale_size_argument.y, 0.0f));
	
	// now rotate the object to the desired position which will then be converted to radians; it will also be rotated along the z axis
	sprite_transformation_matrix = glm::rotate(sprite_transformation_matrix, glm::radians(rotation_degree_argument), glm::vec3(0.0f, 0.0f, 1.0f));

	// this translation is to ensure that the roatation is at the origin (0,0), so we translate the sprite by half its scaling size first and then rotate the sprite
	sprite_transformation_matrix = glm::translate(sprite_transformation_matrix, glm::vec3(-0.5f * scale_size_argument.x, -0.5f * scale_size_argument.y, 0.0f));

	// scale the sprite by the specified dimensions
	sprite_transformation_matrix = glm::scale(sprite_transformation_matrix, glm::vec3(scale_size_argument, 1.0f));

	// within our private sprite_shader_object data member, set the coorelating uniforms within the vertex shader
	this->sprite_shader_object.uniform_matrix_4("sprite_transformation_matrix", sprite_transformation_matrix);
	// within our private sprite_shader_object data member, set the coorelating uniforms within the fragment shader
	this->sprite_shader_object.uniform_vector_3("color_of_sprite", sprite_color_argument);

	// activate texture 0 within the OpenGL function glActivateTexture; this is like a pre-defined texture in which you can assign a texture object to a GL_TEXTURE0-29 for things like layering multiple textures over each other
	glActiveTexture(GL_TEXTURE0);

	// bind the specified texture within the argument of this function the active texture
	texture_object_argument.Bind_Texture();

	// bind the private sprite_vertex_array_object private data member for us to draw the sprite
	glBindVertexArray(this->sprite_vertex_array_object);
	// draw the arrays using the OpenGL function glDrawArrays, which contains the way to draw the vertices which in this case is GL_TRIANGLES, which array index to start on which is 0, and how many values
	// we are drawing which is 6
	glDrawArrays(GL_TRIANGLES, 0, 6);
	// unbind the vertex array object after we are done using it
	glBindVertexArray(0);
}

// define the void private member function vertex_data_initalize

void RENDER_SPRITE_OBJ::vertex_data_intialize()
{
	/* 
	 create a vertex buffer object for our sprite, a VBO is an object that let's us store verticies within something like an array and then 
	 use a vertex array object's attribute pointers to specify how many coordinates to sub-divide in large amount of vertices. 

	 for example, a lot of times you will use your first attribute pointer (0) contain positional data, the second (1) for texture coordinates, and the third (2) for normal coordinates 
	 (coordinates that are perpendicular with the surface of an object)
	*/
	unsigned int sprite_vertex_buffer_object; 

	// the raw vertex data which will represent our positional as well as texture coordinates
	// also note that the positional data is already in normalized device coordinate format (coordinates in range of [-1, 1])
	// texture coordinates will always be in a range of [0-1] with anything above that leading to repeating texture if that is the wrapping method that was selected
	// all of the positional coordinates will make up a square that is made up of 2 right traingles
	float sprite_vertex_data[] =
	{
		// positional coordinates		// texture coordiantes
		// lower right triangle; see how texture coordiantes match up with positional coordiantes
		0.0f, 1.0f,						0.0f, 1.0f,
		1.0f, 0.0f,                     1.0f, 0.0f,
		0.0f, 0.0f,                     0.0f, 0.0f,

		// upper right triangle
		0.0f, 1.0f,						0.0f, 1.0f,
		1.0f, 1.0f,                     1.0f, 1.0f, 
		1.0f, 0.0f,                     1.0f, 0.0f
	};

	// generate a vertex array object with the OpenGL function glGenVertexArrays and the private data member
	glGenVertexArrays(1, &this->sprite_vertex_array_object);

	// generate a vertex buffer object with the prior variable we defined and glGenBuffers
	glGenBuffers(1, &sprite_vertex_buffer_object); 

	// here we bind the vertex buffer object to configure the vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, sprite_vertex_buffer_object); 
	// here we configure our VBO by specifying the type of buffer, the size of the VBO, which array to collect the data from, and the drawing method
	// GL_STATIC_DRAW BASICALLY MEANS THAT THE DATA WE ARE USING WILL BE DEFINED ONLY ONCE AND THEN THIS DATA WILL BE REUSED TO BE DRAWN MULTIPLE TIMES
	// we use the C++ built-in sizeof function to get the byte size of the sprite_vertex_data array
	glBufferData(GL_ARRAY_BUFFER, sizeof(sprite_vertex_data), sprite_vertex_data, GL_STATIC_DRAW);

	// here we also bind the vertex array object private data member for configuration
	glBindVertexArray(this->sprite_vertex_array_object);

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