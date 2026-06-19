#ifndef SUN_HPP__
#define SUN_HPP__

#include "pvz/GameObject/GameObject.hpp"

class GameWorld;

class Sun : public GameObject {
public:
    Sun(int x, int y, GameWorld* world);
    
    void Update() override;
    void OnClick() override;
    
    bool IsSun() const override { return true; }
    
private:
    GameWorld* m_world;
    int m_value;
    int m_lifetime;
    bool m_isFalling;
    int m_targetY;
    static constexpr int VALUE = 25;
    static constexpr int LIFETIME_MAX = 600;
};

#endif