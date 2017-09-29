#include <iostream>
#include <cmath>
#include <bits/stdc++.h>

using namespace std;

#define N 200005

int TOBALANCE = 0; // TOBALANCE is 1 when the condition is Perfectly balanced, 0 otherwise. By changing this constant you can change balance condition.
int NEARLY_BALANCED = 0;
int PERFECTLY_BALANCED = 1;
double BALANCE_RATIO = 3.0/4.0; //You may change this ratio.

struct Node {
    int val;
    int size;
    struct Node * left, * right;
};

Node * init(int val) {
    Node * curr = (Node *) malloc(sizeof(Node));
    curr->val = val;
    curr->size = 1;
    curr->left = curr->right = NULL;
    return curr;
}

int SortedArrayFromBST(Node * curr, int output[], int fst) {
    if (curr) {
        fst = SortedArrayFromBST(curr->left, output, fst);
        output[fst++] = curr->val;
        fst = SortedArrayFromBST(curr->right, output, fst);
    }

    return fst;
}

Node * BSTFromSortedArray(int input[], int len) {
    if (len == 0) return NULL;

    int mid = (len-1)/2;
    Node * curr = init(input[mid]);
    curr->size = len;
    curr->left = BSTFromSortedArray(input, mid);
    curr->right = BSTFromSortedArray(input + mid + 1, len - mid - 1);
    return curr;
}

void deleteNode(Node * curr) {
    if (curr == NULL) return;
    deleteNode(curr->left);
    deleteNode(curr->right);
    delete curr;
}

bool checkNearBalance(Node * curr) {
    if (curr == NULL) return true;

    int sizeL = 0, sizeR = 0;
    if (curr->left) {
        sizeL = curr->left->size;
    }

    if (curr->right) {
        sizeR = curr->right->size;
    }

    return (max(sizeL, sizeR) <= BALANCE_RATIO * curr->size);
}

bool checkPerfectBalance(Node * curr) {
    if (curr == NULL) return true;

    int sizeL = 0, sizeR = 0;
    if (curr->left) {
        sizeL = curr->left->size;
    }

    if (curr->right) {
        sizeR = curr->right->size;
    }

    return (abs(sizeL - sizeR)  <= 1);
}

Node * insert(Node * curr, int val, int out[]) {
    Node * root = curr;
    while (1) {
        (curr -> size)++;
        if (curr -> val < val) {
            if (curr -> right == NULL) {
                curr -> right = init(val);
                break;
            }
            curr = curr -> right;
        }
        else {
            if (curr -> left == NULL) {
                curr -> left = init(val);
                break;
            }
            curr = curr -> left;
        }
    }

    curr = root;
    while (curr) {
        if ((TOBALANCE == PERFECTLY_BALANCED && !checkPerfectBalance(curr)) || (TOBALANCE == NEARLY_BALANCED && !checkNearBalance(curr))) {
            int len = SortedArrayFromBST(curr, out, 0);
            deleteNode(curr);
            curr = BSTFromSortedArray(out, len);
            break;
        }
        if (curr -> val < val)
            curr = curr -> right;
        else
            curr = curr -> left;
    }
    
    return root;
}

bool search(Node * curr, int val) {
    if (curr == NULL) return false;

    if (curr->val == val) {
        return true;
    }
    else if (curr->val < val) {
        return search(curr->right, val);
    }
    else {
        return search(curr->left, val);
    }
}

void printBinaryTree(Node * curr) {
    static int t = 0;
    if (curr == NULL) return;

    t++;
    printBinaryTree(curr->right);
    t--;
    for (int i = 0; i < t; i++) {
        cout << "\t";
    }
    cout << curr->val << "," << curr->size << "\n";
    t++;
    printBinaryTree(curr->left);
    t--;
}

int main() {
    Node * root = NULL;
    int out[N];
    srand((int)time(0));
    clock_t begin = clock();
    for (int i = 1; i <= 10000; i++) {
        int val;
        cin >> val;
        if (root) {
            root = insert(root, val, out);
        } else {
            root = init(val);
        }
        if (!(i % 100))
            cout << i << " " << (double) (clock() - begin) / CLOCKS_PER_SEC << "\n";
    }

    // printBinaryTree(root);

    return 0;
}