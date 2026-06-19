#ifndef PROGRESSMETER_HPP__
#define PROGRESSMETER_HPP__

#include "pvz/GameObject/GameObject.hpp"

class ProgressMeter : public GameObject {
public:
    ProgressMeter(int x, int y, int stage);
    
    void Update() override;
    void OnClick() override;
    
    void SetStage(int stage);
    
private:
    int m_stage;
};

#endif