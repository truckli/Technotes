#include <iostream>
#include <stack>
using namespace std;

struct Node {
    Node *left;
    Node *right;
    int data;
    Node(Node *l, Node *r, int d = 0) :left(l), right(r), data(d) { }
    void visit() {
        cout << data << endl;
    }
};

enum TreeTraverseWay {
    TREE_PRE_ORDER,
    TREE_MID_ORDER,
    TREE_POST_ORDER,
};

void TreeTraverse(Node *tree, int way)
{
    if (!tree) return;
    stack<Node*> s;
    Node *sentinel = new Node(0, 0, 0);
    s.push(sentinel);//sentinel

    Node *pre = sentinel;
    Node *cur = tree;

    //invariant: stack top is cur's parent, pre is previous cur; in stack, parent on top of a child
    while (cur != sentinel) 
    {
        if (pre != cur->left && pre != cur->right) //top -> down
        {
            if (cur->left) {
                if (TREE_PRE_ORDER == way) cur->visit();
                s.push(cur);
                pre = cur;
                cur = cur->left;
            } else if (cur->right) {
                if (TREE_MID_ORDER == way) cur->visit();
                s.push(cur);
                pre = cur;
                cur = cur->right;
            } else {//cur is leaf
                cur->visit();
                pre = cur;
                cur = s.top();
                s.pop();
            }
        } 
        else //down -> top
        { 
            if (pre == cur->left && cur->right) {//one more child
                if (TREE_MID_ORDER == way) cur->visit();
                s.push(cur);
                pre = cur;
                cur = cur->right;
            } else { //no more children
                if (TREE_POST_ORDER == way) cur->visit();
                pre = cur;
                cur = s.top();
                s.pop();
            }
        }
    }
}

int main ()
{
    const int maxnode = 1<<4;
    Node *node[maxnode];
    for (int i = maxnode-1; i > 0; i--)
    {
        node[i] = new Node((i>=(maxnode>>1)?0:node[2*i]), (i>=(maxnode>>1)?0:node[2*i+1]), i);
    }

    Node *root = node[1];
    node[3]->left = 0;

    //TreeTraverse(root, TREE_PRE_ORDER);
    //TreeTraverse(root, TREE_POST_ORDER);
    TreeTraverse(root, TREE_MID_ORDER);

    for (int i = maxnode-1; i > 0; i--)
    {
        delete node[i];
    }

    return 0;
}

