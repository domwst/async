#include <async/util/function2/function2.hpp>

namespace async {

template <class Signature>
using UniqueFunction = fu2::function_base<
    /* IsOwning= */ true,
    /* IsCopyable= */ false,
    /* Capacity= */ fu2::capacity_fixed<64U>,
    /* IsThrowing= */ true,
    /* HasStrongExceptGuarantee= */ false,
    /* Signatures= */ Signature>;

}  // namespace async
