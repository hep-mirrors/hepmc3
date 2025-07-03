#if defined(__linux__) || defined(__darwin__) || defined(__APPLE__) || defined(__FreeBSD__) || defined(__sun)
#include "HepMC3/GenEvent.h"
#include "HepMC3/ReaderFactory.h"
#ifdef _LIBCPP_VERSION
int main() {
    printf("The program cannot process inputs from standard input as std::ios_base::sync_with_stdio(bool) is not implemented in libc++, please use another C++ standard library.\n");
    return 0;
}
#else
using namespace HepMC3;
int main() {
    std::ios_base::sync_with_stdio(false);
    std::shared_ptr<Reader>  input_file = deduce_reader(std::cin);
    int N = 0;
    while (!input_file->failed()) {
        GenEvent evt(Units::GEV, Units::MM);
        input_file->read_event(evt);
        if (input_file->failed()) {
            printf("End of file reached. Exit.\n");
            break;
        }
        N++;
        evt.clear();
    }
    input_file->close();
    if (N != 10) {
        printf("Expected %i events, but obtained %i\n", 10, N);
        return 1;
    }
    return 0;
}
#endif
#else
int main() { return 0; }
#endif
