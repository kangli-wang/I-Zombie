#ifndef COOLDOWNMASK_HPP__
#define COOLDOWNMASK_HPP__

#include "pvz/GameObject/GameObject.hpp"

class CooldownMask : public GameObject {
public:
    CooldownMask(int x, int y);
    
    void Update() override;
    void OnClick() override;
    
    void SetCard(GameObject* card) { m_card = card; }
    
private:
    GameObject* m_card;
};

#endif