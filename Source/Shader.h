#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <sstream>
#include <iostream>

class Shader
{
public:
   
    Shader();
    Shader(std::string& vertexPath, std::string& fragmentPath);
    Shader(std::string&& vertexPath, std::string&& fragmentPath);

    void Bind();
   
    // Uniform Setters
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetFloat3(const std::string& name, float v1, float v2, float v3) const;
    void SetFloat4(const std::string& name, float v1, float v2, float v3, float v4) const;
    void SetMatFloat4v(const std::string& name, const float* value) const;

public:
    uint programID = 0;

private:
    uint CompileShader(const std::string& source, uint type);

    static std::map<std::string, uint> loadedShaders;

};

#endif
