// post processing fragment shader that takes a framebuffer object and manipulates the visuals on scrreen to be outputted on a screen-filled 2D quad via a texture
#version 330 core // specifies we are using the core profile of OpenGL ver. 3.3
// take in the framebuffer_screen_quad_texture_coordiantes from the vertex shader to use them within this fragment shader
in vec2 framebuffer_screen_quad_texture_coordiantes;
// set a output vec4 varible to represent what will be displayed on screen
out vec4 framebuffer_color_output; 

// define a uniform sampler2D variable that will represent the framebuffer object texture that will be displayed on a screen filled quad
uniform sampler2D framebuffer_object_texture;

// define a uniform vec2 array that will store 9 different glm vec2 variables set within C++ source code, this will represent the offsets for the screen-filled quad's texture coordaintes for post processing effects
uniform vec2 framebuffer_texture_coordiante_offsets[9];
/*
	define a uniform integer array that will store 9 different integer values set within C++ source code, this will represent an edge kernel (a kernel is a small matrix-like array of values centered on the current pixel that multiplies
	surrounding pixel values by its kernel values and adds them all together to form a single value) in which an edge kernel is used to highlight the edges of the texture/image

	this will be used for the chaos post processing effect, to see what this effect looks like, look up edge kernel online
*/

uniform int chaos_effect_edge_kernel[9];

/*
	define a uniform float array that will store 9 different float values set within C++ source code, this will represent a blur kernel (a kernel is a small matrix-like array of values centered on the current pixel that multiplies
	surrounding pixel values by its kernel values and adds them all together to form a single value) to blur the framebuffer texture

	this will be used for the screen shake
*/

uniform float screen_shake_effect_blur_kernel[9];

// uniform boolean variable to determine if the chaos post processing effect has occured that is sent to this variable within C++ source code
bool post_processing_effect_chaos;

// uniform boolean variable to determine if the confuse post processing effect has occured that is sent to this variable within C++ source code
bool post_processing_effect_confuse;

// uniform boolean variable to determine if the screen shake post processing effect has occured that is sent to this variable within C++ source code
bool post_processing_effect_screen_shake;

// main function; NOTICE HOW in GLSL main functions are defined as void; aka they dont return any value like a C++ main which returns 0 and is defined as int
void main()
{
	// set the inital out vec4 variable's RGBA coordinates with a value of 0
	framebuffer_color_output = vec4(0.0);
	// create a vec3 array with 9 different glm vec3 variables, this will represent all of the sampled rgb values from the framebuffer object texture and texture coordiante offsets
	vec3 post_processing_effect_samples[9];
	// if the chaos or shake effect boolean values are true, begin sampling from the texture offsets
	if (post_processing_effect_chaos || post_processing_effect_screen_shake)
	{
		// create a for loop that represents the amount of values stored within the framebuffer_texture_coordiante_offsets and post_processing_effect_samples (which in this case is 9)
		for(int array_values_iterator = 0; array_values_iterator < 9; array_values_iterator++)
			{
				// use the iterator to store the sampled texture rgb values within the corresponding index
				// same idea applies with the vec2 values stored within framebuffer_texture_coordiante_offsets, but we also add the value of the offsets with the x and y (s and t when dealing with texture coordinates) 
				// of the framebuffer_screen_quad_texture_coordiantes to sample a specific area of the screen sized framebuffer object texture
				post_processing_effect_samples[array_values_iterator] = vec3(texture(framebuffer_object_texture, framebuffer_screen_quad_texture_coordiantes.st + framebuffer_texture_coordiante_offsets[array_values_iterator]));
			}
	}
	// if chaos uniform boolean statment is true, begin processing and displaying that effect on screen
	if (post_processing_effect_chaos)
	{
		 // create a for loop that represents the amount of values stored within the chaos_effect_edge_kernel and post_processing_effect_samples (which in this case is 9)
		 for(int array_values_iterator = 0; array_values_iterator < 9; array_values_iterator++)
		 {
			/*
				output the chaos effect to the screen by taking the out vec4 framebuffer_color_output variable and use a compound addition operator to take the product of the post_processing_effect_samples
				and the chaos_effect_edge_kernel with thier respective indexes provided by the array_values_iterator

				also for the alpha value we set it to a value of 0 for now
			*/
			framebuffer_color_output += vec4(post_processing_effect_samples[array_values_iterator] * chaos_effect_edge_kernel[array_values_iterator], 0.0f);
			// set the framebuffer_color_output's alpha value to full opacity (a value of 1.0f)
			framebuffer_color_output.a = 1.0f;
		 }
	}

	// else if confuse uniform boolean statement is true, simply flip the colors of a regular texture's rgb values with an alpha value of 1.0
	else if (post_processing_effect_confuse)
	{
		framebuffer_color_output = vec4(1.0 - texture(framebuffer_object_texture, framebuffer_screen_quad_texture_coordiantes).rgb, 1.0);
	}

	// else if screen shake uniform boolean statment is true, begin processing and displaying that effect on screen
	else if (post_processing_effect_screen_shake)
	{
		 // create a for loop that represents the amount of values stored within the screen_shake_effect_blur_kernel and post_processing_effect_samples (which in this case is 9)
		 for(int array_values_iterator = 0; array_values_iterator < 9; array_values_iterator++)	
		 {
			/*
				output the chaos effect to the screen by taking the out vec4 framebuffer_color_output variable and use a compound addition operator to take the product of the post_processing_effect_samples
				and the screen_shake_effect_blur_kernel with thier respective indexes provided by the array_values_iterator

				also for the alpha value we set it to a value of 0 for now
			*/
			framebuffer_color_output += vec4(post_processing_effect_samples[array_values_iterator] * screen_shake_effect_blur_kernel[array_values_iterator], 0.0f);
			// set the framebuffer_color_output's alpha value to full opacity (a value of 1.0f)
			framebuffer_color_output.a = 1.0f;
		 }
	}

	// if none of these uniform boolean statments are true, then render the framebuffer object texture on screen as normal with no post processing effects
	else
	{
		framebuffer_color_output = texture(framebuffer_object_texture, framebuffer_screen_quad_texture_coordiantes);
	}

}