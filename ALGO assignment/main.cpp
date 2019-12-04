
#include<bits/stdc++.h>
using namespace std;

struct interval{
int low,high;

};
class Node
{
	public:
	    interval *i;
	int max;
	Node *left;
	Node *right;
	int height;
};


int max(int a, int b);

int height(Node *N)
{
	if (N == NULL)
		return 0;
	return N->height;
}

int max(int a, int b)
{
	return (a > b)? a : b;
}


Node* newNode(interval i)
{
	Node* node = new Node();
	node->i=new interval(i);
	node->max=i.high;
	node->left = NULL;
	node->right = NULL;
	node->height = 1;
	return(node);
}


Node *rightRotate(Node *y)
{
	Node *x = y->left;
	Node *T2 = x->right;


	x->right = y;
	y->left = T2;


	y->height = max(height(y->left),
					height(y->right)) + 1;
	x->height = max(height(x->left),
					height(x->right)) + 1;


	return x;
}


Node *leftRotate(Node *x)
{
	Node *y = x->right;
	Node *T2 = y->left;


	y->left = x;
	x->right = T2;

	x->height = max(height(x->left),
					height(x->right)) + 1;
	y->height = max(height(y->left),
					height(y->right)) + 1;


	return y;
}

int getBalance(Node *N)
{
	if (N == NULL)
		return 0;
	return height(N->left) -
		height(N->right);
}

Node* insert(Node* node, interval i)
{

	if (node == NULL)
		return(newNode(i));
		int l = node->i->low;

	if (i.low < l)
		node->left = insert(node->left, i);
	else
        node->right = insert(node->right,i);
	if(node->max<i.high)
        node->max=i.high;


	node->height = 1 + max(height(node->left),
						height(node->right));

	int balance = getBalance(node);


	if (balance > 1 && i.low < node->left->i->low)
		return rightRotate(node);


	if (balance < -1 && i.low > node->right->i->low)
		return leftRotate(node);


	if (balance > 1 && i.low > node->left->i->low)
	{
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}


	if (balance < -1 && i.low < node->right->i->low)
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}
	if(node->max<i.high)
        node->max=i.high;


	return node;
}
bool doOVerlap(interval i1, interval i2)
{
    if (i1.low <= i2.high && i2.low <= i1.high)
        return true;
    return false;
}
interval *overlapSearch(Node *root, interval i)
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

Node * minValueNode(Node* node)
{
	Node* current = node;
	current->i=node->i;


	while (current->left != NULL)
		current = current->left;

	return current;
}


Node* deleteNode(Node* root, interval i)
{
    int l = root->i->low;

	if (root == NULL)
		return root;

	if (i.low < l )
		root->left = deleteNode(root->left, i);
	else if(i.low > root->i->low)
		root->right = deleteNode(root->right, i);

	else
	{

		if( (root->left == NULL) ||
			(root->right == NULL) )
		{
			Node *temp = root->left ?
						root->left :
						root->right;


			if (temp == NULL)
			{
				temp = root;
				root = NULL;
			}
			else
			*root = *temp;
			free(temp);
		}
		else
		{

			Node* temp = minValueNode(root->right);


			root->i = temp->i;
interval* x = temp->i;

			root->right = deleteNode(root->right,*x);
		}
	}

	if (root == NULL)
	return root;


	root->height = 1 + max(height(root->left),
						height(root->right));


	int balance = getBalance(root);


	if (balance > 1 &&
		getBalance(root->left) >= 0)
		return rightRotate(root);


	if (balance > 1 &&
		getBalance(root->left) < 0)
	{
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}


	if (balance < -1 &&
		getBalance(root->right) <= 0)
		return leftRotate(root);


	if (balance < -1 &&
		getBalance(root->right) > 0)
	{
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}

	return root;
}
void inorder(Node *root)
{
    if (root == NULL) return;

    inorder(root->left);

    cout << "[" << root->i->low << ", " << root->i->high << "]"
         << " max = " << root->max << endl;

    inorder(root->right);
}

void preOrder(Node *root)
{
	if(root != NULL)
	{
		cout << root->i << " ";
		preOrder(root->left);
		preOrder(root->right);
	}
}


int main()
{
    interval ints[] = {{15, 20}, {10, 30}, {17, 19},
        {5, 20}, {12, 15}, {30, 40}
    };
    int n = sizeof(ints)/sizeof(ints[0]);
Node *root = NULL;
for (int i = 0; i < n; i++)
        root = insert(root, ints[i]);
	/*root = insert(root, 9);
	root = insert(root, 5);
	root = insert(root, 10);
	root = insert(root, 0);
	root = insert(root, 6);
	root = insert(root, 11);
	root = insert(root, -1);
	root = insert(root, 1);
	root = insert(root, 2);*/
	inorder(root);
	interval x = {6, 7};
	interval y= {30 , 40};

	cout << "\nSearching for interval [" << x.low << "," << x.high << "]";
    interval *res = overlapSearch(root, x);
    if (res == NULL)
        cout << "\nNo Overlapping Interval";
    else
        cout << "\nOverlaps with [" << res->low << ", " << res->high << "]";
        cout<<endl;
        deleteNode(root,y);
        inorder(root);
	//preOrder(root);
	//root = deleteNode(root, 10);


//	cout << "\nPreorder traversal after"
//		<< " deletion of 10 \n";
//	preOrder(root);

	return 0;
}


