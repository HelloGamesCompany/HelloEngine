#include "Headers.h"
#include "Shader.h"
#include <memory>
#include "Console.h"
#include "ModuleFiles.h"

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

uint Shader::CompileShader(const std::string& source, uint type)
{
	const char* src = source.c_str();

	uint id = glCreateShader(type);

	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int compileResult;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compileResult);
	if (compileResult == GL_FALSE)
	{
		int messageLength;

		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &messageLength);
		std::vector<char> message;
		message.resize(messageLength);
		glGetShaderInfoLog(id, messageLength, &messageLength, &message.front());

		Console::S_Log("Failed to compile shader!");
		
		std::string errorString = "";
		for (int i = 0; i < messageLength; i++)
		{
			errorString += message[i];
		}

		Console::S_Log(errorString);

		glDeleteShader(id);
		return 0;
	}

	return id;
}

Shader::Shader()
{
}

Shader::Shader(std::string& vertexPath, std::string& fragmentPath)
{
	// Get shader name 
	std::string shaderName = ModuleFiles::S_GetFileName(vertexPath, false);

	if (loadedShaders.find(shaderName) != loadedShaders.end())
	{
		this->programID = loadedShaders[shaderName];
		return;
	}
	// Get source code inside a string
	std::string vertexSource = OpenShader(vertexPath);
	std::string fragmentSource = OpenShader(fragmentPath);

	// Create OpenGL programs 
	uint vertexShaderID = CompileShader(vertexSource, GL_VERTEX_SHADER);
	uint fragmentShaderID = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);

	// Link both programs in a new one
	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);
	glValidateProgram(programID);

	// Delete unnecesary programs.
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

Shader::Shader(std::string&& vertexPath, std::string&& fragmentPath)
{	
	std::string shaderName = ModuleFiles::S_GetFileName(vertexPath, false);

	if (loadedShaders.find(shaderName) != loadedShaders.end())
	{
		this->programID = loadedShaders[shaderName];
		return;
	}

	// Get source code inside a string
	std::string vertexSource = OpenShader(vertexPath);
	std::string fragmentSource = OpenShader(fragmentPath);
	
	// Create OpenGL programs 
	unsigned int vertexShaderID = CompileShader(vertexSource, GL_VERTEX_SHADER);
	unsigned int fragmentShaderID = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);
	
	// Link both programs in a new one
	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);
	glValidateProgram(programID);

	// Delete unnecesary programs.
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	loadedShaders[shaderName] = programID;
}

void Shader::Bind()
{
	glUseProgram(programID);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::SetFloat3(const std::string& name, float v1, float v2, float v3) const
{
	glUniform3f(glGetUniformLocation(programID, name.c_str()), v1, v2, v3);
}

void Shader::SetFloat4(const std::string& name, float v1, float v2, float v3, float v4) const
{
	glUniform4f(glGetUniformLocation(programID, name.c_str()), v1, v2, v3, v4);
}

void Shader::SetMatFloat4v(const std::string& name, const float* value) const
{
	glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, value);
}


