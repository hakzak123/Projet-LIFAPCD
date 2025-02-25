#pragma once
#include <SDL3/SDL.h>
#include <iostream>

template<typename T>
class uniqueSDLPtr{ /* smart pointer unique pour pointeur de types alloués avec SDL_malloc() 
    devrait aussi fonctionner pour les types externes à SDL alloués avec malloc() de la librairie standard
    Le destructeur appelle SDL_free()
    */
private :
    T* ptr = nullptr;

public :
    uniqueSDLPtr(){
    }

    uniqueSDLPtr(T* _ptr): ptr(_ptr){
    }

    ~uniqueSDLPtr(){
        SDL_free(ptr);
    }

    T* pointer(){
        return ptr;
    }

    T& operator*(){
        return *ptr;
    }

};