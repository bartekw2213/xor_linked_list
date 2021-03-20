#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_OPTION_LENGTH 30

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
        Node* XOR(Node *a, Node *b) const;
        Node* head;
        Node* end;
        Node* actual;
        Node* actualPrev;
        Node* actualNext;
        void MoveActual(bool moveForward);
        void MoveActualOnBeg();
        void MoveActualOnEnd();
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

Node* LinkedList::XOR(Node *a, Node *b) const { 
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
        printf("%i\n", actual->data);
}

void LinkedList::MoveActualOnBeg() {
    actualPrev = NULL;
    actual = head;
    actualNext = head->npx;
}

void LinkedList::MoveActualOnEnd() {
    actualPrev = end->npx;
    actual = end;
    actualNext = NULL;
}

void LinkedList::MoveActual(bool moveForward) {
    Node* temp = actual;
    
    if (moveForward) {
        // obecny el jest ostatnim - przesuń actual na początek listy
        if(actualNext == NULL) {
            MoveActualOnBeg();
            return;
        }

        actual = actualNext;
        actualPrev = temp;
        
        if (actualNext != NULL)
            actualNext = XOR(actualNext->npx, temp);
    } else {
        // obecny el jest pierwszym - przesuń actual na koniec listy
        if(actualPrev == NULL) {
            MoveActualOnEnd();
            return;
        }

        actual = actualPrev;
        actualNext = temp;

        if(actualPrev != NULL)
            actualPrev = XOR(actualPrev->npx, temp);
    }
}

void LinkedList::Next() {
    if(head == NULL) {
        printf("NULL\n");
        return;
    }

    MoveActual(true);
    printf("%i\n", actual->data);
}

void LinkedList::Prev() {
    if(head == NULL) {
        printf("NULL\n");
        return;
    }

    MoveActual(false);
    printf("%i\n", actual->data);
}

void LinkedList::DelBeg() {
    if(head == NULL)
        return;

    if(actual == head) {
        DelAct();
        return;
    }

    if(actualPrev == head)
        actualPrev = NULL;
    
    Node* temp = head;
    head = head->npx;
    head->npx = XOR(temp, head->npx);
    delete temp;
}

void LinkedList::DelEnd() {
    if(end == NULL)
        return;

    // gdy actual wskazuje na usuwany tył - przesuwam go
    // do tyłu
    if(actual == end) {
        DelAct();
        return;
    }

    if(actualNext == end)
        actualNext = NULL;
    
    Node* temp = end;
    end = end->npx;
    end->npx = XOR(temp, end->npx);
    delete temp;  
}

void LinkedList::DelAct() {
    if(actual == NULL)
        return;

    // list jest jednoelementowa
    if(head == end) {
        delete head;
        head = end = actual = actualPrev = actualNext = NULL;
    }
    // actual wskazuje na pierwszy, wiec usun pierwszy element,
    // a actual ma wskazywać na ostatni element
    else if(head == actual) {
        Node* temp = head;
        head = head->npx;
        head->npx = XOR(temp, head->npx);
    
        actual = end;
        actualNext = NULL;
        actualPrev = end->npx;
    // lista nie jest ani jednoelementowa, ani actual nie wskazuje
    // na początek listy
    } else {
        Node* temp = actual;
        Node* tempNext = actualNext;
        MoveActual(false);
        actualNext = tempNext;
        delete temp;
    }    
}

void LinkedList::PrintForward() const {
    if (head == NULL) {
        printf("NULL\n");
        return;
    }

    Node* currentNode = head;
    Node* prevNode = NULL;

    while(currentNode != NULL) {
        printf("%i ", currentNode->data);
        Node* temp = prevNode;
        prevNode = currentNode;
        currentNode = XOR(temp, currentNode->npx);
    }
    printf("\n");
}

void LinkedList::PrintBackward() const {
    if (end == NULL) {
        printf("NULL\n");
        return;
    }

    Node* currentNode = end;
    Node* nextNode = NULL;

    while(currentNode != NULL) {
        printf("%i ", currentNode->data);
        Node* temp = nextNode;
        nextNode = currentNode;
        currentNode = XOR(temp, currentNode->npx);
    }
    printf("\n");
}

int main() {
    LinkedList l;
    char userOption[MAX_OPTION_LENGTH];
    bool quit = false;

    while(!quit) {
        scanf("%s", userOption);

        if(strncmp(userOption, "ADD_BEG", MAX_OPTION_LENGTH) == 0) {
            int val;
            scanf("%i", &val);
            l.AddBeg(val);
        } 
        else if(strncmp(userOption, "ADD_END", MAX_OPTION_LENGTH) == 0) {
            int val;
            scanf("%i", &val);
            l.AddEnd(val);
        } 
        else if(strncmp(userOption, "ACTUAL", MAX_OPTION_LENGTH) == 0)
            l.Actual();
        else if(strncmp(userOption, "NEXT", MAX_OPTION_LENGTH) == 0)
            l.Next();
        else if(strncmp(userOption, "PREV", MAX_OPTION_LENGTH) == 0)
            l.Prev();
        else if(strncmp(userOption, "DEL_BEG", MAX_OPTION_LENGTH) == 0)
            l.DelBeg();
        else if(strncmp(userOption, "DEL_END", MAX_OPTION_LENGTH) == 0)
            l.DelEnd();
        else if(strncmp(userOption, "DEL_ACT", MAX_OPTION_LENGTH) == 0)
            l.DelAct();
        else if(strncmp(userOption, "PRINT_FORWARD", MAX_OPTION_LENGTH) == 0)
            l.PrintForward();
        else if(strncmp(userOption, "PRINT_BACKWARD", MAX_OPTION_LENGTH) == 0)
            l.PrintBackward();
        else
            quit = true;
    }
    
    return 0;
}