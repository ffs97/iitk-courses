#include <iostream>
#include <cmath>
#include <bits/stdc++.h>

using namespace std;

#define N 200005

int TOBALANCE = 1; // TOBALANCE is 1 when the condition is Perfectly balanced, 0 otherwise. By changing this constant you can change balance condition.
int NEARLY_BALANCED = 0;
int PERFECTLY_BALANCED = 1;
double BALANCE_RATIO = 0.75; //You may change this ratio.

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

void insert(Node *&curr, int val, int out[]) {
    Node * temp = curr;
    while (1) {
        (temp -> size)++;
        if (temp -> val < val) {
            if (temp -> right == NULL) {
                temp -> right = init(val);
                break;
            }
            temp = temp -> right;
        }
        else {
            if (temp -> left == NULL) {
                temp -> left = init(val);
                break;
            }
            temp = temp -> left;
        }
    }

    temp = curr;
    while (temp) {
        if ((TOBALANCE == PERFECTLY_BALANCED && !checkPerfectBalance(temp)) || (TOBALANCE == NEARLY_BALANCED && !checkNearBalance(temp))) {
            int len = SortedArrayFromBST(temp, out, 0);
            deleteNode(temp);
            if (temp == curr)
                curr = BSTFromSortedArray(out, len);
            else
                temp = BSTFromSortedArray(out, len);
            break;
        }
        if (temp -> val < val)
            temp = temp -> right;
        else
            temp = temp -> left;
    }
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

int main() {
    Node * root = NULL;
    int out[N];
    int queries;
    scanf("%d", &queries);
    while (queries--) {
        int c, val;
        scanf("%d%d", &c, &val);
        if (c == 1) {
            //insert
            if (root) {
                insert(root, val, out);
            } else {
                root = init(val);
            }
        } else {
            //search query
            if (search(root, val)) printf("Yes\n");
            else printf("No\n");
        }
    }

    return 0;
}