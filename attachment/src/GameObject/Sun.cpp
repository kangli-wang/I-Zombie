#include "Sun.hpp"
#include "pvz/utils.hpp"
#include "pvz/GameWorld/GameWorld.hpp"

Sun::Sun(int x, int y, GameWorld* world)
    : GameObject(
        ImageID::SUN,
        x,
        y,
        LayerID::SUN,
        80,
        80,
        AnimID::IDLE
      )
    , m_world(world)
    , m_value(VALUE)
    , m_lifetime(LIFETIME_MAX)
    , m_isFalling(true)
    , m_targetY(y + randInt(30, 80)) {}

void Sun::Update() {
    if (IsDead()) return;
    if (m_isFalling) {
        int fallSpeed = 2;
        if (GetY() < m_targetY) {
            MoveTo(GetX(), GetY() + fallSpeed);
        } else {
            m_isFalling = false;
        }
        return;
    }
    m_lifetime--;
    if (m_lifetime <= 0) {
        SetDead();
    }
}

void Sun::OnClick() {
    if (IsDead()) return;
    m_world->AddSunCount(m_value);
    SetDead();
}