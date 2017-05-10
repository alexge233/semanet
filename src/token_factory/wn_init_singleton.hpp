#ifndef WN_INIT_SINGLETON
#define WN_INIT_SINGLETON
#include "includes.ihh"

class wn_init_singleton
{
public:
    static wn_init_singleton & singleton();

private:
    static std::unique_ptr<wn_init_singleton> instance__;
    
    wn_init_singleton();
};
#endif
