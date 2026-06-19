#ifndef POLEVAULTINGZOMBIE_HPP__
#define POLEVAULTINGZOMBIE_HPP__

#include "Zombie.hpp"

// Pole Vaulting Zombie: runs fast, jumps over the first plant it meets.
class PoleVaultingZombie : public Zombie {
public:
    PoleVaultingZombie(int x, int y);

    void Update() override;
    void OnClick() override;

private:
    bool m_hasJumped;           // Whether the zombie has already jumped
    bool m_isJumping;           // Currently in jump animation
    int m_jumpTimer;            // Timer for jump animation duration (counts down)
    int m_jumpStartX;           // X position when jump started (for smooth interpolation)

    static constexpr int MAX_HP = 420;
    static constexpr int RUN_SPEED = 2;          // Speed before jumping
    static constexpr int WALK_SPEED = 1;         // Speed after jumping
    static constexpr int JUMP_DISTANCE = 150;    // Total pixels to move during jump
    static constexpr int JUMP_DURATION = 42;     // Frames for jump animation
};

#endif