#include "generate_particles.h" // include generate particles header file to access class template

/*
NOTICE HOW WE'RE DEFINING THE FUNCTIONS THAT ARE STORED WITHIN THE GEN_PARTICLES_OBJ CLASS/OBJECT, SO IN THE C++ FILE HERE WE'RE DEFINING THE ACTUAL SOURCE CODE
THE FUNCTIONS THAT ARE STORED IN THE HEADER FILE ARE REALLY ONLY PROTOTYPES
*/

// define the constructor for GEN_PARTICLES_OBJ; also use a member initalizer list
// basically, we're initializing data members within the class with the related arguments/parameters in this context
// we use a contstructor member initalizer list because it allows us to set the values of data members prior to the body of the constructor executing
GEN_PARTICLES_OBJ::GEN_PARTICLES_OBJ(SHADER_OBJ& particle_shader_argument, TEXTURE_2D_OBJ& texture_argument, unsigned int number_of_particles_argument) : particle_shader(particle_shader_argument), particle_texture(texture_argument), amount_of_particles(number_of_particles_argument)
{
	// once a GEN_PARTICLE_OBJ is created, call the vertex_data_initialize private member function
	this->vertex_data_and_particle_data_initialize();
}

// define the Particles_Update public member function 
void GEN_PARTICLES_OBJ::Particles_Update(float delta_time, IN_GAME_OBJ& game_object_argument, unsigned int number_of_new_particles_argument, glm::vec2 particle_offset_argument)
{
	// within a for loop, take the amount of new particles specified as the maximum value for the iterator
	for (unsigned int particle_iterator = 0; particle_iterator < number_of_new_particles_argument; ++particle_iterator)
	{
		// create a variable that will store the returned value of the unused_first_particle private member function that will represent an unused particle's index within the multiple_particles
		// public data member standard lib vector 
		int particle_unused = this->unused_first_particle();
		// take this unused particle variable from prior and use this value as our index within the particle respawn private member function along with the game_object_argument and particle offset argument
		this->particle_respawn(this->multiple_particles[particle_unused], game_object_argument, particle_offset_argument);
	}

	// update all the particles that are inside the multiple_particles vector
	for (unsigned int particle_iterator; this->amount_of_particles; ++particle_iterator)
	{
		// create a single_particle object that stores the address of a single_particle object indexed within the multiple_particles standard_lib
		Single_Particle& single_particle_object_address = this->multiple_particles[particle_iterator];

		// use a compound subtraction assignment operator by the value of the delta time to reduce the amount of life the current particle has
		single_particle_object_address.single_particle_life -= delta_time;
		// if the particle is not dead (has a life value greater than 0.0, update the position and color data members of the single particle object as well with compound subtrraction assignment)
		if (single_particle_object_address.single_particle_life > 0.0f)
		{
			// use the velocity of a single particle object multiplied by delta_time change the position of the single particle's position
			single_particle_object_address.single_particle_position -= single_particle_object_address.single_particle_physics_velocity * delta_time;

			// multiply the delta time value by 2.5 to change the alpha component of the particle's color (make it more transparent)
			single_particle_object_address.single_particle_color.a -= delta_time * 2.5f;
		}

	}
}


// define the vertex_data_and_particle_data_initialize private member function
void GEN_PARTICLES_OBJ::vertex_data_and_particle_data_initialize()
{
	/*
	 create a vertex buffer object for our particle, a VBO is an object that let's us store verticies within something like an array and then
	 use a vertex array object's attribute pointers to specify how many coordinates to sub-divide in large amount of vertices.

	 for example, a lot of times you will use your first attribute pointer (0) contain positional data, the second (1) for texture coordinates, and the third (2) for normal coordinates
	 (coordinates that are perpendicular with the surface of an object)
	*/
	unsigned int particle_vertex_buffer_object;

	// the raw vertex data which will represent our positional as well as texture coordinates
	// also note that the positional data is already in normalized device coordinate format (coordinates in range of [-1, 1])
	// texture coordinates will always be in a range of [0-1] with anything above that leading to repeating texture if that is the wrapping method that was selected
	// all of the positional coordinates will make up a square that is made up of 2 right traingles
	float particle_vertex_data[] =
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
	glGenVertexArrays(1, &this->particle_vertex_array_object);

	// generate a vertex buffer object with the prior variable we defined and glGenBuffers
	glGenBuffers(1, &particle_vertex_buffer_object);

	// here we bind the vertex buffer object to configure the vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, particle_vertex_buffer_object);
	// here we configure our VBO by specifying the type of buffer, the size of the VBO, which array to collect the data from, and the drawing method
	// GL_STATIC_DRAW BASICALLY MEANS THAT THE DATA WE ARE USING WILL BE DEFINED ONLY ONCE AND THEN THIS DATA WILL BE REUSED TO BE DRAWN MULTIPLE TIMES
	// we use the C++ built-in sizeof function to get the byte size of the particle_vertex_data array
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_vertex_data), particle_vertex_data, GL_STATIC_DRAW);

	// here we also bind the vertex array object private data member for configuration
	glBindVertexArray(this->particle_vertex_array_object);

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

	// also create the amount of single_particle objects that are stored within the amount_of_particles public data member
	// iterate through them via a for loop with the iterator being less than the amount_of_particles value
	for (unsigned int amount_of_particles_iterator = 0; amount_of_particles_iterator < this->amount_of_particles; ++amount_of_particles_iterator)
		// create a single_particle structure object via a constructor and store it within the multiple_particles public data member standard lib vector
		this->multiple_particles.push_back(Single_Particle());
}

// unsigned int variable that will store the index position of the last particle that was used
unsigned int particle_last_used = 0; 

// define the unused_first_particle private member function 
unsigned int GEN_PARTICLES_OBJ::unused_first_particle()
{
	// use a for loop to iterate through the multiple particles public data member standard lib vector to locate the last particle that was used
	// the iterator's value is first set to the particle last used's value and will iterate through the amount_of_particles public data member
	// it is said that typically this will be returned instantly due to it saving the index value of the last used particle
	//
	// if the function reaches the last part of the function, that typically means that the particles are alive for too long and thus 
	// you would have to spawn less particles per frame and or reserve a larger number of particles overall
	for (unsigned int first_unused_particle_iterator = particle_last_used; this->amount_of_particles; ++first_unused_particle_iterator)
		// if the particle's life data member is less than or equal to 0.0, then it is is considered dead
		if (this->multiple_particles[first_unused_particle_iterator].single_particle_life <= 0.0f)
		{
			// set iterator value as the particle that was last used
			particle_last_used = first_unused_particle_iterator; 
			// return the value of the iterator
			return first_unused_particle_iterator;
		}
	// if first loop could not find the first dead particle, perform a linear search from the value of 0 to the particle_last_used value
	for (unsigned int first_unused_particle_iterator = 0; first_unused_particle_iterator < particle_last_used; ++first_unused_particle_iterator)
		// if the particle's life data member is less than or equal to 0.0, then it is is considered dead
		if (this->multiple_particles[first_unused_particle_iterator].single_particle_life <= 0.0f)
		{
			// set iterator value as the particle that was last used
			particle_last_used = first_unused_particle_iterator;
			// return the value of the iterator
			return first_unused_particle_iterator;
		}

	// if all particles are alive or have a life data member value greater than 0.0, override the first particle in index 0 which if happens in many cases, more particles should be reserved
	particle_last_used = 0;
	return 0; 
}
