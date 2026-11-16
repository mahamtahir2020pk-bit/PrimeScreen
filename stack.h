
// Stack.h - DATA STRUCTURE 5: stack (template, linked nodes). Last in, first out.
// Used as the "CancellationStack": the newest cancelled booking is on top, so "undo" is pop().
#ifndef STACK_H
#define STACK_H

template <typename T>
class Stack {
public:
    struct Node {
        T data;
        Node* next;
    };

private:
    Node* topNode;
    int count;

public:
    Stack() : topNode(nullptr), count(0) {}
    ~Stack() { clear(); }

    bool isEmpty() const { return topNode == nullptr; }
    int size() const { return count; }
    Node* getTop() const { return topNode; }

    void push(const T& value) {
        Node* node = new Node;
        node->data = value;
        node->next = topNode;
        topNode = node;
        count++;
    }

    bool pop(T& out) {
        if (topNode == nullptr) return false;
        Node* node = topNode;
        out = node->data;
        topNode = node->next;
        delete node;
        count--;
        return true;
    }

    void clear() {
        T ignored;
        while (pop(ignored)) {}
    }
};

#endif

