#ifndef ZOMBIECARD_HPP__
#define ZOMBIECARD_HPP__

#include "pvz/GameObject/GameObject.hpp"

class GameWorld;
class CooldownMask;

class ZombieCard : public GameObject {
public:
    ZombieCard(
        int x, 
        int y, 
        int price, 
        ZombieType type, 
        ImageID imageID, 
        GameWorld* world);
    
    void Update() override;
    void OnClick() override;
    
    int GetPrice() const { return m_price; }
    
    bool IsSelected() const { return m_isSelected; }
    void SetSelected(bool selected) { m_isSelected = selected; }

    void StartCooldown();
    bool IsOnCooldown() const { return m_isOnCooldown; }

    ZombieType GetZombieType() const { return m_type; }
    
private:
    int m_price;
    ZombieType m_type;
    bool m_isSelected;
    GameWorld* m_world;

    int m_cooldownTimer;
    int m_cooldownMax;
    bool m_isOnCooldown;
    std::shared_ptr<CooldownMask> m_cooldownMask;
};

#endif