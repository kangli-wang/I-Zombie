#ifndef ZOMBIECARD_HPP__
#define ZOMBIECARD_HPP__

#include "pvz/GameObject/GameObject.hpp"

class GameWorld;

class ZombieCard : public GameObject {
public:
    ZombieCard(int x, int y, int price, GameWorld* world);
    
    void Update() override;
    void OnClick() override;
    
    int GetPrice() const { return m_price; }
    
    bool IsSelected() const { return m_isSelected; }
    void SetSelected(bool selected) { m_isSelected = selected; }
    
private:
    int m_price;
    bool m_isSelected;
    GameWorld* m_world;
};

#endif