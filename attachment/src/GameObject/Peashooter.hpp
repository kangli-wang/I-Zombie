#ifndef PEASHOOTER_HPP__
#define PEASHOOTER_HPP__

#include "Plant.hpp"

class GameWorld;

class Peashooter : public Plant {
public:
    Peashooter(int x, int y, GameWorld* world);
    
    void Update() override;
    void OnClick() override;
    
    void SetWorld(GameWorld* world) { m_world = world; }
    int GetRow() const { return (GetY() - FIRST_ROW_CENTER) / LAWN_GRID_HEIGHT; }
private:
    GameWorld* m_world;
    int m_cooldown;
    static constexpr int HP = 340;
    static constexpr int COOLDOWN_MAX = 32;
};

#endif