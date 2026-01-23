#ifndef CLIENT_CAMERA_H_
#define CLIENT_CAMERA_H_

#include <SDL2pp/SDL2pp.hh>
#include <SDL2/SDL.h>

class Camera {
private:
	SDL2pp::Rect camera_box;

	bool moving_up = false;
	bool moving_down = false;
	bool moving_left = false;
	bool moving_right = false;

	// Verifica si el mouse se encuentra en el margen superior de la pantalla.
	bool is_up_corner(int mouse_x, int mouse_y);

	// Verifica si el mouse se encuentra en el margen inferior de la pantalla.
	bool is_down_corner(int mouse_x, int mouse_y);

	// Verifica si el mouse se encuentra en el margen izquierdo de la pantalla.
	bool is_left_corner(int mouse_x, int mouse_y);
	
	// Verifica si el mouse se encuentra en el margen derecho de la pantalla.
	bool is_right_corner(int mouse_x, int mouse_y);

public:

	explicit Camera(int width, int height);

	// Verifica en cada frame si la camara se esta moviendo en alguna/s direccion/es,
	// En caso afirmativo mueve la camara a donde corresponda.
	void update();
	
	// Actualiza las variables de la camara en función de los valores recibidos como posicion del mouse.
	void check_mouse_position(int mouse_x, int mouse_y);
	
	// Setea todas las variables de la camara para que no se mueva.
	void stop_scrolling();
	
	int get_x();
	int get_y();
};


#endif