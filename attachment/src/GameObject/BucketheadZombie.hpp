#ifndef BUCKETHEADZOMBIE_HPP__
#define BUCKETHEADZOMBIE_HPP__

#include "Zombie.hpp"

class BucketheadZombie : public Zombie {
public:
    BucketheadZombie(int x, int y);

    void Update() override;
    void OnClick() override;

private:
    static constexpr int MAX_HP = 1450;
    static constexpr int THRESHOLD_HP = 200;
    bool m_hasBucket = true;
};

#endif