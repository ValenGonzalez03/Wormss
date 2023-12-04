#ifndef POSITION_CONVERTER_H_
#define POSITION_CONVERTER_H_

const float RATIO_MTS_PX = 210.0 / 9.0; // 23,3 periodico

class PositionConverter {
public:
  explicit PositionConverter();

  // Recibe un numero y devuelve la cantidad de
  // px que representa
  int convert_from_m_to_px(float meters);

  // Recibe un numero (pixeles) y devuelve la 
  // cnatidad de metros que representa
  float convert_from_px_to_m(int px);

};

#endif