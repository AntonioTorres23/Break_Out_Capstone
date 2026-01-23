// a test fragment shader that renders particles into our game screen
#version 330 core // specifies we are using the core profile of OpenGL ver. 3.3

// take in our texture coordinates by using the same type and name we used in our vertex shader but now using in
in vec2 sprite_texture_coordinates;

// take in our particle color by using the same type and name we used in our vertex shader but now using in
in vec4 color_of_particle;
// now we specify a 4-value vector that will be our color output
out vec4 output_color;

// define a sampler2D variable that will coordinate with our texture object created in C++ source which contains the OpenGL texture object
uniform sampler2D texture_image;


// main function; NOTICE HOW in GLSL main functions are defined as void; aka they dont return any value like a C++ main which returns 0 and is defined as int
void main()
{
	// use our output_color variable to output the texture_image with our texture coordinates and our color_of_particle to change the overall RGBA tone through multiplication
	output_color = (texture(texture_image, sprite_texture_coordinates) * color_of_particle);
} 



