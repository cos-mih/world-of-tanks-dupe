#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/camera.h"

#include <vector>
#include <string>
#include <iostream>

#include "Tank.h"
#include "Projectile.h"
#include "Building.h"


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        void Init() override;

    private:
        void GenerateBuildings();
        void GenerateEnemies();

        bool ProjectileHitsTank(Projectile projectile);
        bool ProjectileHitsBuilding(Projectile projectile);

        void RenderGameScene(float deltaTimeSeconds, implemented::Camera* camera);
        void RenderScene(implemented::Camera* camera);
        void RenderEnemies(implemented::Camera* camera);
        void RenderTank(Tank tank, implemented::Camera* camera);
        void RenderHP(Tank tank, implemented::Camera* camera);
        void RenderProjectiles(float deltaTimeSeconds, implemented::Camera* camera);

        void CheckForTankCollisions();
        void CheckForBuildingCollisions();
        void MoveTankBack(Tank tank, glm::vec3 dist, bool isPlayer);
        void ValidateTankPos(Tank tank, bool isPlayer);
        void CheckForWorldBoundaryCollisions();

        void UpdateEnemyMovements(float deltaTimeSeconds);
        void FireEnemyProjectiles();

        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, implemented::Camera* camera, const glm::mat4& modelMatrix, const glm::vec3& color, int hp);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        implemented::Camera* camera;
        implemented::Camera* orthoCamera;
        glm::mat4 projectionMatrix;  
        float fov;

        glm::mat4 modelMatrix;

        float planeLim = 175;

        Tank player;
        glm::vec3 green = glm::vec3(0.478, 0.988, 0.357);
        glm::vec3 blue = glm::vec3(0.031, 0.455, 0.788);
        glm::vec3 pink = glm::vec3(0.929, 0.361, 0.733);
        glm::vec3 purple = glm::vec3(0.525, 0.016, 0.831);
        glm::vec3 red = glm::vec3(1, 0.29, 0);
        glm::vec3 lime = glm::vec3(0.741, 0.961, 0.176);
        glm::vec3 darkred = glm::vec3(0.4, 0, 0);
        glm::vec3 grey = glm::vec3(0.416, 0.443, 0.549);
        glm::vec3 crimson = glm::vec3(0.722, 0.192, 0.192);

        std::vector<Projectile> projectiles;

        glm::vec3 lightPosition = glm::vec3(0, 10, 0);
        unsigned int materialShininess = 30;
        float materialKd = 1;
        float materialKs = 1;

        int buildingNum = 50;
        std::vector<Building> buildings;

        int enemyCount = 30;
        std::vector<Tank> enemies;

        int points;
        float gameTime;
        bool gameOver, gotScore, tankDestroyed, gotMessage;

        ViewportArea miniViewportArea;
        float left, right, bottom, top, zNear, zFar;

    };
}   // namespace m1
