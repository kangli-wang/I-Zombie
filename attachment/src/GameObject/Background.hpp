#ifndef BACKGROUND_HPP__
#define BACKGROUND_HPP__

#include "pvz/GameObject/GameObject.hpp"

class Background : public GameObject{
public: 
    Background();
    void Update() override;
    void OnClick() override;
};

#endif // !BACKGROUND_HPP__