#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Triangle2D.h"
#include "Triangle3D.h"



static void clearScreen(sf::Uint8* pixels, float* zBuffer, int width, int height)
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
    const int iScreenWidth = 1280;
    const int iScreenHeight = 720;
    const float fScreenWidth = static_cast<float>(iScreenWidth);
    const float fScreenHeight = static_cast<float>(iScreenHeight);

    sf::RenderWindow window(sf::VideoMode(iScreenWidth, iScreenHeight), "Projet Rasterizer");
    sf::Clock clock;


    // On créé le tableau de pixels.
    auto* pixels = new sf::Uint8[iScreenWidth * iScreenHeight * 4];
    auto* zBuffer = new float[iScreenWidth * iScreenHeight];

    // On le "vide", mais ici on rend tous les pixels en gris.
    clearScreen(pixels, zBuffer, iScreenWidth, iScreenHeight);

    sf::Texture texture;
    if (!texture.create(iScreenWidth, iScreenHeight))
        return -1;

    // Liste des triangles a afficher
    const int triangleCount = 3;
    std::vector<Triangle3D> mesh;
    //triangles[0] = Triangle2D(10.0f, 10.0f, 200.0f, 10.0f, 95.0f, 200.0f, sf::Color::White, 1.0f);
    //triangles[1] = Triangle2D(500.0f, 500.0f, 1200.0f, 700.0f, 500.0f, 700.0f, sf::Color::Blue, 1.0f);
    //triangles[2] = Triangle2D(150.0f, 20.0f, 800.0f, 800.0f, 1100.0f, 200.0f, sf::Color::Green, 0.0f);
    /*for (int i = 0; i < triangleCount; ++i) {
        mesh.push_back(Triangle3D(
            -100.0f + rand() % 600,
            -100.0f + rand() % 600,
            -100.0f + rand() % 600,
            -100.0f + rand() % 600,
            -100.0f + rand() % 600,
            -100.0f + rand() % 600,
            -100.0f + rand() % 600,
            -100.0f + rand() % 600,
            -100.0f + rand() % 600,
            sf::Color(
                rand() % 255,
                rand() % 255,
                rand() % 255,
                rand() % 55 + 200
            )
        ));
    }*/
    mesh.push_back(Triangle3D(
        0.0f, 0.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, -1.0f, -8.0f, sf::Color::White
    ));
    mesh.push_back(Triangle3D(
        -1.0f, -1.0f, -10.0f, -2.0f, -1.0f, -10.0f, -1.0f, -2.0f, -8.0f, sf::Color::Blue
    ));
    mesh.push_back(Triangle3D(
        1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -2.0f, 2.0f, 2.0f, -1.0f, sf::Color::Green
    ));


    // Triangle Projection
    const float zNear = 0.1f;
    const float zFar = 1000.0f;
    const float fov = 90.0f;
    const float aspectRatio = fScreenHeight / fScreenWidth;
    const float fovRadian = 1.0f / tanf(fov * 0.5f / 180.0f * 3.141592f);
    const float zScaling = zFar / (zFar - zNear);


    std::vector<Triangle2D> triangleToRender;
    for (auto triangle : mesh) {
        sf::Vector2f projectedVertices[3];
        triangle.ProjectedVertices(projectedVertices, aspectRatio, fovRadian, zScaling, fScreenWidth, fScreenHeight);
        triangleToRender.push_back(
            Triangle2D(
                projectedVertices, triangle.Color(), 0
            )
        );
    }




    // Parcours de tous les triangles, de tous les pixels dans leur BoundingBox
    for (auto triangle : triangleToRender) {
        for (int x = triangle.BoundingBox()[0].x; x < triangle.BoundingBox()[1].x; ++x) {
            for (int y = triangle.BoundingBox()[0].y; y < triangle.BoundingBox()[1].y; ++y) {
                if (x < 0 || x >= iScreenWidth || y < 0 || y >= iScreenHeight)
                    continue;
                if (triangle.IsPixelInside(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)))) {
                    setPixelColor(pixels, zBuffer, iScreenWidth, iScreenHeight, x, y, triangle.Color(), triangle.ZIndex());
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