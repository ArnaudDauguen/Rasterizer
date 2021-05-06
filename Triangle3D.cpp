#include "Triangle3D.h"
#include <iostream>

Triangle3D::Triangle3D(float xA, float yA, float zA, float xB, float yB, float zB, float xC, float yC, float zC, sf::Color color)
	: m_color(color)
{
	m_vertices[0] = { xA, yA, zA };
	m_vertices[1] = { xB, yB, zB };
	m_vertices[2] = { xC, yC, zC };
}

void Triangle3D::ProjectedVertices(sf::Vector2f* output, float aspectRatio, float fovRadian, float zScaling, float screenWidth, float screenHeight, sf::Vector3f cameraLocation)
{
	for (int i = 0; i < 3; ++i) {
		sf::Vector3f translatedVertice = m_vertices[i] - cameraLocation;
		output[i].x = (translatedVertice.x * aspectRatio * fovRadian);
		output[i].y = (translatedVertice.y * fovRadian);

		if (translatedVertice.z != 0.0f) {
			output[i].x /= translatedVertice.z;
			output[i].y /= translatedVertice.z;
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


