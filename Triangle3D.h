#ifndef TRIANGLE3D_H
#define TRIANGLE3D_H

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector3.hpp>
#include <SFML/System/Vector2.hpp>

class Triangle3D
{
public:
	Triangle3D(float xA, float yA, float zA, float xB, float yB, float zB, float xC, float yC, float zC, sf::Color color);
	~Triangle3D() = default;

	sf::Vector3f* Vertices() { return m_vertices; }
	sf::Color Color() { return m_color; }

	void ProjectedVertices(sf::Vector2f* output, float aspectRatio, float fovRadian, float zScaling, float screenWidth, float screenHeight);


private:
	sf::Vector3f m_vertices[3];
	sf::Color m_color;
};


#endif