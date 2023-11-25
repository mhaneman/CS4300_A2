#include "EntityManager.h"

EntityManager::EntityManager()
{

}

void EntityManager::update()
{

    // update lifespan
    for (auto e : m_entities)
    {
        if (e->cLifespan)
        {
            if (e->cLifespan->remaining >= e->cLifespan->total)
            {
                e->destory();
            }

            auto entity_color = e->cShape->circle.getFillColor();
            auto modified_alpha = 255.0f - (((float) e->cLifespan->remaining / (float) e->cLifespan->total) * 255.0f);
            e->cShape->circle.setFillColor(sf::Color(entity_color.r, entity_color.g, entity_color.b, modified_alpha));
            e->cLifespan->remaining++;
        }
    }

    // add entites
    for(auto e : m_entitiesToAdd)
    {
        m_entities.push_back(e);
        m_entityMap[e->tag()].push_back(e);
    }
    m_entitiesToAdd.clear();

    // remove entities
    removeDeadEntites(m_entities);
    for (auto& [tag, entityVec] : m_entityMap)
    {
        removeDeadEntites(entityVec);
    }
}

void EntityManager::removeDeadEntites(EntityVec& vec)
{
    // remove unwanted elements from vector
    auto new_end = std::remove_if(vec.begin(), vec.end(), 
        [](auto& e) 
        {
            return !e->isActive();
        }
    );

    // resize the vector
    vec.erase(new_end, vec.end());
}


std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
    auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntites++, tag));
    m_entitiesToAdd.push_back(entity);
    return entity;
}

EntityVec& EntityManager::getEntities()
{
    return m_entities;
}

EntityVec& EntityManager::getEntities(const std::string& tag)
{
    return m_entityMap[tag];
}

