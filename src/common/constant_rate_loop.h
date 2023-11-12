#ifndef CONSTANT_RATE_LOOP_H_
#define CONSTANT_RATE_LOOP_H_

#include <chrono>
#include <cmath>
#include <thread>

using namespace std::chrono;

typedef duration<float, duration<float>> dur_ms;
typedef time_point<steady_clock, milliseconds> time_p_ms;
typedef duration<float> dur_f;

class ConstantRateLoop {
public:
  void loop(milliseconds rate) {

    auto t1 = time_point_cast<milliseconds>(steady_clock::now());
    int it_frames = 0;
    while (true) {
      func_to_execute();

      auto t2 = time_point_cast<milliseconds>(steady_clock::now());

      auto time_func = (t2 - t1);
      auto rest = rate.count() - time_func.count();

      drop_and_rest(rest, rate.count(), &t1, &it_frames);
    }
  }

  void drop_and_rest(float rest, float rate, time_p_ms *t1, int *it) {
    if (rest < 0) {
      auto behind = -rest;
      rest = rate - static_cast<int>(behind) % static_cast<int>(rate);

      auto lost = behind + rest;
      auto lost_ms = duration_cast<milliseconds>(dur_f(lost));

      *t1 += lost_ms;
      it += static_cast<int>(floor(lost / rate));
    }
    auto sleep_ms = duration_cast<milliseconds>(dur_f(rest));
    auto rate_ms = duration_cast<milliseconds>(dur_f(rate));

    std::this_thread::sleep_for(sleep_ms);

    *t1 += rate_ms;
    *it += 1;
  }

  virtual void func_to_execute() = 0;
};

#endif
