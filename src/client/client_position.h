#ifndef CLIENT_POSITION_H_
#define CLIENT_POSITION_H_

// Crea una posicion en pixeles para un objeto
struct Position {
public:
  int _pos_x;
  int _pos_y;

  Position(int position_x, int position_y)
      : _pos_x(position_x), _pos_y(position_y) {}
};

#endif