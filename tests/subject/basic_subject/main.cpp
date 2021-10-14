#include "rest_server.h"


int main() {
    rest_server("127.0.0.1", 51425).run();
    return 0;
}