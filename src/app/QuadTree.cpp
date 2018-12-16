#include "QuadTree.h"
#include "QuadTreeNode.h"
#include <graphic/ShaderProgram.h>
#include <graphic/Camera.h>

//---------------------------------------------------------------------------------------------------------------------
QuadTree::QuadTree(const BoundingBox& boundingBox, const glm::vec2& distanceConfig, const glm::mat4& quadTransform, Grid* grid) :
	Renderable(
		Shader(SHADER_DIR"/quadtree.vert", GL_VERTEX_SHADER),
		Shader(SHADER_DIR"/quadtree.frag", GL_FRAGMENT_SHADER)),
	m_DistanceConfig(distanceConfig),
	m_QuadTransform(quadTransform),
	m_PatchMesh(std::make_shared<QuadTreePatchMesh>(32))
{
	m_LevelsConfig.push_back(LevelConfig(0.0f, 0.35f));
	m_LevelsConfig.push_back(LevelConfig(0.35f, 0.60f));
	m_LevelsConfig.push_back(LevelConfig(0.60f, 0.75f));
	m_LevelsConfig.push_back(LevelConfig(0.75f, 1.0f));
		
	std::vector<glm::vec2> finalDistances(m_LevelsConfig.size());	
	const float distDifference = distanceConfig.y - distanceConfig.x;
	for(int i = m_LevelsConfig.size() - 1, j = 0; i >= 0; --i, ++j)
	{		
		finalDistances[j].x = distanceConfig.x + (m_LevelsConfig[i].min * distDifference);
		finalDistances[j].y = distanceConfig.x + (m_LevelsConfig[i].max * distDifference);
	}

	m_RootNode = std::make_unique<QuadTreeNode>(
		boundingBox, 0, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), finalDistances, m_PatchMesh, nullptr, m_Nodes);
}

//---------------------------------------------------------------------------------------------------------------------
void QuadTree::OnDraw(float time, const Camera& camera)
{
	m_ShaderProgram.setUniform("treeTransformation", m_QuadTransform);	

	std::vector<std::unordered_set<int>> nodesToDraw(m_LevelsConfig.size());
	m_RootNode->ResetNodes();
	m_RootNode->SelectNodesToDraw(nodesToDraw, camera, m_QuadTransform, 0);

	for(auto lodLevel : nodesToDraw)
	{
		for(auto node : lodLevel)
		{
			if(!m_Nodes[node].expired())
			{
				m_Nodes[node].lock()->DrawNode(m_ShaderProgram);
			}			
			else if(node == 1)
			{
				m_RootNode->DrawNode(m_ShaderProgram);
			}
		}
	}

	//m_RootNode->Draw(m_ShaderProgram, camera, m_QuadTransform, 3);
}
