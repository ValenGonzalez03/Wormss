#ifndef CLIENT_POSITION_CONVERTER_H_
#define CLIENT_POSITION_CONVERTER_H_

#include "client_worldview.h"
#include "../common/position.h"

const float RATIO_MTS_PX = 210.0 / 9.0; // 23,3 periodico

class PositionConverter {
public:
  explicit PositionConverter();

  // Recibe un numero y devuelve la cantidad de
  // px que representa
  int convert_from_m_to_px(float meters);

  // Recibe una posicion en m y la devuelve en px
  Position convert_position_to_px(Position pos);
};

#endif