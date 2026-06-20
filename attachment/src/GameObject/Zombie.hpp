#ifndef ZOMBIE_HPP__
#define ZOMBIE_HPP__

#include "pvz/GameObject/GameObject.hpp"

// Base class for all zombies.
// Provides common behavior: walking, eating, HP management.
class Zombie : public GameObject {
public:
    Zombie(ImageID imageID, int x, int y, int hp, AnimID animID = AnimID::WALK)
        : GameObject(imageID, x, y, LayerID::ZOMBIES, 20, 80, animID)
        , m_hp(hp)
        , m_maxHp(hp)
        , m_isEating(false) {}

    virtual ~Zombie() = default;

    virtual void Update() override;
    virtual void OnClick() override = 0;

    virtual void TakeDamage(int damage);
    bool IsZombie() const override { return true; }

    bool IsEating() const { return m_isEating; }
    void StartEating() { m_isEating = true; PlayAnimation(AnimID::EAT); }
    void StopEating() { m_isEating = false; PlayAnimation(AnimID::WALK); }
    virtual bool IsJumping() const { return false; }
    
    void SetWorld(GameWorld* world) { m_world = world; }
    GameWorld* GetWorld() const { return m_world; }
protected:
    int m_hp;
    int m_maxHp;
    bool m_isEating;
    static constexpr int SPEED = 1;
    GameWorld* m_world = nullptr;
};

#endif