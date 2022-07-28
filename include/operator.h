#ifndef CCANNY_INCLUDE_OPERATOR_H
#define CCANNY_INCLUDE_OPERATOR_H

#include "image.hpp"

namespace ccanny {

Image mean_blur(const Image& src, const int k);

} // namespace ccanny

#endif // CCANNY_INCLUDE_OPERATOR_H
