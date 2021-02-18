#include "VertexArray.h"
#include "VertexBufferLayout.h"


VertexArray::VertexArray()
	: m_VerticesNum(0), m_Color(glm::vec4(0.0f)), m_Tex(0), m_Model(glm::mat4(1.0f)), m_Position(glm::vec3(0.1f)), m_Direction(glm::vec3(0.0f)), m_d(0), m_Type(0), m_Timer(0)
{
	GLCall(glGenVertexArrays(1, &m_RenderedID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RenderedID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout, const int layoutID)
{
	Bind();
	const auto& elements = layout.GetElements();
	GLCall(glEnableVertexAttribArray(layoutID));
	vb.Bind();
	GLCall(glVertexAttribPointer(layoutID, elements[layoutID].count, elements[layoutID].type, elements[layoutID].normanized, 0, (void*)0));
	if (layoutID == 0)
		m_VerticesNum += vb.GetLength() / 3;
}

void::VertexArray::Update(glm::vec3 Velocity, glm::vec4 SphereData, std::vector<VertexArray*> Objects) {

	if (CheckCollisions(SphereData, Objects)) m_Timer = 12;
	
	m_Position += m_Direction * Velocity;

	m_Model = glm::translate(glm::mat4(1.0f), m_Position);
}

bool::VertexArray::CheckCollisions(glm::vec4 SphereData, std::vector<VertexArray*> Objects) {
	bool collision = false;
	 
	// Collisions with cube_SC
	if ((m_Position.x >= 100 - m_d) || (m_Position.x <= 0 + m_d)) { m_Direction.x *= -1; collision = true; }
	if ((m_Position.y >= 100 - m_d) || (m_Position.y <= 0 + m_d)) { m_Direction.y *= -1; collision = true; }
	if ((m_Position.z >= 100 - m_d) || (m_Position.z <= 0 + m_d)) { m_Direction.z *= -1; collision = true; }

	// Collisions with sphere_SPH
	if ((m_Type == 1) || (m_Type == 3)) { // if cube or cylinder

		glm::vec3 closestPoint = glm::vec3(fmax(m_Position.x - m_d, fmin(SphereData.x, m_Position.x + m_d)),
										   fmax(m_Position.y - m_d, fmin(SphereData.y, m_Position.y + m_d)),
										   fmax(m_Position.z - m_d, fmin(SphereData.z, m_Position.z + m_d)));

		float distance = sqrt(pow(SphereData.x - closestPoint.x, 2) + pow(SphereData.y - closestPoint.y, 2) + pow(SphereData.z - closestPoint.z, 2)); // distance between object and sphere
		if (distance < SphereData.w) {
			
			glm::vec3 I = glm::vec3(m_Direction.x, m_Direction.y, m_Direction.z);
			glm::vec3 N = glm::vec3(SphereData.x - m_Position.x, SphereData.y - m_Position.y, SphereData.z - m_Position.z);
			
			I = normalize(I);
			N = normalize(N);
			float product = dot(I, N);
			m_Direction = I - 2 * (product)*N;
			collision = true;
		}
	}

	if (m_Type == 2) { // if sphere
		float distance = sqrt(pow(SphereData.x - m_Position.x, 2) + pow(SphereData.y - m_Position.y, 2) + pow(SphereData.z - m_Position.z, 2)); // distance between object and sphere
		if (distance < SphereData.w + m_d) {
			glm::vec3 I = glm::vec3 (m_Direction.x ,m_Direction.y ,m_Direction.z);
			glm::vec3 N = glm::vec3(SphereData.x - m_Position.x, SphereData.y - m_Position.y, SphereData.z - m_Position.z);
			I = normalize(I);
			N = normalize(N);
			float product = dot(I, N);
			m_Direction = I - 2 * (product)*N;
			collision = true;
		}
	}

	// Collisions with other Objects
	

	return collision;
}

void VertexArray::Draw(Shader& shader)
{
	Bind();
	shader.Bind();
	shader.SetUniformMatrix4f("u_M", m_Model);
	shader.SetUniform1i("u_Toggle", m_Tex);
	if (m_Timer == 0) { shader.SetUniform4f("u_Color", m_Color.r, m_Color.g, m_Color.b, m_Color.a); }
	else { shader.SetUniform4f("u_Color", 0.9f, 0.1f, 0.1f, 1.0f); m_Timer -= 1; }
	GLCall(glDrawArrays(GL_TRIANGLES, 0, m_VerticesNum));
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RenderedID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
