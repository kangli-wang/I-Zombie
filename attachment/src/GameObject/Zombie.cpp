#include "Zombie.hpp"
#include "pvz/utils.hpp"

void Zombie::Update() {
    if (IsDead()) return;
    if (m_isEating) {
        return;
    }
    
    MoveTo(GetX() - SPEED, GetY());

    if (GetX() < -50) {
        SetDead();
    }
}

void Zombie::TakeDamage(int damage) {
    m_hp -= damage;
    if (m_hp <= 0) {
        SetDead();
    }
}