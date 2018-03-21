#include <functional>
#include <list>
#include <stdexcept>
#include <vector>
#include <utility>

template<class KeyType, class ValueType, class Hash = std::hash<KeyType> >
class HashMap {
public:
    static const size_t TABLE_RATE = 5; // 1/5 of cells must be not empty
    static const size_t TABLE_REHASH_RATE = 10; // 1/10 of cells will be not empty after rehash
    class iterator;
    typedef typename std::pair<KeyType, ValueType> PairType;
    typedef typename std::list<PairType>::const_iterator const_iterator;
private:
    Hash hash;
    size_t setSize = 0;
    size_t mod = 1;
    std::list<PairType> elementsList;
    std::vector<std::list<iterator>> table;
    
    void updateMod() {
        bool isPrime = false;
        for (mod = TABLE_REHASH_RATE * setSize; !isPrime; mod++) {
            isPrime = true;
            for (size_t i = 2; isPrime && i * i <= mod; i++)
                if (mod % i == 0)
                    isPrime = false;
        }
        table.resize(mod);
    }
    
    void rehash() {
        table.clear();
        updateMod();
        for (auto it = elementsList.begin(); it != elementsList.end(); it++)
            table[hash(it->first) % mod].push_back(it);
    }
    
    void checkForRehash() {
        if (mod < TABLE_RATE * setSize)
            rehash();
    }
public:
    HashMap(Hash _hash = Hash()): hash(_hash) {
        table.resize(mod);
    }
    
    template<typename Iter>
    HashMap(Iter begin, Iter end, Hash _hash = Hash()): HashMap(_hash) {
        for (auto cur = begin; cur != end; cur++)
            setSize++;
        updateMod();
        for (auto cur = begin; cur != end; cur++) {
            elementsList.push_back(*cur);
            auto it = elementsList.end();
            table[hash(cur->first) % mod].push_back(iterator(--it));
        }
    }
    
    HashMap(std::initializer_list<PairType> list, Hash _hash = Hash()): HashMap(list.begin(), list.end(), _hash) {}
    
    size_t size() const {
        return setSize;
    }
    
    bool empty() const {
        return (size() == 0);
    }
    
    Hash hash_function() const {
        return hash;
    }
    
    void insert(PairType const& val) {
        for (auto i : table[hash(val.first) % mod])
            if (i->first == val.first)
                return;
        setSize++;
        elementsList.push_back(val);
        auto it = elementsList.end();
        table[hash(val.first) % mod].push_back(--it);
        checkForRehash();
    }
    
    void erase(KeyType const& key) {
        for (auto it = table[hash(key) % mod].begin(); it != table[hash(key) % mod].end(); it++)
            if ((*it)->first == key) {
                elementsList.erase(*it);
                table[hash(key) % mod].erase(it);
                setSize--;
                checkForRehash();
                return;
            }
    }
    
    iterator begin() {
        return elementsList.begin();
    }
    
    const_iterator begin() const {
        return elementsList.begin();
    }
    
    iterator end() {
        return elementsList.end();
    }
    
    const_iterator end() const {
        return elementsList.end();
    }
    
    iterator find(KeyType const& key) {
        for (auto i : table[hash(key) % mod])
            if (i->first == key)
                return i;
        return end();
    }
    
    const_iterator find(KeyType const& key) const {
        for (auto i : table[hash(key) % mod])
            if (i->first == key)
                return i;
        return end();
    }
    
    ValueType& operator[](KeyType const& key) {
        insert(PairType(key, ValueType()));
        
        for (auto i : table[hash(key) % mod])
            if (i->first == key)
                return static_cast<ValueType&>(i->second);
        
        return elementsList.begin()->second;
    }
    
    ValueType const& at(KeyType const& key) const {
        for (auto i : table[hash(key) % mod])
            if (i->first == key)
                return static_cast<ValueType const&>(i->second);
        
        throw std::out_of_range("There is no such key");
    }
    
    void clear() {
        setSize = 0;
        mod = 1;
        elementsList.clear();
        table.clear();
        table.resize(1);
    }
    
    class iterator : public std::list<PairType>::iterator {
    public:
        iterator(): std::list<PairType>::iterator() {}
        iterator(typename std::list<PairType>::iterator listIter):
        std::list<PairType>::iterator(listIter) {}
        
        std::pair<const KeyType, ValueType>* operator->() {
            return (std::pair<const KeyType, ValueType>*)(std::list<PairType>::iterator::operator->());
        }
        
        std::pair<const KeyType, ValueType>& operator*() {
            return (std::pair<const KeyType, ValueType>&)(std::list<PairType>::iterator::operator*());
        }
    };
};

