#include "resource_manager.h" // include our resource_manager header file

#include <iostream> // include iostream to send default output to the terminal

#include <sstream> // include sstream to import all of the raw source code fstream into a string stream

#include <fstream> // include fstream to read external files and put them in a read buffer

#include "stb_image.h" // include stb_image.h to allow us to convert image formats (.jpg, .png, etc) into data that OpenGL can process into a texture

// instantiate (aka create/define the stored mapped members within this c++ file) the static map member variables
std::map<std::string, TEXTURE_2D_OBJ> RESOURCE_MANAGER::stored_textures;
std::map<std::string, SHADER_OBJ>	  RESOURCE_MANAGER::stored_shaders;

/*
NOTICE HOW WE'RE DEFINING THE FUNCTIONS THAT ARE STORED WITHIN THE RESOURCE_MANAGER CLASS, SO IN THE C++ FILE HERE WE'RE DEFINING THE ACTUAL SOURCE CODE
THE FUNCTIONS THAT ARE STORED IN THE HEADER FILE ARE REALLY ONLY PROTOTYPES
*/

// define our static Shader_Load public function here
SHADER_OBJ RESOURCE_MANAGER::Shader_Load(const char *vertexShaderFilePath, const char *fragmentShaderFilePath, const char *geometryShaderFilePath, std::string shader_name)
{
	// within our stored shaders map, we use the shader name as our index to correspond the following name to the current shader program
	// also note that we are calling the private function Shader_Load_From_Ext_File to read the source code from a ifstream buffer and reutrn it as a c-string array (const char*)
	stored_shaders[shader_name] = Shader_Load_From_Ext_File(vertexShaderFilePath, fragmentShaderFilePath, geometryShaderFilePath);
	// return stored_shaders map with its current shader_name along with the related mapped shader object
	return stored_shaders[shader_name];
}

// define our static Shader_Get public function here
SHADER_OBJ RESOURCE_MANAGER::Shader_Get(std::string shader_name)
{
	// in this function we are simply getting the map of the corresponding shader_name provided in the argument; this will also return the shader program that is mapped to the shader_name
	return stored_shaders[shader_name];
}

// define our static Texture_Load public function here
TEXTURE_2D_OBJ RESOURCE_MANAGER::Texture_Load(const char *textureFilePath, bool contains_alpha_value, std::string texture_name)
{
	// within our stored textures map, we use the texture name as our index to correspond the following name to the current texture object
	// also note that we are calling the private function Texture_Load_From_Ext_File to read the texture image from stb_image and return it as a OpenGL texture object
	stored_textures[texture_name] = Texutre_Load_From_Ext_File(textureFilePath, contains_alpha_value);
	// return stored_textures map with its current texturer_name along with the related mapped texture object
	return stored_textures[texture_name];
}

// define our static Texture_Get public function here
TEXTURE_2D_OBJ RESOURCE_MANAGER::Texture_Get(std::string texture_name)
{
	// in this function we are simply getting the map of the corresponding texture_name provided in the argument; this will also return the texture object that is mapped to the texture_name
	return stored_textures[texture_name];
}

// define our static Clear_All_Resources public function here
void RESOURCE_MANAGER::Clear_All_Resources()
{
	// delete all shaders within the stored_shaders mapped member via a for loop
	// use the auto built-in to automatically detect the data types of the stored_shaders mapped member
	for (auto shader : stored_shaders)
	{
		// delete all shader programs stored within the mapped member which is the second type within the map
		// which is why we call the second method within the shader iterable (shader)
		glDeleteProgram(shader.second.Shader_ID);
	}
	// delete all textures within the stored_textuers mapped member via a for loop
	// use the auto built-in to automatically detect the data types of the stored_textures mapped member
	for (auto texture : stored_textures)
	{
		// delete all texture objects stored within the mapped member which is the second type within the map
		// which is why we call the second method within the texture iterable (texture)
		glDeleteTextures(1, &texture.second.texture_ID);
	}
}

// define our static Shader_Load_From_Ext_File private function here
SHADER_OBJ RESOURCE_MANAGER::Shader_Load_From_Ext_File(const char *vertexShaderFilePath, const char *fragmentShaderFilePath, const char *geometryShaderFilePath)
{
	// create string variables that are used to store the source code from the string stream
	std::string vertexShaderString;
	std::string fragmentShaderString;
	std::string geometryShaderString;
	// try statment to attempt opening and reading the source code from the shader file paths
	try
	{
		// try to open the shader files with ifstream
		std::ifstream vertexShaderIFSTREAM(vertexShaderFilePath);
		std::ifstream fragmentShaderIFSTREAM(fragmentShaderFilePath);
		// create some string streams
		std::stringstream vertexShaderSTRINGSTREAM, fragmentShaderSTRINGSTREAM;
		// use the string streams and try to read the ifstream buffers into the string streams
		// you use the rdbuf method function to achieve this
		vertexShaderSTRINGSTREAM   << vertexShaderIFSTREAM.rdbuf();
		fragmentShaderSTRINGSTREAM << fragmentShaderIFSTREAM.rdbuf();
		// now we close the IFSTREAM shader variables
		vertexShaderIFSTREAM.close();
		fragmentShaderIFSTREAM.close();
		// convert string stream into a regular string and store this within the string variable we created eariler in this function
		vertexShaderString = vertexShaderSTRINGSTREAM.str();
		fragmentShaderString = fragmentShaderSTRINGSTREAM.str();
		// if there is a geometryShaderFilePath, we also attempt to grab the geometry shader source code from the external file path
		if (geometryShaderFilePath != nullptr)
		{
			// try to open the geometry shader file with ifstream
			std::ifstream geometryShaderIFSTREAM(geometryShaderFilePath);
			// create a geometry shader string sream
			std::stringstream geometryShaderSTRINGSTREAM; 
			// use the string stream and try to read the ifsream buffer into the string stream
			// you use the rdbuf method function to achieve this
			geometryShaderSTRINGSTREAM << geometryShaderIFSTREAM.rdbuf();
			// now we close the IFSTREAM geometry shader variable
			geometryShaderIFSTREAM.close();
			// convert string stream into a regular string and store this within the geometry shader string variable created earlier in this function
			geometryShaderString = geometryShaderSTRINGSTREAM.str();
		}
	}
	// if there is an error, catch it here and thrown a custom exception statment that we send to default output with c out
	catch (std::exception load_shader_file_error)
	{
		std::cout << "ERROR::SHADER_FILES::FAILED_TO_READ_SHADER_FILE(S)" << std::endl;
	}
	// now convert the string variables to a c_string style array and store them in new const char pointer variablse
	const char *vertexShaderCstringSourceCode   = vertexShaderString.c_str();
	const char *fragmentShaderCstringSourceCode = fragmentShaderString.c_str();
	const char* geometryShaderCstringSourceCode = geometryShaderString.c_str();
	// create a shader object with the previous shader source code that we have gathered prior
	SHADER_OBJ shader_object; 
	// now use the method function Compile_Shaders within the object to create a shader program
	// if the geometryShaderFilePath argument is not a nullptr, then use the geometryShaderCstringSourceCode as one of the arguments in the Compile_Shader function
	// else, set the geometry shader argument in the Compile_Shaders member function as a nullptr
	shader_object.Compile_Shaders(vertexShaderCstringSourceCode, fragmentShaderCstringSourceCode, geometryShaderFilePath != nullptr ? geometryShaderCstringSourceCode : nullptr);
	// return the shader_object to send it to the mapped stored_shaders member within the Shader_Load static member public function from prior
	return shader_object;
}

// define our static Texture_Load_From_Ext_File private function here

TEXTURE_2D_OBJ RESOURCE_MANAGER::Texutre_Load_From_Ext_File(const char *textureFilePath, bool contains_alpha_value)
{
	// create a TEXTURE_2D_OBJECT
	TEXTURE_2D_OBJ texture_object_2D;
	// if the boolean argument contains_alpha_value is true, then the internal and loaded format of the image will be GL_RGBA
	if (contains_alpha_value)
	{
		texture_object_2D.texture_format_internally = GL_RGBA;
		texture_object_2D.texture_format_loaded     = GL_RGBA;
	}
	// load the provided image within the textureFilePath
	// create 3 integer variables that will store the image's width, height, and amount of color channels
	int width_of_texture, height_of_texture, number_of_color_channels; 
	// create a unsigned char pointer variable that will store the converted image data via the stb_image function stbi_load()
	// this will also grab the width, height, and number_of_color_channels inside of the stbi_load function as well and will fill these variables with the corresponding values given by the function
	// this is why we use the addresses of our prior variables so that it can just use those preexisting variables and fill them with the data gathered from the function
	unsigned char* texture_data = stbi_load(textureFilePath, &width_of_texture, &height_of_texture, &number_of_color_channels, 0);
	// we now use the method function Create_Texture within the object to create a OpenGL compatible object with the provided arguments as well as the default member values that are stored in the TEXTURE_2D_OBJ constructor initalizer list
	texture_object_2D.Create_Texture(width_of_texture, height_of_texture, texture_data);

	// free the image data from stb_image to get ready for the next texture if there
	stbi_image_free(texture_data);

	// return the texture_object_2D to send it to the mapped stored_textures member within the Texture_Load static member public function from prior
	return texture_object_2D;
}