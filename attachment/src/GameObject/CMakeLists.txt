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
)

target_link_libraries(
  GameObject
  Framework
)
