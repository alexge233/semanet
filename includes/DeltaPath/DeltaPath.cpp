#include "DeltaPath.hpp"

std::mutex DeltaPath::_mtx;


DeltaPath::DeltaPath (
			const std::shared_ptr<cgpp::Node> from,
			const std::shared_ptr<cgpp::Node> to,
			float delta
		      )
: _from ( from ), _to ( to ), _delta ( delta ) // NOTE: TEST from & to
{ }


bool DeltaPath::operator< ( const DeltaPath & rhs )
{
  return (*this->_from->asToken() ) < (*rhs._from->asToken() );
}


bool DeltaPath::operator== ( const DeltaPath & rhs )
{
  return (*this->_from->asToken() ) == (*rhs._from->asToken() );
}


std::shared_ptr<cgpp::Node> DeltaPath::From ( ) const
{
  return _from;
}


std::shared_ptr<cgpp::Node> DeltaPath::To ( ) const
{
  return _to;
}


float DeltaPath::Delta ( ) const
{
  return _delta;
}


void DeltaPath::Echo ( ) const
{
  std::lock_guard<std::mutex> lock ( _mtx );
  std::cout << "[\"" << _from->asToken()->value() << "\",\""
	    << _to->asToken()->value() << "\"] = " << _delta << std::endl;
}