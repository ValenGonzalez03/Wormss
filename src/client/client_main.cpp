#include <iostream>
#include "client_cliente.h"

int main(int argc, char* argv[]) {
    try {
        int ret = -1;

        if (argc < 3) {
            std::cerr << "Bad program call. Expected " << argv[0]
                      << " with two arguments (hostname and servname).\n";
            return ret;
        }
        const char* hostname = argv[1];
        const char* servname = argv[2];

        Cliente cliente(hostname, servname);

        ret = 0;
        return ret;
    } catch (const std::exception& err) {
        std::cerr << "Something went wrong and an exception was caught: " << err.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Something went wrong and an unknown exception was caught.\n";
        return 1;
    }
}
