#include "Brain.hpp"
#include "pvz/utils.hpp"

Brain::Brain(int x, int y, int row)
: GameObject(
    ImageID::BRAIN_ICON, 
    x, 
    y, 
    LayerID::UI, 
    32, 
    31,
    AnimID::NO_ANIMATION
), m_row(row), m_isEaten(false) {}

void Brain::Update() {}

void Brain::OnClick() {}