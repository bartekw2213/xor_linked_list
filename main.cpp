#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <iostream>

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
        void DelActualHead();   // usunięcie actual który jest 1. elementem
        void DelActualEnd();    // usunięcie actual który jest ostatnim elementem
        void DelActualNormal(); // usunięcie actual który jest normalnym elementem
        void MoveCursorToSearchedEl(Node** cursor, Node** prevCursor, int val);
        void DelCursor(Node** cursor, Node** prevCursor); 
    public:
        LinkedList();
        ~LinkedList();
        void AddBeg(int x);
        void AddEnd(int x);
        void AddAct(int x);
        void Actual() const;
        void Next();
        void Prev();
        void DelBeg();
        void DelEnd();
        void DelAct();
        void DelVal(int x);
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

void LinkedList::AddAct(int x) {
    if(actual == NULL || actual == head) {
        AddBeg(x);
        return;
    } 

    Node* newNode = new Node(x);
    newNode->npx = XOR(actualPrev, actual);
    actualPrev->npx = XOR(newNode, XOR(actualPrev->npx, actual));
    actual->npx = XOR(newNode, XOR(actual->npx, actualPrev));
    actualPrev = newNode;
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
    } else if (!moveForward) {
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

void LinkedList::DelActualHead() {
    Node* temp = head;
    head = head->npx;
    head->npx = XOR(temp, head->npx);

    actual = end;
    actualNext = NULL;
    actualPrev = end->npx;
    delete temp;
}

void LinkedList::DelActualEnd() {
    Node* temp = actual;
    MoveActual(false);
    actualNext = NULL;
    actual->npx = actualPrev;
    end = actual;

    delete temp;
}

void LinkedList::DelActualNormal() {
    Node* temp = actual;
    MoveActual(false);
    actualNext = XOR(temp->npx, actual);
    actual->npx = XOR(actualPrev, actualNext);
    actualNext->npx = XOR(actual, XOR(actualNext->npx, temp));

    delete temp;
}

void LinkedList::DelAct() {
    if(actual == NULL)
        return;

    if(head == end) {
        delete head;
        head = end = actual = actualPrev = actualNext = NULL;
    }
    // pierwszy element to actual - usun go, a actual przesun na koniec
    else if(head == actual)
        DelActualHead();
    else if(end == actual)
        DelActualEnd();
    // wykonaj normalne usuniecie
    else
        DelActualNormal();
}

void LinkedList::MoveCursorToSearchedEl(Node** cursor, Node** prevCursor, int val) {
    while(*cursor != NULL && (*cursor)->data != val) {
        Node* temp = *prevCursor;
        *prevCursor = *cursor;
        *cursor = XOR(temp, (*cursor)->npx);
    }
}

void LinkedList::DelCursor(Node** cursor, Node** prevCursor) {
    Node* nextNode = XOR((*cursor)->npx, *prevCursor);

    // zmień składową npx elementów przed i po usuwanym elemencie
    if(*prevCursor != NULL)
        (*prevCursor)->npx = XOR(XOR(*cursor, (*prevCursor)->npx), nextNode);
    if(nextNode != NULL)
        nextNode->npx = XOR(*prevCursor, XOR(nextNode->npx, *cursor));

    // jeśli usuwany element jest actualPrev lub acutalNext - zmień te składowe
    if(*cursor == actualPrev)
        actualPrev = *prevCursor;
    else if(*cursor == actualNext)
        actualNext = nextNode;

    delete *cursor;

    // przesuń kursor na kolejny element
    *cursor = nextNode;
} 

void LinkedList::DelVal(int x) {
    if(head == NULL)
        return;
    while (head != NULL && head->data == x)
        DelBeg();
    while (end != NULL && end->data == x)
        DelEnd();
    while (actual != NULL && actual->data == x)
        DelAct();

    Node* cursor = head;
    Node* prevCursor = NULL;

    while(true) {
        MoveCursorToSearchedEl(&cursor, &prevCursor, x);

        if(cursor != NULL)
            DelCursor(&cursor, &prevCursor);
        else
            break;
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

    while(std::cin >> userOption) {
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
        else if(strncmp(userOption, "ADD_ACT", MAX_OPTION_LENGTH) == 0) {
            int val;
            scanf("%i", &val);
            l.AddAct(val);
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
        else if(strncmp(userOption, "DEL_VAL", MAX_OPTION_LENGTH) == 0) {
            int val;
            scanf("%i", &val);
            l.DelVal(val);
        } 
        else if(strncmp(userOption, "PRINT_FORWARD", MAX_OPTION_LENGTH) == 0)
            l.PrintForward();
        else if(strncmp(userOption, "PRINT_BACKWARD", MAX_OPTION_LENGTH) == 0)
            l.PrintBackward();
    }
    
    return 0;
}