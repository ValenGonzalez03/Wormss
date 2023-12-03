#include "client_position_converter.h"

PositionConverter::PositionConverter(){}

int PositionConverter::convert_from_m_to_px(float meters) {
    return meters * RATIO_MTS_PX;
}

Position PositionConverter::convert_position_to_px(Position pos) {
    int world_height = 360; //modificar despues
    int pos_x_px = convert_from_m_to_px(pos.get_position_x());
    int pos_y_px = world_height - convert_from_m_to_px(pos.get_position_y());
    Position position(pos_x_px, pos_y_px);
    return position;
}