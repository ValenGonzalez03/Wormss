#ifndef STATES_CODES_H_
#define STATES_CODES_H

#include <cstdint>

// Estados del gusano usados por el cliente o el servidor
namespace WORM_STATES {
extern uint8_t STOPPED;
extern uint8_t MOVING;
extern uint8_t AIMING;
extern uint8_t SHOOTING;
extern uint8_t JUMPING;
extern uint8_t INACTIVE;
} // namespace WORM_STATES

#endif
