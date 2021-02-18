#pragma once

class VertexBuffer
{
private: 
	unsigned int m_RendererID;
	int m_Size;
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	inline int GetLength() const { return m_Size; }
};