#ifndef _TTT_BASE_H_
#define _TTT_BASE_H_
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>

#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#define TTT_CALL __cdecl
#if defined(TTTRTC_EXPORT)
#define TTT_API __declspec(dllexport)
#define TTT_CAPI extern "C" __declspec(dllexport)
#else
#define TTT_API __declspec(dllimport)
#define TTT_CAPI extern "C" __declspec(dllimport)
#endif
#elif defined(__APPLE__)
#define TTT_API __attribute__((visibility("default")))
#define TTT_CAPI __attribute__((visibility("default"))) extern "C"
#define TTT_CALL
#elif defined(__ANDROID__) || defined(__linux__)
#define TTT_API __attribute__((visibility("default")))
#define TTT_CAPI extern "C" __attribute__((visibility("default")))
#define TTT_CALL
#else
#define TTT_API
#define TTT_CAPI extern "C"
#define TTT_CALL
#endif
namespace TTTRtc {
    template<class T>
    class AutoPtr2 {
        typedef T value_type;
        typedef T* pointer_type;
    public:
        AutoPtr2(pointer_type p = 0)
            :ptr_(p)
        {}
        ~AutoPtr2() {
            if (ptr_)
                ptr_->release();
        }
        operator bool() const { return ptr_ != (pointer_type)0; }
        value_type& operator*() const {
            return *get();
        }

        pointer_type operator->() const {
            return get();
        }

        pointer_type get() const {
            return ptr_;
        }

        pointer_type release() {
            pointer_type tmp = ptr_;
            ptr_ = 0;
            return tmp;
        }

        void reset(pointer_type ptr = 0) {
            if (ptr != ptr_ && ptr_)
                ptr_->release();
            ptr_ = ptr;
        }
        template<class C1, class C2>
        bool queryInterface(C1* c, C2 iid) {
            pointer_type p = NULL;
            if (c && !c->queryInterface(iid, (void**)&p))
            {
                reset(p);
            }
            return p != NULL;
        }
    private:
        AutoPtr2(const AutoPtr2&);
        AutoPtr2& operator=(const AutoPtr2&);
    private:
        pointer_type ptr_;
    };
}

#endif
