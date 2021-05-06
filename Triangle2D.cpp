#include <limits>
#include <iostream>

#include "Triangle2D.h"

const int intMax = std::numeric_limits<int>::max();

Triangle2D::Triangle2D(float xA, float yA, float xB, float yB, float xC, float yC, sf::Color color, float zIndex)
	: m_zIndex(zIndex), m_color(color)
{
	m_vertices[0] = sf::Vector2f(xA, yA);
	m_vertices[1] = sf::Vector2f(xB, yB);
	m_vertices[2] = sf::Vector2f(xC, yC);

	CalculateBoundingBox();
}

Triangle2D::Triangle2D(sf::Vector2f* vertices, sf::Color color, float zIndex)
	: m_zIndex(zIndex), m_color(color)
{
	m_vertices[0] = { vertices[0].x, vertices[0].y };
	m_vertices[1] = { vertices[1].x, vertices[1].y };
	m_vertices[2] = { vertices[2].x, vertices[2].y };

	CalculateBoundingBox();
}

//Triangle2D::~Triangle2D()
//{
//}

const void Triangle2D::CalculateBoundingBox()
{
	m_boundingBox[0].x = m_boundingBox[0].y = intMax;
	m_boundingBox[1].x = m_boundingBox[1].y = -intMax;

	for (auto& vertices : m_vertices) {
		if (vertices.x < m_boundingBox[0].x) m_boundingBox[0].x = static_cast<int>(vertices.x);
		if (vertices.x > m_boundingBox[1].x) m_boundingBox[1].x = static_cast<int>(vertices.x);
		if (vertices.y < m_boundingBox[0].y) m_boundingBox[0].y = static_cast<int>(vertices.y);
		if (vertices.y > m_boundingBox[1].y) m_boundingBox[1].y = static_cast<int>(vertices.y);
	}
}

bool Triangle2D::IsPixelInside(sf::Vector2f point)
{
	float d1, d2, d3;
	bool has_neg, has_pos;

	d1 = sign(point, m_vertices[0], m_vertices[1]);
	d2 = sign(point, m_vertices[1], m_vertices[2]);
	d3 = sign(point, m_vertices[2], m_vertices[0]);

	has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

	return !(has_neg && has_pos);
}

float Triangle2D::sign(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}
