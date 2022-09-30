#include "Headers.h"
#include "Shader.h"

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

static unsigned int CompileShader(const std::string& source, unsigned int type)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "Failed to compile Shader!" << std::endl;
		std::cout << message << std::endl;

		glDeleteShader(id);
		return 0;
	}

	return id;
}

Shader::Shader()
{
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	CreateShader(vertexPath, fragmentPath);
}

void Shader::CreateShader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexSource = OpenShader(vertexPath);
	std::string fragmentSource = OpenShader(fragmentPath);

	unsigned int vertexShaderID = CompileShader(vertexSource, GL_VERTEX_SHADER);
	unsigned int fragmentShaderID = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);
	glValidateProgram(programID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
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

void Shader::SetMatFloat4v(const std::string& name, const float* value)
{
	glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, value);
}


