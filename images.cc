#include "images.h"

Image cond(const Region &region, const Image &this_way, const Image &that_way) {
    auto helper = [](bool b, Color c, Color d) {
        return b ? c : d;
    };
    return lift(helper, region, this_way, that_way);
}

Image lerp(const Blend &blend, const Image &this_way, const Image &that_way) {
    auto helper = [](Fraction f, Color c, Color d) {
        return c.weighted_mean(d, f);
    };
    return lift(helper, blend, this_way, that_way);
}

namespace {
    auto shadow(const Color with, const Image &image, const Blend &blend) {
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
