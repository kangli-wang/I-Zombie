#include "ConeheadZombie.hpp"
#include "pvz/utils.hpp"

ConeheadZombie::ConeheadZombie(int x, int y)
    : Zombie(ImageID::CONEHEAD_ZOMBIE, x, y, MAX_HP, AnimID::WALK) {}

void ConeheadZombie::Update() {
    Zombie::Update();
    if (IsDead()) return;

    if (m_hasCone && m_hp <= THRESHOLD_HP) {
        m_hasCone = false;
        ChangeImage(ImageID::REGULAR_ZOMBIE);
    }
}

void ConeheadZombie::OnClick() {}