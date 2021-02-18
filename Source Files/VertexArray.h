#pragma once

#include "VertexBuffer.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

class VertexBufferLayout;

class VertexArray
{
private:
	unsigned int m_RenderedID;
	int m_VerticesNum;
	glm::vec4 m_Color;
	glm::mat4 m_Model;
	int m_Tex;
	glm::vec3 m_Direction;
	glm::vec3 m_Position;
	int m_d;
	int m_Type;
	int m_Timer;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout, const int layoutID);
	void Draw(Shader& shader);
	void Update(glm::vec3 Velocity, glm::vec4 SphereData, std::vector<VertexArray*> Objects);
	bool CheckCollisions(glm::vec4 SphereData, std::vector<VertexArray*> Objects);

	void Bind() const;
	void Unbind() const;

	inline int GetVertices() const { return m_VerticesNum; }
	inline void init() { m_Position = glm::vec3 (m_d+0.1f); }
	inline void SetColor(glm::vec4 Color) { m_Color = Color; }
	inline void SetModel(glm::mat4 Model) { m_Model = Model; }
	inline void SetTex(int Tex) { m_Tex = Tex; }
	inline void SetDirection(glm::vec3 Direction) { m_Direction = Direction; }
	inline void SetD(int D) { m_d = D; }
	inline void SetType(int type) { m_Type = type; }
	inline glm::vec3 GetPosition() { return m_Position; }
	inline glm::vec3 GetDirection() { return m_Direction; }
	inline int GetD() { return m_d; }
	inline int GetType() { return m_Type; }
};