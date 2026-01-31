#ifndef POST_PROCESSING_HEADER
#define POST_PROCESSING_HEADER

#include <glad/glad.h>  // include GLAD; a lib that loads the addresses of OpenGL function pointers
#include <glm/glm.hpp> // include glm, a library that allows for us to create OpenGL vectors, matrices, etc.


#include "process_texture.h" // include our process_texture header file to use the TEXTURE_2D_OBJ here
#include "process_shader.h" // include our process_shader header file to use the SHADER_OBJ here
#include "render_sprite.h" // include render_sprite header file to allow for RENDER_SPRITE_OBJ to be used here


// class template for our post processing object
class POST_PROCESSING_OBJ
{
public: // all of our public data/function members

	// a public SHADER_OBJ data member that stores the post processing shader
	SHADER_OBJ Post_Processing_Shader_Object;
	// a public TEXTURE_2D_OBJ data member that stores the post processing frame buffer object texture for the screen-filled quad
	TEXTURE_2D_OBJ Post_Processing_Texture_Object;

	// public data members that store the width and height of the screen I believe
	unsigned int Post_Processing_Width, Post_Processing_Height;

	// 3 public boolean data members that store the state of the effects defined in the post processing shaders (i.e. confuse, chaos, screen shake)
	bool Confuse_Effect, Chaos_Effect, Screen_Shake_Effect; 

	// constructor of POST_PROCESSING_OBJ that takes a shader object, unsigned integer width, and unsigned int height as parameters/arguments
	POST_PROCESSING_OBJ(SHADER_OBJ shader_object_argument, unsigned int width_argument, unsigned int height_argument);

	// public void function that sets up the framebuffer operations prior to the game rendering
	void Start_Render();

	// public void function that stores all the framebuffer object data gathered after rendering the game into a texture object
	void Finish_Render();
	
	// public void function that renders the framebuffer object texture onto a screen-filled quad; takes post processing time as an argument
	void Render_Post_Processing(float post_processing_time);

private: // all of our private data/function members

	// private data members that stores both the multi sampled frame buffer object and a regular framebuffer object
	// a multi-sampled frame buffer object is a frame buffer object that supports multi-sampled anti aliasing which helps prevent that pixely look
	// the regular framebuffer object is used to blit the multi-sampled color-buffer over to the texture that stores the results
	// blitting means to copy a block of pixels from the read frame buffer (the multi-sampled framebuffer object) to the draw/write framebuffer (regular framebuffer object)
	// remember we used the unsigned int data type to create a lot of OpenGL Objects
	unsigned int multi_sampled_framebuffer_object, framebuffer_object;
	// private data member that stores a render buffer object
	// a render buffer object is similar to storing a texture for a framebuffer object attachment, however, a render buffer object is an actual buffer that cannot be directly read from
	// due to render buffer object's data being in a native format and not having to be converted into a texture format, it makes them much faster to write to
	unsigned int renderbuffer_object;
	// private data member that stores the screen-filled quad's vertex array object
	unsigned int post_processing_vertex_array_object;

	// private void function that will begin initializing the vertex data for the screen-filled quad that will display the framebuffer object texture
	void vertex_data_initialze();

}; 

#endif // !POST_PROCESSING_HEADER

