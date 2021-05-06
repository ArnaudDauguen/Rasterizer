#ifndef TRIANGLE3D_H
#define TRIANGLE3D_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Triangle3D
{
public:
	Triangle3D(sf::Vector3f a, sf::Vector3f b, sf::Vector3f c, sf::Color color);
	Triangle3D(float xA, float yA, float zA, float xB, float yB, float zB, float xC, float yC, float zC, sf::Color color);
	~Triangle3D() = default;

	sf::Vector3f* Vertices() { return m_vertices; }
	sf::Color Color() { return m_color; }
	sf::Vector3f Normal() { return m_normal; }

	void ProjectedVertices(sf::Vector2f* output, float aspectRatio, float fovRadian, float zScaling, float screenWidth, float screenHeight, sf::Vector3f cameraLocation);
	sf::Vector3f CalculateNormal();


private:

	sf::Vector3f m_vertices[3];
	sf::Color m_color;
	sf::Vector3f m_normal;
};


#endif