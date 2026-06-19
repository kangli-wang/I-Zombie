#include "ZombieCard.hpp"
#include "pvz/utils.hpp"
#include "pvz/GameWorld/GameWorld.hpp"

ZombieCard::ZombieCard(int x, int y, int price, GameWorld* world)
    : GameObject(
        ImageID::ZOMBIE_CARD_REGULAR,
        x,
        y,
        LayerID::UI,
        SEED_WIDTH,
        SEED_HEIGHT,
        AnimID::NO_ANIMATION
      )
    , m_price(price)
    , m_isSelected(false)
    , m_world(world) {}

void ZombieCard::Update() {}

void ZombieCard::OnClick() {
    if (m_world->GetSunCount() < m_price) {
        m_isSelected = false;
        return;
    }
    m_isSelected = !m_isSelected;
}