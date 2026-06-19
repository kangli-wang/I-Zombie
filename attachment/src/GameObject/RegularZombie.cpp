#include "RegularZombie.hpp"
#include "pvz/utils.hpp"

RegularZombie::RegularZombie(int x, int y)
    : Zombie(ImageID::REGULAR_ZOMBIE, x, y, MAX_HP, AnimID::WALK) {}

void RegularZombie::OnClick() {}

void RegularZombie::Update() {
    Zombie::Update();
}