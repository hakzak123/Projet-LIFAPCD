#pragma once
#include <iostream>


namespace hz{

template<class Vector>
class VectorIterator{
    public :
    using valuetype = typename Vector::Ttype;
    VectorIterator(valuetype* _ptr): ptr(_ptr){}
    VectorIterator& operator++(){
        ptr++;
        return *this;
    }

    valuetype& operator*(){
        return *ptr;
    }

    bool operator==(const VectorIterator& other){
        return ptr = other.ptr;
    }

    bool operator!=(const VectorIterator& other){
        return ptr != other.ptr;
    }

    private :
    valuetype* ptr;

};

template<class T>
    class vector{
        private:
            T* arr = nullptr;
            size_t _size=0;
            size_t _capacity=0;
            size_t _reallocs=0;

        public :
            using Ttype = T;
            using Iterator = VectorIterator<vector<T>>;

            vector(){
            }

            ~vector(){
                if(arr!=nullptr){
                    size_t oldsize = _size;
                    for(int i = 0; i<oldsize; ++i){
                        pop_back();
                        //std::cout << "clear " << i << '\n' ;
                    }
                    ::operator delete(arr,_capacity*sizeof(T));
                    _size = 0;
                    _capacity = 0;
                    arr = nullptr;
                    //std::cout << "clear successful\n";
                }
            }

            vector(const vector<T>& other): _size(other._size), _capacity(other._capacity){
                arr = (T*)::operator new(other._size*sizeof(T));
                for(int i=0; i<other._size;++i){
                    new (&arr[i]) T(other.arr[i]);
                }
            }

            vector(const std::initializer_list<T>& l){
                int i=0;
                arr = (T*)::operator new(l.size()*sizeof(T));
                for(const auto&entry : l){
                    new (&arr[i]) T(entry);
                    i++;
                }
                _size = l.size();
                _capacity = _size;
            }

            void operator=(const vector<T>& other){
                if(this!=&other){
                    T* newarr = (T*)::operator new(other._size*sizeof(T));
                    for(int i=0; i<other._size;++i){
                        new (&newarr[i]) T(std::move(other.arr[i]));
                    }
                    ::operator delete(arr,_capacity*sizeof(T));
                    arr = newarr;
                    _size = other._size; 
                    _capacity = other._capacity;
                    _reallocs++;
                }
            }

            bool operator==(const vector& other){
                if(this->_size != other._size){
                    return false;
                }
                for(int i=0;i<this->_size;++i){
                    if(this->arr[i]!=other.arr[i]){
                        return false;
                    }
                }
                return true;
            }
            
            T& operator[](const size_t& index){
                return arr[index];
            }

            void reserve(const size_t& s){
                T* newtab = (T*)::operator new((_capacity+s)*sizeof(T));
                for(int i=0;i<_size;++i){
                    new (&newtab[i]) T(std::move(arr[i]));
                }
                ::operator delete(arr,_capacity*sizeof(T));
                arr=newtab;
                _capacity+=s;
                _reallocs++;
            }

            void resize(const size_t& s){
                size_t difference = _size-s;
                if(s<_size){
                    for(int i=0;i<difference;++i){
                        pop_back();
                    }
                }
                else if(s>_size){
                    if(_capacity<s){
                        reserve(s-_capacity);
                    }
                    for(int i=_size;i<s;++i){
                        new (&arr[i]) T();
                    }
                    _size = s;
                    _capacity = s;
                }
            }

            void push_back(const T& element){
                if(_size==_capacity){
                    reserve(_capacity*2+1);
                }
                new (&arr[_size]) T(std::move(element));
                _size++;
            }

            void pop_back(){
                if(_size>0){
                    _size--;
                    arr[_size].~T();
                }
            }

            void clear(){
                if(arr!=nullptr){
                    size_t oldsize = _size;
                    for(int i = 0; i<oldsize; ++i){
                        pop_back();
                    }
                    _size = 0;
                }
            }
            
            void shrink_to_fit(){
                T* newtab = (T*)::operator new((_size)*sizeof(T));
                for(int i=0;i<_size;++i){
                    new (&newtab[i]) T(std::move(arr[i]));
                }
                ::operator delete(arr,_capacity*sizeof(T));
                arr=newtab;
                _capacity = _size;
                _reallocs++;
            }


            size_t size(){
                return _size;
            }

            size_t capacity(){
                return capacity;
            }

            void printstats(){
                std::cout << '\n' << "size: " << _size <<'\n' << "capacity: " << _capacity <<'\n'<< "reallocations: " << _reallocs <<'\n' << "pointer: " << arr << '\n';
            }

            Iterator begin(){
                return Iterator(arr);
            }

            Iterator end(){
                return Iterator(arr+_size);
            }

    };

}