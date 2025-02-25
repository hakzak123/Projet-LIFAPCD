#pragma once
#include <iostream>

namespace hz{

template<class T>
class unique_ptr{
    private:
        T* ptr=nullptr;
        bool allocated = false;
    public:
        unique_ptr(){}

        unique_ptr(T * const &_ptr): ptr(_ptr){
        }

        ~unique_ptr(){
            delete ptr;
        }

        T* operator->(){
            return ptr;
        }

        T& operator*(){
            return *ptr;
        }


};

}