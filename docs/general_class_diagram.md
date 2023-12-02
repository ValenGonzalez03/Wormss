Abrir con alt+d:

Asociación (--> o --)
Agregación (o--)
Composición (*--)
Herencia (<|--)
Implementación (<|..)
Dependencia (..>)

```plantuml
@startuml
class Beam {
  - position : Position
  - texture : Texture* 
  - &renderer : Renderer
  - angle : int
  - pivot : SDL_Point

  + Beam(Position &pos, std::vector<Texture *> &beam_texture,
       Renderer &rend, int angle)
  + render(int frame): void
}

class Client {
  - receiver_queue : Queue<GameState>
  - sender_queue : Queue<std::shared_ptr<Command>>
  - handle_jump_forward(bool &is_jumping) : void
  - handle_jump_backward(bool &is_jumping) : void
  - get_opposite_direction() : int
  - print_state(uint8_t state) : string 
  - func_to_execute() : bool
}

class ConstantRateLoop {
  + loop(dur_f rate) : void
  + drop_and_rest(float rest, float rate, time_p_ms* t1, int* it) : void
  {abstract} + func_to_execute() : bool
}

class Lobby {
  - &prot : ClientProtocol
  + Lobby(ClientProtocol &prot)
  + run_lobby() : void
}

'class PositionConverter {
'  + PositionConverter()
'  + convert_from_m_to_px(float m) : int
'  + convert_position_to_px(Position pos) : 'Position
'}

class ClientProtocol {
  - skt : Socket
  + ClientProtocol::ClientProtocol(Socket &&socket)
  + receive_id() : uint8_t
  + send_command(Command &cmd) : void
  + process_game_state() : GameState
  + recv_string(bool* was_closed) : string
  + send_string(string str, bool* was_closed) : void
  + recv_float(bool* was_closed) : float
  + recv_world_names(bool* was_closed) : vector<string>
  + recv_world(WorldView &world, bool* was_closed)
  + recv_and_add_beam(WorldView &world, bool* was_closed) : void
  + send_world_name_selected(string world_name, bool* was_closed): void
  + close_socket() : void
}

class ClientReceiverThread {
  - &prot : ClientProtocol
  - &receiver_queue : Queue<GameState>
  + ClientReceiverThread(ClientProtocol &prot, Queue<GameState> &recv_queue)
}

class ResourcePool {
  - &renderer : Renderer 
  - texture_arrays : map<string, vector<Texture *>> 
  - fonts : map<string, shared_ptr<Font>> 
  - sounds : map<string, Mix_Chunk*> 
  - background : shared_ptr<Texture> 
  - gMusic : Mix_Music*
  - add_texture(string texture_name, string path, int width, int height, int amount_frames) : void
  - get_texture(string name) : vector<Texture*>
  - add_short_beam() : void
  - add_long_beam() : void
  - add_worm_walking() : void
  - add_worm_jumping() : void
  + ResourcePool(Renderer &rend)
  + initialize() : void
  + get_short_beam_texture() : vector<Texture*>
  + get_long_beam_texture() : vector<Texture*>
  + get_worm_walking() : vector<Texture*>
  + add_background(string path) : void
  + get_background() : shared_ptr<Texture*>
}

struct client_SDL {
  + window : Window
  + renderer : Renderer
  + resource_pool : ResourcePool
  + world_view : WorldView
}

class ClientSenderThread {
  - &prot : ClientProtocol
  - &sender_queue : Queue<shared_ptr<Command>>
  + ClientSenderThread(ClientProtocol &protocol,
      Queue<shared_ptr<Command>> &send_queue)
  {abstract} + run() : void
}

struct client_state {
  + is_running : bool
  + is_jumping : bool
  + run_phase : int
  + jump_phase : int
}

class WorldView {
  - &resource_pool : ResourcePool
  - &renderer : Renderer
  - beams : vector<beams>
  - worms : vector<worms>
  - render_background() : void
  + WorldView(ResourcePool &rp, Renderer &rend)
  + add_short_beam(int pos_x, int pos_y, int angle) : void
  + add_long_beam(int pos_x, int pos_y, int angle) : void
  + update(GameState &gs) : void
  + render(int frame, client_state &worm_state) : void
  + set_background(string path) : void
}

class WormView {
  - &rend : Renderer
  - &walking_texture : vector<Texture*>
  + WormView(Renderer &rend, vector<Texture*> walking)
  + render(int frame, int pos_x, int pos_y, Worm &worm, client_state &cs) : void
  + render_worm_idle(int frame, int pos_x, int pos_y, Worm &worm, client_state &worm_state) : void
  + render_worm_running(int frame, int pos_x, int pos_y, Worm &worm, client_state &worm_state) : void
  + render_worm_jumping(int frame, int pos_x, int pos_y, Worm &worm, client_state &worm_state) : void
  - choose_flip_direction(Worm &worm) : SDL_RendererFlip
}

class Socket {}

class Queue {}

class Worm {}

ConstantRateLoop <|-- Client

ClientReceiverThread --|> Thread
ClientSenderThread --|> Thread

ClientProtocol --> Socket
client_SDL --> WorldView
client_SDL --> ResourcePool

WorldView --> ResourcePool
WorldView --> Beam
WorldView --> WormView

WormView --> Worm

Lobby --> ClientProtocol

Client --> ClientReceiverThread
Client --> ClientSenderThread
Client --> Queue
Client --> client_SDL
Client --> ClientProtocol
Client --> client_state

@enduml
```