#include "HepMC3/GenEvent.h"
#include "HepMC3/GenRunInfo.h"

// protobuf header files
#include "HepMC3.pb.h"

#include <memory>
#include <string>

namespace HepMC3 {

namespace Serialize {

std::string GenRunInfo(GenRunInfo const &run_info);
std::string GenEvent(GenEvent const &evt);

} // namespace Serialize

namespace Deserialize {

bool GenRunInfo(std::string const &,
                std::shared_ptr<HepMC3::GenRunInfo> run_info);

// Partial deserialization where the caller has already parsed the pb message
void FillGenRunInfo(HepMC3_pb::GenRunInfoData const &gri_pb,
                    std::shared_ptr<HepMC3::GenRunInfo> run_info);

bool GenEvent(std::string const &, HepMC3::GenEvent &evt);

// Partial deserialization where the caller has already parsed the pb message
void FillGenEvent(HepMC3_pb::GenEventData const &ged_pb, HepMC3::GenEvent &evt);

} // namespace Deserialize

} // namespace HepMC3