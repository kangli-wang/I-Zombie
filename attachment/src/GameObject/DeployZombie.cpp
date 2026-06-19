#include "DeployZombie.hpp"
#include "pvz/utils.hpp"
#include "pvz/GameWorld/GameWorld.hpp"
#include "RegularZombie.hpp"

DeployZombie::DeployZombie(int x, int y, int row, int col, GameWorld* world)
    : GameObject(
        ImageID::NONE,
        x,
        y,
        LayerID::UI,
        LAWN_GRID_WIDTH,
        LAWN_GRID_HEIGHT, 
        AnimID::NO_ANIMATION
      )
    , m_row(row)
    , m_col(col)
    , m_world(world) {}

void DeployZombie::Update() {}

void DeployZombie::OnClick() {
    auto world = m_world;
    // check if the zombie is on the right side of the red line
    int deploymentCol = INITIAL_ZOMBIE_DEPLOYMENT_START_COL + world->GetCurrentStage();
    if (m_col <= deploymentCol) {
        return;
    }
    // check if the player has enough sun to deploy a zombie
    if (world->GetSunCount() < 50) {
        return;
    }
    world->AddSunCount(-50);
    // Create a new RegularZombie at the clicked position and add it to the game world
    auto zombie = std::make_shared<RegularZombie>(GetX(), GetY());
    world->AddObject(zombie);
}