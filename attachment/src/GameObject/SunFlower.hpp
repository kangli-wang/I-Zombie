#ifndef SUNFLOWER_HPP__
#define SUNFLOWER_HPP__

#include "Plant.hpp"

class Sunflower : public Plant {
public:
    Sunflower(int x, int y);
    
    void Update() override;
    void OnClick() override;

    bool IsSunflower() const override { return true; }
    
private:
    static constexpr int HP = 340;
};

#endif