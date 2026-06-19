#include "BucketheadZombie.hpp"
#include "pvz/utils.hpp"

BucketheadZombie::BucketheadZombie(int x, int y)
    : Zombie(ImageID::BUCKET_HEAD_ZOMBIE, x, y, MAX_HP, AnimID::WALK) {}

void BucketheadZombie::Update() {
    Zombie::Update();
    if (IsDead()) return;

    if (m_hasBucket && m_hp <= THRESHOLD_HP) {
        m_hasBucket = false;
        ChangeImage(ImageID::REGULAR_ZOMBIE);
    }
}

void BucketheadZombie::OnClick() {}