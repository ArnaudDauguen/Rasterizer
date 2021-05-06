#include "Triangle3D.h"
#include <iostream>

Triangle3D::Triangle3D(float xA, float yA, float zA, float xB, float yB, float zB, float xC, float yC, float zC, sf::Color color)
	: m_color(color)
{
	m_vertices[0] = { xA, yA, zA };
	m_vertices[1] = { xB, yB, zB };
	m_vertices[2] = { xC, yC, zC };
}

void Triangle3D::ProjectedVertices(sf::Vector2f* output, float aspectRatio, float fovRadian, float zScaling, float screenWidth, float screenHeight)
{
	for (int i = 0; i < 3; ++i) {
		output[i].x = (m_vertices[i].x * aspectRatio * fovRadian);
		output[i].y = (m_vertices[i].y * fovRadian);

		if (m_vertices[i].z != 0.0f) {
			output[i].x /= m_vertices[i].z;
			output[i].y /= m_vertices[i].z;
		}

		//Viewport scaling
		output[i].x += 1.0f;
		output[i].y += 1.0f;
		
		output[i].x *= 0.5f * screenWidth;
		output[i].y *= 0.5f * screenHeight;

	}
}

//Triangle3D::~Triangle3D()
//{
//}


