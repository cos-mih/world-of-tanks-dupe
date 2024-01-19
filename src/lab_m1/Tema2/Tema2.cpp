#include "lab_m1/Tema2/Tema2.h"
#include <cstdlib>


using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    camera = new implemented::Camera();
    camera->Set(-glm::vec3(0, -5, 12), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    orthoCamera = new implemented::Camera();
    orthoCamera->Set(glm::vec3(0, 100, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("projectile");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("tracks");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "tracks.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("body");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "body.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("turret");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "hatch.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("cannon");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "cannon.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("brownstone");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "brownstone.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("storefront");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "storefront.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("residence");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "residence.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("heart");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "heart.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Shader* shader = new Shader("TankShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "TankVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "TankFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    fov = RADIANS(60);
    projectionMatrix = glm::perspective(fov, window->props.aspectRatio, 0.01f, 200.0f);

    player = Tank(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), blue, pink, purple, 1);
    points = 0;
    gameTime = 120;
    gameOver = false;
    gotScore = false;
    tankDestroyed = false;
    gotMessage = false;

    GenerateBuildings();
    GenerateEnemies();

    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);

    left = -40.0f;
    right = 40.0f;
    bottom = -40.0f;
    top = 40.0f;

    zNear = 0.2f;
    zFar = 200.0f;
}


void Tema2::GenerateBuildings()
{
    for (int i = 0; i < buildingNum; i++)
    {
        float x = -150.0f + (float)(rand()) / (float)(RAND_MAX / 300.0f);
        float z = -150.0f + (float)(rand()) / (float)(RAND_MAX / 300.0f);

        float length = 0, width = 0;
        int type = rand() % 3;
        Mesh* mesh = nullptr;
        switch (type) {
            case 0:
                mesh = meshes["brownstone"];
                length = 14; width = 14;
                break;
            case 1:
                mesh = meshes["storefront"];
                length = 20; width = 19;
                break;
            case 2:
                mesh = meshes["residence"];
                length = 13; width = 16;
        }

        int orientation = rand() % 4;
        int angle = orientation * 90;

        Building newBuilding = Building(mesh, glm::vec3(x, 0, z), angle, length, width, lime);
        buildings.push_back(newBuilding);
    }
}

void Tema2::GenerateEnemies()
{
    for (int i = 0; i < enemyCount; i++)
    {
        float x = -120.0f + (float)(rand()) / (float)(RAND_MAX / 240.0f);
        float z = -120.0f + (float)(rand()) / (float)(RAND_MAX / 240.0f);

        Tank newEnemy = Tank(glm::vec3(x, 0, z), glm::vec3(0, 0, 1), grey, darkred, crimson, 4);
        enemies.push_back(newEnemy);
    }
}

bool Tema2::ProjectileHitsTank(Projectile projectile)
{
    for (int i = 0; i < enemyCount; i++)
    {
        if (enemies[i].isHit(projectile) && !projectile.isEnemyProjectile())
        {
            if (enemies[i].getHP() == 1)
            {
                points += 1;
            }
            enemies[i].decrementHP();

            return true;
        }
    }

    if (player.isHit(projectile) && projectile.isEnemyProjectile())
    {
        player.decrementHP();

        return true;
    }

    return false;
}

bool Tema2::ProjectileHitsBuilding(Projectile projectile)
{
    for (int i = 0; i < buildingNum; i++)
    {
        if (buildings[i].isHit(projectile.getPosition(), 0.4))
        {
            return true;
        }
    }

    return false;
}

void Tema2::RenderGameScene(float deltaTimeSeconds, implemented::Camera* camera)
{
    RenderScene(camera);
    RenderEnemies(camera);
    RenderTank(player, camera);
    RenderProjectiles(deltaTimeSeconds, camera);
}

void Tema2::RenderScene(implemented::Camera* camera)
{
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(7));

    RenderSimpleMesh(meshes["plane"], shaders["TankShader"], camera, modelMatrix, green, -1);

    for (int i = 0; i < buildingNum; i++)
    {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, buildings[i].getPosition());
        modelMatrix = glm::rotate(modelMatrix, RADIANS(buildings[i].getAngle()), glm::vec3(0, 1, 0));

        RenderSimpleMesh(buildings[i].getMesh(), shaders["TankShader"], camera, modelMatrix, buildings[i].getColor(), -1);
    }
}

void Tema2::RenderEnemies(implemented::Camera* camera)
{
    for (int i = 0; i < enemyCount; i++)
    {
        RenderTank(enemies[i], camera);
    }
}

void m1::Tema2::RenderTank(Tank tank, implemented::Camera* camera)
{
    glm::vec3 cannonHinge = glm::vec3(0, 1, 0);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, tank.getPosition());
    modelMatrix = glm::rotate(modelMatrix, tank.getAngle(), glm::vec3(0, 1, 0));

    RenderSimpleMesh(meshes["tracks"], shaders["TankShader"], camera, modelMatrix, tank.getTracksCannonColor(), tank.getHP());
    RenderSimpleMesh(meshes["body"], shaders["TankShader"], camera, modelMatrix, tank.getBodyColor(), tank.getHP());
  
    modelMatrix = glm::rotate(modelMatrix, tank.getTurretAngle(), glm::vec3(0, 1, 0));

    RenderSimpleMesh(meshes["turret"], shaders["TankShader"], camera, modelMatrix, tank.getTurretColor(), tank.getHP());
    
    modelMatrix = glm::translate(modelMatrix, cannonHinge);
    modelMatrix = glm::rotate(modelMatrix, tank.getCannonAngle(), glm::cross(glm::vec3(0, 1, 0), tank.getTurretDir()));
    modelMatrix = glm::translate(modelMatrix, -cannonHinge);

    RenderSimpleMesh(meshes["cannon"], shaders["TankShader"], camera, modelMatrix, tank.getTracksCannonColor(), tank.getHP());

    RenderHP(tank, camera);
}

void Tema2::RenderHP(Tank tank, implemented::Camera* camera)
{
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, tank.getPosition() + glm::vec3(0, 3, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.6));

    for (int i = 0; i < tank.getHP(); i++)
    {
        RenderSimpleMesh(meshes["heart"], shaders["TankShader"], camera, modelMatrix, red, -1);
        modelMatrix = glm::translate(modelMatrix, 0.5f * glm::cross(tank.getTankDir(), glm::vec3(0, 1, 0)));
    }
}

void Tema2::RenderProjectiles(float deltaTimeSeconds, implemented::Camera* camera)
{

    for (int i = 0; i < projectiles.size(); i++)
    {
        if (!projectiles[i].reachedLifetime()) {
            if (ProjectileHitsTank(projectiles[i]) || ProjectileHitsBuilding(projectiles[i]))
            {
                projectiles.erase(projectiles.begin() + i);
            }
            else
            {
                if (!gameOver)
                {
                    projectiles[i].Translate(deltaTimeSeconds);
                }
                modelMatrix = glm::translate(glm::mat4(1), projectiles[i].getPosition());
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.34f));
                RenderSimpleMesh(meshes["projectile"], shaders["TankShader"], camera, modelMatrix, red, -1);
            }
        }
        else
        {
            projectiles.erase(projectiles.begin() + i);
        }

    }
}

void Tema2::CheckForTankCollisions()
{
    float overlap = 0;
    glm::vec3 offset = glm::vec3(0);

    for (int i = 0; i < enemyCount; i++)
    {
        overlap = player.collidesWithTank(enemies[i]);
        if (overlap > 0)
        {
            glm::vec3 dir = glm::normalize(enemies[i].getPosition() - player.getPosition());
            offset = 0.5f * overlap * dir;
            player.TranslateTankDir(-offset);
            enemies[i].TranslateTankDir(offset);
            camera->MoveWithPosition(-offset);
            orthoCamera->MoveWithPosition(-offset);
        }

        for (int j = 0; j < enemyCount; j++)
        {
            if (i == j)
            {
                continue;
            }

            overlap = enemies[i].collidesWithTank(enemies[j]);
            if (overlap > 0)
            {
                offset = 0.5f * overlap * glm::normalize(enemies[j].getPosition() - enemies[i].getPosition());
                enemies[i].TranslateTankDir(-offset);
                enemies[j].TranslateTankDir(offset);
            }
        }
    }
}

void Tema2::CheckForBuildingCollisions()
{
    for (int i = 0; i < buildingNum; i++)
    {
        if (buildings[i].isHit(player.getPosition(), 2))
        {
            glm::vec3 dir = glm::normalize(buildings[i].getPosition() - player.getPosition());
            player.TranslateTankDir(-0.5f * dir);
            camera->MoveWithPosition(-0.5f * dir);
            orthoCamera->MoveWithPosition(-0.5f * dir);
        }

        for (int j = 0; j < enemyCount; j++)
        {
            if (buildings[i].isHit(enemies[j].getPosition(), 2))
            {
                glm::vec3 dir = glm::normalize(buildings[i].getPosition() - enemies[j].getPosition());
                enemies[j].TranslateTankDir(-0.5f * dir);
            }
        }
    }
}

void Tema2::MoveTankBack(Tank tank, glm::vec3 dist, bool isPlayer)
{
    if (isPlayer) {
        player.TranslateTankDir(dist);
        camera->MoveWithPosition(dist);
        orthoCamera->MoveWithPosition(dist);
    }
    else {
        tank.TranslateTankDir(dist);
    }
}

void Tema2::ValidateTankPos(Tank tank, bool isPlayer)
{
    if (tank.getPosition().x >= planeLim)
    {
        MoveTankBack(tank, -0.3f * glm::vec3(1, 0, 0), isPlayer);
    }
    else if (tank.getPosition().x <= -planeLim)
    {
        MoveTankBack(tank, -0.3f * glm::vec3(-1, 0, 0), isPlayer);
    }
    else if (tank.getPosition().z >= planeLim)
    {
        MoveTankBack(tank, -0.3f * glm::vec3(0, 0, 1), isPlayer);
    }
    else if (tank.getPosition().z <= -planeLim)
    {
        MoveTankBack(tank, -0.3f * glm::vec3(0, 0, -1), isPlayer);
    }
}

void Tema2::CheckForWorldBoundaryCollisions()
{
    ValidateTankPos(player, true);

    for (int i = 0; i < enemyCount; i++)
    {
        ValidateTankPos(enemies[i], false);
    }
}

void Tema2::UpdateEnemyMovements(float deltaTimeSeconds)
{
    for (int i = 0; i < enemyCount; i++)
    {
        if (!enemies[i].isActive())
        {
            continue;
        }

        if (enemies[i].stateActive())
        {
            enemies[i].decrementStateInterval(deltaTimeSeconds);
            enemies[i].applyMovement(deltaTimeSeconds);
        }
        else
        {
            enemies[i].generateNewState();
        }
    }
}

void Tema2::FireEnemyProjectiles()
{
    for (int i = 0; i < enemyCount; i++)
    {
        if (enemies[i].isActive() && glm::distance(enemies[i].getPosition(), player.getPosition()) < 10)
        {
            enemies[i].aimTurret(player.getPosition());

            if (enemies[i].canFire()) {
                Projectile newProjectile = Projectile(enemies[i].getCannonTipPos(), enemies[i].getTurretDir(), true);
                projectiles.push_back(newProjectile);
                enemies[i].resetTimeElapsed();
            }
        }
    }
}

void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.702f, 0.988f, 0.988f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{
    if (player.getHP() <= 0)
    {
        tankDestroyed = true;
        gameOver = true;

        if (!gotMessage) {
            cout << "TANK DESTROYED! YOU LOST!!!" << endl;
            gotMessage = true;
        }
    }

    gameTime -= deltaTimeSeconds;
    if (gameTime <= 0)
    {
        gameOver = true;
        if (!gotScore)
        {
            cout << "FINAL SCORE: " << points << endl;
            gotScore = true;
        }
    }

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);

    if (!gameOver)
    {
        UpdateEnemyMovements(deltaTimeSeconds);
    }

    player.incrementTimeElapsed(deltaTimeSeconds);
    for (int i = 0; i < enemyCount; i++) {
        enemies[i].incrementTimeElapsed(deltaTimeSeconds);
    }

    CheckForTankCollisions();
    CheckForBuildingCollisions();
    CheckForWorldBoundaryCollisions();
    FireEnemyProjectiles();

    projectionMatrix = glm::perspective(fov, window->props.aspectRatio, 0.01f, 200.0f);
    RenderGameScene(deltaTimeSeconds, camera);

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);

    projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
    RenderGameScene(deltaTimeSeconds, orthoCamera);

}


void Tema2::FrameEnd()
{
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, implemented::Camera* camera, const glm::mat4& modelMatrix, const glm::vec3& color, int hp)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    int loc_light_pos = glGetUniformLocation(shader->program, "light_position");
    glUniform3fv(loc_light_pos, 1, glm::value_ptr(camera->position));

    int loc_eye_pos = glGetUniformLocation(shader->program, "eye_position");
    glUniform3fv(loc_eye_pos, 1, glm::value_ptr(camera->position));

    int loc_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(loc_shininess, materialShininess);

    int loc_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(loc_kd, materialKd);

    int loc_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(loc_ks, materialKs);

    int loc_hp = glGetUniformLocation(shader->program, "hp");
    glUniform1i(loc_hp, hp);

    int loc_obj_color = glGetUniformLocation(shader->program, "obj_color");
    glUniform3fv(loc_obj_color, 1, glm::value_ptr(color));

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{  
    float distance = player.getSpeed() * deltaTime;
    float deltaAngle = player.getAngularSpeed() * deltaTime;
    if (!gameOver && !tankDestroyed)
    {
        if (window->KeyHold(GLFW_KEY_W)) {
            player.TranslateTank(distance);
            camera->MoveForward(distance, player.getTankDir());
            orthoCamera->MoveForward(distance, player.getTankDir());
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            player.TranslateTank(-distance);
            camera->MoveForward(-distance, player.getTankDir());
            orthoCamera->MoveForward(-distance, player.getTankDir());
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            player.RotateTank(deltaAngle);
            camera->RotateThirdPerson_OY(deltaAngle);
            orthoCamera->RotateThirdPerson_OY(deltaAngle);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            player.RotateTank(-deltaAngle);
            camera->RotateThirdPerson_OY(-deltaAngle);
            orthoCamera->RotateThirdPerson_OY(-deltaAngle);
        }
    }
 

}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    float sensitivity;

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        sensitivity = 0.001f;
        camera->RotateThirdPerson_OY(sensitivity * deltaX);
        player.RotateTurret(-sensitivity * deltaX);
    }
    else if (!tankDestroyed)
    {
        sensitivity = 0.003f;
        player.RotateTurret(-sensitivity * deltaX);
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && player.canFire())
    {
        Projectile newProjectile = Projectile(player.getCannonTipPos(), player.getTurretDir(), false);
        projectiles.push_back(newProjectile);
        player.resetTimeElapsed();
    }
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
