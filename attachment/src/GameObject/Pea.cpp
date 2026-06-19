#include "Pea.hpp"
#include "pvz/utils.hpp"
#include "pvz/GameWorld/GameWorld.hpp"

Pea::Pea(int x, int y, GameWorld* world)
    : GameObject(
        ImageID::PEA,
        x,
        y,
        LayerID::PROJECTILES,
        28,
        28,
        AnimID::NO_ANIMATION
      )
    , m_world(world) {}

void Pea::Update() {
    if (IsDead()) return;
    MoveTo(GetX() + SPEED, GetY());
    if (GetX() > WINDOW_WIDTH) {
        SetDead();
        return;
    }
}

void Pea::OnClick() {}