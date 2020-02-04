#include "images.h"

Image cond(const Region &region, const Image &this_way, const Image &that_way) {
    auto condition = [](bool b, Color c, Color d) {
        return b ? c : d;
    };
    return lift(condition, region, this_way, that_way);
}

Image lerp(const Blend &blend, const Image &this_way, const Image &that_way) {
    auto lerp_point = [](Fraction f, Color c, Color d) {
        return c.weighted_mean(d, f);
    };
    return lift(lerp_point, blend, this_way, that_way);
}

namespace { // helper
    Image shadow(const Color with, const Image &image, const Blend &blend) {
        auto shadower = [with](Color c, Fraction f) {
            return c.weighted_mean(with, f);
        };
        return lift(shadower, image, blend);
    }
}

Image darken(const Image &image, const Blend &blend) {
    return shadow(Colors::black, image, blend);
}

Image lighten(const Image &image, const Blend &blend) {
    return shadow(Colors::white, image, blend);
}
