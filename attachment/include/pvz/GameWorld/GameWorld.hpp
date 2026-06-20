#ifndef GAMEWORLD_HPP__
#define GAMEWORLD_HPP__

#include <list>
#include <memory>

#include "pvz/Framework/WorldBase.hpp"
#include "pvz/Framework/TextBase.hpp"
#include "pvz/GameObject/GameObject.hpp"
#include "pvz/utils.hpp"

class ZombieCard;
class RedLine;
class DeployZombie;

class GameWorld : public WorldBase {
public:
  // Consider:
  // Use shared_from_this() instead of "this" to create a pointer to oneself?
  // Use unique_ptr<> / shared_ptr<> to manage GameObjects?
  GameWorld() = default;
  ~GameWorld() = default;

  // framework methods
  void Init() override;
  LevelStatus Update() override;
  void CleanUp() override;

  // game object management
  void AddObject(std::shared_ptr<GameObject> object);
  void RemoveDeadObjects();

  // game state related methods
  int GetSunCount() const { return m_sunCount; }
  int GetCurrentStage() const { return m_currentStage; }
  
  // state modification methods
  void AddSunCount(int amount) { m_sunCount += amount; }
  
  // Collision world query methods
  bool HasPlantAt(int row, int col) const;
  bool HasZombieOnRow(int row, int minX) const;

  // plant generation method
  void GeneratePlants();

  void SetSelectedZombieType(ZombieType type) { m_selectedZombieType = type; m_hasSelected = true; }
  ZombieType GetSelectedZombieType() const { return m_selectedZombieType; }
  void DeployZombieByType(int x, int y, ZombieType type);
  void ClearSelectedZombieType(){m_hasSelected = false;};
  bool HasSelectedZombieType() const { return m_hasSelected; }
  void DeselectAllCards();
  bool HasPlantNear(int x, int y, int range) const;
  int GetNearestPlantX(int x, int y, int range) const;
private:
  // helper methods
  bool IsColliding(GameObject* a, GameObject* b);

  // Stage-specific plant generation (called by GeneratePlants)
  void GenerateStage1();
  void GenerateStage2();
  void GenerateStage3();
  void GenerateStage4();
  void GenerateStage5();

  // Member variables

  // List of all game objects in the world
  std::list<std::shared_ptr<GameObject>> m_objects;
  
  // Game state variables
  int m_sunCount;
  bool m_brainEaten[5];
  int m_currentStage;
  bool m_plantGrid[5][9];

  // UI Text objects
  std::shared_ptr<TextBase> m_sunCountText;
  std::shared_ptr<TextBase> m_brainEatenText;
  std::shared_ptr<TextBase> m_stageText;
  
  // Game objects that need to be accessed globally (e.g., for collision checks)
  std::shared_ptr<RedLine> m_redLine;

  ZombieType m_selectedZombieType = ZombieType::Regular;
  bool m_hasSelected = false;
};

#endif // !GAMEWORLD_HPP__
