#ifndef GAME_LOGIC_HEADER // if game logic header doesn't exist
#define GAME_LOGIC_HEADER // define game logic header

#include <glad/glad.h>  // include GLAD; a lib that loads the addresses of OpenGL function pointers
#include <GLFW/glfw3.h> // include glfw3; a lib that ties OpenGL to a window and callback functions within a window

// Enumerations that represent the current state that the game is in; I.E. ACTIVE, MENU, WINDOW
enum CurrentStateOfGame
{
	ACTIVE_GAME,
	MENU_GAME,
	WINDOW_GAME
};

// Game class that stores all game related states/functionality
class GAME_OBJ
{
public: // all public members/functions
	
	
	CurrentStateOfGame	Game_State; // current state of game enumeration stored in Game_State var
	bool				Key_Pressed_Buffer[1024]; // buffer that stores key's that are pressed by player
	unsigned int		Width_Of_Screen, Height_Of_Screen; // stores the width and height of the actual game window
	
	GAME_OBJ(unsigned int width_of_window, unsigned int height_of_window); // constructor with arguments required to create a GAME_OBJ object
	~GAME_OBJ(); // deconstructor of GAME_OBJ
	// initalize the current state of the game (load all the shaders, textures, and levels)
	void Initalize_Game();
	// game loops
	void Process_User_Input(float delta_time); // function that processes keys/movement done by player
	
	void Update_Game(float delta_time); // updates game to reflect prior user/movement and state of ball

	void Render_Game();                 // renders the game on the players screen



};


#endif // !GAME_LOGIC_HEADER

