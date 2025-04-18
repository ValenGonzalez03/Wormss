#include "client_position_converter.h"

PositionConverter::PositionConverter(){}

int PositionConverter::convert_x_from_m_to_px(float meters) {
    return meters * RATIO_MTS_PX;
}

int PositionConverter::convert_y_from_m_to_px(float meters) {
    float world_height = 360.0f;
    return world_height - meters * RATIO_MTS_PX;
}

Position PositionConverter::convert_position_to_px(Position pos) {
    float world_height = 360.0f; //modificar despues
    float pos_x_px = pos.get_position_x() * RATIO_MTS_PX;
    float pos_y_px = world_height - pos.get_position_y() * RATIO_MTS_PX;;
    Position position(pos_x_px, pos_y_px);
    return position;
}