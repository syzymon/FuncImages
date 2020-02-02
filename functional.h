#pragma once

auto compose() {
    return [](const auto &x) {
        return x;
    };
}

auto compose(const auto &f) {
    return f;
}

auto compose(const auto &f, const auto &g) {
    return [&f, &g](const auto &x) {
        return g(f(x));
    };
}

template<typename F, typename G, typename... Rest>
auto compose(const F &f, const G &g, const Rest &... rest) {
    return compose(compose(f, g), rest...);
}

auto lift(const auto &h) {
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