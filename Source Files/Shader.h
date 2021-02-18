#pragma once

#include <iostream>
#include <unordered_map>
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
#include <vector>

class Shader
{
private:
	const char* vertex_file_path;
	const char* fragment_file_path;
	unsigned int m_RendererID;
	// caching for uniforms
	std::unordered_map<std::string, int> m_UniformLocationCashe;
public:
	Shader(const char* vertex_file_path, const char* fragment_file_path);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Set Uniforms
	void SetUniformMatrix4f(const std::string& name, const glm::mat4& matrix);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform1i(const std::string& name, int v0);
	void SetUniform1f(const std::string& name, float v0);
private:
	int GetUniformLocation(const std::string& name);
	unsigned int LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
};




