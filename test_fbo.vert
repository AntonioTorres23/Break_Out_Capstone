// post processing vertex shader that takes a framebuffer object and manipulates the visuals on scrreen to be outputted on a screen-filled 2D quad via a texture 
#version 330 core // specifies we are using the core profile of OpenGL ver. 3.3

// our vbo attribute pointer location which contains both the screen-filled quad vertex data as well as its texture coordinates for the frame buffer object texture
layout (location = 0) in vec4 framebuffer_quad_data; 

// we want to send our texture coordinates out to the fragment shader for processing
out vec2 framebuffer_screen_quad_texture_coordiantes;



void main()
{
gl_Position = vec4(framebuffer_quad_data.xy, 0.0f, 1.0f);

/* 
send our texture coordintes to the fragment shader by setting our corresponding variable to the values .zw that are within our 
vector 4 variable

this is how we set up our framebuffer_quad_data with the x and y being positional data and z and w being our texture coordinates
*/

vec2 texture_coordiantes_in_vertex_shader = framebuffer_quad_data.zw; 

framebuffer_screen_quad_texture_coordiantes = texture_coordiantes_in_vertex_shader;

}