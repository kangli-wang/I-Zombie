#ifndef PLANT_HPP__
#define PLANT_HPP__

#include "pvz/GameObject/GameObject.hpp"

class Plant : public GameObject {
public:
    Plant(ImageID imageID, int x, int y, LayerID layer, int width, int height, AnimID animID, int hp)
        : GameObject(imageID, x, y, layer, width, height, animID)
        , m_hp(hp)
        , m_maxHp(hp) {}
    
    virtual ~Plant() = default;
    
    virtual void Update() override = 0;
    virtual void OnClick() override = 0;
    
    bool IsPlant() const override { return true; }
    virtual bool IsSunflower() const { return false; }
    
    int GetHp() const { return m_hp; }
    int GetMaxHp() const { return m_maxHp; }
    
    void TakeDamage(int damage) {
        m_hp -= damage;
        if (m_hp <= 0) {
            SetDead();
        }
    }
    
protected:
    int m_hp;
    int m_maxHp;
};

#endif