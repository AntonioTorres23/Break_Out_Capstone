// a test fragment shader that demonstrates how we would render text onto our game screen
#version 330 core // specifies we are using the core profile of OpenGL ver. 3.3

// take in our texture coordinates by using the same type and name we used in our vertex shader but now using in

in vec2 text_texture_coordinates;

// now we specify a 4-value vector that will be our color output

out vec4 output_color;

// define a sampler2D variable that will coordinate with our texture object created in C++ source which contains the OpenGL texture object

uniform sampler2D texture_image;

// define a 3-value vector that will store a 3-value color which will assist in changing the color of the text
uniform vec3 color_of_text;

// main function; NOTICE HOW in GLSL main functions are defined as void; aka they dont return any value like a C++ main which returns 0 and is defined as int
void main()
{
	// here we create a vec4 variable that will represent a RGBA color value with the RGB portion generating a white color
	// within the Alpha value however, we take the bitmap texture and its related coordinates to get that value
	// also keep in mind that we are only getting the r value of our texture since the text will only have a white value within the bitmap
	vec4 text_data = vec4(1.0, 1.0, 1.0, texture(texture_image, text_texture_coordinates).r);
	// then for our output variable we take the color_of_text uniform variable with a alpha value of 1 (full opacity) and multiply that by the
	// prior text_data variable
	output_color = vec4(color_of_text, 1.0) * text_data;
	//output_color = vec4(color_of_sprite, 1.0);
} 



