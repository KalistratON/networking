#ifndef HELPER_HPP
#define HELPER_HPP

#include <memory>

namespace networking {
namespace internal {

template<class O, class I>
O* ToImpl (std::shared_ptr<I>& thePtr)
{
    return static_cast <O*> (thePtr.get());
}

}
}

#endif