#include "Grid.h"
#include <vector>
#include <glm/gtc/matrix_transform.inl>

struct VertexInfo
{
	glm::vec3 position;
	glm::vec4 color;

	VertexInfo(glm::vec3 pos, glm::vec3 col) : position(pos), color(glm::vec4(col.x, col.y, col.z, 1.0)) {}
};

Grid::Grid(glm::ivec2 size) :
	m_Size(size),
	m_VertexShader(SHADER_DIR"/grid.vert", GL_VERTEX_SHADER),
	m_FragmentShader(SHADER_DIR"/grid.frag", GL_FRAGMENT_SHADER),
	m_ShaderProgram({ m_VertexShader,m_FragmentShader })
{
	int totalVerticesSize = (size.x + 1) * (size.y + 1);

	std::vector<VertexInfo> vertices;
	vertices.reserve(totalVerticesSize);

	std::vector<GLuint> indices;
	indices.reserve(totalVerticesSize * 4);
	
	/* Init algorithm to create vertices */
	glm::vec3 color(0.0f, 1.0f, 0.0f);
	
	float currentY = 0.0f;
	float currentX = 0.0f;
	int verticesPerRow = 0;
	for(int i = 0; i < totalVerticesSize; ++i)
	{
		vertices.push_back(VertexInfo(glm::vec3(currentX, currentY, 0.0f), color));
		++verticesPerRow;

		if(verticesPerRow == (size.x + 1))
		{
			++currentY;
			currentX = 0.0f;
			verticesPerRow = 0;
		}
		else
		{
			++currentX;
		}
	}

	for(int indexY = 0; indexY < size.y; ++indexY)
	{
		for (int indexX = 0; indexX < size.x; ++indexX)
		{
			int currentIndex = ((size.x + 1) * indexY) + indexX;

			int increment = size.x + 1;

			indices.push_back(currentIndex);
			indices.push_back(currentIndex + 1);
			indices.push_back(currentIndex + increment);
			
			indices.push_back(currentIndex + increment);
			indices.push_back(currentIndex + 1);
			indices.push_back(currentIndex + increment + 1);			
		}
	}

	m_TotalIndices = indices.size();
	
	/* End algorithm to create vertices */


	glGenVertexArrays(1, &m_Vao);
	glBindVertexArray(m_Vao);

	GLuint vbo;
	GLuint ibo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexInfo), vertices.data(), GL_STATIC_DRAW);

	m_ShaderProgram.setAttribute("position", 3, sizeof(VertexInfo), offsetof(VertexInfo, position));
	m_ShaderProgram.setAttribute("color", 4, sizeof(VertexInfo), offsetof(VertexInfo, color));

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Grid::Draw(float time, const glm::mat4& projection, const glm::mat4& view, const Transform& localTransform)
{
	m_ShaderProgram.use();

	// Set the uniforms
	m_ShaderProgram.setUniform("projection", projection);
	m_ShaderProgram.setUniform("view", view);
	
	auto transformation = glm::mat4();	
	
	m_ShaderProgram.setUniform("transformation", transformation);
	

	// Bind VAO
	glBindVertexArray(m_Vao);
	
	//mProjectionMatrix = glm::mat4();

	//m_ShaderProgram.setUniform("size", m_Size);

	// Draw Triangles depending on the size
	glDrawElements(
		GL_TRIANGLES,      // mode
		m_TotalIndices,         // count
		GL_UNSIGNED_INT,   // type
		NULL               // element array buffer offset
	);

	// Unbind VAO
	glBindVertexArray(0);

	// Unbind program
	m_ShaderProgram.unuse();
}


