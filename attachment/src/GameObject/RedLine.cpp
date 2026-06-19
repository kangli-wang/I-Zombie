#include "RedLine.hpp"
#include "pvz/utils.hpp"

RedLine::RedLine(int x, int y)
    : GameObject(
        ImageID::RED_LINE,
        x,
        y,
        LayerID::UI,
        22,                   
        502,
        AnimID::NO_ANIMATION
      ) {}

void RedLine::Update() {}

void RedLine::OnClick() {}

void RedLine::SetPosition(int x, int y) {
    MoveTo(x, y);
}