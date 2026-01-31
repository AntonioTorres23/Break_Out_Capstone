// post processing fragment shader that takes a framebuffer object and manipulates the visuals on scrreen to be outputted on a screen-filled 2D quad via a texture 
#version 330 core // specifies we are using the core profile of OpenGL ver. 3.3
// take in the framebuffer_screen_quad_texture_coordiantes from the vertex shader to use them within this fragment shader
in vec2 framebuffer_screen_quad_texture_coordiantes;
// set a output vec4 varible to represent what will be displayed on screen
out vec4 framebuffer_color_output; 

// define a uniform sampler2D variable that will represent the framebuffer object texture that will be displayed on a screen filled quad
//uniform sampler2D framebuffer_object_texture;

void main()
{
	framebuffer_color_output = vec4(1.0, 0.0, 0.0, 1.0);
}