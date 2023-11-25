#include "Game.h"


Game::Game(const std::string& config)
{
    init(config);
}


void Game::init(const std::string& path)
{
    // TODO: read in config file here
    // use the premade PlayerConfig, EnemConfig, BulletConfig variables

    // std::ifstream f_in(path);

    // f_in >> m_playerConfig.SR >> m_playerConfig.CR

    m_window.create(sf::VideoMode(1280, 720), "Assignment 2");
    m_window.setFramerateLimit(60);

    /* temporary */
    spawnPlayer();
    /* temporary */

}


void Game::run()
{
    while (m_running)
    {
        m_entities.update();

        if (!m_paused)
        {
            sEnemySpawner();
            sMovement();
            sCollision();
        }

        sUserInput();
        sRender();

        m_currentFrame++;
    }
}


void Game::setPaused(bool paused)
{
    m_paused = paused;
}


/* ------------------------------------------------------
------------------------ SPAWNING ----------------------- 
-------------------------------------------------------*/


void Game::spawnPlayer()
{
    float mid_x = m_window.getSize().x / 2.0f;
    float mid_y = m_window.getSize().x / 2.0f;

    auto entity = m_entities.addEntity("player");
    entity->cTransform =    std::make_shared<CTransform>(Vec2(200.0f, 200.0f), Vec2(1.0f, 1.0f), 0.0f);
    entity->cShape =        std::make_shared<CShape>(32.0f, 8, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.0f);
    entity->cCollision =    std::make_shared<CCollision>(32.0f);
    entity->cInput =        std::make_shared<CInput>();
    m_player = entity;
}

void Game::spawnEnemy()
{
    float enemy_x = rand() % m_window.getSize().x;
    float enemy_y = rand() % m_window.getSize().y;

    auto entity = m_entities.addEntity("enemy");
    entity->cTransform =    std::make_shared<CTransform>(Vec2(enemy_x, enemy_y), Vec2(1.0f, -1.0f), 0.0f);
    entity->cShape =        std::make_shared<CShape>(32.0f, 3, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.0f);
    entity->cCollision =    std::make_shared<CCollision>(32.0f);

    // record when the most recent enemy was spawned
    m_lastEnemySpawnTime = m_currentFrame;
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity)
{

}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& target)
{
    auto bullet = m_entities.addEntity("bullet");
    auto player_position = entity->cTransform->position;
    auto vel = player_position.direction_to(target) * 20.0f;

    bullet->cTransform =    std::make_shared<CTransform>(player_position, vel, 0);
    bullet->cShape =        std::make_shared<CShape>(10, 8, sf::Color(255, 255, 255), sf::Color::Transparent, 2);
    bullet->cCollision =    std::make_shared<CCollision>(10.0f);
    bullet->cLifespan =     std::make_shared<CLifespan>(20);  
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity, const Vec2& target)
{
    auto special = m_entities.addEntity("special");
    auto player_position = entity->cTransform->position;
    auto vel = player_position.direction_to(target) * 20.0f;

    special->cTransform =    std::make_shared<CTransform>(player_position, vel, 0);
    special->cShape =        std::make_shared<CShape>(40, 4, sf::Color(255, 255, 255), sf::Color::Transparent, 2);
    special->cCollision =    std::make_shared<CCollision>(10.0f);
    special->cLifespan =     std::make_shared<CLifespan>(200); 
}


/* ------------------------------------------------------
------------------------ SYSTEMS ------------------------ 
-------------------------------------------------------*/

void Game::sEnemySpawner()
{
    if (m_currentFrame - m_lastEnemySpawnTime >= 60)
    {
        spawnEnemy();
        m_lastEnemySpawnTime = m_currentFrame;
    }
    
}


void Game::sCollision()
{
    // detect collision between enemy and bullet
    for (auto b : m_entities.getEntities("bullet"))
    {
        auto bullet_position = b->cTransform->position;
        auto bullet_radius = b->cCollision->radius;

        for (auto e : m_entities.getEntities("enemy"))
        {
            auto entity_position = e->cTransform->position;
            auto entity_radius = e->cCollision->radius;

            if (bullet_position.distance_to(entity_position) <= bullet_radius + entity_radius)
            {
                e->destory();
                b->destory();
            }
        }
    }

    // detect collision between enemy and bullet
    for (auto b : m_entities.getEntities("special"))
    {
        auto bullet_position = b->cTransform->position;
        auto bullet_radius = b->cCollision->radius;

        for (auto e : m_entities.getEntities("enemy"))
        {
            auto entity_position = e->cTransform->position;
            auto entity_radius = e->cCollision->radius;

            if (bullet_position.distance_to(entity_position) <= bullet_radius + entity_radius)
            {
                e->destory();
            }
        }
    }

    // detect collision between enemy and player
    for (auto e : m_entities.getEntities("enemy"))
    {
        if (m_player->cTransform->position.distance_to(e->cTransform->position) <= m_player->cCollision->radius + e->cCollision->radius)
        {
            m_player->cTransform->position = {m_window.getSize().x/2.0f, m_window.getSize().y/2.0f};
        }
    }
}


void Game::sRender()
{
    m_window.clear();

    for (auto e : m_entities.getEntities())
    {
        e->cShape->circle.setPosition(e->cTransform->position.x, e->cTransform->position.y);
        e->cTransform->angle += 1.0f;
        e->cShape->circle.setRotation(m_player->cTransform->angle);
        m_window.draw(e->cShape->circle);
    }

    // set the position of the shape based on the entitiy transform -> pos
    m_player->cShape->circle.setPosition(m_player->cTransform->position.x, m_player->cTransform->position.y);

    // set the rotation of the shaped based on the entitys transfrom -> angle
    m_player->cTransform->angle += 1.0f;
    m_player->cShape->circle.setRotation(m_player->cTransform->angle);

    m_window.draw(m_player->cShape->circle);

    m_window.display();

}


void Game::sMovement()
{

    // player movement
    m_player->cTransform->velocity = {0, 0};

    if (m_player->cInput->up)       m_player->cTransform->velocity.y = -5.0f;
    if (m_player->cInput->down)     m_player->cTransform->velocity.y = 5.0f;
    if (m_player->cInput->left)     m_player->cTransform->velocity.x = -5.0f;
    if (m_player->cInput->right)    m_player->cTransform->velocity.x = 5.0f;


    if (m_player->cTransform->position.x - m_player->cCollision->radius <= 0 && m_player->cTransform->velocity.x < 0)
    {
        m_player->cTransform->velocity.x = 0;
    }

    if (m_player->cTransform->position.x + m_player->cCollision->radius >= m_window.getSize().x && m_player->cTransform->velocity.x > 0)
    {
        m_player->cTransform->velocity.x = 0;
    }

    if (m_player->cTransform->position.y - m_player->cCollision->radius <= 0 && m_player->cTransform->velocity.y < 0)
    {
        m_player->cTransform->velocity.y = 0;
    }

    if (m_player->cTransform->position.y + m_player->cCollision->radius >= m_window.getSize().y && m_player->cTransform->velocity.y > 0)
    {
        m_player->cTransform->velocity.y = 0;
    }

    // enemy movement
    for (auto e : m_entities.getEntities("enemy"))
    {
        if (e->cTransform->position.x - e->cCollision->radius  <= 0 || e->cTransform->position.x + e->cCollision->radius >= m_window.getSize().x)
        {
            e->cTransform->velocity.x *= -1;
        }

        if (e->cTransform->position.y  - e->cCollision->radius <= 0 || e->cTransform->position.y + e->cCollision->radius >= m_window.getSize().y)
        {
            e->cTransform->velocity.y *= -1;
        }
    }

    // enemy movement
    for (auto e : m_entities.getEntities("special"))
    {
        if (e->cTransform->position.x - e->cCollision->radius  <= 0 || e->cTransform->position.x + e->cCollision->radius >= m_window.getSize().x)
        {
            e->cTransform->velocity.x *= -1;
        }

        if (e->cTransform->position.y  - e->cCollision->radius <= 0 || e->cTransform->position.y + e->cCollision->radius >= m_window.getSize().y)
        {
            e->cTransform->velocity.y *= -1;
        }
    }

    // update velocites for all entites
    for (auto e : m_entities.getEntities())
    {
        e->cTransform->position.x += e->cTransform->velocity.x;
        e->cTransform->position.y += e->cTransform->velocity.y;   
    }
}


void Game::sUserInput()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            m_running = false;
        }

        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
                case sf::Keyboard::W:
                    m_player->cInput->up = true;
                    break;

                case sf::Keyboard::A:
                    m_player->cInput->left = true;
                    break;

                case sf::Keyboard::S:
                    m_player->cInput->down = true;
                    break;

                case sf::Keyboard::D:
                    m_player->cInput->right = true;
                    break;

                case sf::Keyboard::Space:
                    setPaused(true);
                    break;

                default: 
                    break;
            }
        }

        if (event.type == sf::Event::KeyReleased)
        {
            switch (event.key.code)
            {
                case sf::Keyboard::W:
                    m_player->cInput->up = false;
                    break;

                case sf::Keyboard::A:
                    m_player->cInput->left = false;
                    break;

                case sf::Keyboard::S:
                    m_player->cInput->down = false;
                    break;

                case sf::Keyboard::D:
                    m_player->cInput->right = false;
                    break;

                case sf::Keyboard::Space:
                    setPaused(false);
                    break;

                default: 
                    break;
            }
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
            }

            if (event.mouseButton.button == sf::Mouse::Right)
            {
                spawnSpecialWeapon(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
            }
        }
    }
}