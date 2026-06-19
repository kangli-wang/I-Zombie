#include "PoleVaultingZombie.hpp"
#include "pvz/utils.hpp"
#include "pvz/GameWorld/GameWorld.hpp"

PoleVaultingZombie::PoleVaultingZombie(int x, int y)
    : Zombie(ImageID::POLE_VAULTING_ZOMBIE, x, y, MAX_HP, AnimID::RUN)
    , m_hasJumped(false)
    , m_isJumping(false)
    , m_jumpTimer(0)
    , m_jumpStartX(x) {}

void PoleVaultingZombie::OnClick() {}

void PoleVaultingZombie::Update() {
    if (IsDead()) return;
    // State 1: Jumping animation in progress
    // The zombie moves gradually left over the jump duration.
    if (m_isJumping) {
        // Calculate progress: fraction of jump completed
        int elapsed = JUMP_DURATION - m_jumpTimer;          // frames elapsed so far
        int totalDistance = JUMP_DISTANCE;                  // total pixels to move

        // Compute current X position using linear interpolation
        // (elapsed / JUMP_DURATION) * totalDistance
        int currentX = m_jumpStartX - (totalDistance * elapsed) / JUMP_DURATION;
        MoveTo(currentX, GetY());

        m_jumpTimer--;
        if (m_jumpTimer <= 0) {
            m_isJumping = false;
            PlayAnimation(AnimID::WALK);
        }
        return;
    }
    // State 2: Running (before the first jump)
    // Speed = 2, checks for plants 40px ahead.
    if (!m_hasJumped) {
        // Check if there is a plant 40 pixels to the left
        if (m_world->HasPlantNear(GetX() - 40, GetY(), 40)) {
            // Start jumping: record start position, set state, play animation
            m_hasJumped = true;
            m_isJumping = true;
            m_jumpTimer = JUMP_DURATION;
            m_jumpStartX = GetX();          // remember where we started the jump
            PlayAnimation(AnimID::JUMP);
            return;                         // No movement this frame (starts jump)
        }

        // No plant in front: continue running left
        MoveTo(GetX() - RUN_SPEED, GetY());

        if (GetX() < -50) {
            SetDead();                      // off-screen
        }
        return;
    }

    // State 3: Walking (after the jump)
    // Speed = 1, behaves like a regular zombie.
    if (m_isEating) {
        // Eating animation takes priority, no movement
        return;
    }

    MoveTo(GetX() - WALK_SPEED, GetY());

    if (GetX() < -50) {
        SetDead();
    }
}