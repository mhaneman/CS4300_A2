#pragma once

#include <vector>
#include <map>
#include <memory>
#include <string>

#include <iostream>
#include <algorithm>

#include "Entity.h"

typedef std::vector <std::shared_ptr<Entity>>   EntityVec;
typedef std::map    <std::string, EntityVec>    EntityMap;

class EntityManager
{
private:
    EntityVec   m_entities;
    EntityVec   m_entitiesToAdd;
    EntityMap   m_entityMap;
    size_t      m_totalEntites = 0;

public:
    EntityManager();
    void update();
    void removeDeadEntites(EntityVec& vec);
    std::shared_ptr<Entity> addEntity(const std::string& tag);
    EntityVec& getEntities();
    EntityVec& getEntities(const std::string& tag);
};