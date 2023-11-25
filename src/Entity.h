#pragma once

#include <memory>
#include <string>

#include "Components.h"

class Entity
{
    friend class EntityManager;

    bool        m_active  = true;
    size_t      m_id      = 0;
    std::string m_tag     = "default";

    Entity(const size_t id, const std::string & tag)
        : m_id(id)
        , m_tag(tag) {}

    public:

        // conponent pointers
        std::shared_ptr<CTransform>     cTransform;
        std::shared_ptr<CShape>         cShape;
        std::shared_ptr<CCollision>     cCollision;
        std::shared_ptr<CInput>         cInput;
        std::shared_ptr<CScore>         cScore;
        std::shared_ptr<CLifespan>      cLifespan;

        // private member access functions
        bool isActive() const;
        const std::string& tag() const;
        const size_t id() const;
        void destory();
};
