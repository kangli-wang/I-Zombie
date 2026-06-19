#include "Background.hpp"
#include "pvz/utils.hpp"

Background::Background()
    : GameObject(
        ImageID::BACKGROUND,
        WINDOW_WIDTH / 2,
        WINDOW_HEIGHT / 2,
        LayerID::BACKGROUND,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        AnimID::NO_ANIMATION
      ) {}

void Background::Update() {}

void Background::OnClick() {}