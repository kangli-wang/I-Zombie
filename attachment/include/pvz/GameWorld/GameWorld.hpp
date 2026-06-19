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

  void Init() override;

  LevelStatus Update() override;

  void CleanUp() override;

  void AddObject(std::shared_ptr<GameObject> object);

  void RemoveDeadObjects();

  int GetSunCount() const { return m_sunCount; }
  
  void AddSunCount(int amount) { m_sunCount += amount; }
  
  int GetCurrentStage() const { return m_currentStage; }

private:
  std::list<std::shared_ptr<GameObject>> m_objects;
  
  // Game state variables
  int m_sunCount;
  bool m_brainEaten[5];
  int m_currentStage;
  bool m_plantGrid[5][9];

  // Text objects for displaying game state
  std::shared_ptr<TextBase> m_sunCountText;
  std::shared_ptr<TextBase> m_brainEatenText;
  std::shared_ptr<TextBase> m_stageText;
  
  // RedLine object to indicate zombie deployment line
  std::shared_ptr<RedLine> m_redLine;

  // Zombie card for player to select
  std::shared_ptr<ZombieCard> m_regularCard;
};

#endif // !GAMEWORLD_HPP__
