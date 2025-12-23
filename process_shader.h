#ifndef PROCESS_SHADER_HEADER
#define PROCESS_SHADER_HEADER


#include <string> // include C++ string library

#include <glad/glad.h> //include GLAD; a lib that loads the addresses of OpenGL function pointer
#include <glm/glm.hpp> // include glm for uniform vector/matrix sets
#include <glm/gtc/type_ptr.hpp> // enable type pointers for glm vectors/matrices and such

// shader class template that is able to compile shaders from external files and convert them into a shader program
// as well as allow for uniform sets to provided shader program
class SHADER_OBJ
{
public: // all public members/functions
	// shader ID 
	unsigned int Shader_ID;
	// class constructor
	SHADER_OBJ() {}
	// activate the current shader program with the activate function wherever it is located within memory
	SHADER_OBJ& Activate();

	// compile shader from external source code files
	// we use char pointers to store the address in memory of our arguments provided in this function
	void Compile_Shaders(const char* vertexSourceCode, const char* fragmentSourceCode, const char* geometrySourceCode = nullptr);	
	// uniform setting functions
	void	uniform_float(const char* uniform_variable_name, float uniform_value, bool activateShader = false);
	void	uniform_integer(const char* uniform_variable_name, int uniform_value, bool activateShader = false);
	// set vector as constant to prevent address of vector from changing within function
	void	uniform_vector_2(const char* uniform_variable_name, const glm::vec2 &uniform_value, bool activateShader = false);
	// overloaded function if user wants to use singular float values for vector coordinates instead of a glm vector value
	void	uniform_vector_2(const char* uniform_variable_name, float x_coordinate, float y_coordinate, bool activateShader = false);
	// set vector as constant to prevent address of vector from changing within function
	void	uniform_vector_3(const char* uniform_variable_name, const glm::vec3 &uniform_value, bool activateShader = false);
	// overloaded function if user wants to use singular float values for vector coordinates instead of a glm vector value
	void	uniform_vector_3(const char* uniform_variable_name, float x_coordinate, float y_coordinate, float z_coordinate, bool activateShader = false);
	// set vector as constant to prevent address of vector from changing within function
	void	uniform_vector_4(const char* uniform_variable_name, const glm::vec4& uniform_value, bool activateShader = false);
	// overloaded function if user wants to use singular float values for vector coordinates instead of a glm vector value
	void	uniform_vector_4(const char* uniform_variable_name, float x_coordinate, float y_coordinate, float z_coordinate, float w_coordinate, bool activateShader = false);
	// set vector as constant to prevent address of matrix from changing within function
	void	uniform_matrix_4(const char* uniform_variable_name, const glm::mat4& uniform_value, bool activateShader = false);

private: // all private members/functions that aren't accessible outside of the class
	
	// validate shader source code linking or compile errors
	void	Validate_Shader_Errors(unsigned int shader, std::string shader_type);
};

#endif // !PROCESS_SHADER_HEADER

