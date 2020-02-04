#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

template<typename... Fs>
auto compose() {
    return [](const auto &x) {
        return x;
    };
}

template<typename F, typename... Fs>
auto compose(const F &f, const Fs &...fs) {
    return [f, fs...](const auto &x) {
        return compose(fs...)(f(x));
    };
}

template<typename H>
auto lift(const H &h) {
    return [h](const auto &p [[maybe_unused]]) {
        return h();
    };
}

template<typename H, typename... Fs>
auto lift(const H &h, const Fs &... fs) {
    return [h, fs...](const auto &p) {
        return h(fs(p)...);
    };
}

#endif  // FUNCTIONAL_H