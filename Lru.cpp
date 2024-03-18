#include <iostream>
#include <unordered_map>
using namespace std;

class Node
{
public:
    string data;
    Node *prev;
    Node *next;

    Node(string data)
    {
        this->data = data;
        this->next = nullptr;
        this->prev = nullptr;
    }
};

unordered_map<int, Node *> doesItExist;
int sizeOfCache = 3;
Node *head;
Node *tail;

Node *insertNode(string value)
{
    Node *curr = new Node(value); // store the actual value
    curr->next = head->next;
    head->next = curr;
    curr->next->prev = curr;
    curr->prev = head;

    return curr;
}

void display()
{
    Node *current = head->next;
    while (current != tail)
    {
        cout << current->data << " ";
        current = current->next;
    }
    cout << endl;
}

Node *deleteNode(Node *nodeToDelete, string value)
{
    if (nodeToDelete == nullptr || nodeToDelete->prev == nullptr || nodeToDelete->next == nullptr)
    {
        return nullptr;
    }

    nodeToDelete->prev->next = nodeToDelete->next;
    nodeToDelete->next->prev = nodeToDelete->prev;

    // Delete the node from memory
    delete nodeToDelete;

    Node *locationStoredAt = insertNode(value);
    return locationStoredAt;
}

void removeFromMap(Node *nodeToBeReplaced)
{
    for (auto it = doesItExist.begin(); it != doesItExist.end(); ++it)
    {
        if (it->second == nodeToBeReplaced)
        {
            doesItExist.erase(it);

            break; // Stop iterating after the key-value pair is removed
        }
    }
}

string get(int key)
{
    // if this key isn't already present in the cache
    if (doesItExist.find(key) == doesItExist.end())
    {
        return "";
    }

    // points to the address location where the actual value is stored
    // but we must replace it from wherver it was to next to head and change the value in map as well
    Node *nodeAlreadyPresent = doesItExist[key];
    Node *newLocationItWasPutAfterReplacingAsLru = deleteNode(nodeAlreadyPresent, nodeAlreadyPresent->data);

    doesItExist[key] = newLocationItWasPutAfterReplacingAsLru;
    return newLocationItWasPutAfterReplacingAsLru->data;
}

int put(int key, string value)
{
    if (doesItExist.size() < sizeOfCache)
    {
        // if the key is not present , this mean we must add it to the dll after the head and then add the location here
        if (doesItExist.find(key) == doesItExist.end())
        {
            Node *locationNodeIsStoredAt = insertNode(value);
            doesItExist[key] = locationNodeIsStoredAt;
        }
        else
        {
            // means that the key is already present , we only need to delete the node from its older position
            // then put it after the head and update its location in map
            Node *nodeToBeDeleted = doesItExist[key];
            Node *locationNodeIsStoredAt = deleteNode(nodeToBeDeleted, nodeToBeDeleted->data);
            doesItExist[key] = locationNodeIsStoredAt;
        }
    }
    else
    {
        // means that the map is full so we must replace the second last from both node and map and then add new node after head
        // and add it to map
        Node *nodeToBeReplaced = tail->prev;
        Node *locationNewNodeIsStoredAt = deleteNode(nodeToBeReplaced, value);
        removeFromMap(nodeToBeReplaced);
        doesItExist[key] = locationNewNodeIsStoredAt;
    }
}

int main()
{

    head = new Node("");
    tail = new Node("");
    head->next = tail;
    tail->prev = head;

    put(1, "hello");
    put(2, "world");
    get(3);
    get(2);
    put(3, "final");
    get(1);
    put(4, "should replace");

    display();
    return 0;
}