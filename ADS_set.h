#ifndef ADS_SET_H
#define ADS_SET_H

#include <functional>
#include <algorithm>
#include <iostream>
#include <stdexcept>


template <typename Key, size_t N = 7>
class ADS_set {

public:
    class Iterator;
    using value_type = Key;
    using key_type = Key;
    using reference = key_type&;
    using const_reference = const key_type&;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using iterator = Iterator;
    using const_iterator = Iterator;
    using key_equal = std::equal_to<key_type>; // Hashing
    using hasher = std::hash<key_type>;        // Hashing


private:
    size_type n, nmax;
    float maxfactor;

    struct Element {
        Key wert;
        bool status; //besetzt == true
        Element *next;

        Element() {
            this->status = false;
            this->next = nullptr;
        }
    };

    
    Element ** pointerarray = nullptr;

    size_type hash_value(const key_type& k) const { return hasher{}(k)%nmax;}

    Element * firstElement() const{
        if (n==0) return nullptr;
        for (size_t i = 0; i < nmax; i++) {
            if (pointerarray[i] != nullptr) {
                return pointerarray[i];
            }
        }
    }

    Element * lastElement() const{
        if (n==0) return nullptr;
        Element * current = nullptr;
        for (size_type i = 0; i < nmax; i++){
            if(pointerarray[i]!=nullptr){
                current = pointerarray[i];
            }
        }
        return current;
    }

    Element * insert_unchecked(const key_type& k){
        
        auto schlussel = hash_value(k);
        Element *current = pointerarray[schlussel];
        if (count(k) == false) {
            
            // показывает на ячейку в которую будем добавлять
            if (current->status == true) { //wenn es ein element auf dieser stelle gibt, добавляем в голову списка
                Element *help = new Element;
                
                help->next = current;
                help->wert = k;
                pointerarray[schlussel] = help;
                help->status = true;
            }
            else if (current->status == false) {
                current->wert = k;
                current->status = true;
                current->next = nullptr;
            }
           
            // delete help;
        }
        n++;
        return current;
        
    }
    
    
    void rehash(size_type sz){
        if ((double)(sz) > nmax * maxfactor){
            
            size_type old_nmax{nmax};
            Element ** old_pointerarray = nullptr;
            old_pointerarray = pointerarray;
            Element * current = nullptr;
            
            while ((double)(sz) > nmax * maxfactor) nmax = nmax*2 +1;
                   
            pointerarray = new Element * [nmax];
            for (size_t i = 0; i < nmax; i++) {
                pointerarray[i] = new Element;
            }


            for (size_t i = 0; i < old_nmax; i++ ){
                current = old_pointerarray[i];
                while (current!=0){
                    if (current->status==true){
                        insert_unchecked(current->wert);
                        n--;
                }
                    current = current->next;
            }
        }

            Element *deleteEl = nullptr;
            for (size_t i = 0; i < old_nmax; i++) {
                current = old_pointerarray[i];

                while (current != nullptr) {
                    deleteEl = current;
                    current = current->next;
                    delete deleteEl; // удаляем связанные элементы по адресу, пока не наткнемся на 0
                }
            }

            delete[] old_pointerarray;
        }
                   
    }
/*
    void reserve(size_type sz){
        if (sz > nmax*maxfactor){
            size_type new_nmax {nmax};
            while (sz > new_nmax*maxfactor) new_nmax = new_nmax * 2 + 1;
           // rehash(new_nmax);
        }
    }
  */


    

public:
    //Konstruktor
    ADS_set(): n{0}, nmax{N}, maxfactor{0.7}, pointerarray{new Element *[this->nmax]} {
        for(size_t i=0; i < nmax; i++){
            pointerarray[i] = new Element;
        }
    }

    ADS_set(std::initializer_list<key_type> ilist): ADS_set(){
        insert(ilist);
    }
    template<typename InputIt> ADS_set(InputIt first, InputIt last): ADS_set(){
        insert(first, last);
    }
    
    ADS_set(const ADS_set& other){throw std::runtime_error("not implemented");}
    ~ADS_set(){  //удаление всего!
        Element *current = nullptr;
        Element *deleteEl = nullptr;
        for (size_t i = 0; i < nmax; i++) {
            current = pointerarray[i];
            
            while (current != nullptr) {
                deleteEl = current;
                current = current->next;
                delete deleteEl; // удаляем связанные элементы по адресу, пока не наткнемся на 0
            }
        }
        
        delete[] pointerarray;
}

    ADS_set& operator=(const ADS_set& other){throw std::runtime_error("not implemented");}
    ADS_set& operator=(std::initializer_list<key_type> ilist){throw std::runtime_error("not implemented");}

    size_type size() const{ return n;}
    
    bool empty() const{
        return !n;
    }
    
    size_type count(const key_type& key) const {
        size_t index = hash_value(key);
         Element *current= pointerarray[index];
        
        if (pointerarray[index] != 0) {
           // Element *current{pointerarray[index]};
            while (current!= nullptr)  {
                if (current->status == true && key_equal{}(current->wert, key)){
                    return 1;
                }
                else {
                current = current->next;
                }
            }
        }
        return 0;
    }

    iterator find(const key_type& key) const{throw std::runtime_error("not implemented");};

    void clear(){throw std::runtime_error("not implemented");};
    void swap(ADS_set& other){throw std::runtime_error("not implemented");};

    void insert(std::initializer_list<key_type> ilist){
        if ((double)(n + ilist.size()) > nmax * maxfactor)
            rehash(n+ilist.size());
        for (const auto& k: ilist)
            if (count(k)==0) insert_unchecked(k);
    }
    
    std::pair<iterator,bool> insert(const key_type& key){throw std::runtime_error("not implemented");}
    
    template<typename InputIt> void insert(InputIt first, InputIt last){
        for (auto it = first; it != last; ++it) {
            if (count(*it)==0) {
                rehash(n+1);
                insert_unchecked(*it);
            }
        }
    }

    size_type erase(const key_type& key){throw std::runtime_error("not implemented");}

    const_iterator begin() const { throw std::runtime_error("not implemented");}
    const_iterator end() const{ throw std::runtime_error("not implemented");;
    }

    void dump(std::ostream& o = std::cerr) const{
        o << "Hashing nmax = " << nmax << " n = " << n << "\n";
        Element *current = nullptr;
        for (size_type i = 0; i < nmax; ++i) {
            current = pointerarray[i]; // zeigt auf erste element;
            o << i << ": ";

            while (current != nullptr) { // wenn элемент на который покаывает не равен 0

                if (current->status == true) {
                    o << current->wert << " ";
                }

                current = current->next;
            }
            o << std::endl;
        }

        
    }

    friend bool operator==(const ADS_set& lhs, const ADS_set& rhs){
        throw std::runtime_error("not implemented");
    }
    friend bool operator!=(const ADS_set& lhs, const ADS_set& rhs){
        throw std::runtime_error("not implemented");
    }
};





template <typename Key, size_t N>
class ADS_set<Key,N>::Iterator {
    Element * pos;
    Element * first;
    Element * last;


public:
    using value_type = Key;
    using difference_type = std::ptrdiff_t;
    using reference = const value_type&;
    using pointer = const value_type*;
    using iterator_category = std::forward_iterator_tag;

    explicit Iterator(Element * pos = nullptr, Element * first, Element * last):  pos{pos}, last{last}, first{first}
    {

    }
    reference operator*() const{return pos->wert;}
    pointer operator->() const{ return &pos->wert;};
    Iterator& operator++(){
        if (this->pos != nullptr&& pos->next!= nullptr){ pos = pos->next; return *this; }
        else{
            for ( const auto &i = pos; i!=end().pos;  )
        }
    }
    Iterator operator++(int){
        auto rc = *this;
        ++*this;
        return rc;
    }
    friend bool operator==(const Iterator& lhs, const Iterator& rhs) { return lhs.pos == rhs.pos;};
    friend bool operator!=(const Iterator& lhs, const Iterator& rhs){ return lhs.pos != rhs.pos;};
};

template <typename Key, size_t N> void swap(ADS_set<Key,N>& lhs, ADS_set<Key,N>& rhs) { lhs.swap(rhs); }

#endif // ADS_SET_H
