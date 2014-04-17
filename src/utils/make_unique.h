#ifndef MAKE_UNIQUE_H
#define MAKE_UNIQUE_H
#include <memory>
 
template <typename Value, typename ... Arguments>
std::unique_ptr<Value> make_unique(Arguments && ... arguments_for_constructor)
{
    return std::unique_ptr<Value>(
        new Value(std::forward<Arguments>(arguments_for_constructor)...)
    );
}

#endif
