#ifndef CLIENT_POSITION_H_
#define CLIENT_POSITION_H_

// Crea una posicion en pixeles para un objeto
class Position{
private:
    int pos_x;
    int pos_y;

public:
    Position(int position_x, int position_y) : pos_x(position_x), pos_y(position_y) {}
};

#endif