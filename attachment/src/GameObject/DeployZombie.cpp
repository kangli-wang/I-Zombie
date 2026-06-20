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

    // Check if the cell is to the right of the red line
    int deploymentCol = INITIAL_ZOMBIE_DEPLOYMENT_START_COL + world->GetCurrentStage();
    if (m_col <= deploymentCol) {
        return;
    }

    ZombieType type = world->GetSelectedZombieType();

    // Get the price for the selected zombie type
    int price = 50;
    switch (type) {
        case ZombieType::Regular:     price = 50;  break;
        case ZombieType::Conehead:    price = 75;  break;
        case ZombieType::Buckethead:  price = 125; break;
        case ZombieType::PoleVaulting: price = 75; break;
    }

    // Check if the player has enough sun
    if (world->GetSunCount() < price) {
        return;
    }

    // Deduct sun cost and deploy the zombie
    world->AddSunCount(-price);
    world->DeployZombieByType(GetX(), GetY(), type);

    world->ClearSelectedZombieType();
    world->DeselectAllCards();
}