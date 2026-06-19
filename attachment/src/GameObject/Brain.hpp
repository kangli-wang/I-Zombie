#ifndef BRAIN_HPP__
#define BRAIN_HPP__

#include "pvz/GameObject/GameObject.hpp"

class Brain : public GameObject {
public:
    Brain(int x, int y, int row);

    void Update() override;
    void OnClick() override;

    int GetRow() const { return m_row; }

    bool isEaten() const { return m_isEaten; }
    void setEaten() { m_isEaten = true; SetDead();}

    bool IsBrain() const override { return true; }
    
private:
    int m_row;
    bool m_isEaten = false;
};

#endif