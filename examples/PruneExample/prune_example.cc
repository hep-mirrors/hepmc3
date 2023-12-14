#include <iostream>
#include <fstream>
#include <chrono>
#include "prune.h"

/** A simple timer */
struct timer
{
    /** Type of clock */
    using clock = std::chrono::high_resolution_clock;
    /** Type of time point */
    using point = typename clock::time_point;
    /** Unit of duration */
    using nano  = std::chrono::nanoseconds;

    /** Constructor */
    timer(size_t orig, std::ostream& out, bool enable)
        : _out(out),
          _orig(orig),
          _after(orig),
          _enable(enable)
    {}
    /** Desructor - reports result */
    ~timer()
    {
        if (not _enable) return;
        auto end = clock::now();
        auto ns  = std::chrono::duration_cast<nano>(end - _start);

        _out << _orig << "\t" << _after << "\t" << ns.count() << std::endl;
    }
    /** Start of timing */
    point _start = clock::now();
    /** Where to write result */
    std::ostream& _out;
    /** Initial size of event */
    size_t _orig;
    /** Size of event after pruning */
    size_t _after;
    /** If reporting is enabled */
    bool _enable;
};



/** Show usage information */
void usage(const std::string& progname, std::ostream& out=std::cout)
{
    out << "Usage: " << progname << " [OPTIONS] [< INPUT] [> OUTPUT]\n\n"
        << "Options:\n"
        << "  -h,--help            This help\n"
        << "  -i        FILENAME   Input file name\n"
        << "  -o        FILENAME   Output file name\n"
        << "  -n        EVENTS     Maximum number of events\n"
        << "  -t                   Enable timing\n"
        << "  -v                   Increase verbosity\n"
        << std::endl;
}

int main(int argc, char** argv)
{
    std::string inFileName  = "-";
    std::string outFileName = "-";
    int         verb        = 0;
    int         max         = -1;
    bool        timing      = false;
    for (int i = 1; i < argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-h" || arg == "--help") {
            usage(argv[0]);
            return 0;
        }

        if (arg[0] == '-') {
            switch (arg[1]) {
            case 'i':
                inFileName  = argv[++i];
                break;
            case 'o':
                outFileName = argv[++i];
                break;
            case 'v':
                verb++;
                break;
            case 't':
                timing != timing;
                break;
            case 'n':
                max         = std::stoi(argv[++i]);
                break;
            default:
                std::cerr << argv[0] << ": Unknown option " << arg << std::endl;
                return 1;
            }
        } else {
            std::cerr << argv[0] << ": Unknown option " << arg << std::endl;
            return 1;
        }
    }

    std::ifstream* inPtr    = (inFileName == "-" //
                               ? nullptr       //
                               : new std::ifstream(inFileName.c_str()));
    std::ofstream* outPtr   = (outFileName == "-" //
                               ? nullptr        //
                               : new std::ofstream(outFileName.c_str()));

    std::istream& inStream  = (inPtr ? *inPtr : std::cin);
    std::ostream& outStream = (outPtr ? *outPtr : std::cout);

    HepMC3::ReaderAscii reader(inStream);
    HepMC3::WriterAscii writer(outStream);
    HepMC3::GenEvent    event;

    // The selection of particles
    auto select = [](HepMC3::ConstGenParticlePtr& particle) {
        switch (particle->status()) {
        case 1: // Final st
        case 2: // Decayed
        case 4: // Beam
            return true;
        }
        // if (particle->pid() == 9902210) return true;
        return false;
    };

    // Test if we have more events to do.
    auto more = [max](int iev) { return max <= 0 || iev < max; };

    int iev = 0;
    while (more(iev) && reader.read_event(event) && !reader.failed()) {
        iev++;

        {
            timer t(event.particles().size(),std::clog,timing);
            HepMC3::prune_particles1(event, select);
            t._after = event.particles().size();
        }

        writer.write_event(event);
    }

    reader.close();
    writer.close();

    if (inPtr)  inPtr ->close();
    if (outPtr) outPtr->close();

    return 0;
}
//
// EOF
//
