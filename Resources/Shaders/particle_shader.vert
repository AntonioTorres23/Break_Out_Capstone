// a vertex shader that render particles into our game screen
#version 330 core // specifies we are using the core profile of OpenGL ver. 3.3

// our vbo attribute pointer location which contains both the position of the sprite as well as its texture coordinates
layout (location = 0) in vec4 sprite_vertex_data; 

// we want to send our texture coordinates out to the fragment shader for processing
out vec2 sprite_texture_coordinates;
// we also want to send our uniform values from the single particle structure to the fragment shader
out vec4 color_of_particle;


// ONLY NEED THE PROJECTION MATRIX FOR THIS SHADER NO TRANSFORMATION MATRIX
// also define a 4x4 projection matrix in which our orthographic projection matrix in C++ source is sent to this variable via a uniform
uniform mat4 sprite_orthographic_projection_matrix;

// define a two-value glm vector that will represent the positional offset of the particle in which our glm vector in C++ is sent to this variable via a uniform
uniform vec2 offset_of_particle; 
// define a four-value glm vector that will represent the color of the particle in which our glm vector in C++ is sent to this variable via a uniform
uniform vec4 particle_color; 

// main function; NOTICE HOW in GLSL main functions are defined as void; aka they dont return any value like a C++ main which returns 0 and is defined as int
void main()
{
	/* 
	send our texture coordintes to the fragment shader by setting our corresponding variable to the values .zw that are within our sprite_vertex_data
	vector 4 variable

	this is how we set up our sprite_vertex_data with the x and y being positional data and z and w being our texture coordinates
	*/

	sprite_texture_coordinates = sprite_vertex_data.zw;

	// set a float value which will represent the scale of a particle since we have no transformation matrix
	float scale_of_particle = 10.0f; 

	// set the outgoing color_of_particle variable to the value of the particle_color uniform variable
	color_of_particle = particle_color;

	/* 	
	 gl_Position automatically does perspective division (divide z coordinate by w coordinate) as well as calculates coordinates into Normalized Device Coordinates, i.e. coordinates in range of [-1, 1]
	 You may see that our raw vertex data is already in Normalized Device Coordiante Format
	 
	 in addition, we put our x and y values from sprite_vertex_data into a 4-value vector in which vertex.xy are the x and y values, the z value is 0.0 since we are not doing using 3D, and a homogenous value
	 of 1.0 for transformations, scaling, rotating with the transformation matrix

	 this brings us to our multiplication where we multiply this 4-value vector with our sprite positional data with the transformation matrix and the projection matrix. It goes from right to left in this exact order:

	 Projection/Perspective Matrix * View Matrix * Transformation Matrix * vertex data

	 It has to match this order or else there will be improper data due to the way the 4x4 matrices calculate our vertex data

	 Also note how we don't need a view matrix for the break out game due to it using the full screen but in another type of game you would need it
	*/

	// here we multiply the x and y Normalized Device Coordinate values of sprite_vertex_data by the scale_of_particle value as well as add the vec2 offset_of_particle to the resulting product
	gl_Position = sprite_orthographic_projection_matrix * vec4((sprite_vertex_data.xy * scale_of_particle) + offset_of_particle, 0.0, 1.0);
}





