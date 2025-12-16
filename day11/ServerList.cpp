#include <iostream>

#include "Server.cpp"

// Node class for linked list
class ServerNode {
public:
    Server* data;
    ServerNode* next;

    ServerNode(Server* serverData) : data(serverData), next(nullptr) {}
};

// Simple linked list class
class ServerLinkedList {
private:
    ServerNode* head;

public:
    ServerLinkedList() : head(nullptr) {}

    void insert(Server* server) {
        ServerNode* newNode = new ServerNode(server);
        newNode->next = head;
        head = newNode;
    }

    void display() {
        ServerNode* current = head;
        while (current != nullptr) {
            std::cout << "Server Name: " << current->data->getName() << std::endl;
            current = current->next;
        }
    }

    ~ServerLinkedList() {
        ServerNode* current = head;
        while (current != nullptr) {
            ServerNode* temp = current;
            current = current->next;
            delete temp;
        }
    }

    bool operator==(const ServerLinkedList& other) const {
        ServerNode* current1 = head;
        ServerNode* current2 = other.head;

        for (; current1 != nullptr && current2 != nullptr; ) {
            if (current1->data->getName() != current2->data->getName()) {
                return false;
            }
            current1 = current1->next;
            current2 = current2->next;
        }

        return current1 == nullptr && current2 == nullptr;
    }
};