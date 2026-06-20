#include "PoleVaultingZombie.hpp"
#include "pvz/utils.hpp"
#include "pvz/GameWorld/GameWorld.hpp"

PoleVaultingZombie::PoleVaultingZombie(int x, int y)
    : Zombie(ImageID::POLE_VAULTING_ZOMBIE, x, y, MAX_HP, AnimID::RUN)
    , m_hasJumped(false)
    , m_isJumping(false)
    , m_jumpTimer(0){}

void PoleVaultingZombie::OnClick() {}

void PoleVaultingZombie::Update() {
    if (IsDead()) return;
    // State 1: Jumping animation in progress
    // The zombie moves gradually left over the jump duration.
    if (m_isJumping) {
        m_jumpTimer--;
        int progress = 42 - m_jumpTimer;
        int totalDist = GetX() - m_jumpTargetX;
        int currentX = GetX() - (totalDist * 1) / 42;
        MoveTo(currentX, GetY());
    
        if (m_jumpTimer <= 0) {
            m_isJumping = false;
            MoveTo(m_jumpTargetX, GetY());
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
            int plantX = m_world->GetNearestPlantX(GetX() - 40, GetY(), 40);
            int targetX = plantX - 220;
            m_hasJumped = true;
            m_isJumping = true;
            m_jumpTimer = JUMP_DURATION;
            m_jumpTargetX = targetX;
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