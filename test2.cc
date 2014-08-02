#include <stdio.h>
#include <memory>

auto hoge() {
    int cnt = 3;
    auto done = []() {
        printf("done!\n");
    };
    auto f = [cnt, done]() mutable {
        printf("cnt:%d %p\n", cnt, &cnt);
        if (--cnt <= 0) {
            done();
        }
    };
    printf("org-cnt:%d %p\n", cnt, &cnt);
    return f;
}

template <typename T1, typename T2, typename T3, typename DONE>
auto depends(T1&& t1, T2&& t2, T3&& t3, DONE&& done) {
    struct Scope {
        DONE done;
        Scope(const DONE& d) : done(d) {}
        ~Scope() {
            this->done();
        }
    };
    std::shared_ptr<Scope> fin(new Scope(done));

    auto f1 = [fin, &t1]() {
        t1();
    };

    auto f2 = [fin, &t2]() {
        t2();
    };

    auto f3 = [fin, &t3]() {
        t3();
    };

    f3();
    f1();
    return f2;
}

int main() {
    auto f = depends([]{printf("1\n");},
            []{printf("2\n");},
            []{printf("3\n");},
            []{printf("done\n");});

    f();
    return 0;
}
