#include "Triangle3D.h"
#include <iostream>

Triangle3D::Triangle3D(sf::Vector3f a, sf::Vector3f b, sf::Vector3f c, sf::Color color)
	: m_color(color)
{
	m_vertices[0] = { a.x, a.y, a.z };
	m_vertices[1] = { b.x, b.y, b.z };
	m_vertices[2] = { c.x, c.y, c.z };

	CalculateNormal();
}

Triangle3D::Triangle3D(float xA, float yA, float zA, float xB, float yB, float zB, float xC, float yC, float zC, sf::Color color)
	: m_color(color)
{
	m_vertices[0] = { xA, yA, zA };
	m_vertices[1] = { xB, yB, zB };
	m_vertices[2] = { xC, yC, zC };

	CalculateNormal();
}

void Triangle3D::ProjectedVertices(sf::Vector3f* output, float aspectRatio, float fovRadian, float zScaling, float screenWidth, float screenHeight, float zNear, sf::Vector3f cameraLocation)
{
	for (int i = 0; i < 3; ++i) {
		output[i].x = (m_vertices[i].x * aspectRatio * fovRadian);
		output[i].y = -(m_vertices[i].y * fovRadian); //Je sais pas pourquoi mais sans le - mes triangles se dessinnent à l'envers
		output[i].z = m_vertices[i].z * zScaling - zNear * zScaling;

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

sf::Vector3f Triangle3D::CalculateNormal()
{
	sf::Vector3f line1 = {
		m_vertices[1].x - m_vertices[0].x,
		m_vertices[1].y - m_vertices[0].y,
		m_vertices[1].z - m_vertices[0].z
	};
	sf::Vector3f line2 = {
		m_vertices[2].x - m_vertices[0].x,
		m_vertices[2].y - m_vertices[0].y,
		m_vertices[2].z - m_vertices[0].z
	};

	m_normal = {
		line1.y * line2.z - line1.z * line2.y,
		line1.z * line2.x - line1.x * line2.z,
		line1.x * line2.y - line1.y * line2.x
	};

	float l = sqrtf(m_normal.x * m_normal.x + m_normal.y * m_normal.y + m_normal.z * m_normal.z);
	m_normal /= l;
	return m_normal;
}

//Triangle3D::~Triangle3D()
//{
//}


