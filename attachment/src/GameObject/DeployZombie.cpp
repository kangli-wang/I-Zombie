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
    if (!world->HasSelectedZombieType()) {
        return;
    }
    // check if the zombie is on the right side of the red line
    int deploymentCol = INITIAL_ZOMBIE_DEPLOYMENT_START_COL + world->GetCurrentStage();
    if (m_col <= deploymentCol) {
        return;
    }
    ZombieType type = world->GetSelectedZombieType();
    // check if the player has enough sun to deploy a zombie
    int price = 50;
    switch (type) {
        case ZombieType::Regular:   price = 50; break;
        case ZombieType::Conehead:  price = 75; break;
        case ZombieType::Buckethead: price = 125; break;
        case ZombieType::PoleVaulting: price = 75; break;
    }
    if (world->GetSunCount() < price) {
        return;
    }
    // check if there is already a plant on this cell
    if (m_world->HasPlantAt(m_row, m_col)) {
        return;
    }
    // Deduct sun cost and deploy a new zombie
    world->AddSunCount(-price);
    // create a new RegularZombie at the clicked position and add it to the game world
    world->DeployZombieByType(GetX(), GetY(), type);
}