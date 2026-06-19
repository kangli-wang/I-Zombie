#ifndef PEA_HPP__
#define PEA_HPP__

#include "pvz/GameObject/GameObject.hpp"

class GameWorld;

class Pea : public GameObject {
public:
    Pea(int x, int y, GameWorld* world);
    
    void Update() override;
    void OnClick() override;
    
    bool IsPea() const override { return true; }
    
private:
    GameWorld* m_world;
    static constexpr int SPEED = 8;
    static constexpr int DAMAGE = 24;
};

#endif