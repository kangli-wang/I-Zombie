#ifndef REPEATER_HPP__
#define REPEATER_HPP__

#include "Plant.hpp"

class GameWorld;

class Repeater : public Plant {
public:
    Repeater(int x, int y, GameWorld* world);

    void Update() override;
    void OnClick() override;
    
    int GetRow() const { return (GetY() - FIRST_ROW_CENTER) / LAWN_GRID_HEIGHT; }

private:
    GameWorld* m_world;
    int m_cooldown;     
    int m_secondPeaDelay;
    bool m_hasFiredFirst;

    static constexpr int HP = 340;
    static constexpr int COOLDOWN_MAX = 32; 
    static constexpr int SECOND_PEA_DELAY = 10;
};

#endif