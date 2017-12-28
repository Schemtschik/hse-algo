#include <algorithm>
#include <iostream>

struct Node;
typedef Node* PNode;

struct Node {
    int value;
    PNode next;

    Node(int value): value(value), next(nullptr) {}
};

PNode merge(PNode first, PNode second) {
    if (first->value > second->value)
        std::swap(first, second);

    PNode prev = first;
    PNode head = first;
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

PNode sort(PNode list, size_t count) { // Merge Sort
    if (count <= 1)
        return list;

    PNode middle = list;
    for (size_t i = 0; i < count / 2 - 1; i++)
        middle = middle->next;
    PNode preMiddle = middle;
    middle = middle->next;
    preMiddle->next = nullptr;

    list = sort(list, count / 2);
    middle = sort(middle, count - count / 2);

    return merge(list, middle);
}

void clear(PNode list) {
    if (list == nullptr)
        return;

    PNode cur = list;
    while (list->next != nullptr) {
        PNode toDelete = cur;
        cur = cur->next;
        delete toDelete;
    }

    delete cur;
}

int main() {
    PNode head = nullptr, tail = nullptr;

    size_t n = 0;
    std::cin >> n;

    for (size_t i = 0; i < n; i++) {
        int t;
        std::cin >> t;

        if (head == nullptr) {
            head = new Node(t);
            tail = head;
        } else {
            tail->next = new Node(t);
            tail = tail->next;
        }
    }

    head = sort(head, n);

    for (PNode cur = head; cur != nullptr; cur = cur->next)
        std::cout << cur->value << ' ';
    std::cout << std::endl;

    return 0;
}
