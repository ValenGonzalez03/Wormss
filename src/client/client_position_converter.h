#ifndef CLIENT_POSITION_CONVERTER_H_
#define CLIENT_POSITION_CONVERTER_H_

#include "client_worldview.h"
#include "../common/position.h"

const float RATIO_MTS_PX = 210.0 / 9.0; // 23,3 periodico

class PositionConverter {
public:
    PositionConverter(){}

    // Recibe un numero y devuelve la cantidad de
    // px que representa
    int convert_from_m_to_px(float meters) {
        return meters * RATIO_MTS_PX;
    }

    // Recibe una posicion en m y la devuelve en px
    Position convert_position_to_px(Position pos) {
        int world_height = 360; //modificar despues
        int pos_x_px = convert_from_m_to_px(pos.get_position_x());
        int pos_y_px = world_height - convert_from_m_to_px(pos.get_position_y());
        Position position(pos_x_px, pos_y_px);
        return position;
    }
};

#endif