#ifndef RESOURCE_MANAGER_HEADER
#define RESOURCE_MANAGER_HEADER

#include <map> // built-in C++ library that allows us to map two seperate variables to each other, think of it like a dictionary in python
#include <string> // include string library to load shaders and textures by name

#include <glad/glad.h> // include glad to access OpenGL pointer functions

#include "process_texture.h" // include our process texture header file
#include "process_shader.h"  // include our process shader header file

// resource_manager_obj class template
/*
uses static functions which is a function that belongs to a class iteslf rather than an object itself; 
meaning that you can call the function with RESOURCE_MANAGER::FUNCTION rather than having to assign an object to that class
i.e. RESOURCE_MANAGER object; object.function()
in addition to this static functions only have access to other static data members or functions
*/
class RESOURCE_MANAGER
{
public: // all of our public data members/functions
	// map our shader object to a string to store for future use
	static std::map<std::string, SHADER_OBJ> stored_shaders;
	// map our texture object to a string to store for future use
	static std::map<std::string, TEXTURE_2D_OBJ> stored_textures;
	// static function that loads and generates a shader program from a file loading vertex, fragment, and or geometry shader's source code. If geometry shader is not a nullptr, load it as well
	static SHADER_OBJ Shader_Load(const char *vertexShaderFilePath, const char *fragmentShaderFilePath, const char *geometryShaderFilePath, std::string shader_name);
	// get a shader that is already stored
	static SHADER_OBJ Shader_Get(std::string shader_name);
	// static function that loads and generates a texture from a file
	static TEXTURE_2D_OBJ Texture_Load(const char* textureFilePath, bool contains_alpha_value, std::string texture_name);
	// get a texture that is already stored
	static TEXTURE_2D_OBJ Texture_Get(std::string texture_name);

	// de-allocate all loaded resources
	static void Clear_All_Resources();

private: // all of our private data members/functions
	// the constructor for this class, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static)
	// think of it as this constructor is private therefore its not publicly available outside of the class, meaning we couldn't create a resource manager object
	RESOURCE_MANAGER() {}
	// loads and creates a shader from a file
	static SHADER_OBJ Shader_Load_From_Ext_File(const char* vertexShaderFilePath, const char* fragmentShaderFilePath, const char* geometryShaderFilePath = nullptr);
	// loads and creates a shader from a file
	static TEXTURE_2D_OBJ Texutre_Load_From_Ext_File(const char* textureFilePath, bool contains_alpha_value);
};
#endif // !RESOURCE_MANAGER_HEADER
