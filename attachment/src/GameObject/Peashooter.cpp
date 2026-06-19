#include "Peashooter.hpp"
#include "pvz/utils.hpp"
#include "pvz/GameWorld/GameWorld.hpp"
#include "Pea.hpp"

Peashooter::Peashooter(int x, int y, GameWorld* world)
    : Plant(
        ImageID::PEASHOOTER,
        x,
        y,
        LayerID::PLANTS,
        60,
        80,
        AnimID::IDLE,
        HP
      )
    , m_world(world)
    , m_cooldown(0) {}

void Peashooter::Update() {
    if (IsDead()) return;
    
    if (m_cooldown > 0) {
        m_cooldown--;
        return;
    }
    int row = GetRow();
    int minX = GetX();
    
    if (m_world->HasZombieOnRow(row, minX)) {
        auto pea = std::make_shared<Pea>(GetX() + 30, GetY() + 20, m_world);
        m_world->AddObject(pea);
        
        m_cooldown = COOLDOWN_MAX;  // 重置冷却
    }
}

void Peashooter::OnClick() {}