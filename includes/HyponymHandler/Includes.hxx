#include <vector>
#include <memory>
#include <cassert>

extern "C" {
  #include <wn.h>
};

#include <cgpp/Node/Node.hpp>

#include "../SemanticGraph/SemanticGraph.hpp"
#include "../SynsetHandler/SynsetHandler.hpp"
#include "../Wrapper/Wrapper.hpp"
