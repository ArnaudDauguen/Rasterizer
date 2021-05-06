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
    std::vector<Triangle3D> mesh;
    sf::Vector3f cubeVertices[8] = {
        {0.f, 0.f, 4.f}, //0
        {1.f, 0.f, 4.f}, //1
        {0.f, 1.f, 4.f}, //2
        {1.f, 1.f, 4.f}, //3
        {0.f, 0.f, 5.f}, //4
        {1.f, 0.f, 5.f}, //5
        {0.f, 1.f, 5.f}, //6
        {1.f, 1.f, 5.f}  //7
    };
    mesh.push_back(Triangle3D(cubeVertices[0], cubeVertices[2], cubeVertices[3], sf::Color::White)); //Front
    mesh.push_back(Triangle3D(cubeVertices[0], cubeVertices[3], cubeVertices[1], sf::Color::White));

    mesh.push_back(Triangle3D(cubeVertices[4], cubeVertices[6], cubeVertices[2], sf::Color::Cyan)); //Left
    mesh.push_back(Triangle3D(cubeVertices[4], cubeVertices[2], cubeVertices[0], sf::Color::Cyan));

    mesh.push_back(Triangle3D(cubeVertices[5], cubeVertices[7], cubeVertices[6], sf::Color::Yellow)); //Back
    mesh.push_back(Triangle3D(cubeVertices[5], cubeVertices[6], cubeVertices[4], sf::Color::Yellow));

    mesh.push_back(Triangle3D(cubeVertices[1], cubeVertices[3], cubeVertices[7], sf::Color::Green)); //Right
    mesh.push_back(Triangle3D(cubeVertices[1], cubeVertices[7], cubeVertices[5], sf::Color::Green));

    mesh.push_back(Triangle3D(cubeVertices[2], cubeVertices[6], cubeVertices[7], sf::Color::Blue)); //Top
    mesh.push_back(Triangle3D(cubeVertices[2], cubeVertices[7], cubeVertices[3], sf::Color::Blue));
    
    mesh.push_back(Triangle3D(cubeVertices[5], cubeVertices[4], cubeVertices[0], sf::Color::Red)); //Bottom
    mesh.push_back(Triangle3D(cubeVertices[5], cubeVertices[0], cubeVertices[1], sf::Color::Red));
    


    // Camera
    sf::Vector3f cameraLocation = { 0.0f, 0.0f, 0.0f };
    sf::Vector3f cameraRight = { 1.0f, 0.0f, 0.0f };    // x
    sf::Vector3f cameraUp = { 0.0f, 1.0f, 0.0f };       // y
    sf::Vector3f cameraForward = { 0.0f, 0.0f, 1.0f };  // z, currently not used
    float cameraSpeed = 2.0f;





    // On garde la fenêtre ouverte.
    while (window.isOpen())
    {
        sf::Event event;

        sf::Time elapsed = clock.restart();
        float deltaTime = elapsed.asSeconds();
        //std::cout << deltaTime << std::endl;

        // Si on demande la fermeture de la fenêtre...
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();


            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
                cameraLocation += cameraForward * (cameraSpeed * deltaTime);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                cameraLocation += -1.0f * cameraForward * (cameraSpeed * deltaTime);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                cameraLocation += cameraRight * (cameraSpeed * deltaTime);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                cameraLocation += -1.0f * cameraRight * (cameraSpeed * deltaTime);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                cameraLocation += cameraUp * (cameraSpeed * deltaTime);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                cameraLocation += -1.0f * cameraUp * (cameraSpeed * deltaTime);
        }
        //std::cout << cameraLocation.x << " " << cameraLocation.y << " " << cameraLocation.z << std::endl;


        window.clear();
        clearScreen(pixels, zBuffer, iScreenWidth, iScreenHeight);





        


        // Triangle Projection
        const float zNear = 0.1f;
        const float zFar = 1000.0f;
        const float fov = 90.0f;
        const float aspectRatio = fScreenHeight / fScreenWidth;
        const float fovRadian = 1.0f / tanf(fov * 0.5f / 180.0f * 3.141592f); // 1/(demi fov en radian)
        const float zScaling = zFar / (zFar - zNear);


        std::vector<Triangle2D> triangleToRender;
        for (auto inTri : mesh) {
            
            sf::Vector3f translatedVertices[3] = {
                inTri.Vertices()[0] - cameraLocation,
                inTri.Vertices()[1] - cameraLocation,
                inTri.Vertices()[2] - cameraLocation 
            };

            Triangle3D triangle = Triangle3D(translatedVertices[0], translatedVertices[1], translatedVertices[2], inTri.Color());
            
            // Determine si le triangle est visible (face a la camera) et s'il faut l'afficher
            triangle.CalculateNormal().x;
            bool isTriangleFacingCamera = (
                triangle.Normal().x * (triangle.Vertices()[0].x - cameraLocation.x) +
                triangle.Normal().y * (triangle.Vertices()[0].y - cameraLocation.y) +
                triangle.Normal().z * (triangle.Vertices()[0].z - cameraLocation.z)
            ) < 0.0f;
            if (!isTriangleFacingCamera) continue;

            sf::Vector2f projectedVertices[3];
            triangle.ProjectedVertices(projectedVertices, aspectRatio, fovRadian, zScaling, fScreenWidth, fScreenHeight, cameraLocation);

            /*std::cout << " " << std::endl;
            std::cout << projectedVertices[0].x << " " << projectedVertices[0].y << std::endl;
            std::cout << projectedVertices[1].x << " " << projectedVertices[1].y << std::endl;
            std::cout << projectedVertices[2].x << " " << projectedVertices[2].y << std::endl;*/
            triangleToRender.push_back(
                Triangle2D(
                    projectedVertices, triangle.Color(), 0
                )
            );
        }


        // Parcours de tous les triangles, de tous les pixels dans leur BoundingBox
        for (auto triangle : triangleToRender) {
            for (int x = triangle.BoundingBox()[0].x; x < triangle.BoundingBox()[1].x; ++x) {
                if (x < 0 || x >= iScreenWidth)
                    continue;
                for (int y = triangle.BoundingBox()[0].y; y < triangle.BoundingBox()[1].y; ++y) {
                    if (y < 0 || y >= iScreenHeight)
                        continue;
                    if (triangle.IsPixelInside(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)))) {
                        setPixelColor(pixels, zBuffer, iScreenWidth, iScreenHeight, x, y, triangle.Color(), triangle.ZIndex());
                    }
                }
            }
        }








        // On ecrit notre image à l'écran.
        sf::Sprite sprite(texture);
        texture.update(pixels);
        window.draw(sprite);

        window.display();
    }


}