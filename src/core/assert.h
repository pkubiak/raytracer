#ifndef CG1RAYTRACER_ASSERT_HEADER
#define CG1RAYTRACER_ASSERT_HEADER

#include <iostream>
#include <cstdlib>


class Assert {
public:
    Assert(bool cond) : assertion(cond) {}
    ~Assert() {
        if (!assertion) {
            std::cerr << std::endl;
            abort();
        }
    }
    template <class T>
    inline Assert& operator<<(const T& message) {
        if (!assertion)
            std::cerr << message;
        return *this;
    }
private:
    bool assertion;
};

class NullAssert {
public:
    NullAssert(bool cond) {}
    template <class T>
    inline NullAssert& operator<<(const T& message) { return *this; }
};

#ifndef NDEBUG
#define assert(cond) Assert(cond) << "Assertion failure at " << __FILE__ << ":" << __LINE__ << " -- "
#define release_assert(cond) Assert(cond) << "Fatal error at at " << __FILE__ << ":" << __LINE__ << " -- "
#else
#define assert(cond) NullAssert(cond)
#define release_assert(cond) Assert(cond) << "Fatal error -- "
#endif



#define UNREACHABLE do { release_assert(false) << "UNREACHABLE"; exit(3); } while(0)
#define NOT_IMPLEMENTED do { release_assert(false) << "NOT IMPLEMENTED"; exit(3); } while(0)


#endif
