#include <iostream>
#include <queue>
using namespace std;

enum COLOR { RED, BLACK };

struct interval{
int low,high;
public:
    interval & operator = (const interval &t);
};

class Node {
public:
 interval *i;
 int max;
COLOR color;
Node *left, *right, *parent;

Node(interval i){
    parent = left = right = NULL;
    this->i=new interval(i);
    max = i.high;
    color = RED;
}


Node *uncle() {

	if (parent == NULL or parent->parent == NULL)
	return NULL;
	if (parent->isOnLeft())
	return parent->parent->right;
	else
	return parent->parent->left;
}


bool isOnLeft() { return this == parent->left; }


Node *sibling() {

	if (parent == NULL)
	return NULL;

	if (isOnLeft())
	return parent->right;

	return parent->left;
}


void moveDown(Node *nParent) {
	if (parent != NULL) {
	if (isOnLeft()) {
		parent->left = nParent;
	} else {
		parent->right = nParent;
	}
	}
	nParent->parent = parent;
	parent = nParent;
}

bool hasRedChild() {
	return (left != NULL and left->color == RED) or
		(right != NULL and right->color == RED);
}
};

class RBTree {
public :Node *root;


void leftRotate(Node *x) {

	Node *nParent = x->right;


	if (x == root)
	root = nParent;

	x->moveDown(nParent);

	x->right = nParent->left;

	if (nParent->left != NULL)
	nParent->left->parent = x;


	nParent->left = x;
}

void rightRotate(Node *x) {

	Node *nParent = x->left;


	if (x == root)
	root = nParent;

	x->moveDown(nParent);


	x->left = nParent->right;

	if (nParent->right != NULL)
	nParent->right->parent = x;


	nParent->right = x;
}

void swapColors(Node *x1, Node *x2) {
	COLOR temp;
	temp = x1->color;
	x1->color = x2->color;
	x2->color = temp;
}

void swapValues(Node *u, Node *v) {
	interval *temp;
	temp = u->i;
	u->i = v->i;
	v->i = temp;
}


void fixRedRed(Node *x) {

	if (x == root) {
	x->color = BLACK;
	return;
	}

	Node *parent = x->parent, *grandparent = parent->parent,
		*uncle = x->uncle();

	if (parent->color != BLACK) {
	if (uncle != NULL && uncle->color == RED) {

		parent->color = BLACK;
		uncle->color = BLACK;
		grandparent->color = RED;
		fixRedRed(grandparent);
	} else {

		if (parent->isOnLeft()) {
		if (x->isOnLeft()) {

			swapColors(parent, grandparent);
		} else {
			leftRotate(parent);
			swapColors(x, grandparent);
		}

		rightRotate(grandparent);
		} else {
		if (x->isOnLeft()) {

			rightRotate(parent);
			swapColors(x, grandparent);
		} else {
			swapColors(parent, grandparent);
		}


		leftRotate(grandparent);
		}
	}
	}
}


Node *successor(Node *x) {
	Node *temp = x;

	while (temp->left != NULL)
	temp = temp->left;

	return temp;
}


Node *BSTreplace(Node *x) {

	if (x->left != NULL and x->right != NULL)
	return successor(x->right);


	if (x->left == NULL and x->right == NULL)
	return NULL;


	if (x->left != NULL)
	return x->left;
	else
	return x->right;
}


void deleteNode(Node *v) {
	Node *u = BSTreplace(v);


	bool uvBlack = ((u == NULL or u->color == BLACK) and (v->color == BLACK));
	Node *parent = v->parent;

	if (u == NULL) {

	if (v == root) {

		root = NULL;
	} else {
		if (uvBlack) {

		fixDoubleBlack(v);
		} else {

		if (v->sibling() != NULL)

			v->sibling()->color = RED;
		}


		if (v->isOnLeft()) {
		parent->left = NULL;
		} else {
		parent->right = NULL;
		}
	}
	delete v;
	return;
	}

	if (v->left == NULL or v->right == NULL) {

	if (v == root) {

		v->i = u->i;
		v->left = v->right = NULL;
		delete u;
	} else {

		if (v->isOnLeft()) {
		parent->left = u;
		} else {
		parent->right = u;
		}
		delete v;
		u->parent = parent;
		if (uvBlack) {

		fixDoubleBlack(u);
		} else {

		u->color = BLACK;
		}
	}
	return;
	}

	swapValues(u, v);

	deleteNode(u);
}
void fixDoubleBlack(Node *x) {
	if (x == root)

	return;

	Node *sibling = x->sibling(), *parent = x->parent;
	if (sibling == NULL) {

	fixDoubleBlack(parent);
	} else {
	if (sibling->color == RED) {

		parent->color = RED;
		sibling->color = BLACK;
		if (sibling->isOnLeft()) {

		rightRotate(parent);
		} else {

		leftRotate(parent);
		}
		fixDoubleBlack(x);
	} else {

		if (sibling->hasRedChild()) {

		if (sibling->left != NULL and sibling->left->color == RED) {
			if (sibling->isOnLeft()) {

			sibling->left->color = sibling->color;
			sibling->color = parent->color;
			rightRotate(parent);
			} else {

			sibling->left->color = parent->color;
			rightRotate(sibling);
			leftRotate(parent);
			}
		} else {
			if (sibling->isOnLeft()) {

			sibling->right->color = parent->color;
			leftRotate(sibling);
			rightRotate(parent);
			} else {

			sibling->right->color = sibling->color;
			sibling->color = parent->color;
			leftRotate(parent);
			}
		}
		parent->color = BLACK;
		} else {

		sibling->color = RED;
		if (parent->color == BLACK)
			fixDoubleBlack(parent);
		else
			parent->color = BLACK;
		}
	}
	}
}


void levelOrder(Node *x) {
	if (x == NULL)

	return;

	queue<Node *> q;
	Node *curr;


	q.push(x);

	while (!q.empty()) {

	curr = q.front();
	q.pop();


	cout <<"[" <<curr->i->low <<","<<curr->i->high<<"]" <<" ";


	if (curr->left != NULL)
		q.push(curr->left);
	if (curr->right != NULL)
		q.push(curr->right);
	}
}


void inorder(Node *x) {
	if (x == NULL)
	return;
	inorder(x->left);
	/*if(x->left && x->left->max >x->right->max ) {if(x->i->high < x->left->max) x->max = x->left->max;} //else x->max = x->left->i->high;}
	if(x->right && x->right->max >x->left->max){if(x->i->high < x->right->max) x->max = x->right->max;} //else x->max = x->right->i->high;}
	if(!(x->left) && !(x->right)){x->max = x->i->high;}*/
	cout <<"[" <<x->i->low <<","<<x->i->high<<"]" <<"-->"<<x->color <<" "<<x->max<<endl;
	inorder(x->right);
}
void updateMax(Node*x){
    if (x == NULL)
    return;
    updateMax(x->left);
    updateMax(x->right);
    if(x->left && x->left->max >x->right->max ) {if(x->i->high < x->left->max) x->max = x->left->max;} //else x->max = x->left->i->high;}
	if(x->right && x->right->max >x->left->max){if(x->i->high < x->right->max) x->max = x->right->max;} //else x->max = x->right->i->high;}
	if(!(x->left) && !(x->right)){x->max = x->i->high;}
}

public:

RBTree() { root = NULL; }

Node *getRoot() { return root; }


Node *search(interval i) {
	Node *temp = root;
	while (temp != NULL) {
	if (i.low < temp->i->low) {
		if (temp->left == NULL)
		break;
		else
		temp = temp->left;
	} else if (i.low == temp->i->low && i.high==temp->i->high) {
		break;
	} else {
		if (temp->right == NULL)
		break;
		else
		temp = temp->right;
	}
	}

	return temp;
}


void insert(interval i) {
	Node *newNode = new Node(i);
	if (root == NULL) {

	newNode->color = BLACK;
	root = newNode;
	} else {
	Node *temp = search(i);

	if (temp->i->low==i.low && temp->i->high==i.high) {

		return;
	}


	newNode->parent = temp;

	if (i.low < temp->i->low)
		temp->left = newNode;
	else
		temp->right = newNode;
	// fix red red voilaton if exists
	if (root->max < i.high)
        root->max = i.high;
	fixRedRed(newNode);
	if (temp->max < i.high)
        temp->max = i.high;
	}
}


void deleteByVal(interval i) {
	if (root == NULL)

	return;

	Node *v = search(i), *u;

	if (v->i->low != i.low || v->i->high != i.high ) {
	cout << "No node found to delete with value: [" <<i.low<<","<<i.high<<"]"  << endl;
	return;
	}

	deleteNode(v);
}

// prints inorder of the tree
void printInOrder() {
	cout << "Inorder: " << endl;
	if (root == NULL)
	cout << "Tree is empty" << endl;
	else
	inorder(root);
	cout << endl;
}


// prints level order of the tree
void printLevelOrder() {
	cout << "Level order: " << endl;
	if (root == NULL)
	cout << "Tree is empty" << endl;
	else
	levelOrder(root);
	cout << endl;
}
};

bool doOVerlap(interval i1, interval i2)
{
    if (i1.low <= i2.high && i2.low <= i1.high)
        return true;
    return false;
}
interval *overlapSearch(Node *root,interval i)
{
    // Base Case, tree is empty
    if (root == NULL) return NULL;

    // If given interval overlaps with root
    if (doOVerlap(*(root->i), i))
        return root->i;

    // If left child of root is present and max of left child is
    // greater than or equal to given interval, then i may
    // overlap with an interval is left subtree
    if (root->left != NULL && root->left->max >= i.low)
        return overlapSearch(root->left, i);

    // Else interval can only overlap with right subtree
    return overlapSearch(root->right, i);
}
interval &interval::operator = (const interval &t)
{
    low=t.low;
    high=t.high;

    return *this;
}

int main() {

     interval ints[] = {{15, 20}, {10, 30}, {17, 19},
        {5, 20}, {12, 15}, {30, 40}
    };
    int n = sizeof(ints)/sizeof(ints[0]);
RBTree tree;

for (int i = 0; i < n; i++)
        tree.insert(ints[i]);

Node *root=tree.root;
tree.printInOrder();
tree.printLevelOrder();

interval x = {6, 7};
	interval y= {30 , 40};

	cout << "\nSearching for interval [" << x.low << "," << x.high << "]";
    interval *res = overlapSearch(root,x);
    if (res == NULL)
        cout << "\nNo Overlapping Interval";
    else
        cout << "\nOverlaps with [" << res->low << ", " << res->high << "]";
        cout<<endl;

tree.deleteByVal(ints[5]);
tree.updateMax(root);
tree.printInOrder();
tree.printLevelOrder();

return 0;
}
