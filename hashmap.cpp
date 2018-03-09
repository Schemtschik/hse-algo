#include <functional>
#include <list>
#include <stdexcept>
#include <vector>
#include <utility>

template<class KeyType, class ValueType, class Hash = std::hash<KeyType> >
class HashMap {
public:
    //typedef typename std::list<std::pair<KeyType, ValueType>>::iterator iterator;
    class iterator;
    typedef typename std::list<std::pair<KeyType, ValueType>>::const_iterator const_iterator;
private:
    Hash hash;
    size_t _size = 0;
    size_t mod = 1;
    std::list<std::pair<KeyType, ValueType>> _list;
    std::vector<std::list<iterator>> v;

    void updateMod() {
        bool isPrime = false;
        for (mod = 10 * _size; !isPrime; mod++) {
            isPrime = true;
            for (size_t i = 2; isPrime && i * i <= mod; i++)
                if (mod % i == 0)
                    isPrime = false;
        }
        v.resize(mod);
    }

    void rehash() {
        v.clear();
        updateMod();
        for (auto it = _list.begin(); it != _list.end(); it++)
            v[hash(it->first) % mod].push_back(it);
    }

    void checkForRehash() {
        if (mod < 5 * _size)
            rehash();
    }
public:
    HashMap(Hash _hash = Hash()): hash(_hash) {
        v.resize(mod);
    }

    template<typename Iter>
    HashMap(Iter begin, Iter end, Hash _hash = Hash()): HashMap(_hash) {
        for (auto cur = begin; cur != end; cur++)
            _size++;
        updateMod();
        for (auto cur = begin; cur != end; cur++) {
            _list.push_back(*cur);
            auto it = _list.end();
            v[hash(cur->first) % mod].push_back(iterator(--it));
        }
    }

    HashMap(std::initializer_list<std::pair<KeyType, ValueType>> list, Hash _hash = Hash()): HashMap(list.begin(), list.end(), _hash) {}

    size_t size() const {
        return _size;
    }

    bool empty() const {
        return (size() == 0);
    }

    Hash hash_function() const {
        return hash;
    }

    void insert(std::pair<KeyType, ValueType> val) {
        for (auto i : v[hash(val.first) % mod])
            if (i->first == val.first)
                return;
        _size++;
        _list.push_back(val);
        auto it = _list.end();
        v[hash(val.first) % mod].push_back(--it);
        checkForRehash();
    }

    void erase(KeyType key) {
        for (auto it = v[hash(key) % mod].begin(); it != v[hash(key) % mod].end(); it++)
            if ((*it)->first == key) {
                _list.erase(*it);
                v[hash(key) % mod].erase(it);
                _size--;
                checkForRehash();
                return;
            }
    }

    iterator begin() {
        return _list.begin();
    }

    const_iterator begin() const {
        return _list.begin();
    }

    iterator end() {
        return _list.end();
    }

    const_iterator end() const {
        return _list.end();
    }

    iterator find(KeyType key) {
        for (auto i : v[hash(key) % mod])
            if (i->first == key)
                return i;
        return end();
    }

    const_iterator find(KeyType key) const {
        for (auto i : v[hash(key) % mod])
            if (i->first == key)
                return i;
        return end();
    }

    ValueType& operator[](KeyType key) {
        insert(std::pair<KeyType, ValueType>(key, ValueType()));

        for (auto i : v[hash(key) % mod])
            if (i->first == key)
                return static_cast<ValueType&>(i->second);

        return _list.begin()->second;
        //throw std::runtime_error("Something went wrong in [] operator");
    }

    ValueType const& at(KeyType key) const {
        for (auto i : v[hash(key) % mod])
            if (i->first == key)
                return static_cast<ValueType const&>(i->second);

        throw std::out_of_range("There is no such key");
    }

    void clear() {
        _size = 0;
        mod = 1;
        _list.clear();
        v.clear();
        v.resize(1);
    }

    class iterator : public std::list<std::pair<KeyType, ValueType>>::iterator {
    public:
        iterator(): std::list<std::pair<KeyType, ValueType>>::iterator() {}
        iterator(typename std::list<std::pair<KeyType, ValueType>>::iterator listIter):
                std::list<std::pair<KeyType, ValueType>>::iterator(listIter) {}

        std::pair<const KeyType, ValueType>* operator->() {
            return (std::pair<const KeyType, ValueType>*)(std::list<std::pair<KeyType, ValueType>>::iterator::operator->());
        }

        std::pair<const KeyType, ValueType>& operator*() {
            return (std::pair<const KeyType, ValueType>&)(std::list<std::pair<KeyType, ValueType>>::iterator::operator*());
        }
    };
};

