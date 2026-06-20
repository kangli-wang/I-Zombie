#include "Repeater.hpp"
#include "pvz/utils.hpp"
#include "pvz/GameWorld/GameWorld.hpp"
#include "Pea.hpp"

Repeater::Repeater(int x, int y, GameWorld* world)
    : Plant(
        ImageID::REPEATER,
        x,
        y,
        LayerID::PLANTS,
        60,
        80,
        AnimID::IDLE,
        HP
      )
    , m_world(world)
    , m_cooldown(0)
    , m_secondPeaDelay(0)
    , m_hasFiredFirst(false) {}

void Repeater::Update() {
    if (IsDead()) return;

    if (m_hasFiredFirst) {
        m_secondPeaDelay--;
        if (m_secondPeaDelay <= 0) {
            auto pea2 = std::make_shared<Pea>(GetX() + 30, GetY() + 20, m_world);
            m_world->AddObject(pea2);
            m_hasFiredFirst = false;
            m_cooldown = COOLDOWN_MAX;
        }
        return;
    }

    if (m_cooldown > 0) {
        m_cooldown--;
        return;
    }

    int row = GetRow();
    int minX = GetX();

    if (m_world->HasZombieOnRow(row, minX)) {
        auto pea1 = std::make_shared<Pea>(GetX() + 30, GetY() + 20, m_world);
        m_world->AddObject(pea1);

        m_hasFiredFirst = true;
        m_secondPeaDelay = SECOND_PEA_DELAY;
    }
}

void Repeater::OnClick() {}