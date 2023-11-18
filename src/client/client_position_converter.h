#ifndef CLIENT_POSITION_CONVERTER_H_
#define CLIENT_POSITION_CONVERTER_H_

const float RATIO_MTS_PX = 210.0 / 9.0; // 23,3 periodico

class PositionConverter {
public:
    PositionConverter(){}

    int convert_from_m_to_px(float meters) {
        return meters * RATIO_MTS_PX;
    }
};

#endif