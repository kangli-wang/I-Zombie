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
#include <iostream>  // 用于调试输出
void GameWorld::Init() {
  m_objects.clear();
  
  // Initialize game world state
  m_sunCount = 10000;
  m_currentStage = 0;
  
  for (int i = 0; i < 5; ++i) {
    m_brainEaten[i] = false;
    for (int j = 0; j < 9; ++j) {
      m_plantGrid[i][j] = false;
    }
  }
  
  m_sunCountText = std::make_shared<TextBase>(
    SUN_COUNTER_X, 
    SUN_COUNTER_Y, 
    std::to_string(m_sunCount),
    0.0,0.0,0.0,
    true);
  
    m_brainEatenText = std::make_shared<TextBase>(
    WINDOW_WIDTH - 120,
    WINDOW_HEIGHT - 35,
    "Brains: 5/5",
    1.0,0.2,0.2,
    false);
  
    m_stageText = std::make_shared<TextBase>(
    WINDOW_WIDTH - 120,
    WINDOW_HEIGHT - 65,
    "Stage: 1/5",
    1.0,0.2,0.2,
    false);

  // Add background and brains to the game world
  auto background = std::make_shared<Background>();
  m_objects.push_back(background);

  // Add brains to the game world
  for (int row = 0; row < 5; ++row) {
        int x = 35;
        int y = FIRST_ROW_CENTER + row * LAWN_GRID_HEIGHT;
        auto brain = std::make_shared<Brain>(x, y, row);
        m_objects.push_back(brain);
    }

  // Add progress meter to the game world
  int progressX = WINDOW_WIDTH / 2;
  int progressY = 50;
  auto progressMeter = std::make_shared<ProgressMeter>(progressX, progressY, m_currentStage);
  m_objects.push_back(progressMeter);

  // Add RedLine to the game world
  int deploymentCol = INITIAL_ZOMBIE_DEPLOYMENT_START_COL + m_currentStage;
  int redLineX = FIRST_COL_CENTER + deploymentCol * LAWN_GRID_WIDTH - LAWN_GRID_WIDTH / 2;
  int redLineY = LAWN_GRID_CENTER_Y;
  m_redLine = std::make_shared<RedLine>(redLineX, redLineY);
  m_objects.push_back(m_redLine);

  // Add zombie card to the game world
  int cardX = ZOMBIE_CARD_FIRST_X;
  int cardY = ZOMBIE_CARD_Y;
  m_regularCard = std::make_shared<ZombieCard>(
    cardX, 
    cardY, 
    50,
    this
  );
  m_objects.push_back(m_regularCard);
  
  // Add DeployZombie zones to the game world
  for (int row = 0; row < GAME_ROWS; ++row) {
    for (int col = 0; col < GAME_COLS; ++col) {
      int x = FIRST_COL_CENTER + col * LAWN_GRID_WIDTH;
      int y = FIRST_ROW_CENTER + row * LAWN_GRID_HEIGHT;
      auto zone = std::make_shared<DeployZombie>(x, y, row, col, this);
      m_objects.push_back(zone);
    }
  }

  GeneratePlants();
}

LevelStatus GameWorld::Update() {
  for (auto& obj : m_objects) {
    if (!obj->IsDead()) {
      obj->Update();
    }
  }

  for (auto& zombieObj : m_objects) {
    if (!zombieObj->IsZombie()) continue;
    if (zombieObj->IsDead()) continue;
    for (auto& brainObj : m_objects) {
      if (!brainObj->IsBrain()) continue;
      if (brainObj->IsDead()) continue;     
      Brain* brain = static_cast<Brain*>(brainObj.get());
      if (brain->isEaten()) continue;
      if (IsColliding(zombieObj.get(), brainObj.get())) {
        // Zombie eats the brain
        brain->setEaten();
        zombieObj->SetDead();
        m_brainEaten[brain->GetRow()] = true;      
        break;
      }
    }
  }

  // ===== 检查阶段是否完成 =====
  bool allEaten = true;
  for (int i = 0; i < 5; ++i) {
    if (!m_brainEaten[i]) {
        allEaten = false;
        break;
    }
  }

  if (allEaten) {
    if (m_currentStage == 4) {
        // 最终阶段完成 → 胜利
        std::cout << "GAME WIN! All stages completed!" << std::endl;
        return LevelStatus::WINNING;
    } 
    else {
      m_currentStage++;
      std::cout << "Stage " << (m_currentStage + 1) << " started!" << std::endl;

      for (int i = 0; i < 5; ++i) {
        m_brainEaten[i] = false;
      }
      
      for (auto& obj : m_objects) {
        if (obj->IsBrain()) {
          obj->SetDead();
        }
      }
      
      for (auto& obj : m_objects) {
        if (obj->IsPlant()) {
          obj->SetDead();
        }
      }

      for (int row = 0; row < 5; ++row) {
        int x = 35;
        int y = FIRST_ROW_CENTER + row * LAWN_GRID_HEIGHT;
        auto brain = std::make_shared<Brain>(x, y, row);
        m_objects.push_back(brain);
      }

      GeneratePlants();

      int deploymentCol = INITIAL_ZOMBIE_DEPLOYMENT_START_COL + m_currentStage;
      int redLineX = FIRST_COL_CENTER + deploymentCol * LAWN_GRID_WIDTH - LAWN_GRID_WIDTH / 2;
      m_redLine->SetPosition(redLineX, LAWN_GRID_CENTER_Y);
      
      for (auto& obj : m_objects) {
        if (obj->IsProgressMeter()) {
          ProgressMeter* meter = static_cast<ProgressMeter*>(obj.get());
          meter->SetStage(m_currentStage);
          break;
        }
      }

      m_stageText->SetText("Stage: " + std::to_string(m_currentStage + 1) + "/5");
      m_brainEatenText->SetText("Brains: 0/5");
    }
  }
  // ===== 6. 失败检测（新增） =====
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
      std::cout << "GAME LOSE! No sun, no zombies, brains remain!" << std::endl;
      CleanUp();
      return LevelStatus::LOSING;
    }
  }
  // ===== 僵尸 vs 植物碰撞检测 =====
  for (auto& zombieObj : m_objects) {
    if (!zombieObj->IsZombie()) continue;
    if (zombieObj->IsDead()) continue;
    RegularZombie* zombie = static_cast<RegularZombie*>(zombieObj.get());
    bool isEating = false;
    for (auto& plantObj : m_objects) {
      if (!plantObj->IsPlant()) continue;
      if (plantObj->IsDead()) continue;
      if (IsColliding(zombieObj.get(), plantObj.get())) {
        std::cout << "Zombie is eating a plant!" << std::endl;
        zombie->StartEating();
        isEating = true;
        Plant* plant = static_cast<Plant*>(plantObj.get());
        plant->TakeDamage(4);
        break;
      }
    }
    if (!isEating) {
      zombie->StopEating();
    }
  } 
  // ===== 豌豆 vs 僵尸碰撞检测 =====
  for (auto& peaObj : m_objects) {
    if (!peaObj->IsPea()) continue;
    if (peaObj->IsDead()) continue;
    
    for (auto& zombieObj : m_objects) {
      if (!zombieObj->IsZombie()) continue;
      if (zombieObj->IsDead()) continue;
      if (IsColliding(peaObj.get(), zombieObj.get())) {
        RegularZombie* zombie = static_cast<RegularZombie*>(zombieObj.get());
        zombie->TakeDamage(24);
        peaObj->SetDead();
        std::cout << "Pea hit zombie!" << std::endl;
        break;
      }
    }
  }
  RemoveDeadObjects();
  m_sunCountText->SetText(std::to_string(m_sunCount));
  int remaining = 0;
  for (int i = 0; i < 5; ++i) {
    if (!m_brainEaten[i]) remaining++;
  }
  m_brainEatenText->SetText("Brains: " + std::to_string(remaining) + "/5");
  m_stageText->SetText("Stage: " + std::to_string(m_currentStage + 1) + "/5");
  return LevelStatus::ONGOING;
}

void GameWorld::CleanUp() {
  m_objects.clear();
}

void GameWorld::AddObject(std::shared_ptr<GameObject> object) {
  m_objects.push_back(object);
}

void GameWorld::RemoveDeadObjects() {
  auto it = m_objects.begin();
  while (it != m_objects.end()) {
    if ((*it)->IsDead()) {
      it = m_objects.erase(it);
    } 
    else {
      ++it;
    }
  }
}

bool GameWorld::IsColliding(GameObject* a, GameObject* b) {
    int ax1 = a->GetX() - a->GetWidth() / 2;
    int ax2 = a->GetX() + a->GetWidth() / 2;
    int ay1 = a->GetY() - a->GetHeight() / 2;
    int ay2 = a->GetY() + a->GetHeight() / 2;
    
    int bx1 = b->GetX() - b->GetWidth() / 2;
    int bx2 = b->GetX() + b->GetWidth() / 2;
    int by1 = b->GetY() - b->GetHeight() / 2;
    int by2 = b->GetY() + b->GetHeight() / 2;
    
    return !(ax2 < bx1 || ax1 > bx2 || ay2 < by1 || ay1 > by2);
}

bool GameWorld::HasPlantAt(int row, int col) const {
    int targetX = ColToX(col);
    int targetY = RowToY(row);
    for (auto& obj : m_objects) {
        if (!obj->IsPlant()) continue;
        if (obj->IsDead()) continue;
        if (std::abs(obj->GetX() - targetX) < 40 && std::abs(obj->GetY() - targetY) < 50) {
            return true;
        }
    }
    return false;
}

bool GameWorld::HasZombieOnRow(int row, int minX) const {
    int targetY = RowToY(row); 
    for (auto& obj : m_objects) {
        if (!obj->IsZombie()) continue;
        if (obj->IsDead()) continue;
        if (obj->GetY() != targetY) continue;
        if (obj->GetX() < minX) continue;
        return true;
    }
    return false;
}

void GameWorld::GeneratePlants() {
    switch (m_currentStage) {
        case 0:
            GenerateStage1();
            break;
        case 1:
            GenerateStage2();
            break;
        case 2:
            GenerateStage3();
            break;
        case 3:
            GenerateStage4();
            break;
        case 4:
            GenerateStage5();
            break;
        default:
            break;
    }
}

// 第1阶段：简单入门
void GameWorld::GenerateStage1() {
    // 行0：列4 豌豆射手
    m_objects.push_back(std::make_shared<Peashooter>(ColToX(4), RowToY(0), this));
    // 行1：列2 向日葵
    m_objects.push_back(std::make_shared<Sunflower>(ColToX(2), RowToY(1)));
    // 行2：列4 豌豆射手
    m_objects.push_back(std::make_shared<Peashooter>(ColToX(4), RowToY(2), this));
    // 行3：列2 向日葵
    m_objects.push_back(std::make_shared<Sunflower>(ColToX(2), RowToY(3)));
    // 行4：列3 豌豆射手
    m_objects.push_back(std::make_shared<Peashooter>(ColToX(3), RowToY(4), this));
}

// 第2阶段：增加植物密度
void GameWorld::GenerateStage2() {
    // 每行至少一个攻击植物
    for (int row = 0; row < 5; ++row) {
        m_objects.push_back(std::make_shared<Peashooter>(ColToX(4), RowToY(row), this));
    }
    // 额外向日葵
    m_objects.push_back(std::make_shared<Sunflower>(ColToX(2), RowToY(1)));
    m_objects.push_back(std::make_shared<Sunflower>(ColToX(2), RowToY(3)));
}

// 第3阶段：增加后排植物
void GameWorld::GenerateStage3() {
    // 前排豌豆射手（列4-5）
    for (int row = 0; row < 5; ++row) {
        m_objects.push_back(std::make_shared<Peashooter>(ColToX(4), RowToY(row), this));
        m_objects.push_back(std::make_shared<Peashooter>(ColToX(5), RowToY(row), this));
    }
    // 向日葵
    m_objects.push_back(std::make_shared<Sunflower>(ColToX(2), RowToY(0)));
    m_objects.push_back(std::make_shared<Sunflower>(ColToX(2), RowToY(2)));
    m_objects.push_back(std::make_shared<Sunflower>(ColToX(2), RowToY(4)));
}

// 第4阶段：更密集
void GameWorld::GenerateStage4() {
    // 多行多列豌豆射手
    for (int row = 0; row < 5; ++row) {
        m_objects.push_back(std::make_shared<Peashooter>(ColToX(3), RowToY(row), this));
        m_objects.push_back(std::make_shared<Peashooter>(ColToX(5), RowToY(row), this));
        // 偶数列额外增加
        if (row % 2 == 0) {
            m_objects.push_back(std::make_shared<Peashooter>(ColToX(4), RowToY(row), this));
        }
    }
    // 向日葵
    m_objects.push_back(std::make_shared<Sunflower>(ColToX(2), RowToY(1)));
    m_objects.push_back(std::make_shared<Sunflower>(ColToX(2), RowToY(3)));
}

// 第5阶段：最终防线（最密集）
void GameWorld::GenerateStage5() {
    // 所有行，列3-5 全部填满豌豆射手
    for (int row = 0; row < 5; ++row) {
        for (int col = 3; col <= 5; ++col) {
            m_objects.push_back(std::make_shared<Peashooter>(ColToX(col), RowToY(row), this));
        }
    }
    // 向日葵
    m_objects.push_back(std::make_shared<Sunflower>(ColToX(2), RowToY(0)));
    m_objects.push_back(std::make_shared<Sunflower>(ColToX(2), RowToY(2)));
    m_objects.push_back(std::make_shared<Sunflower>(ColToX(2), RowToY(4)));
}