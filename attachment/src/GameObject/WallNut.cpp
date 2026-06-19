#include "WallNut.hpp"
#include "pvz/utils.hpp"

WallNut::WallNut(int x, int y)
    : Plant(ImageID::WALLNUT, x, y, LayerID::PLANTS, 60, 80, AnimID::IDLE, HP) {}

void WallNut::Update() {
    if (IsDead()) return;

    if (!m_isCracked && m_hp < CRACK_THRESHOLD) {
        m_isCracked = true;
        ChangeImage(ImageID::WALLNUT_CRACKED);
    }
}

void WallNut::OnClick() {}