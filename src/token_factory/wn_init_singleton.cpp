#include "wn_init_singleton.hpp"

std::unique_ptr<wn_init_singleton> wn_init_singleton::instance__;

wn_init_singleton::wn_init_singleton()
{}

wn_init_singleton & wn_init_singleton::singleton()
{
    if (!instance__) {
        instance__.reset(new wn_init_singleton());
        int res = wninit();
        if (res) throw std::runtime_error("failed to init wordnet");
    }
    return * instance__.get();
}
