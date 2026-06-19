#ifndef REDLINE_HPP__
#define REDLINE_HPP__

#include "pvz/GameObject/GameObject.hpp"

class RedLine : public GameObject {
public:
    RedLine(int x, int y);
    
    void Update() override;
    void OnClick() override;
    
    void SetPosition(int x, int y);
};

#endif