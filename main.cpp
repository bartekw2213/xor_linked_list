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

void LinkedList::MoveActual(bool moveForward) {
    Node* temp = actual;
    
    if (moveForward) {
        actual = actualNext;
        actualPrev = temp;

        if (actualNext != NULL)
            actualNext = XOR(actualNext->npx, temp);
    } else {
        actual = actualPrev;
        actualNext = temp;

        if(actualPrev != NULL)
            actualPrev = XOR(actualPrev->npx, temp);
    }
}

void LinkedList::Next() {
    // Nie wiadomo co zrobić gdy nie ma już kolejnego elementu, póki co
    // nie przemieszczam wtedy wskaźnika
    if(actual == end) {
        printf("%i\n", actual->data);
        return;
    } 

    if(head == NULL) {
        printf("NULL\n");
        return;
    }

    MoveActual(true);
    printf("%i\n", actual->data);
}

void LinkedList::Prev() {
    // Nie wiadomo co zrobić gdy nie ma już wcześniejszego elementu, 
    // póki co nie przemieszczam wtedy wskaźnika
    if(actual == head) {
        printf("%i\n", actual->data);
        return;
    } 

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

    // gdy actual wskazuje na usuwany przód - przesuwam go
    // do przodu
    if(actual == head) {
        MoveActual(true);

        // jeśli lista jest jednoelementowa to wszystkie wskaźniki
        // actual ustawiam na null (actual, actualPrev, actualNext)
        if(head == end)
            actualPrev = NULL;
    }
    
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
        MoveActual(false);

        // jeśli lista jest jednoelementowa to wszystkie wskaźniki
        // actual ustawiam na null (actual, actualPrev, actualNext)
        if(head == end)
            actualNext = NULL;
    }
    
    Node* temp = end;
    end = end->npx;
    end->npx = XOR(temp, end->npx);
    delete temp;  
}

void LinkedList::DelAct() {
    if(actual == NULL)
        return;
    
    if(head == end)
        DelBeg();
    else if(head == actual) {
        DelBeg();
        actual = end;
        actualNext = NULL;
        actualPrev = end->npx;
    }

    Node* temp = actual;
    Node* tempNext = actualNext;
    MoveActual(false);
    actualNext = tempNext;
    delete temp;
}

void LinkedList::PrintForward() const {
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