#include "CooldownMask.hpp"
#include "pvz/utils.hpp"

CooldownMask::CooldownMask(int x, int y)
    : GameObject(
        ImageID::COOLDOWN_MASK,
        x,
        y,
        LayerID::COOLDOWN_MASK,
        50,
        70,
        AnimID::NO_ANIMATION
      )
    , m_card(nullptr) {}

void CooldownMask::Update() {}

void CooldownMask::OnClick() {}