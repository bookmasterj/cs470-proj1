#include <iostream>
#include <random>
#include <chrono>

using namespace std;

class Node {
public:
    int key;
    Node* child = nullptr;
    //refers to next child on same level, if no such child is nullptr
    Node* next = nullptr;
    //leftmost child points to parent, otherwise refers to prev child on same level
    Node* prev = nullptr;
    Node(int k) {
        this->key = k;
    }
};

bool isEmpty(Node* node) {
    return node == nullptr;
}
//if prev points to node's parent, returns true
bool isLeftmostChild(Node* node) {
    if(node->prev != nullptr) {
        if(node->prev->child == node) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        cout << "Error: node->prev is nullptr";
        return false;
    }
}

Node* findLastChild(Node* parent) {
    Node* child = parent->child;
    //loop through to last child of parent 
    while(child != nullptr) {
        child = child->next;
    }
    return child;
}

Node* meld(Node* node1, Node* node2) {
    if(isEmpty(node1)) {
        return node2;
    }
    else if(isEmpty(node2)) {
        return node1;
    }
    //make node2 a child of node1
    else if(node1->key <= node2->key) {    
        if(!isEmpty(node1->child)) { //node2 is not the first child of node1
            node2->next = node1->child;
            node1->child->prev = node2;
        }
        node2->prev = node1;
        node1->child = node2;
        return node1;
    }
    //make node1 a child of node2
    else {
        if(!isEmpty(node2->child)) { //node1 is not the first child of node2
            node1->next = node2->child;
            node2->child->prev = node1;
        }
        node1->prev = node2;
        node2->child = node1;
        return node2;
    }
}

Node* mergePairs(Node* child) {
    if(isEmpty(child)) {
        return nullptr;
    }
    else if(isEmpty(child->next)) {
        return child;
    }
    else {
        Node* nextPair = child->next->next;
        Node* second = child->next;

        child->next = nullptr;
        second->next = nullptr;
        Node* melded = meld(child, second);
        return meld(melded, mergePairs(nextPair));
    }
}


    


class PairingHeap {
public:
    Node* root = nullptr;

    Node* findMin() {
        return root;
    }
    
    Node* insert(int key) {
        Node* newNode = new Node(key);
        this->root = meld(this->root, newNode);
        return newNode;
    }

    void decreaseKey(Node* node, int newKey) {

        //remove subtree rooted at node
        if(isEmpty(node->prev)) {
            cout << "Error: calling decreaseKey on root";
        }
        else {
            if(isLeftmostChild(node)) {
                //prev refers to node's parent
                node->prev->child = node->next;
            }
            else {
                //prev refers to node's sibling
                node->prev->next = node->next;
            }  
        }
        node->key = newKey;
        this->root = meld(this->root, node);
    }

    Node* extractMin() {
        if(isEmpty(this->root)) {
            cout << "Error: extractMin called on empty heap";
            return nullptr;
        }
        else {
            Node* r = this->root;
            if(isEmpty(this->root->child)) {
                this->root = nullptr;
            }
            else {
                //remove the root by merging root's children
                this->root = mergePairs(this->root->child);
            }
            return r;
        }
    }

    //curr should initially be the heap's root, parents should initially be empty
    void printHeap(vector<Node*> parents) {

        if(parents.size() == 0) { //initial call, prints root and recurses with root as only parent
            cout << this->root->key << "\n";
            if(isEmpty(this->root->child)) {
                return;
            }
            else {
                parents.push_back(this->root);
                printHeap(parents);
            }
        }
        else { //recursive calls
            vector<Node*> new_parents;
            for(int i = 0; i < parents.size(); i++) { //loop through parents
                Node* curr = parents.at(i)->child;
                while(!isEmpty(curr->next)) { //loop through all children of parent and print them
                    cout << curr->key << " - ";
                    if(!isEmpty(curr->child)) {
                        new_parents.push_back(curr);
                    } 
                    curr = curr->next;
                }
                cout << curr->key << " | ";
                if(!isEmpty(curr->child)) {
                    new_parents.push_back(curr);
                }
                if(i == parents.size() - 1) { //print newline if at end of loop
                    cout << "\n";
                }
            }
            if(new_parents.size() == 0) { //base case, no more children to print
                return;
            }
            else {
                printHeap(new_parents);
            }
        }
    }
};

int main() {

    int n = 10;
    int e = 1;
    int lower_bound = 0;
    int upper_bound = 100;

    //init random number gen for testing
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937 gen(seed);
    uniform_int_distribution<> distrib(lower_bound, upper_bound);

    PairingHeap pHeap;

    for(int i = 0; i < n; i++) {
        pHeap.insert(distrib(gen));
    }
    for(int i = 0; i < e; i++) {
        Node* minNode = pHeap.extractMin();
        cout << "Extracted min: " << minNode->key << "\n";
    }
    vector<Node*> parents;
    pHeap.printHeap(parents);
}