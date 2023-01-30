#ifndef ASSIGN2_LINKEDLIST_H
#define ASSIGN2_LINKEDLIST_H

#include <iostream>
#include "Node.h"
#include "../core/Tile.h"
#include "../contract/Serialisable.h"

/*
 * A restricted (see the serialiseToString() method's description for the reasoning)
 * templated LinkedList class, allowing for flexibility and re-usability.
 */
template<typename T>
class LinkedList : public Serialisable {
public:
    LinkedList();

    ~LinkedList() override;

    // Appends a Node to the end of the list.
    void addNode(Node<T> *inputNode);

    // Creates a node out of the passed value, and appends it to the end of list.
    void add(T *inputNodeValue);

    /*
     * Creates a node out of the passed value, and inserts it at the desired
     * position.
     */
    void insert(T *inputNodeValue, int pos);

    // Removes a given Node from the list, taking into account all cases.
    void removeNode(Node<T> *nodeToRemove);

    // Serialises the LinkedList into its string representation.
    std::string serialiseToString() override;

    // Returns the size of the list.
    int size() const;

    // Iterates the list and destroys all objects in it.
    void del();

    // Prints out the LinkedList's contents.
    void print();

    // Sorts the LinkedLis's elements in ascending order.
    void sort();

    // Overload equality operator to check if elements are equivalent.
    bool operator==(const LinkedList<T> &other) const;

    void forEach(std::function<void(T *)> doSomething);

    T *getIf(std::function<bool(T *)> predicate);

    bool checkForEach(std::function<bool(T *)> predicate);

    /*
     * Overloads the [] operator, so that the LinkedList can be accessed with the
     * pointer syntax (like an Array or Vector).
     */
    Node<T> *operator[](int pos);

    // Returns the head node of the LinkedList.
    Node<T> *getHead();

    // Returns the tail node of the LinkedList.
    Node<T> *getTail();

    // Removes and returns the first element's value in the LinkedList.
    T *pollFirst();

    // Returns the desired element's value.
    T *get(int pos);

    /*
     * Adds all elements in the given LinkedList by duplicating them.
     * Similar to Copy Constructor, except allows you to reuse this object.
     */
    void duplicate(LinkedList<T> *list);


private:
    // Pointer to the head node.
    Node<T> *head;
    // Pointer to the tail node.
    Node<T> *tail;
    // Length of the LinkedList.
    int length;
};


template<typename T>
LinkedList<T>::LinkedList() {
    /*
     * Enforce a template/generics constraint, such that T extends Serialisable,
     * or in other words, T MUST be a subclass of Serialisable or Serialisable.
     * Since this LinkedList is marked serialisable, then its elements must also
     * be serialisable, so that it can correctly be serialised.
     */
    static_assert(std::is_base_of<Serialisable, T>::value,
                  "type parameter of this class must derive from Serialisable");
    head = nullptr;
    tail = nullptr;
    length = 0;
}

template<typename T>
LinkedList<T>::~LinkedList() {
    this->del();
}

template<typename T>
void LinkedList<T>::del() {
    // If there is nothing to destroy then just return.
    if (length == 0) {
        return;
    }

    Node<T> *currNode = head;

    // Destroy all the Nodes in the list.
    while (currNode->getNextNode() != nullptr) {
        currNode = currNode->getNextNode();
        delete currNode->getPreviousNode();
        currNode->setPreviousNode(nullptr);
    }

    // Destroy the last node.
    delete currNode;
    // Set the fields to nullptr to indicate that they are no longer in use.
    head = nullptr;
    tail = nullptr;
    length = 0;
}

template<typename T>
Node<T> *LinkedList<T>::operator[](int pos) {
    Node<T> *currNode;

    // Emulating python's behaviour, passing -1 will return the tail.
    if (pos == -1) {
        currNode = tail;
    } else {
        currNode = head;
        for (int i = 0; i < pos; i++) {
            currNode = currNode->getNextNode();
        }
    }

    return currNode;
}

template<typename T>
void LinkedList<T>::removeNode(Node<T> *nodeToRemove) {
    // Base case.
    if (head == nullptr || nodeToRemove == nullptr) {
        return;
    }

    // If node to be deleted is head node.
    if (head == nodeToRemove) {
        head = nodeToRemove->getNextNode();
    }

    // Change next only if the node to be deleted is not the last node.
    if (nodeToRemove->getNextNode() != nullptr) {
        nodeToRemove->getNextNode()->setPreviousNode(nodeToRemove->getPreviousNode());
    }

    // Change previous node only if the node to be deleted is not the first node.
    if (nodeToRemove->getPreviousNode() != nullptr) {
        nodeToRemove->getPreviousNode()->setNextNode(nodeToRemove->getNextNode());
    }

    // If the node to remove is the tail, then we must update LinkedList's tail.
    if (nodeToRemove == tail) {
        tail = nodeToRemove->getPreviousNode();
    }

    // Decrement the length.
    --this->length;

    // Destroy object.
    delete nodeToRemove;
}

template<typename T>
Node<T> *LinkedList<T>::getHead() {
    return head;
}

template<typename T>
Node<T> *LinkedList<T>::getTail() {
    return tail;
}

template<typename T>
void LinkedList<T>::addNode(Node<T> *inputNode) {
    // List must be empty.
    if (tail == nullptr) {
        head = inputNode;
        tail = inputNode;

        inputNode->setPreviousNode(nullptr);
        inputNode->setNextNode(nullptr);
    } else {
        inputNode->setPreviousNode(tail);
        inputNode->setNextNode(nullptr);
        tail = inputNode;
    }

    length++;
}

template<typename T>
int LinkedList<T>::size() const {
    return length;
}

template<typename T>
void LinkedList<T>::add(T *inputNodeValue) {
    // List must be empty
    if (head == nullptr) {
        head = new Node<T>(inputNodeValue);
        tail = head;
    } else {
        tail = new Node<T>(inputNodeValue, tail, nullptr);
        tail->getPreviousNode()->setNextNode(tail);
    }

    length++;
}

template<typename T>
void LinkedList<T>::print() {
    if (head == nullptr) {
        std::cout << "The list is empty!" << std::endl;
    } else {
        Node<T> *currNode = head;
        while (currNode != nullptr) {
            std::cout << currNode->getValue().getValue() << ", ";
            currNode = currNode->getNextNode();
        }

        std::cout << std::endl;
    }
}

template<typename T>
void LinkedList<T>::insert(T *inputNodeValue, int pos) {
    // Swap the head node with the new node.
    if (pos == 0) {
        head->setPreviousNode(new Node<T>(inputNodeValue, nullptr, head));
        head = head->getPreviousNode();
    } else if (pos == length) {
        // Swap the tail node with the new node.
        tail->setNextNode(new Node<T>(inputNodeValue, tail, nullptr));
        tail = tail->getNextNode();
    } else {
        // Replace the node at the pos with the new node.
        Node<T> *existingNode = (*this)[pos];

        existingNode->getPreviousNode()->setNextNode(
                new Node<T>(inputNodeValue, existingNode->getPreviousNode(), existingNode));
        existingNode->setPreviousNode(existingNode->getPreviousNode()->getNextNode());
    }

    length++;
}

template<typename T>
void LinkedList<T>::duplicate(LinkedList<T> *list) {
    this->del();
    addNode(list->getHead()->getValue());
    Node<T> *currNode = list->getHead();

    while (currNode->getNextNode() != nullptr) {
        addNode(currNode->getNextNode()->getValue());
        currNode = currNode->getNextNode();
    }
}

template<typename T>
T *LinkedList<T>::get(int pos) {
    return (*this)[pos]->getValue();
}

template<typename T>
void LinkedList<T>::sort() {
    LinkedList<T> *sorted = new LinkedList<T>();
    sorted->addNode(this->head->getValue());

    for (int i = 1; i < this->length; i++) {
        Node<T> *unsortNode = (*this)[i];
        int j = 0;
        bool insert = false;

        while (!insert && j < sorted->length) {
            Node<T> *sortNode = sorted[j];
            if (sortNode->getValue() < unsortNode->getValue()) {
                sorted->insert(unsortNode->getValue(), j);
                insert = true;
            }
            j++;
        }

        if (!insert) {
            sorted->addNode(unsortNode->getValue());
        }

    }

    duplicate(sorted);
    delete sorted;
}

template<typename T>
T *LinkedList<T>::pollFirst() {
    // Copy the head's value before we destroy it.
    T *headValueCopy = new T(*head->getValue());
    // Destroy the head and its value.
    removeNode(head);

    return headValueCopy;
}

template<typename T>
std::string LinkedList<T>::serialiseToString() {
    std::string serialisedLinkedList;

    /*
     * Similar to print() method, but instead of the output being standard output
     * it is the serialisedLinkedList string.
     */
    for (int i = 0; i < this->length; i++) {
        serialisedLinkedList.append(this->get(i)->serialiseToString());

        if (i != this->length - 1) {
            serialisedLinkedList.append(", ");
        }
    }

    return serialisedLinkedList;
}

template<typename T>
void LinkedList<T>::forEach(std::function<void(T *)> doSomething) {
    for (int i = 0; i < length; ++i) {
        doSomething(get(i));
    }
}

template<typename T>
T *LinkedList<T>::getIf(std::function<bool(T *)> predicate) {
    T *toReturn = nullptr;

    for (int i = 0; i < length && toReturn == nullptr; ++i) {
        T *curr = get(i);

        if (predicate(curr)) {
            toReturn = curr;
        }
    }

    return toReturn;
}

template<typename T>
bool LinkedList<T>::checkForEach(std::function<bool(T *)> predicate) {
    bool toCheck = false;

    for (int i = 0; i < length; ++i) {
        T *curr = get(i);

        if (predicate(curr)) {
            toCheck = true;
        }
    }

    return toCheck;
}

template<typename T>
bool LinkedList<T>::operator==(const LinkedList<T> &other) const {
    bool equivalent = true;

    for (int i = 0; i < length; ++i) {
        if (this->get(i) != other.get(i)) {
            equivalent = false;
        }
    }

    return equivalent;
}

#endif // ASSIGN2_LINKEDLIST_H