#ifndef CONEHEADZOMBIE_HPP__
#define CONEHEADZOMBIE_HPP__

#include "Zombie.hpp"

class ConeheadZombie : public Zombie {
public:
    ConeheadZombie(int x, int y);

    void Update() override;
    void OnClick() override;

private:
    static constexpr int MAX_HP = 560;
    static constexpr int THRESHOLD_HP = 200;
    bool m_hasCone = true;
};

#endif