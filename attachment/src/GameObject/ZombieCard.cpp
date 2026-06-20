#include "ZombieCard.hpp"
#include "pvz/utils.hpp"
#include "pvz/GameWorld/GameWorld.hpp"
#include "CooldownMask.hpp"

ZombieCard::ZombieCard(int x, int y, int price, ZombieType type, ImageID imageID, GameWorld* world)
    : GameObject(imageID, x, y, LayerID::UI, SEED_WIDTH,SEED_HEIGHT, AnimID::NO_ANIMATION)
    , m_price(price)
    , m_type(type)
    , m_isSelected(false)
    , m_world(world) 
    , m_cooldownTimer(0)
    , m_cooldownMax(120)
    , m_isOnCooldown(false)
    , m_cooldownMask(nullptr){}

void ZombieCard::Update() {
    if (m_isOnCooldown) {
        m_cooldownTimer--;
        if (m_cooldownTimer <= 0) {
            m_isOnCooldown = false;
            if (m_cooldownMask) {
                m_cooldownMask->SetDead();
                m_cooldownMask = nullptr;
            }
        }
    }
}

void ZombieCard::OnClick() {
    if (m_isOnCooldown) {
        return;
    }
    
    if (m_world->GetSunCount() < m_price) {
        m_isSelected = false;
        m_world->ClearSelectedZombieType();
        return;
    }

    if (!m_isSelected) {
        m_world->DeselectAllCards();
        m_isSelected = true;
        m_world->SetSelectedZombieType(m_type);
    } else {
        m_isSelected = false;
        m_world->ClearSelectedZombieType();
    }
}

void ZombieCard::StartCooldown() {
    m_isOnCooldown = true;
    m_cooldownTimer = m_cooldownMax;
    
    m_cooldownMask = std::make_shared<CooldownMask>(GetX(), GetY());
    m_world->AddObject(m_cooldownMask);
}