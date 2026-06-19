#ifndef REGULARZOMBIE_HPP__
#define REGULARZOMBIE_HPP__

#include "Zombie.hpp"

class RegularZombie : public Zombie {
public:
    RegularZombie(int x, int y);
    
    void Update() override;
    void OnClick() override;
    
private:
    static constexpr int MAX_HP = 260;
};

#endif