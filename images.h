#ifndef IMAGES_H
#define IMAGES_H

#include <functional>
#include <cmath>
#include "coordinate.h"
#include "color.h"
#include "functional.h"

using Fraction = double;

template<typename T>
using Base_image = std::function<T(const Point)>;

using Region = Base_image<bool>;

using Image = Base_image<Color>;

using Blend = Base_image<Fraction>;

template<typename T>
Base_image<T> constant(const T &t) {
    return [t](const Point p [[maybe_unused]]) {
        return t;
    };
}

template<typename T>
Base_image<T> rotate(Base_image<T> image, double phi) {
    return [image, phi](const Point p) {
        auto converted = to_polar(p);
        return compose(from_polar, image)(
                Point(converted.first, converted.second - phi, true)
        );
    };
}

template<typename T>
Base_image<T> translate(Base_image<T> image, Vector v) {
    return [image, v](const Point p) {
        return image(
                Point(p.first - v.first, p.second - v.second, false)
        );
    };
}

template<typename T>
Base_image<T> scale(Base_image<T> image, double s) {
    return [image, s](const Point p) {
        return image(
                Point(p.first / s, p.second / s, false)
        );
    };
}

template<typename T>
Base_image<T> vertical_stripe(double d, T this_way, T that_way) {
    return [d, this_way, that_way](const Point p) {
        return fabs(p.first) <= d / 2 ? this_way : that_way;
    };
}

template<typename T>
Base_image<T> circle(Point q, double r, T inner, T outer) {
    return translate(static_cast<Base_image<T>>(
                             compose(to_polar,
                                     vertical_stripe(2 * r, inner, outer))),
                     std::make_pair(q.first, q.second));
}

template<typename T>
Base_image<T> checker(double d, T this_way, T that_way) {
    return [d, this_way, that_way](const Point p) {
        int parity = std::abs(
                static_cast<int>(std::floor(p.first / d)) +
                static_cast<int>(std::floor(p.second / d)));
        return parity % 2 == 0 ? this_way : that_way;
    };
}

template<typename T>
Base_image<T> polar_checker(double d, int n, T this_way, T that_way) {
    auto sc = [n, d](const Point p) {
        return Point(p.first, d * n * p.second / (2 * M_PI), false);
    };
    return compose(to_polar, sc, checker(d, this_way, that_way));
}

template<typename T>
Base_image<T> rings(Point q, double d, T this_way, T that_way) {
    return translate(static_cast<Base_image<T>>(
                             compose(to_polar, checker(d, this_way, that_way))
                     ), std::make_pair(q.first, q.second));
}

Image cond(const Region &region, const Image &this_way, const Image &that_way);

Image lerp(const Blend &blend, const Image &this_way, const Image &that_way);

Image darken(const Image &image, const Blend &blend);

Image lighten(const Image &image, const Blend &blend);

#endif  // IMAGES_H