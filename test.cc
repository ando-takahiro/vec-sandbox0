#include <stdio.h>

template < typename F >
struct Wrapper {
    F f;
    constexpr Wrapper(F _f) : f(_f) {}
    template < typename Context >
    void operator () (Context context) const {
        f(context);
    };
};

template < typename F >
constexpr auto def(F f) {
    return Wrapper<F>(f);
}

struct FFF {
    template < typename X >
    void operator () (X x) const {
        printf("%d\n", x);
    }
};

constexpr auto t1(def(FFF()));

int main(void)
{
    t1(100);
    return 0;
}

//! [code]
