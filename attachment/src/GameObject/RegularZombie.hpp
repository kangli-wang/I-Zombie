#ifndef REGULARZOMBIE_HPP__
#define REGULARZOMBIE_HPP__

#include "pvz/GameObject/GameObject.hpp"

class RegularZombie : public GameObject {
public:
    RegularZombie(int x, int y);
    
    void Update() override;
    void OnClick() override;
    
    void TakeDamage(int damage);
    bool IsEating() const { return m_isEating; }
    void StartEating() { m_isEating = true; PlayAnimation(AnimID::EAT); }
    void StopEating() { m_isEating = false; PlayAnimation(AnimID::WALK); }
    
    bool IsZombie() const override { return true; }
    
private:
    int m_hp;
    bool m_isEating;
    static constexpr int SPEED = 1;
    static constexpr int MAX_HP = 260;
};

#endif