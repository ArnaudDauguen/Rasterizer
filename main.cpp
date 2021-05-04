#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Triangle2D.h"


static void emptyArray(sf::Uint8* pixels, float* zBuffer, int width, int height)
{
    for (int j = 0; j < height; ++j)
    {
        for (int i = 0; i < width; ++i)
        {
            pixels[4 * (j * width + i)] = 50;
            pixels[4 * (j * width + i) + 1] = 50;
            pixels[4 * (j * width + i) + 2] = 50;
            pixels[4 * (j * width + i) + 3] = 255;
            zBuffer[j * width + i] = -1;
        }
    }
}


static void setPixelColor(sf::Uint8* pixels, float* zBuffer, int width, int height, int x, int y, sf::Color* color, float zIndex)
{
    if (zBuffer[y * width + x] < zIndex)
    {
        pixels[4 * (y * width + x)] = color->r;
        pixels[4 * (y * width + x) + 1] = color->g;
        pixels[4 * (y * width + x) + 2] = color->b;
        pixels[4 * (y * width + x) + 3] = color->a;
        zBuffer[y * width + x] = zIndex;
    }
}


int main() {
    const int image_width = 1280;
    const int image_height = 720;

    sf::RenderWindow window(sf::VideoMode(image_width, image_height), "Projet Rasterizer");
    sf::Clock clock;


    // On créé le tableau de pixels.
    auto* pixels = new sf::Uint8[image_width * image_height * 4];
    auto* zBuffer = new float[image_width * image_height];

    // On le "vide", mais ici on rend tous les pixels en gris.
    emptyArray(pixels, zBuffer, image_width, image_height);

    sf::Texture texture;
    if (!texture.create(image_width, image_height))
        return -1;

    // Liste des triangles a afficher
    const int triangleCount = 100;
    Triangle2D triangles[triangleCount];
    //triangles[0] = Triangle2D(10.0f, 10.0f, 200.0f, 10.0f, 95.0f, 200.0f, sf::Color::White, 1.0f);
    //triangles[1] = Triangle2D(500.0f, 500.0f, 1200.0f, 700.0f, 500.0f, 700.0f, sf::Color::Blue, 1.0f);
    //triangles[2] = Triangle2D(150.0f, 20.0f, 800.0f, 800.0f, 1100.0f, 200.0f, sf::Color::Green, 0.0f);
    for (int i = 0; i < triangleCount; ++i) {
        triangles[i] = Triangle2D(
            -100 + rand() % 400,
            -100 + rand() % 400,
            -100 + rand() % 400,
            -100 + rand() % 400,
            -100 + rand() % 400,
            -100 + rand() % 400,
            sf::Color(
                rand() % 255,
                rand() % 255,
                rand() % 255,
                rand() % 55 + 200
            ),
            triangleCount + 1 - i
        );
    }

    // Parcours de tous les triangles, de tous les pixels dans leur BoundingBox
    for (int i = 0; i < triangleCount; ++i) {
        for (int x = triangles[i].BoundingBox()[0].x; x < triangles[i].BoundingBox()[1].x; ++x) {
            for (int y = triangles[i].BoundingBox()[0].y; y < triangles[i].BoundingBox()[1].y; ++y) {
                if (x < 0 || x >= image_width || y < 0 || y >= image_height)
                    continue;
                if (triangles[i].IsPixelInside(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)))) {
                    setPixelColor(pixels, zBuffer, image_width, image_height, x, y, triangles[i].Color(), triangles[i].ZIndex());
                }
            }
        }
    }










    sf::Sprite sprite(texture);
    texture.update(pixels);

    // On garde la fenêtre ouverte.
    while (window.isOpen())
    {
        sf::Event event;

        // Si on demande la fermeture de la fenêtre...
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        // On ecrit notre image à l'écran.
        window.draw(sprite);

        window.display();
    }


}