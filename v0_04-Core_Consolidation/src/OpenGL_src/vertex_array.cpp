#include "OpenGL/vertex_array.h"

VertexArray::VertexArray() {
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
}

VertexArray::~VertexArray() {
	Release();
}

VertexArray::VertexArray(VertexArray&& other) noexcept
	: m_vertexArrayID(other.m_vertexArrayID) {
	other.m_vertexArrayID = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept {
	if (this != &other) {
		Release();
		std::swap(m_vertexArrayID, other.m_vertexArrayID);
	}
	return *this;
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (size_t i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		glVertexAttribPointer(i, element.count, element.type,
			element.normalized, layout.GetStride(), (const void*)offset);
		glEnableVertexAttribArray(i);
		offset += element.count * VertexAttribute::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const {
	glBindVertexArray(m_vertexArrayID);
}

void VertexArray::Unbind() const {
	glBindVertexArray(0);
}

