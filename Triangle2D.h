#ifndef TRIANGLE2D_H
#define TRIANGLE2D_H

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

class Triangle2D
{
public:
	Triangle2D() : Triangle2D(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, sf::Color::White, 0.0f){}
	Triangle2D(float xA, float yA, float xB, float yB, float xC, float yC, sf::Color color, float zIndex);
	~Triangle2D() = default;

	const void CalculateBoundingBox();
	sf::Vector2i* BoundingBox() { return m_boundingBox; }
	sf::Color* Color() { return &m_color; }
	float ZIndex() { return m_zIndex; }

	bool IsPixelInside(sf::Vector2f point);
private:
	float m_zIndex;
	sf::Color m_color;
	sf::Vector2f m_vertices[3];

	sf::Vector2i m_boundingBox[2];

	float sign(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3); // Determine de quel côté du demi plan p2p3 le point p1 se trouve
	
};


#endif