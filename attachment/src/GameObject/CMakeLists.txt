cmake_minimum_required(VERSION 3.16)

add_library(
  GameObject
  STATIC
  GameObject.cpp
  Background.cpp
  Brain.cpp
  ProgressMeter.cpp
  RedLine.cpp
  ZombieCard.cpp
  DeployZombie.cpp
  RegularZombie.cpp
  SunFlower.cpp
  Peashooter.cpp
  Pea.cpp
  Sun.cpp
  Zombie.cpp
  ConeHeadZombie.cpp
  BucketHeadZombie.cpp
  WallNut.cpp
  PoleVaultingZombie.cpp
  CooldownMask.cpp
)

target_link_libraries(
  GameObject
  Framework
)
