#include <memory>
#include <vector>

#include "../DeltaPath/DeltaPath.hpp"
#include "../SemanticGraph/SemanticGraph.hpp"

#include <boost/lexical_cast.hpp>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_connection.h>

#include <cgpp/Token/Token.hpp>