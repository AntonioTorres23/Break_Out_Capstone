#include "process_shader.h" // include the process shader header file

#include <iostream> // include iostream for default output

/*
NOTICE HOW WE'RE DEFINING THE FUNCTIONS THAT ARE STORED WITHIN THE SHADER_OBJ CLASS/OBJECT, SO IN THE C++ FILE HERE WE'RE DEFINING THE ACTUAL SOURCE CODE
THE FUNCTIONS THAT ARE STORED IN THE HEADER FILE ARE REALLY ONLY PROTOTYPES
*/
// define activate shader member function by using the address of the current shader object and then calling the data method shader_ID of the current shader object using the this pointer and then using the arrow pointer to both call the value pointer as well as the dot to call a method function
SHADER_OBJ &SHADER_OBJ::Activate()
{
	// use this pointing to use the data member titled Shader_ID that is actually within this class object
	glUseProgram(this->Shader_ID);
	// return the entire object 
	return *this;
}

// define compile shaders function
void SHADER_OBJ::Compile_Shaders(const char* vertexSourceCode, const char* fragmentSourceCode, const char* geometrySourceCode)
{
	// create 3 unsigned int variables that will be our shaders
	unsigned int vertex_Shader, fragment_Shader, geometry_Shader;
	// create vertex shader
	vertex_Shader = glCreateShader(GL_VERTEX_SHADER);
	// add vertex shader source code to vertex_Shader
	glShaderSource(vertex_Shader, 1, &vertexSourceCode, NULL);
	// compile shader
	glCompileShader(vertex_Shader);
	// check for compile errors in vertex shader
	Validate_Shader_Errors(vertex_Shader, "VERTEX");
	
	// create fragment shader
	fragment_Shader = glCreateShader(GL_FRAGMENT_SHADER);
	// add vertex shader source code to fragment_Shader
	glShaderSource(fragment_Shader, 1, &fragmentSourceCode, NULL);
	// compile shader
	glCompileShader(fragment_Shader);
	// check for compile errors in fragment shader
	Validate_Shader_Errors(fragment_Shader, "FRAGMENT");

	// create and compile a geometry shader if source code is not equal to nullptr
	if (geometrySourceCode != nullptr)
	{
		// create geometry shader
		geometry_Shader = glCreateShader(GL_GEOMETRY_SHADER);
		// add vertex shader source code to geometry_Shader
		glShaderSource(geometry_Shader, 1, &geometrySourceCode, NULL);
		// compile shader
		glCompileShader(geometry_Shader);
		// check for compile errors in geometry shader
		Validate_Shader_Errors(geometry_Shader, "GEOMETRY");
	}
	// create shader program with data member shader_id stored within SHADER_OBJ
	this->Shader_ID = glCreateProgram();
	// attach vertex shader to program
	glAttachShader(this->Shader_ID, vertex_Shader);
	// attach fragment shader to program
	glAttachShader(this->Shader_ID, fragment_Shader);
	// if geometry shader source code is not equal to nullptr, attach it to the shader program as well
	if (geometrySourceCode != nullptr)
	{
		glAttachShader(this->Shader_ID, geometry_Shader);
	}
	// link all shaders together within this shader program
	glLinkProgram(this->Shader_ID);
	// check if there are any linking errors within the shader program
	Validate_Shader_Errors(this->Shader_ID, "PROGRAM");
	// delete shaders since they are linked to the Shader_ID program now so we don't need them anymore
	glDeleteShader(vertex_Shader);
	glDeleteShader(fragment_Shader);
	// if geometry source code is not a nullptr, delete the geometry shader as well
	if (geometrySourceCode != nullptr)
	{
		glDeleteShader(geometry_Shader);
	}
}

// define our uniform float function
void	SHADER_OBJ::uniform_float(const char* uniform_variable_name, float uniform_value, bool activateShader)
{
	// if activateShader is true, activate the shader program by using our member function
	if (activateShader)
	{
		this->Activate();
	}
	// look up the name of the uniform in the shader program that was provided and set the corresponding value specified in the arguments
	glUniform1f(glGetUniformLocation(this->Shader_ID, uniform_variable_name), uniform_value);
	

}
// define our uniform int function
void	SHADER_OBJ::uniform_integer(const char* uniform_variable_name, int uniform_value, bool activateShader)
{
	// if activateShader is true, activate the shader program by using our member function
	if (activateShader)
	{
		this->Activate();
	}
	// look up the name of the uniform in the shader program that was provided and set the corresponding value specified in the arguments
	glUniform1i(glGetUniformLocation(this->Shader_ID, uniform_variable_name), uniform_value);
}

// define our uniform vector 2 function
// set vector as constant to prevent address of vector from changing within function
void	SHADER_OBJ::uniform_vector_2(const char* uniform_variable_name, const glm::vec2& uniform_value, bool activateShader)
{
	// if activateShader is true, activate the shader program by using our member function
	if (activateShader)
	{
		this->Activate();
	}
	// look up the name of the uniform in the shader program that was provided and set the corresponding value specified in the arguments
	glUniform2f(glGetUniformLocation(this->Shader_ID, uniform_variable_name), uniform_value.x, uniform_value.y);
}
// define our uniform vector 2 function
// overloaded function if user wants to use singular float values for vector coordinates instead of a glm vector value
void	SHADER_OBJ::uniform_vector_2(const char* uniform_variable_name, float x_coordinate, float y_coordinate, bool activateShader)
{ 
	// if activateShader is true, activate the shader program by using our member function
	if (activateShader)
	{
		this->Activate();
	}
	// look up the name of the uniform in the shader program that was provided and set the corresponding value specified in the arguments
	glUniform2f(glGetUniformLocation(this->Shader_ID, uniform_variable_name), x_coordinate, y_coordinate);
}
// define our uniform vector 3 function
// set vector as constant to prevent address of vector from changing within function
void	SHADER_OBJ::uniform_vector_3(const char* uniform_variable_name, const glm::vec3& uniform_value, bool activateShader)
{
	// if activateShader is true, activate the shader program by using our member function
	if (activateShader)
	{
		this->Activate();
	}
	// look up the name of the uniform in the shader program that was provided and set the corresponding value specified in the arguments
	glUniform3f(glGetUniformLocation(this->Shader_ID, uniform_variable_name), uniform_value.x, uniform_value.y, uniform_value.z);
}
// define our uniform vector 3 function
// overloaded function if user wants to use singular float values for vector coordinates instead of a glm vector value
void	SHADER_OBJ::uniform_vector_3(const char* uniform_variable_name, float x_coordinate, float y_coordinate, float z_coordinate, bool activateShader)
{
	// if activateShader is true, activate the shader program by using our member function
	if (activateShader)
	{
		this->Activate();
	}
	// look up the name of the uniform in the shader program that was provided and set the corresponding value specified in the arguments
	glUniform3f(glGetUniformLocation(this->Shader_ID, uniform_variable_name), x_coordinate, y_coordinate, z_coordinate);
}
// define our uniform vector 4 function
// set vector as constant to prevent address of vector from changing within function
void	SHADER_OBJ::uniform_vector_4(const char* uniform_variable_name, const glm::vec4& uniform_value, bool activateShader)
{
	// if activateShader is true, activate the shader program by using our member function
	if (activateShader)
	{
		this->Activate();
	}
	// look up the name of the uniform in the shader program that was provided and set the corresponding value specified in the arguments
	glUniform4f(glGetUniformLocation(this->Shader_ID, uniform_variable_name), uniform_value.x, uniform_value.y, uniform_value.z, uniform_value.w);
}
// define our uniform vector 4 function
// overloaded function if user wants to use singular float values for vector coordinates instead of a glm vector value
void	SHADER_OBJ::uniform_vector_4(const char* uniform_variable_name, float x_coordinate, float y_coordinate, float z_coordinate, float w_coordinate, bool activateShader)
{
	// if activateShader is true, activate the shader program by using our member function
	if (activateShader)
	{
		this->Activate();
	}
	// look up the name of the uniform in the shader program that was provided and set the corresponding value specified in the arguments
	glUniform4f(glGetUniformLocation(this->Shader_ID, uniform_variable_name), x_coordinate, y_coordinate, z_coordinate, w_coordinate);
}
// define our uniform matrix 4 function
// set vector as constant to prevent address of matrix from changing within function
void	SHADER_OBJ::uniform_matrix_4(const char* uniform_variable_name, const glm::mat4& uniform_value, bool activateShader)
{
	// if activateShader is true, activate the shader program by using our member function
	if (activateShader)
	{
		this->Activate();
	}
	// look up the name of the uniform in the shader program that was provided and set the corresponding value specified in the arguments
	glUniformMatrix4fv(glGetUniformLocation(this->Shader_ID, uniform_variable_name), 1, false, glm::value_ptr(uniform_value));
}

// define our validate shader errors private function
void	SHADER_OBJ::Validate_Shader_Errors(unsigned int shader, std::string shader_type)
{
	// define a shader_success integer 
	int shader_success;
	// create a char array of 1024 characters in length to store error logs
	char errorLogs[1024];
	// if shader type is not equal to program, check for compile errors
	if (shader_type != "PROGRAM")
	{
		// get shader compile status and return either true or false based on if the compile was successful or not
		glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_success);

		// if shader_success returns 1 or false from prior gl function, get error logs and send it to default output
		if (!shader_success)
		{
			// get error logs and store them in errorLogs array
			glGetShaderInfoLog(shader, 1024, NULL, errorLogs);
			// send errorLogs to default output 
			std::cout << "COMPILE::ERROR::" << shader_type << '\n' << errorLogs << '\n' << std::endl;
		}
	}
	// if shader is equal to a program, check for linking errors
	else
	{
		// get shader compile status and return either true or false based on if the compile was successful or not
		glGetShaderiv(shader, GL_LINK_STATUS, &shader_success);
		// if shader_success returns 1 or false from prior gl function, get error logs and send it to default output
		if (!shader_success)
		{
			// get error logs and store them in errorLogs array
			glGetShaderInfoLog(shader, 1024, NULL, errorLogs);
			// send errorLogs to default output 
			std::cout << "LINK::ERROR::" << shader_type << '\n' << errorLogs << '\n' << std::endl;
		}
	}
}