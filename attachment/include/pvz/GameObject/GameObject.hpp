#ifndef GAMEOBJECT_HPP__
#define GAMEOBJECT_HPP__

#include "pvz/Framework/ObjectBase.hpp"

// Declares the class name GameWorld so that its pointers can be used.
class GameWorld;

class GameObject : public ObjectBase {
public:
    GameObject(ImageID imageID, int x, int y, LayerID layer, int width, int height, AnimID animID)
    : ObjectBase(imageID, x, y, layer, width, height, animID), m_isDead(false) {}
    
    virtual ~GameObject() = default;
    
    virtual void Update() = 0;
    virtual void OnClick() = 0;
    
    bool IsDead() const { return m_isDead; }
    void SetDead() { m_isDead = true; }

    void SetWorld(GameWorld* world) { m_world = world; }
    GameWorld* GetWorld() const { return m_world; }

    virtual bool IsZombie() const { return false; }
    virtual bool IsBrain() const { return false; }
    virtual bool IsProgressMeter() const { return false; }
    virtual bool IsPlant() const { return false; }
    
private:
    bool m_isDead;
    GameWorld* m_world = nullptr;
};

#endif // !GAMEOBJECT_HPP__
