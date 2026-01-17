#ifndef GAME_LOGIC_HEADER // if game logic header doesn't exist
#define GAME_LOGIC_HEADER // define game logic header

#include <glad/glad.h>  // include GLAD; a lib that loads the addresses of OpenGL function pointers
#include <GLFW/glfw3.h> // include glfw3; a lib that ties OpenGL to a window and callback functions within a window
#include <vector>		// don't get this confused with a glm vector, this is a standard library that allows us to organize and store variables/objects in a more efficient way; it has methods that make parsing through this data much easier than something like an array 
#include <tuple>		// include tuple, a standard librarry that allows for the creation of tuples, a FIXED-sized collection of various values

#include "game_level_logic.h" // include the game_level_logic header file to access the GAME_LEVEL_OBJ class 

// Enumerations that represent the current state that the game is in; I.E. ACTIVE, MENU, WINDOW
enum CurrentStateOfGame
{
	ACTIVE_GAME,
	MENU_GAME,
	WINDOW_GAME
};

// Enumerations that represent the ball's bouncing direction off an object's AABB once a collision occurs
enum BallBounceDirection
{
	DIRECTION_UP,
	DIRECTION_RIGHT,
	DIRECTION_DOWN,
	DIRECTION_LEFT
};

// a tuple that will store everything needed to calculate a collision as well as resolution of the collision (physics like the ball bouncing off of tiles/blocks/bricks
// we use the typedef keyword to define the tuple provided as a data type to use for things like functions or creating objects/vars based on the tuple
// the tuple takes 3 parameters: a boolean value to see if a collision is true or false, a BallBounceDirection enumeration to see where the ball bounced off an AABB, and a 2-value GLM vector to store the position of the ball upon collision (difference vector)
typedef std::tuple<bool, BallBounceDirection, glm::vec2> BallCollision; 

// Constant variables that store the player's scale size and player's velocity for use in the game_logic c++ file

const glm::vec2 SCALE_SIZE_OF_PLAYER(100.0f, 20.0f);

const float PLAYER_PHYSICS_VELOCITY(500.0f);

// Constant 2-value glm vector variable that stores the BALL_OBJECT_PHYSICS_VELOCITY to be the initial velocity value upon creating a GAME_BALL_OBJ
const glm::vec2 BALL_OBJECT_PHYSICS_VELOCITY(100.0f, -350.0f);

// Constant float variable that stores the BALL_OBJECT_RADIUS to be the inital radius value upon create a GAME_BALL_OBJ
const float BALL_OBJECT_RADIUS = 12.5f;


// Game class that stores all game related states/functionality
class GAME_OBJ
{
public: // all public members/functions
	
	
	CurrentStateOfGame	Game_State; // current state of game enumeration stored in Game_State var
	bool				Key_Pressed_Buffer[1024]; // buffer that stores key's that are pressed by player
	unsigned int		Width_Of_Screen, Height_Of_Screen; // stores the width and height of the actual game window

	std::vector<GAME_LEVEL_OBJ> Game_Levels; // stores all of our levels within a standard lib vector that take the GAME_LEVEL_OBJ as its data type

	unsigned int Game_Level; // index that specifies a specific level in particular?
	
	GAME_OBJ(unsigned int width_of_window, unsigned int height_of_window); // constructor with arguments required to create a GAME_OBJ object
	~GAME_OBJ(); // deconstructor of GAME_OBJ
	// initalize the current state of the game (load all the shaders, textures, and levels)
	void Initalize_Game();
	// game loops
	void Process_User_Input(float delta_time); // function that processes keys/movement done by player
	
	void Update_Game(float delta_time); // updates game to reflect prior user/movement and state of ball

	void Render_Game();                 // renders the game on the players screen

	void Axis_Aligned_Bounding_Box_Collisions(); // function that processes game behavior if a collision occurs with two objects 

	void Level_Reset(); // function that resets level after player has lost ball

	void Player_Reset(); // function that resets player position after player has lost ball

};


#endif // !GAME_LOGIC_HEADER

