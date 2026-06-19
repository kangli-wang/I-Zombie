#ifndef DEPLOYZOMBIE_HPP__
#define DEPLOYZOMBIE_HPP__

#include "pvz/GameObject/GameObject.hpp"

class GameWorld;

class DeployZombie : public GameObject {
public:
    DeployZombie(int x, int y, int row, int col, GameWorld* world);
    
    void Update() override;
    void OnClick() override;
    
    int GetRow() const { return m_row; }
    int GetCol() const { return m_col; }
    
private:
    int m_row;
    int m_col;
    GameWorld* m_world;
};

#endif