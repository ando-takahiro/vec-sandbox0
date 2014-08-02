#pragma once

#include <list>

namespace vec {
namespace task {

    template < typename Context, typename Allocator = typename Context::allocator_type >
    class chain_scheduler {
        class next_func {
            chain_scheduler& owner;
        public:
            next_func(chain_scheduler& o) : owner(o) {}
            void operator () () {
                this->owner.step();
            }
        };

        typedef void (*event_function)(Context& context, std::exception& e);
        typedef void (*task_function)(Context& context, next_func&& next);
        Context& context;
        std::list<
            task_function,
            typename Allocator::template rebind<task_function>::other> tasks;
        event_function on_error_handler;

    public:
        chain_scheduler(Context& c, const Allocator& a = Allocator()) :
            context(c), tasks(a) {}

        template < typename Task >
        void push(Task&& t) {tasks.push_back(t);}

        void step() {
            if (!tasks.empty()) {
                task_function f = tasks.front();
                tasks.pop_front();
                f(this->context, next_func(*this));
            }
        }

        template < typename OnError >
        void on_error(OnError&& on_error) {
            this->on_error_handler = on_error;
        }

        void emit_error(std::exception& e) {
            if (this->on_error_handler) {
                this->on_error_handler(context, e);
            } else {
                throw e;
            }
        }
    };

}
}

