#include "Sunflower.hpp"
#include "pvz/utils.hpp"

Sunflower::Sunflower(int x, int y)
    : Plant(
        ImageID::SUNFLOWER,
        x,
        y,
        LayerID::PLANTS,
        60,
        80,
        AnimID::IDLE,
        HP
      ) {}

void Sunflower::Update() {
    if (IsDead()) return;
}

void Sunflower::OnClick() {}