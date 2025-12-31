#include <glad/glad.h>  // include GLAD; a lib that loads the addresses of OpenGL function pointers
#include <GLFW/glfw3.h> // include glfw3; a lib that ties OpenGL to a window and callback functions within a window

#include "game_logic.h"       // include game_logic header to allow us to create a GAME_OBJ and access enumerations of the game state
#include "resource_manager.h" // include resourrce_manager header file to load/create textures and shaders as well as the static functions that are associated with the class 

#include <iostream> // include iostream to send default output to the terminal

// prototype call-back glfw functions; baically you can call these whatever you want and then tie them to actual GLFW lib callback-functions along with the GLFWwindow pointer variable you created

/* 
   this function contains a glfw window pointer parameter, the width of the window, and the height of the window
   the idea is this, you tie this function to the glfw framebuffer size callback function and whenever the window 
   is resized by a user, it will set the glviewport to those new window dimensions
*/
void glfw_callback_window_resize(GLFWwindow* glfw_window_argument, int width_resize_of_window, int height_resize_of_window);

/*
	this function contains a glfw window pointer parameter, an input (or key that was pressed) parameter, an input_scan_code parameter, an action parameter, and a mode parameter
	the idea is you tie this function to the glfw set key callback function and if the glfw window detects any key is pressed by the user, the function we created will go through
	and process the logic that is defined within the glfw_callback_keyboard_input function  

	each argument after the window parameter ties to a "key property" that is grabbed from the glfw set key call back function.

	input: the key that was pressed or released by the user

	input_scan_code : the system-specific scancode of a key; basically it is a unchanging number that represents the physical location of the key on a keyboard
	and not the character. Meaning it will get the actual location of the key on the keyboard, i.e. the position of 'd' will never change depending on the language
	layout that the user chooses for thier OS. 
	
	Think of this like a MAC Address rather than an IP Address, a computer's IP address can change due to many factors but a computer's NIC MAC Address will never
	change (for the most part unless you like change the NIC).

	input_action: detects the action of the key that was just pressed; states like GLFW_PRESS, GLFW_RELEASE, and GLFW_REPEAT are some of the states that a key can be in. 
	
	input_mode: this detects which "modifier keys" where held down, these include the shift key, control key, caps lock key, alt key, num lock key, etc. 
*/
void glfw_callback_keyboard_input(GLFWwindow* glfw_window_argument, int input, int input_scan_code, int input_action, int input_mode);

// create a constant global variable that stores the desired width of the screen
const unsigned int WIDTH_OF_SCREEN = 1200;
// create a constant global variable that stores the desired height of the screen
const unsigned int HEIGHT_OF_SCREEN = 1200;

// create a GAME_OBJ object that contains the WIDTH_OF_SCREEN and HEIGHT_OF_SCREEN as its constructor parameters
GAME_OBJ game_breakout(WIDTH_OF_SCREEN, HEIGHT_OF_SCREEN);

int main(int integer_arg, char *character_c_string_arg[]) // main function of C++; take in two arguments, a integer argument, and a char pointer array argument (essetially a c-string pointer) 
{
	// initalize glfw 
	glfwInit();
	// provide window hints to glfw to let it know what version of OpenGL we are working in (OpenGL ver. 3.30 aka 3.3)
	// this specifies the major version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// this specifies the minor version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// tell glfw that we are only using the core profile of OpenGL and not other profiles
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// tell glfw if the windowed mode window can be resized by the user, this is set to false
	glfwWindowHint(GLFW_RESIZABLE, false);

	// create the window pointer variable which takes our screen dimension global constant variables as well as a string for the name of the window in our OS
	GLFWwindow* glfw_window = glfwCreateWindow(WIDTH_OF_SCREEN, HEIGHT_OF_SCREEN, "Breakout Capstone", nullptr, nullptr); 
	// this makes the window specified within the function the current window within the calling thread; what I believe this means is that anything that is drawn,
	// bound, and or done within OpenGL and GLFW will end up on this specific window. 
	glfwMakeContextCurrent(glfw_window);

	// load all OpenGL function pointer with GLAD, use this statement to intalize glad, if it comes back as false, we send default output to the screen saying a failure has occured
	// the gladLoadGLLoader function is a int value that I think is what we are using as our boolean statment value; the GLADloadproc is a void function that does what the name specifies
	// and the glfwGetProcAddress attempts to get the location of the GLADloadproc function
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Cannot Initalize GLAD: Terminating Application Now" << std::endl; 
		// return standard error and end the main function (return -1)
		return -1; 
	}
	return 0;
}