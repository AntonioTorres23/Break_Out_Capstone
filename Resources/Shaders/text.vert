// a test vertex shader that demonstrates how we would render text onto our game screen
#version 330 core // specifies we are using the core profile of OpenGL ver. 3.3

// our vbo attribute pointer location which contains both the position of the text as well as its texture coordinates
layout (location = 0) in vec4 text_vertex_data; 

// we want to send our texture coordinates out to the fragment shader for processing
out vec2 text_texture_coordinates;

// also define a 4x4 projection matrix in which our orthographic projection matrix in C++ source is sent to this variable via a uniform
uniform mat4 text_orthographic_projection_matrix;

// main function; NOTICE HOW in GLSL main functions are defined as void; aka they dont return any value like a C++ main which returns 0 and is defined as int
void main()
{
	/* 
	send our texture coordintes to the fragment shader by setting our corresponding variable to the values .zw that are within our text_vertex_data
	vector 4 variable

	this is how we set up our text_vertex_data with the x and y being positional data and z and w being our texture coordinates
	*/

	text_texture_coordinates = text_vertex_data.zw;

	/* 	
	 gl_Position automatically does perspective division (divide z coordinate by w coordinate) as well as calculates coordinates into Normalized Device Coordinates, i.e. coordinates in range of [-1, 1]
	 You may see that our raw vertex data is already in Normalized Device Coordiante Format
	 
	 in addition, we put our x and y values from text_vertex_data into a 4-value vector in which vertex.xy are the x and y values, the z value is 0.0 since we are not doing using 3D, and a homogenous value
	 of 1.0 for transformations, scaling, rotating with the transformation matrix

	 this brings us to our multiplication where we multiply this 4-value vector with our text positional data with the projection matrix. It goes from right to left in this exact order:

	 Projection/Perspective Matrix * View Matrix * Transformation Matrix * vertex data

	 It has to match this order or else there will be improper data due to the way the 4x4 matrices calculate our vertex data

	 Also note how we don't need a view matrix for the break out game due to it using the full screen but in another type of game you would need it
	*/

	gl_Position = text_orthographic_projection_matrix * vec4(text_vertex_data.xy, 0.0, 1.0); 
}





