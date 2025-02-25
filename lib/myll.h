#pragma once
#include <iostream>

namespace hz{
using std::cout;

template<class T>
struct node{
    node<T>* next = nullptr;
    T* object;

};

template<class T>
class list{
    private :
        node<T>* _head = nullptr;
        size_t _size=0;
    public :

        void add(const T &newobj){
            if(_head == nullptr){
                _head = new node<T>;
                _head->object = new T(std::move(newobj));
            }
            else{
                node<T>* current = _head;
                while(current->next!=nullptr){
                    current = current->next;
                }
                current->next = new node<T>;
                current->next->object = new T(std::move(newobj));
            }
            _size++;
        }

        ~list(){
            node<T>* current = _head;
            while(current!=nullptr){
                node<T>* next = current->next;
                delete current->object;
                current->next = nullptr;
                current = next;
            }
            _head = nullptr;
        }

        void print_fundamental(){
            node<T>* current = _head;
            while(current!=nullptr){
                cout << *(current->object) << "  ";
                current = current->next;
            }
            cout << '\n';
        }
};
}