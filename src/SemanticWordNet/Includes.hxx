#include <vector>
#include <memory>
#include <thread>
#include <tuple>
#include <limits>
#include <fstream>

extern "C" {
  #include <wn.h>
};

#include <cgpp/Node/Node.hpp>

#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/unordered_set.hpp>

#include "../DeltaPath/DeltaPath.hpp"
#include "../SemanticGraph/SemanticGraph.hpp"
#include "../Senses/Senses.hpp"