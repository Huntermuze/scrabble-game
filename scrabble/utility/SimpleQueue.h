#ifndef ASSIGNMENT_2_NEW_SIMPLEQUEUE_H
#define ASSIGNMENT_2_NEW_SIMPLEQUEUE_H

#include "Node.h"

/*
 * This is a very minimal Queue abstract data structure that is implemented via a
 * Linked List data structure.
 *
 * Why was this created? Why not just use the STL queue or even the linked list
 * already made?
 * This was created because it facilitates a certain kind of use-case that the
 * other two do not offer. That is, being able to add items to the queue, poll
 * them (without destroying the object like the STL queue and custom linked list
 * implementation), then re-add them to the back of the queue. Instead of
 * adding additional parameters and making the custom linked list code messier,
 * and suffering the overhead of having to copy each item (very expensive for
 * big objects like Player), it was decided to create another, stripped-down,
 * and specific version.
 */
template<typename T>
class SimpleQueue {
public:
    SimpleQueue();

    ~SimpleQueue();

    void enqueue(T *value);

    int size() const;

    T *operator[](int pos);

    T *dequeue();

    T *dequeueThenEnqueue();

private:
    Node<T> *head;
    Node<T> *tail;
    int length;
};

template<typename T>
SimpleQueue<T>::SimpleQueue() {
    head = nullptr;
    tail = nullptr;
    length = 0;
}

template<typename T>
SimpleQueue<T>::~SimpleQueue() {
    if (length == 0) {
        return;
    }

    Node<T> *currNode = head;

    while (currNode->getNextNode() != nullptr) {
        currNode = currNode->getNextNode();
        delete currNode->getPreviousNode();
        currNode->setPreviousNode(nullptr);
    }

    delete currNode;
    head = nullptr;
    tail = nullptr;
    length = 0;
}

template<typename T>
void SimpleQueue<T>::enqueue(T *value) {
    if (head == nullptr) {
        head = new Node<T>(value);
        tail = head;
    } else {
        tail = new Node<T>(value, tail, nullptr);
        tail->getPreviousNode()->setNextNode(tail);
    }

    ++length;
}

template<typename T>
T *SimpleQueue<T>::dequeue() {
    T *valueToReturn = nullptr;

    if (head != nullptr) {
        Node<T> *oldHead = head;

        head = head->getNextNode();
        oldHead->setNextNode(nullptr);
        head->setPreviousNode(nullptr);

        valueToReturn = oldHead->getValue();
        oldHead->setValue(nullptr);

        delete oldHead;

        --this->length;
    }

    return valueToReturn;
}

template<typename T>
T *SimpleQueue<T>::dequeueThenEnqueue() {
    T *nodeValueToEnqueue = dequeue();
    enqueue(nodeValueToEnqueue);

    return nodeValueToEnqueue;
}

template<typename T>
T *SimpleQueue<T>::operator[](int pos) {
    Node<T> *currNode;

    if (pos == -1) {
        currNode = tail;
    } else {
        currNode = head;
        for (int i = 0; i < pos; i++) {
            currNode = currNode->getNextNode();
        }
    }

    return currNode->getValue();
}

template<typename T>
int SimpleQueue<T>::size() const {
    return length;
}

#endif //ASSIGNMENT_2_NEW_SIMPLEQUEUE_H
