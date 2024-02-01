#include "HepMC3/GenEvent.h"
#include "HepMC3/GenRunInfo.h"

#include <memory>
#include <string>

namespace HepMC3 {
namespace Serialize {
std::string GenRunInfo(const GenRunInfo &run_info);
std::string GenEvent(const GenEvent &evt);
} // namespace Serialize
namespace Deserialize {
bool GenRunInfo(std::string const &,
                std::shared_ptr<HepMC3::GenRunInfo> run_info);
bool GenEvent(std::string const &, HepMC3::GenEvent &evt);
} // namespace DeSerialize
} // namespace HepMC3