#pragma once

#include <typeindex>

namespace vec {
namespace task {

    template <typename Body>
    class _task {
        Body body;

    public:
        _task(const Body& b) : body(b) {}

        template < typename Context >
        friend Context& operator << (Context& c, _task&& f) {
            c.scheduler.push(f.body);
            return c;
        }
    };

    template <typename Body>
    _task<Body> exec(const Body&& b) {return _task<Body>(b);}

    //template <template <typename C, typename N> F(C& c, N&& n)>
    //_task<Body> exec(const F&& f) {return _task<F<C, N>>(f);}

    /*
    template <typename... Args>
    class _depends {
        Args... args;
    public:
        _depends(Args... a) : a(args) {}

        template < typename Context >
        friend Context& operator << (Context& c, _once&& f) {
            return c;
        }
    };

    template <typename... Args>
    _task<Body> depends(Args... args) {
        int count = sizeof...(Args);
        return _depends<Args...>(args);
    }*/
}
}

