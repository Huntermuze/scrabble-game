#ifndef ASSIGN2_NODE_H
#define ASSIGN2_NODE_H

template<typename T>
class Node {
public:
    ~Node();

    explicit Node(T *value);

    Node(T *value, Node<T> *previous, Node<T> *next);

    Node(Node<T> &other, Node<T> *previous, Node<T> *next);

    Node(Node<T> &other);

    T *getValue();

    Node<T> *getPreviousNode();

    Node<T> *getNextNode();

    void setNextNode(Node<T> *newNextNode);

    void setPreviousNode(Node<T> *newPreviousNode);

    void setValue(T *newValue);

private:
    T *value;
    Node<T> *previous;
    Node<T> *next;
};

template<typename T>
Node<T>::Node(T *value) {
    this->value = value;
    this->previous = nullptr;
    this->next = nullptr;
}

template<typename T>
Node<T>::Node(T *value, Node<T> *previous, Node<T> *next) {
    this->value = value;
    this->previous = previous;
    this->next = next;
}

template<typename T>
Node<T>::Node(Node<T> &other, Node<T> *previous, Node<T> *next) {
    this->value = other.value;
    this->previous = previous;
    this->next = next;
}

template<typename T>
Node<T>::Node(Node<T> &other) {
    this->value = other.value;
    this->previous = other.previous;
    this->next = other.next;
}

template<typename T>
T *Node<T>::getValue() {
    return value;
}

template<typename T>
Node<T> *Node<T>::getPreviousNode() {
    return previous;
}

template<typename T>
Node<T> *Node<T>::getNextNode() {
    return next;
}

template<typename T>
void Node<T>::setNextNode(Node<T> *newNextNode) {
    next = newNextNode;
}

template<typename T>
void Node<T>::setPreviousNode(Node<T> *newPreviousNode) {
    previous = newPreviousNode;
}

template<typename T>
void Node<T>::setValue(T *newValue) {
    value = newValue;
}

template<typename T>
Node<T>::~Node() {
    delete value;
}

#endif // ASSIGN2_NODE_H