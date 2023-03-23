#include "Headers.h"
#include "Shader.h"
#include <memory>
#include "Console.h"
#include "ModuleFiles.h"
#include "Uniform.h"

std::map<std::string, uint> Shader::loadedShaders;

// We use this method to open the shader because it is easier to format the given file inside a single string this way.
static std::string OpenShader(const std::string& path)
{
	std::ifstream stream(path);

	std::string line;
	std::stringstream ss;

	while (getline(stream, line))
	{
		ss << line << "\n";
	}

	return ss.str();
}

Shader::Shader()
{
}

Shader::Shader(std::string shaderPath)
{
	// Get shader name 
	std::string shaderName = ModuleFiles::S_GetFileName(shaderPath, false);

	if (loadedShaders.find(shaderName) != loadedShaders.end())
	{
		this->data.ID = loadedShaders[shaderName];
		return;
	}
	data.name = shaderName;

	RetriveShader(shaderPath);

	CompileShader();
}

void Shader::Clear()
{
	glDeleteShader(data.ID);
}

//Shader::Shader(std::string& vertexPath, std::string& fragmentPath)
//{
	// Get shader name 
	//std::string shaderName = ModuleFiles::S_GetFileName(vertexPath, false);

	//if (loadedShaders.find(shaderName) != loadedShaders.end())
	//{
		//this->data.ID = loadedShaders[shaderName];
		//return;
	//}
	// Get source code inside a string
	/*std::string vertexSource = OpenShader(vertexPath);
	std::string fragmentSource = OpenShader(fragmentPath);*/
	//RetriveShader(vertexPath);

	// Create OpenGL programs 
	/*uint vertexShaderID = CompileShader(vertexSource, GL_VERTEX_SHADER);
	uint fragmentShaderID = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);*/
	//CompileShader();

	// Link both programs in a new one
	/*programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);
	glValidateProgram(programID);*/

	// Delete unnecesary programs.
	/*glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);*/
//}

//Shader::Shader(std::string&& vertexPath, std::string&& fragmentPath)
//{	
//	std::string shaderName = ModuleFiles::S_GetFileName(vertexPath, false);

	//if (loadedShaders.find(shaderName) != loadedShaders.end())
	//{
	//	this->programID = loadedShaders[shaderName];
	//	return;
	//}

	//// Get source code inside a string
	//std::string vertexSource = OpenShader(vertexPath);
	//std::string fragmentSource = OpenShader(fragmentPath);
	//
	//// Create OpenGL programs 
	//unsigned int vertexShaderID = CompileShader(vertexSource, GL_VERTEX_SHADER);
	//unsigned int fragmentShaderID = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);
	//
	//// Link both programs in a new one
	//programID = glCreateProgram();
	//glAttachShader(programID, vertexShaderID);
	//glAttachShader(programID, fragmentShaderID);
	//glLinkProgram(programID);
	//glValidateProgram(programID);

	//// Delete unnecesary programs.
	//glDeleteShader(vertexShaderID);
	//glDeleteShader(fragmentShaderID);

	//loadedShaders[shaderName] = programID;
//}


void Shader::RetriveShader(const std::string& shaderPath)
{
	const char* vDefine = "#define VERTEX_PROGRAM\n";
	const char* fDefine = "#define FRAGMENT_PROGRAM\n";

	std::ifstream shaderFile;

	//Ensure ifstream objects can throw exceptions:
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		//Open File
		shaderFile.open(shaderPath.c_str());
		std::stringstream shaderStream;

		//Read file's buffer contents into streams
		shaderStream << shaderFile.rdbuf();

		//Close file handlers
		shaderFile.close();

		//Convert stream into string
		std::string fullFile = shaderStream.str();

		//Gets version line
		std::string version = fullFile.substr(0, fullFile.find("\n") + 1);
		std::size_t i = fullFile.find(version);
		fullFile.erase(i, version.length());

		//Code
		data.vertexCode = version + vDefine + fullFile;
		data.fragmentCode = version + fDefine + fullFile;
	}
	catch (std::ifstream::failure e)
	{
		Console::S_Log("ERROR: Shader file not succesfully read.", LogType::ERR);
	}
}

void Shader::CompileShader()
{
	const char* vShaderCode = data.vertexCode.c_str();
	const char* fShaderCode = data.fragmentCode.c_str();

	uint vertex, fragment;
	int success;
	char infoLog[512];
	std::string err;

	//Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	//Print compiler errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		err = "ERROR::SHADER_VERTEX::COMPILATION_FAILED\n ";
		err.append(infoLog);
		Console::S_Log(err, LogType::ERR);
	}

	//Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	//Print compiler errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		err = "ERROR::SHADER_FRAGMENT::COMPILATION_FAILED\n ";
		err.append(infoLog);
		Console::S_Log(err, LogType::ERR);
	}

	//Shader Program
	data.ID = glCreateProgram();
	glAttachShader(data.ID, vertex);
	glAttachShader(data.ID, fragment);
	//glProgramParameteri(ID, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE); //Allows getting the compiled binary
	glLinkProgram(data.ID);
	//Print Linking errors if any
	glGetProgramiv(data.ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(data.ID, 512, NULL, infoLog);
		err = "ERROR::SHADER_PROGRAM::LINKING_FAILED\N ";
		err.append(infoLog);
		Console::S_Log(err, LogType::ERR);
	}
	else
	{
		err = "SUCCESS: Shader '";
		err += data.name;
		err += "' is compiled!";
		Console::S_Log(err, LogType::INFO);

		//UniformParser();
	}

	//Delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::Recompile(std::string shaderPath)
{
	ShaderData previousData = data;
	Clear();	

	RetriveShader(shaderPath);
	CompileShader();

	//Gives values to unchanged uniforms.
	UniformCompare(previousData);
}

void Shader::UniformParser(std::vector<Uniform*>& vec)
{
	GLint maxNameLen, count;

	glGetProgramiv(data.ID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLen);
	glGetProgramiv(data.ID, GL_ACTIVE_UNIFORMS, &count);

	UniformData uni;
	std::vector<GLchar>uniName(maxNameLen, 0);

	int layerCount = 0;
	for (GLint i = 0; i < count; ++i)
	{
		uni.index = i;
		glGetActiveUniform(data.ID, i, maxNameLen, &uni.read, &uni.size, &uni.type, uniName.data());

		uni.name = uniName.data();

		if (uni.name == "view" || uni.name == "projection" || uni.name == "model") continue;

		switch (uni.type)
		{
			case GL_BOOL: vec.push_back((Uniform*) new UniBool(uni)); break;
			case GL_INT: vec.push_back((Uniform*) new UniInt(uni)); break;
			case GL_UNSIGNED_INT: vec.push_back((Uniform*) new UniUInt(uni)); break;
			case GL_FLOAT: vec.push_back((Uniform*) new UniFloat(uni)); break;
			case GL_FLOAT_VEC2: vec.push_back((Uniform*) new UniFloat2(uni)); break;
			case GL_FLOAT_VEC3: vec.push_back((Uniform*) new UniFloat3(uni)); break;
			case GL_FLOAT_VEC4: vec.push_back((Uniform*) new UniFloat4(uni)); break;
			case GL_DOUBLE: vec.push_back((Uniform*) new UniDouble(uni)); break;
			case GL_SAMPLER_2D: 
				vec.push_back((Uniform*) new UniSampler2D(uni, layerCount));
				layerCount++;
				break;
			case GL_FLOAT_MAT4: vec.push_back((Uniform*) new UniFloat4x4(uni)); break;
		}
	}

	std::string aux = "A total of '" + std::to_string(vec.size()) + "' uniform variables have been parsed in the shader'" + data.name + "'.";
	Console::S_Log(aux, LogType::INFO);

}

void Shader::UniformCompare(ShaderData previousShader)
{

}

//uint Shader::CompileShader(const std::string& source, uint type)
//{
//	const char* src = source.c_str();
//
//	uint id = glCreateShader(type);
//
//	glShaderSource(id, 1, &src, nullptr);
//	glCompileShader(id);
//
//	int compileResult;
//	glGetShaderiv(id, GL_COMPILE_STATUS, &compileResult);
//	if (compileResult == GL_FALSE)
//	{
//		int messageLength = 0;
//
//		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &messageLength);
//		std::vector<char> message;
//		message.resize(messageLength);
//		if (messageLength != 0)
//			glGetShaderInfoLog(id, messageLength, &messageLength, &message.front());
//
//		Console::S_Log("Failed to compile shader!");
//
//		std::string errorString = "";
//		for (int i = 0; i < messageLength; i++)
//		{
//			errorString += message[i];
//		}
//
//		Console::S_Log(errorString);
//
//		glDeleteShader(id);
//		return 0;
//	}
//
//	return id;
//}

void Shader::Bind()
{
	glUseProgram(data.ID);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(data.ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(data.ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(data.ID, name.c_str()), value);
}

void Shader::SetDouble(const std::string& name, double value) const
{
	glUniform1d(glGetUniformLocation(data.ID, name.c_str()), value);
}

void Shader::SetFloat2v(const std::string& name, const float* value) const
{
	glUniform2fv(glGetUniformLocation(data.ID, name.c_str()), 1, value);
}

void Shader::SetFloat3v(const std::string& name, const float* value) const
{
	glUniform3fv(glGetUniformLocation(data.ID, name.c_str()), 1, value);
}

void Shader::SetFloat4(const std::string& name, float v1, float v2, float v3, float v4) const
{
	glUniform4f(glGetUniformLocation(data.ID, name.c_str()), v1, v2, v3, v4);
}

void Shader::SetFloat4v(const std::string& name, const float* value) const
{
	glUniform4fv(glGetUniformLocation(data.ID, name.c_str()), 1, value);
}

void Shader::SetMatFloat4v(const std::string& name, const float* value) const
{
	glUniformMatrix4fv(glGetUniformLocation(data.ID, name.c_str()), 1, GL_FALSE, value);
}

void Shader::SetTexture(const std::string& name, uint id, int layer)
{
	glActiveTexture(GL_TEXTURE0 + layer);
	glBindTexture(GL_TEXTURE_2D, id);
	SetInt(name, layer);
}

