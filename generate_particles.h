#ifndef GENERATE_PARTICLES_HEADER
#define GENERATE_PARTICLES_HEADER

#include <vector>		// don't get this confused with a glm vector, this is a standard library that allows us to organize and store variables/objects in a more efficient way; it has methods that make parsing through this data much easier than something like an array 

#include <glad/glad.h>  // include GLAD; a lib that loads the addresses of OpenGL function pointers
#include <glm/glm.hpp> // include glm, a library that allows for us to create OpenGL vectors, matrices, etc.


#include "process_texture.h" // include our process_texture header file to use the TEXTURE_2D_OBJ here
#include "process_shader.h" // include our process_shader header file to use the SHADER_OBJ here
#include "in_game_obj.h"   // include in_game_obj header file to allow for IN_GAME_OBJ to be used here

// a structure that stores various data members for a single particle
struct Single_Particle
{
	// two glm two-value vectors that store a single particle's position as well as its physics velocity
	glm::vec2 single_particle_position, single_particle_physics_velocity; 

	// 4-value glm vector that stores the color of a single particle (RGBA values)
	glm::vec4 single_particle_color;

	// float value data member that stores the life of a single particle
	float single_particle_life;

	// constructor for Single Particle that also has a member initalizer list so that once a Single_Particle object is defined/created, it also initalizes the data members within Single_Particle
	// we use a contstructor member initalizer list because it allows us to set the values of data members prior to the body of the constructor executing

	Single_Particle() : single_particle_position(0.0f), single_particle_physics_velocity(0.0f), single_particle_color(1.0f), single_particle_life(0.0f)
	{

	}

};

// GEN_PARTICLES_OBJ is a class that allows us to generate multiple particle effects in game
class GEN_PARTICLES_OBJ
{
public: // public data/function members

	// constructor of GEN_PARTICLES_OBJ that takes a SHADER_OBJ address, TEXTURE_2D_OBJ address, and a unsigned integer which specifies the amount of particles desired as our parameters/arguments
	GEN_PARTICLES_OBJ(SHADER_OBJ& particle_shader_argument, TEXTURE_2D_OBJ& texture_argument, unsigned int number_of_particles_argument);

	// public member void function that updates all current particles as well as thier attributes
	// parameters/arguments: 1. a delta time argument/parameter, 2. an IN_GAME_OBJ address argument/parameter, 3. unsigned int parameter that represents the new amount of desired particles particles argument/parameter
	// 4. an 2-value glm vector that represents the offset of a particle argument/parameter, if not provided, it will be set to a default position of (0.0, 0.0)
	void Particles_Update(float delta_time, IN_GAME_OBJ& game_object_argument, unsigned int number_of_new_particles_argument, glm::vec2 particle_offset_argument = glm::vec2(0.0f, 0.0f));

	// public member void function that renders and draws all particles
	void Render_And_Draw_Particles();

private: // private data/function members

	// standard lib vector private data member that stores Single_Particle structure objects
	std::vector<Single_Particle> multiple_particles; 

	// unsigned int private data member that stores the amount of particles
	unsigned int amount_of_particles;

	// private SHADER_OBJ data member that stores the particle's shader
	SHADER_OBJ particle_shader;

	// private TEXTURE_2D_OBJ data member that stores the particle's texture
	TEXTURE_2D_OBJ particle_texture;

	// private data member that represents a particle's Vertex Array Object
	unsigned int particle_vertex_array_object;

	// private function member that initalizes a particle's vertex data as well as its structure data
	void vertex_data_and_particle_data_initialize();

	// unsigned int private function member that will return the first single_particle within the multiple_particles standard lib vector by using an index. 
	// the life data member will by less than or equal to 0.0 if no particle is currrently inactive
	unsigned int unused_first_particle();
	// private member function that will respawn a particle
	// public member void function that updates all current particles as well as thier attributes
	// parameters/arguments: 1. an address of a Single Particle struct object argument/parameter, 2. an IN_GAME_OBJ address argument/parameter, 
	// 3.n 2-value glm vector that represents the offset of a particle argument/parameter, if not provided, it will be set to a default position of (0.0, 0.0)
	void particle_respawn(Single_Particle& single_particle_object_argument, IN_GAME_OBJ& game_object_argument, glm::vec2 particle_offset_argument = glm::vec2(0.0f, 0.0f));

};
#endif // !GENERATE_PARTICLES_HEADER