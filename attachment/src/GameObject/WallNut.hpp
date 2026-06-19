#ifndef WALLNUT_HPP__
#define WALLNUT_HPP__

#include "Plant.hpp"

class WallNut : public Plant {
public:
    WallNut(int x, int y);

    void Update() override;
    void OnClick() override;

private:
    static constexpr int HP = 3600;
    static constexpr int CRACK_THRESHOLD = HP / 3;
    bool m_isCracked = false;
};

#endif