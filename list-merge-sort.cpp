#include <algorithm>
#include <cmath>
#include <cstdio>
#include <stack>
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>

using namespace std;

struct Node;
typedef Node* PNode;

struct Node {
    int value;
    PNode next;

    Node(int value): value(value), next(nullptr) {}
};

PNode sort(PNode list, int count) { // Merge Sort
    if (count <= 1)
        return list;

    PNode middle = list;
    for (int i = 0; i < count / 2 - 1; i++)
        middle = middle->next;
    PNode preMiddle = middle;
    middle = middle->next;
    preMiddle->next = nullptr;

    list = sort(list, count / 2);
    middle = sort(middle, count - count / 2);

    if (list->value > middle->value)
        swap(list, middle);

    PNode prev = list;
    PNode head = list;
    list = list->next;
    while (list != nullptr) {
        while (middle != nullptr && middle->value < list->value) {
            prev->next = middle;
            prev = prev->next;
            middle = middle->next;
            prev->next = list;
        }

        list = list->next;
        prev = prev->next;
    }

    prev->next = middle;

    return head;
}

int main() {
    ios_base::sync_with_stdio(false);

    PNode head = nullptr, tail = nullptr;

    int n;
    cin >> n;

    for (int i = 0; i < n; i++) {
        int t;
        cin >> t;

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
        cout << cur->value << ' ';
    cout << endl;

    return 0;
}