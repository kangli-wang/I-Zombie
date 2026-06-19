#include "ProgressMeter.hpp"
#include "pvz/utils.hpp"

ProgressMeter::ProgressMeter(int x, int y, int stage)
    : GameObject(
        ImageID::PROGRESS_METER_STAGE_1,  // 默认显示第 1 阶段
        x,
        y,
        LayerID::UI,
        158,//width
        24,//height
        AnimID::NO_ANIMATION
      )
    , m_stage(stage) {}

void ProgressMeter::Update() {}

void ProgressMeter::OnClick() {}

void ProgressMeter::SetStage(int stage) {
    m_stage = stage;
    switch (stage) {
        case 0:
            ChangeImage(ImageID::PROGRESS_METER_STAGE_1);
            break;
        case 1:
            ChangeImage(ImageID::PROGRESS_METER_STAGE_2);
            break;
        case 2:
            ChangeImage(ImageID::PROGRESS_METER_STAGE_3);
            break;
        case 3:
            ChangeImage(ImageID::PROGRESS_METER_STAGE_4);
            break;
        case 4:
            ChangeImage(ImageID::PROGRESS_METER_STAGE_5);
            break;
        default:
            ChangeImage(ImageID::PROGRESS_METER_STAGE_1);
            break;
    }
}