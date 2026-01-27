// post processing vertex shader that takes a framebuffer object and manipulates the visuals on scrreen to be outputted on a screen-filled 2D quad via a texture 
#version 330 core // specifies we are using the core profile of OpenGL ver. 3.3

// our vbo attribute pointer location which contains both the screen-filled quad vertex data as well as its texture coordinates for the frame buffer object texture
layout (location = 0) in vec4 framebuffer_quad_data; 

// we want to send our texture coordinates out to the fragment shader for processing
out vec2 framebuffer_screen_quad_texture_coordiantes;

// uniform boolean variable to determine if the chaos post processing effect has occured that is sent to this variable within C++ source code
bool post_processing_effect_chaos;

// uniform boolean variable to determine if the confuse post processing effect has occured that is sent to this variable within C++ source code
bool post_processing_effect_confuse;

// uniform boolean variable to determine if the screen shake post processing effect has occured that is sent to this variable within C++ source code
bool post_processing_effect_screen_shake;

// uniform float variable to indicate the time of the post processing that is sent to this variable within C++ source code
float post_processing_time;

// main function; NOTICE HOW in GLSL main functions are defined as void; aka they dont return any value like a C++ main which returns 0 and is defined as int
void main()
{
	
	/* 	
	 gl_Position automatically does perspective division (divide z coordinate by w coordinate) as well as calculates coordinates into Normalized Device Coordinates, i.e. coordinates in range of [-1, 1]
	 You may see that our raw vertex data is already in Normalized Device Coordiante Format
	 
	 in addition, we put our x and y values from sprite_vertex_data into a 4-value vector in which vertex.xy are the x and y values, the z value is 0.0 since we are not doing using 3D, and a homogenous value
	 of 1.0 for transformations, scaling, rotating with the transformation matrix

	 since we are using a screen-filled quad frame buffer object we don't need to set any matrices for transformations, or viewing, or projection

	*/
	gl_Position = vec4(framebuffer_quad_data.xy, 0.0f, 1.0f);

	/* 
	send our texture coordintes to the fragment shader by setting our corresponding variable to the values .zw that are within our 
	vector 4 variable

	this is how we set up our framebuffer_quad_data with the x and y being positional data and z and w being our texture coordinates
	*/

	vec2 texture_coordiantes_in_vertex_shader = framebuffer_quad_data.zw;  
	

	// if chaos uniform boolean statement is true, create a chaos strength variable that will store a constant float value for the offset of the texture coordinates
	if (post_processing_effect_chaos)
	{
		float chaos_strength = 0.3;
		// create a new 2-value vector variable that will store an offset of the pre-existing texture coordinate value by taking the
		// sum of the texture_coordiantes_in_vertex_shader's x-coordinate and sin value of post_processing_time (to keep the time value clamped at -1 and 1) multiplied by the strength value
		// same concept applies to the texture_coordiantes_in_vertex_shader's y-coordinate however this time we use the cos value of post_processing_time (to keep the time value clamped at -1 and 1)
		vec2 texture_chaos_coordiante_offset = vec2(texture_coordiantes_in_vertex_shader.x + sin(post_processing_time) * chaos_strength, texture_coordiantes_in_vertex_shader.y + cos(post_processing_time) * chaos_strength);
		// apply the calculated texture_chaos_coordiante_offset to framebuffer_screen_quad_texture_coordiantes out vec2 variable
		framebuffer_screen_quad_texture_coordiantes = texture_chaos_coordiante_offset;
	}
	// else if confuse uniform boolean statement is true, create an offset within the texture_coordiantes_in_vertex_shader's x and y values
	else if (post_processing_effect_confuse)
	{
		// within the texture_coordiantes_in_vertex_shader, flip the texture coordiantes backwards by subtracting 1 from both the x and y values
		// send these values to the out vec2 variable
		framebuffer_screen_quad_texture_coordiantes = vec2(texture_coordiantes_in_vertex_shader.x - 1.0, texture_coordiantes_in_vertex_shader.y - 1.0);
	}
	// if no uniform boolean statments are true, set the out vec2 texture coordiantes variable as normal
	else
	{
		framebuffer_screen_quad_texture_coordiantes =  texture_coordiantes_in_vertex_shader;
	}
	// if shake uniform boolean value is true, offset the positional coordiantes of the quad within gl_Position
	if (shake)
	{
		// create a screen_shake_strength variable that will store a constant float value to offset the positional coordinates
		float screen_shake_strength = 0.01;
		// use a compound addition operator on the x positional coordiante value with the cos of post_processing_time times 10 multiplied by screen_shake_strength
		// we use a cos value to keep the time value clamped at -1 and 1
		gl_Position.x += cos(post_processing_time * 10) * screen_shake_strength;
		// use a compound addition operator on the x positional coordiante value with the cos of post_processing_time times 15 multiplied by screen_shake_strength
		// we use a cos value to keep the time value clamped at -1 and 1
		gl_Position.y += cos(post_processing_time * 15) * screen_shake_strength;
	}
}