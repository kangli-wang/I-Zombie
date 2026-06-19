#include "RegularZombie.hpp"
#include "pvz/utils.hpp"

RegularZombie::RegularZombie(int x, int y)
    : GameObject(
        ImageID::REGULAR_ZOMBIE,
        x,
        y,
        LayerID::ZOMBIES,
        20,
        80,
        AnimID::WALK
      )
    , m_hp(MAX_HP)
    , m_isEating(false) {}

void RegularZombie::Update() {
    if (IsDead()) return;
    
    if (m_isEating) {
        return;
    }
    
    MoveTo(GetX() - SPEED, GetY());
    
    if (GetX() < -50) {
        SetDead();
    }
}

void RegularZombie::OnClick() {}

void RegularZombie::TakeDamage(int damage) {
    m_hp -= damage;
    if (m_hp <= 0) {
        SetDead();
    }
}