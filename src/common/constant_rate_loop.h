#ifndef CONSTANT_RATE_LOOP_H_
#define CONSTANT_RATE_LOOP_H_

#include <chrono>
#include <cmath>
#include <thread>

typedef std::chrono::duration<std::chrono::milliseconds> dur_ms;
typedef std::chrono::time_point<std::chrono::steady_clock,
                                std::chrono::milliseconds>
    time_p_ms;
typedef std::chrono::milliseconds ms;
typedef std::chrono::duration<float> dur_f;

class ConstantRateLoop {
public:
  void loop(dur_ms rate) {

    auto t1 =
        std::chrono::time_point_cast<ms>(std::chrono::steady_clock::now());
    int it_frames = 0;
    while (true) {
      func_to_excecute();

      auto t2 =
          std::chrono::time_point_cast<ms>(std::chrono::steady_clock::now());

      dur_ms time_func(t2 - t1);
      auto rest = rate.count().count() - time_func.count().count();

      drop_and_rest(rest, rate.count().count(), &t1, &it_frames);
    }
  }

  void drop_and_rest(float rest, float rate, time_p_ms *t1, int *it) {
    if (rest < 0) {
      auto behind = -rest;
      rest = rate - static_cast<int>(behind) % static_cast<int>(rate);

      auto lost = behind + rest;
      auto lost_ms = std::chrono::duration_cast<ms>(dur_f(lost));

      *t1 += lost_ms;
      it += static_cast<int>(floor(lost / rate));
    }
    auto sleep_ms = std::chrono::duration_cast<ms>(dur_f(rest));
    auto rate_ms = std::chrono::duration_cast<ms>(dur_f(rate));

    std::this_thread::sleep_for(sleep_ms);

    *t1 += rate_ms;
    *it += 1;
  }

  virtual void func_to_excecute() = 0;
};

#endif
