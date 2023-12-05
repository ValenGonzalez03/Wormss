#include "position_converter.h"

PositionConverter::PositionConverter(){}

int PositionConverter::convert_from_m_to_px(float meters) {
    return meters * RATIO_MTS_PX;
}

float PositionConverter::convert_from_px_to_m(int px) {
    return float(px) / RATIO_MTS_PX;
}