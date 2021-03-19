#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct Node {
    int data;
    Node* npx;
    Node (int x, Node* ptr = NULL) {
        data = x;
        npx = ptr;
    };
};

class LinkedList {
    private:
        Node* XOR(Node *a, Node *b);
        Node* head;
        Node* end;
        Node* actual;
        Node* actualPrev;
        Node* actualNext;
    public:
        LinkedList();
        ~LinkedList();
        void AddBeg(int x);
        void AddEnd(int x);
        void Actual() const;
        void Next();
        void Prev();
        void DelBeg();
        void DelEnd();
        void DelAct();
        void PrintForward() const;
        void PrintBackward() const;
};

Node* LinkedList::XOR(Node *a, Node *b)  { 
    return (Node*)((uintptr_t)a ^ (uintptr_t)b); 
} 

LinkedList::LinkedList() {
    head = end = actual = actualPrev = actualNext = NULL;
}

LinkedList::~LinkedList() {
    Node* currentNode = head;
    Node* prevNode = NULL;

    while(currentNode != NULL) {
        Node* temp = prevNode;
        prevNode = currentNode;
        currentNode = XOR(temp, currentNode->npx);
        delete temp;
    }
    delete prevNode;
}

void LinkedList::AddBeg(int x) {
    if(head == NULL) {
        head = new Node(x);
        end = head;
        actual = head;
        return;
    }

    Node* temp = new Node(x, head);
    head->npx = XOR(temp, head->npx);
    head = temp;

    if(actualPrev == NULL)
        actualPrev = head;
}

void LinkedList::AddEnd(int x) {
    if(end == NULL)
        return AddBeg(x);

    Node* temp = new Node(x, end);
    end->npx = XOR(temp, end->npx);
    end = temp;

    if(actualNext == NULL)
        actualNext = end;
}

void LinkedList::Actual() const {
    if(actual == NULL)
        printf("NULL\n");
    else
        printf("%i\n", *actual);
}

// Na tym skończyłem
void LinkedList::Next() {
    // Nie wiadomo co zrobić gdy nie ma już kolejnego elementu, póki co
    // nie przemieszczam wtedy wskaźnika
    if(actual == end) {
        printf("%i\n", *actual);
        return;
    }


}

int main() {
    
    return 0;
}