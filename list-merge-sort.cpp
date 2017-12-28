#include <algorithm>
#include <iostream>

template <typename T>
struct Node {
    T value;
    Node<T>* next;

    Node(T value): value(value), next(nullptr) {}
};

template <typename T>
Node<T>* merge(Node<T>* first, Node<T>* second) {
    if (first->value > second->value)
        std::swap(first, second);

    Node<T>* prev = first;
    Node<T>* head = first;
    first = first->next;
    while (first != nullptr) {
        while (second != nullptr && second->value < first->value) {
            prev->next = second;
            prev = prev->next;
            second = second->next;
            prev->next = first;
        }

        first = first->next;
        prev = prev->next;
    }

    prev->next = second;

    return head;
}

template <typename T>
Node<T>* sort(Node<T>* list, size_t count) { // Merge Sort
    if (count <= 1)
        return list;

    Node<T>* middle = list;
    for (size_t i = 0; i < count / 2 - 1; i++)
        middle = middle->next;
    Node<T>* preMiddle = middle;
    middle = middle->next;
    preMiddle->next = nullptr;

    list = sort(list, count / 2);
    middle = sort(middle, count - count / 2);

    return merge(list, middle);
}

template <typename T>
void clear(Node<T>* list) {
    if (list == nullptr)
        return;

    Node<T>* next = nullptr;
    while (list != nullptr) {
        next = list->next;
        delete list;
        list = next;
    }
}

int main() {
    Node<int>* head = nullptr;
    Node<int>* tail = nullptr;

    size_t n = 0;
    std::cin >> n;

    for (size_t i = 0; i < n; i++) {
        int t;
        std::cin >> t;

        if (head == nullptr) {
            head = new Node<int>(t);
            tail = head;
        } else {
            tail->next = new Node<int>(t);
            tail = tail->next;
        }
    }

    head = sort(head, n);

    for (Node<int>* cur = head; cur != nullptr; cur = cur->next)
        std::cout << cur->value << ' ';
    std::cout << std::endl;

    clear(head);

    return 0;
}
