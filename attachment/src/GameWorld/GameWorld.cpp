#include "pvz/GameWorld/GameWorld.hpp"
#include "../GameObject/Background.hpp"
#include "../GameObject/Brain.hpp"
#include "../GameObject/ProgressMeter.hpp"
#include "../GameObject/RedLine.hpp"
#include "../GameObject/ZombieCard.hpp"
#include "../GameObject/DeployZombie.hpp"
#include "../GameObject/RegularZombie.hpp"
#include "../GameObject/SunFlower.hpp"
#include "../GameObject/Peashooter.hpp"
#include "../GameObject/Sun.hpp"
#include "../GameObject/WallNut.hpp"
#include "../GameObject/ConeheadZombie.hpp"
#include "../GameObject/BucketheadZombie.hpp"
#include "../GameObject/PoleVaultingZombie.hpp"

// Init()
// Called once when the game starts (after pressing Enter).
// Initializes all game state and creates initial objects.
void GameWorld::Init() {
  m_objects.clear();

  // Initialize game state variables
  m_sunCount = 150;
  m_currentStage = 0; 

  for (int i = 0; i < 5; ++i) {
    m_brainEaten[i] = false;                     // No brains eaten yet
    for (int j = 0; j < 9; ++j) {
      m_plantGrid[i][j] = false;                 // All grid cells empty
    }
  }

  // Create UI text objects
  // Sun counter
  m_sunCountText = std::make_shared<TextBase>(
    SUN_COUNTER_X, SUN_COUNTER_Y,
    std::to_string(m_sunCount),
    0.0, 0.0, 0.0,   // Black
    true
  );

  // Brain counter
  m_brainEatenText = std::make_shared<TextBase>(
    WINDOW_WIDTH - 120, WINDOW_HEIGHT - 35,
    "Brains: 5/5",
    1.0, 0.2, 0.2,   // Red
    false
  );

  // Stage indicator
  m_stageText = std::make_shared<TextBase>(
    WINDOW_WIDTH - 120, WINDOW_HEIGHT - 65,
    "Stage: 1/5",
    1.0, 0.2, 0.2,   // Red
    false
  );

  // Create background
  auto background = std::make_shared<Background>();
  m_objects.push_back(background);

  // Create brains
  for (int row = 0; row < 5; ++row) {
    int x = 35;
    int y = RowToY(row);
    auto brain = std::make_shared<Brain>(x, y, row);
    m_objects.push_back(brain);
  }

  // Create progress meter
  int progressX = WINDOW_WIDTH - 100;
  int progressY = 12;
  auto progressMeter = std::make_shared<ProgressMeter>(progressX, progressY, m_currentStage);
  m_objects.push_back(progressMeter);

  // Create red line
  int deploymentCol = INITIAL_ZOMBIE_DEPLOYMENT_START_COL + m_currentStage;
  int redLineX = FIRST_COL_CENTER + deploymentCol * LAWN_GRID_WIDTH - LAWN_GRID_WIDTH / 2;
  int redLineY = LAWN_GRID_CENTER_Y;
  m_redLine = std::make_shared<RedLine>(redLineX, redLineY);
  m_objects.push_back(m_redLine);

  // Create zombie card
  int cardX = ZOMBIE_CARD_FIRST_X;
  int cardY = ZOMBIE_CARD_Y;
  auto regularCard = std::make_shared<ZombieCard>(
    cardX, cardY, 50,
    ZombieType::Regular,
    ImageID::ZOMBIE_CARD_REGULAR,
    this
  );
  m_objects.push_back(regularCard);

  auto coneheadCard = std::make_shared<ZombieCard>(
    cardX + ZOMBIE_CARD_SPACING, cardY, 75,
    ZombieType::Conehead,
    ImageID::ZOMBIE_CARD_CONEHEAD,
    this
  );
  m_objects.push_back(coneheadCard);

  auto bucketheadCard = std::make_shared<ZombieCard>(
    cardX + 2 * ZOMBIE_CARD_SPACING, cardY, 100,
    ZombieType::Buckethead,
    ImageID::ZOMBIE_CARD_BUCKET,
    this
  );
  m_objects.push_back(bucketheadCard);

  auto poleCard = std::make_shared<ZombieCard>(
    cardX + 3 * ZOMBIE_CARD_SPACING, cardY, 75,
    ZombieType::PoleVaulting,
    ImageID::ZOMBIE_CARD_POLE,
    this
  );
  m_objects.push_back(poleCard);

  // Create deployment zombie zones
  for (int row = 0; row < GAME_ROWS; ++row) {
    for (int col = 0; col < GAME_COLS; ++col) {
      int x = ColToX(col);
      int y = RowToY(row);
      auto zone = std::make_shared<DeployZombie>(x, y, row, col, this);
      m_objects.push_back(zone);
    }
  }

  // Generate initial plant defense for stage 0
  GeneratePlants();
}

// Update()
// Called every frame (~30 times per second).
// Updates all objects, handles collisions, stage progression,
// win/lose conditions, and UI text updates.
LevelStatus GameWorld::Update() {
  // Update all alive objects
  for (auto& obj : m_objects) {
    if (!obj->IsDead()) {
      obj->Update();
    }
  }

  // Collision: Zombie vs Brain
  for (auto& zombieObj : m_objects) {
    if (!zombieObj->IsZombie()) continue;
    if (zombieObj->IsDead()) continue;

    for (auto& brainObj : m_objects) {
      if (!brainObj->IsBrain()) continue;
      if (brainObj->IsDead()) continue;

      Brain* brain = static_cast<Brain*>(brainObj.get());
      if (brain->isEaten()) continue;

      if (IsColliding(zombieObj.get(), brainObj.get())) {
        brain->setEaten();             // Mark brain as eaten
        zombieObj->SetDead();          // Zombie dies after eating
        m_brainEaten[brain->GetRow()] = true;
        break;
      }
    }
  }

  // Stage progression check
  bool allEaten = true;
  for (int i = 0; i < 5; ++i) {
    if (!m_brainEaten[i]) {
      allEaten = false;
      break;
    }
  }

  if (allEaten) {
    // All 5 brains eaten -> advance stage or win
    if (m_currentStage == 4) {
      // Stage 5 complete -> win
      return LevelStatus::WINNING;
    } else {
      // Advance to next stage
      m_currentStage++;

      // Reset brain eaten flags
      for (int i = 0; i < 5; ++i) {
        m_brainEaten[i] = false;
      }

      // Remove old brains
      for (auto& obj : m_objects) {
        if (obj->IsBrain()) {
          obj->SetDead();
        }
      }

      // Remove old plants
      for (auto& obj : m_objects) {
        if (obj->IsPlant()) {
          obj->SetDead();
        }
      }

      // Create new brains
      for (int row = 0; row < 5; ++row) {
        int x = 35;
        int y = RowToY(row);
        auto brain = std::make_shared<Brain>(x, y, row);
        m_objects.push_back(brain);
      }

      // Generate plants for the new stage
      GeneratePlants();

      // Move red line right by one column
      int deploymentCol = INITIAL_ZOMBIE_DEPLOYMENT_START_COL + m_currentStage;
      int redLineX = FIRST_COL_CENTER + deploymentCol * LAWN_GRID_WIDTH - LAWN_GRID_WIDTH / 2;
      m_redLine->SetPosition(redLineX, LAWN_GRID_CENTER_Y);

      // Update progress meter
      for (auto& obj : m_objects) {
        if (obj->IsProgressMeter()) {
          ProgressMeter* meter = static_cast<ProgressMeter*>(obj.get());
          meter->SetStage(m_currentStage);
          break;
        }
      }

      // Update UI text
      m_stageText->SetText("Stage: " + std::to_string(m_currentStage + 1) + "/5");
      m_brainEatenText->SetText("Brains: 0/5");
    }
  }

  // Lose condition check
  // Lose if: brains remain, sun < 50, and no zombies on field
  bool anyBrainLeft = false;
  for (int i = 0; i < 5; ++i) {
    if (!m_brainEaten[i]) {
      anyBrainLeft = true;
      break;
    }
  }

  if (anyBrainLeft) {
    bool hasZombie = false;
    for (auto& obj : m_objects) {
      if (obj->IsZombie() && !obj->IsDead()) {
        hasZombie = true;
        break;
      }
    }

    if (m_sunCount < 50 && !hasZombie) {
      CleanUp();
      return LevelStatus::LOSING;
    }
  }

  // Collision: Zombie vs Plant 
  for (auto& zombieObj : m_objects) {
    if (!zombieObj->IsZombie()) continue;
    if (zombieObj->IsDead()) continue;

    Zombie* zombie = static_cast<Zombie*>(zombieObj.get());
    
    if (zombie->IsJumping()) {
        continue;
    }
    
    bool isEating = false;

    for (auto& plantObj : m_objects) {
      if (!plantObj->IsPlant()) continue;
      if (plantObj->IsDead()) continue;

      if (IsColliding(zombieObj.get(), plantObj.get())) {
        if (!zombie->IsEating()) {
          zombie->StartEating();
        }
        isEating = true;

        Plant* plant = static_cast<Plant*>(plantObj.get());
        plant->TakeDamage(4);

        // Sunflower drops 6 suns when it dies
        if (plant->IsDead() && plant->IsSunflower()) {
          for (int i = 0; i < 6; ++i) {
            int sunX = plant->GetX() + randInt(-30, 30);
            int sunY = plant->GetY() + randInt(-30, 30);
            auto sun = std::make_shared<Sun>(sunX, sunY, this);
            m_objects.push_back(sun);
          }
        }
        break;
      }
    }

    // If not eating any plant, resume walking
    if (!isEating && zombie->IsEating()) {
      zombie->StopEating();
    }
  }

  // Collision: Pea vs Zombie
  for (auto& peaObj : m_objects) {
    if (!peaObj->IsPea()) continue;
    if (peaObj->IsDead()) continue;

    for (auto& zombieObj : m_objects) {
      if (!zombieObj->IsZombie()) continue;
      if (zombieObj->IsDead()) continue;

      if (IsColliding(peaObj.get(), zombieObj.get())) {
        RegularZombie* zombie = static_cast<RegularZombie*>(zombieObj.get());
        zombie->TakeDamage(24);
        peaObj->SetDead();           // Pea disappears on hit
        break;
      }
    }
  }

  // Remove dead objects
  RemoveDeadObjects();

  // Update UI text
  m_sunCountText->SetText(std::to_string(m_sunCount));

  int remaining = 0;
  for (int i = 0; i < 5; ++i) {
    if (!m_brainEaten[i]) remaining++;
  }
  m_brainEatenText->SetText("Brains: " + std::to_string(remaining) + "/5");
  m_stageText->SetText("Stage: " + std::to_string(m_currentStage + 1) + "/5");

  return LevelStatus::ONGOING;
}

// CleanUp()
// Called when game ends. Clears all objects.
void GameWorld::CleanUp() {
  m_objects.clear();
}

// AddObject()
// Adds a new game object to the world.
// Called by other objects (e.g., Peashooter creating Pea).
void GameWorld::AddObject(std::shared_ptr<GameObject> object) {
  m_objects.push_back(object);
}

// RemoveDeadObjects()
// Removes all objects marked as dead from the container.
// Called every frame after all updates and collisions.
// Uses iterator-based loop for safe deletion.
void GameWorld::RemoveDeadObjects() {
  auto it = m_objects.begin();
  while (it != m_objects.end()) {
    if ((*it)->IsDead()) {
      it = m_objects.erase(it);
    } else {
      ++it;
    }
  }
}

// IsColliding()
// Checks if two GameObject rectangles overlap.
bool GameWorld::IsColliding(GameObject* a, GameObject* b) {
  // Calculate edges of object A
  int ax1 = a->GetX() - a->GetWidth() / 2;
  int ax2 = a->GetX() + a->GetWidth() / 2;
  int ay1 = a->GetY() - a->GetHeight() / 2;
  int ay2 = a->GetY() + a->GetHeight() / 2;

  // Calculate edges of object B
  int bx1 = b->GetX() - b->GetWidth() / 2;
  int bx2 = b->GetX() + b->GetWidth() / 2;
  int by1 = b->GetY() - b->GetHeight() / 2;
  int by2 = b->GetY() + b->GetHeight() / 2;

  // No overlap if one is completely left/right/above/below the other
  return !(ax2 < bx1 || ax1 > bx2 || ay2 < by1 || ay1 > by2);
}

// HasPlantAt()
// Checks if a specific grid cell has a living plant.
// Used by DeployZombie to prevent deployment on occupied cells.
bool GameWorld::HasPlantAt(int row, int col) const {
  int targetX = ColToX(col);
  int targetY = RowToY(row);

  for (auto& obj : m_objects) {
    if (!obj->IsPlant()) continue;
    if (obj->IsDead()) continue;

    // Approximate check: within half the grid cell size
    if (std::abs(obj->GetX() - targetX) < 40 &&
        std::abs(obj->GetY() - targetY) < 50) {
      return true;
    }
  }
  return false;
}

// HasZombieOnRow()
// Checks if there is a zombie on a given row to the right of minX.
// Used by Peashooter to decide whether to fire.
bool GameWorld::HasZombieOnRow(int row, int minX) const {
  int targetY = RowToY(row);

  for (auto& obj : m_objects) {
    if (!obj->IsZombie()) continue;
    if (obj->IsDead()) continue;
    if (obj->GetY() != targetY) continue;   // Same row
    if (obj->GetX() < minX) continue;       // To the right
    return true;
  }
  return false;
}

// GeneratePlants()
// Generates plants based on the current stage.
// Called during Init() and after each stage advancement.
void GameWorld::GeneratePlants() {
  switch (m_currentStage) {
    case 0: GenerateStage1(); break;
    case 1: GenerateStage2(); break;
    case 2: GenerateStage3(); break;
    case 3: GenerateStage4(); break;
    case 4: GenerateStage5(); break;
    default: break;
  }
}

// Stage 1
// 3 sunflowers + 3 peashooters
void GameWorld::GenerateStage1() {
  m_objects.push_back(std::make_shared<Sunflower>(ColToX(2), RowToY(0)));
  m_objects.push_back(std::make_shared<Sunflower>(ColToX(2), RowToY(2)));
  m_objects.push_back(std::make_shared<Sunflower>(ColToX(2), RowToY(4)));

  m_objects.push_back(std::make_shared<Peashooter>(ColToX(4), RowToY(1), this));
  m_objects.push_back(std::make_shared<Peashooter>(ColToX(4), RowToY(3), this));
  m_objects.push_back(std::make_shared<Peashooter>(ColToX(3), RowToY(4), this));
}

// Stage 2
// One peashooter per row + extra sunflowers
void GameWorld::GenerateStage2() {
  m_objects.push_back(std::make_shared<Sunflower>(ColToX(2), RowToY(0)));
  m_objects.push_back(std::make_shared<Sunflower>(ColToX(2), RowToY(1)));
  m_objects.push_back(std::make_shared<Sunflower>(ColToX(2), RowToY(3)));
  m_objects.push_back(std::make_shared<Sunflower>(ColToX(2), RowToY(4)));

  for (int row = 0; row < 5; ++row) {
    m_objects.push_back(std::make_shared<Peashooter>(ColToX(4), RowToY(row), this));
  }
}

// Stage 3
// Two peashooters per row (columns 4 and 5)
void GameWorld::GenerateStage3() {
  for (int row = 0; row < 5; ++row) {
    m_objects.push_back(std::make_shared<Peashooter>(ColToX(4), RowToY(row), this));
    m_objects.push_back(std::make_shared<Peashooter>(ColToX(5), RowToY(row), this));
  }
  m_objects.push_back(std::make_shared<Sunflower>(ColToX(2), RowToY(0)));
  m_objects.push_back(std::make_shared<Sunflower>(ColToX(2), RowToY(1)));
  m_objects.push_back(std::make_shared<Sunflower>(ColToX(2), RowToY(3)));
  m_objects.push_back(std::make_shared<Sunflower>(ColToX(2), RowToY(4)));
}

// Stage 4
// Peashooters in columns 3 and 5 on all rows,
// plus extra in column 4 on even rows
void GameWorld::GenerateStage4() {
  // 前排坚果墙（列3）
  for (int row = 0; row < 5; ++row) {
    m_objects.push_back(std::make_shared<WallNut>(ColToX(3), RowToY(row)));
  }

  // 豌豆射手（列4-5）
  for (int row = 0; row < 5; ++row) {
    m_objects.push_back(std::make_shared<Peashooter>(ColToX(4), RowToY(row), this));
    m_objects.push_back(std::make_shared<Peashooter>(ColToX(5), RowToY(row), this));
  }
  // 向日葵
  for (int row = 0; row < 5; ++row) {
    m_objects.push_back(std::make_shared<Sunflower>(ColToX(2), RowToY(row)));
  }
}

// Stage 5
// Peashooters in columns 3, 4, 5 on all rows
void GameWorld::GenerateStage5() {
  // 前排坚果墙（列2-3）
  for (int row = 0; row < 5; ++row) {
    m_objects.push_back(std::make_shared<WallNut>(ColToX(3), RowToY(row)));
  }
  // 中排坚果墙（列4）
  for (int row = 0; row < 5; ++row) {
    m_objects.push_back(std::make_shared<WallNut>(ColToX(4), RowToY(row)));
  }
  // 豌豆射手（列4-5）
  for (int row = 0; row < 5; ++row) {
    m_objects.push_back(std::make_shared<Peashooter>(ColToX(5), RowToY(row), this));
  }
  // 向日葵
  for (int row = 0; row < 5; ++row) {
    m_objects.push_back(std::make_shared<Sunflower>(ColToX(2), RowToY(row)));
  }
}

void GameWorld::DeployZombieByType(int x, int y, ZombieType type) {
  std::shared_ptr<Zombie> zombie;

  switch (type) {
    case ZombieType::Regular:
      zombie = std::make_shared<RegularZombie>(x, y);
      break;
    case ZombieType::Conehead:
      zombie = std::make_shared<ConeheadZombie>(x, y);
      break;
    case ZombieType::Buckethead:
      zombie = std::make_shared<BucketheadZombie>(x, y);
      break;
    case ZombieType::PoleVaulting:
      zombie = std::make_shared<PoleVaultingZombie>(x, y);
      zombie->SetWorld(this);
      break;
    default:
      zombie = std::make_shared<RegularZombie>(x, y);
      break;
  }
  AddObject(zombie);
}

void GameWorld::DeselectAllCards() {
  for (auto& obj : m_objects) {
    if (auto card = std::dynamic_pointer_cast<ZombieCard>(obj)) {
      card->SetSelected(false);
    }
  }
}

bool GameWorld::HasPlantNear(int x, int y, int range) const {
    for (auto& obj : m_objects) {
        if (!obj->IsPlant()) continue;
        if (obj->IsDead()) continue;
        if (std::abs(obj->GetX() - x) < range && std::abs(obj->GetY() - y) < 50) {
            return true;
        }
    }
    return false;
}

int GameWorld::GetNearestPlantX(int x, int y, int range) const {
    int nearestX = x;
    int minDist = range;
    
    for (auto& obj : m_objects) {
        if (!obj->IsPlant()) continue;
        if (obj->IsDead()) continue;
        
        int dx = std::abs(obj->GetX() - x);
        if (dx < minDist && std::abs(obj->GetY() - y) < 50) {
            minDist = dx;
            nearestX = obj->GetX();
        }
    }
    return nearestX;
}