// projectLast.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include<fstream>
#include <chrono>

using namespace std;
struct bstnode {
	int data;
	bstnode *left;
	bstnode *right;
	bstnode(int a) {
		data = a;
		left = NULL;
		right = NULL;
	}
};
bstnode *root = NULL;
void insertbst(bstnode * node, bstnode *prev, int insertdata) {
	if (node == NULL) {
		bstnode *newnode = new bstnode(insertdata);
		node = newnode;
		if (prev->data <= node->data) {
			prev->right = node;
		}
		else
		{
			prev->left = node;
		}
		//cout << insertdata << "eklendi" << endl;
	}
	else if (node->data <= insertdata) {
		insertbst(node->right, node, insertdata);
	}
	else {
		insertbst(node->left, node, insertdata);
	}

}
void insertstart(int data) {
	if (root == NULL) {
		bstnode *newnode = new bstnode(data);
		root = newnode;

	}
	else if (root->data <= data) {

		insertbst(root->right, root, data);
	}
	else {

		insertbst(root->left, root, data);
	}
}
bool searchbst(bstnode *node, int data) {
	if (node == NULL) {
		return false;
	}
	else if (node->data == data) {
		return true;
	}
	else if (node->data < data) {
		return searchbst(node->right, data);
	}
	else {
		return searchbst(node->left, data);
	}
}
void searchanswer(int data) {
	if (searchbst(root, data)) {
		//cout << data << " bulundu." << endl;
	}
	else {
		cout << data << " bulunamadý." << endl;
	}
}
int leftmostreplacebst(bstnode *node, bstnode * prev) {
	if (node->right == NULL) {

		if (node->left == NULL) {
			if (prev->data <= node->data) {
				prev->right = NULL;
			}
			else
			{
				prev->left = NULL;
			}
		}
		else {
			if (prev->data <= node->data) {
				prev->right = node->left;
			}
			else
			{
				prev->left = node->left;
			}

		}
		return node->data;
	}
	else {
		return leftmostreplacebst(node->right, node);
	}
}
void deletebst(bstnode *node, bstnode *prev, int data) {
	if (node == NULL) {
		//cout << data << " Not Found!" << endl;
		return;
	}
	else if (node->data == data) {
		//cout << data << " Deleted!" << endl;
		//checkleftmost;
		if (node->left == NULL) {
			if (prev == NULL) {
				node = node->right;
			}
			else
			{
				if (prev->data <= node->data) {
					prev->right = node->right;
				}
				else
				{
					prev->left = node->right;
				}

			}
		}
		else
		{
			node->data = leftmostreplacebst(node->left, node);

		}
	}
	else if (node->data < data) {
		return deletebst(node->right, node, data);
	}
	else {
		return deletebst(node->left, node, data);
	}
}
void traversebst(bstnode *node) {
	if (node == NULL) {
		return;
	}

	else if (node == root) {

	}
	if (node->right != NULL) {

		traversebst(node->right);
	}
	if (node->left != NULL) {

		traversebst(node->left);
	}



}


class BTreeNode
{
	int *keys;
	int t;
	BTreeNode **C;
	int n;
	bool leaf;

public:

	BTreeNode(int _t, bool _leaf);

	void traverse();

	BTreeNode *search(int k);

	int findKey(int k);


	void insertNonFull(int k);


	void splitChild(int i, BTreeNode *y);

	void remove(int k);

	void removeFromLeaf(int idx);

	void removeFromNonLeaf(int idx);

	int getPred(int idx);

	int getSucc(int idx);

	void fill(int idx);

	void borrowFromPrev(int idx);

	void borrowFromNext(int idx);

	void merge(int idx);

	friend class BTree;
};

class BTree
{
	BTreeNode *root;
	int t;
public:

	BTree(int _t)
	{
		root = NULL;
		t = _t;
	}

	void traverse()
	{
		if (root != NULL) root->traverse();
	}
	BTreeNode* search(int k)
	{
		return (root == NULL) ? NULL : root->search(k);
	}

	void insert(int k);

	void remove(int k);

};

BTreeNode::BTreeNode(int t1, bool leaf1)
{
	t = t1;
	leaf = leaf1;

	keys = new int[2 * t - 1];
	C = new BTreeNode *[2 * t];

	n = 0;
}

int BTreeNode::findKey(int k)
{
	int idx = 0;
	while (idx<n && keys[idx] < k)
		++idx;
	return idx;
}

void BTreeNode::remove(int k)
{
	int idx = findKey(k);

	if (idx < n && keys[idx] == k)
	{

		if (leaf)
			removeFromLeaf(idx);
		else
			removeFromNonLeaf(idx);
	}
	else
	{
		if (leaf)
		{
			//cout << "The key " << k << " is does not exist in the tree\n";
			return;
		}

		bool flag = ((idx == n) ? true : false);

		if (C[idx]->n < t)
			fill(idx);

		if (flag && idx > n)
			C[idx - 1]->remove(k);
		else
			C[idx]->remove(k);
	}
	return;
}

void BTreeNode::removeFromLeaf(int idx)
{
	for (int i = idx + 1; i<n; ++i)
		keys[i - 1] = keys[i];

	n--;

	return;
}

void BTreeNode::removeFromNonLeaf(int idx)
{

	int k = keys[idx];

	if (C[idx]->n >= t)
	{
		int pred = getPred(idx);
		keys[idx] = pred;
		C[idx]->remove(pred);
	}

	else if (C[idx + 1]->n >= t)
	{
		int succ = getSucc(idx);
		keys[idx] = succ;
		C[idx + 1]->remove(succ);
	}

	else
	{
		merge(idx);
		C[idx]->remove(k);
	}
	return;
}

int BTreeNode::getPred(int idx)
{
	// Keep moving to the right most node until we reach a leaf
	BTreeNode *cur = C[idx];
	while (!cur->leaf)
		cur = cur->C[cur->n];

	return cur->keys[cur->n - 1];
}

int BTreeNode::getSucc(int idx)
{

	BTreeNode *cur = C[idx + 1];
	while (!cur->leaf)
		cur = cur->C[0];

	return cur->keys[0];
}

void BTreeNode::fill(int idx)
{
	if (idx != 0 && C[idx - 1]->n >= t)
		borrowFromPrev(idx);

	else if (idx != n && C[idx + 1]->n >= t)
		borrowFromNext(idx);

	else
	{
		if (idx != n)
			merge(idx);
		else
			merge(idx - 1);
	}
	return;
}

void BTreeNode::borrowFromPrev(int idx)
{

	BTreeNode *child = C[idx];
	BTreeNode *sibling = C[idx - 1];

	for (int i = child->n - 1; i >= 0; --i)
		child->keys[i + 1] = child->keys[i];

	if (!child->leaf)
	{
		for (int i = child->n; i >= 0; --i)
			child->C[i + 1] = child->C[i];
	}

	child->keys[0] = keys[idx - 1];

	if (!leaf)
		child->C[0] = sibling->C[sibling->n];

	keys[idx - 1] = sibling->keys[sibling->n - 1];

	child->n += 1;
	sibling->n -= 1;

	return;
}

void BTreeNode::borrowFromNext(int idx)
{

	BTreeNode *child = C[idx];
	BTreeNode *sibling = C[idx + 1];

	child->keys[(child->n)] = keys[idx];

	if (!(child->leaf))
		child->C[(child->n) + 1] = sibling->C[0];

	keys[idx] = sibling->keys[0];

	for (int i = 1; i<sibling->n; ++i)
		sibling->keys[i - 1] = sibling->keys[i];

	if (!sibling->leaf)
	{
		for (int i = 1; i <= sibling->n; ++i)
			sibling->C[i - 1] = sibling->C[i];
	}

	child->n += 1;
	sibling->n -= 1;

	return;
}

void BTreeNode::merge(int idx)
{
	BTreeNode *child = C[idx];
	BTreeNode *sibling = C[idx + 1];

	child->keys[t - 1] = keys[idx];

	for (int i = 0; i<sibling->n; ++i)
		child->keys[i + t] = sibling->keys[i];

	if (!child->leaf)
	{
		for (int i = 0; i <= sibling->n; ++i)
			child->C[i + t] = sibling->C[i];
	}

	for (int i = idx + 1; i<n; ++i)
		keys[i - 1] = keys[i];

	for (int i = idx + 2; i <= n; ++i)
		C[i - 1] = C[i];

	child->n += sibling->n + 1;
	n--;

	delete(sibling);
	return;
}
void BTree::insert(int k)
{
	if (root == NULL)
	{
		root = new BTreeNode(t, true);
		root->keys[0] = k;
		root->n = 1;
	}
	else
	{
		if (root->n == 2 * t - 1)
		{
			BTreeNode *s = new BTreeNode(t, false);

			s->C[0] = root;

			s->splitChild(0, root);

			int i = 0;
			if (s->keys[0] < k)
				i++;
			s->C[i]->insertNonFull(k);

			root = s;
		}
		else
			root->insertNonFull(k);
	}
}
void BTreeNode::insertNonFull(int k)
{
	int i = n - 1;

	if (leaf == true)
	{
		while (i >= 0 && keys[i] > k)
		{
			keys[i + 1] = keys[i];
			i--;
		}

		keys[i + 1] = k;
		n = n + 1;
	}
	else
	{
		while (i >= 0 && keys[i] > k)
			i--;

		if (C[i + 1]->n == 2 * t - 1)
		{
			splitChild(i + 1, C[i + 1]);

			if (keys[i + 1] < k)
				i++;
		}
		C[i + 1]->insertNonFull(k);
	}
}

void BTreeNode::splitChild(int i, BTreeNode *y)
{

	BTreeNode *z = new BTreeNode(y->t, y->leaf);
	z->n = t - 1;

	for (int j = 0; j < t - 1; j++)
		z->keys[j] = y->keys[j + t];

	if (y->leaf == false)
	{
		for (int j = 0; j < t; j++)
			z->C[j] = y->C[j + t];
	}

	y->n = t - 1;

	for (int j = n; j >= i + 1; j--)
		C[j + 1] = C[j];

	C[i + 1] = z;

	for (int j = n - 1; j >= i; j--)
		keys[j + 1] = keys[j];

	keys[i] = y->keys[t - 1];

	n = n + 1;
}

void BTreeNode::traverse()
{
	int i;
	for (i = 0; i < n; i++)
	{
		if (leaf == false)
			C[i]->traverse();
		cout << " " << keys[i];
	}

	if (leaf == false)
		C[i]->traverse();
}
BTreeNode *BTreeNode::search(int k)
{
	int i = 0;
	while (i < n && k > keys[i])
		i++;

	if (keys[i] == k)
		return this;

	if (leaf == true)
		return NULL;
	return C[i]->search(k);
}

void BTree::remove(int k)
{
	if (!root)
	{
		cout << "The tree is empty\n";
		return;
	}

	root->remove(k);

	if (root->n == 0)
	{
		BTreeNode *tmp = root;
		if (root->leaf)
			root = NULL;
		else
			root = root->C[0];
		delete tmp;
	}
	return;
}
int * myarray = new int[600001];
int heapnum = 1;
void traverseheap() {
	for (int i = 1; i < heapnum; i++) {
		cout << myarray[i] << " ";
	}
	cout << endl;
}

void insertHeap(int number) {
	myarray[heapnum] = number;
	int counter = heapnum;
	heapnum++;
	bool flag = true;
	while (counter > 1 && flag) {
		if (myarray[counter] < myarray[counter / 2]) {
			int temp = myarray[counter];
			myarray[counter] = myarray[counter / 2];
			myarray[counter / 2] = temp;
		}
		else {
			flag = false;
		}
		counter = counter / 2;
	}
	//traverseheap();
}
void deleteHeap() {
	int counter = 1;
	heapnum--;
	int temp = myarray[counter];
	myarray[counter] = myarray[heapnum];
	myarray[heapnum] = temp;
	bool flag = true;
	while (flag) {
		if (counter * 2 + 1 < heapnum) {
			if (myarray[counter] <= myarray[counter * 2] && myarray[counter] <= myarray[counter * 2 + 1]) {
				flag = false;

			}
			else {
				if (myarray[counter * 2 + 1] < myarray[counter * 2]) {
					temp = myarray[counter];
					myarray[counter] = myarray[counter * 2 + 1];
					myarray[counter * 2 + 1] = temp;
					counter = counter * 2 + 1;

				}
				else {
					temp = myarray[counter];
					myarray[counter] = myarray[counter * 2];
					myarray[counter * 2] = temp;
					counter = counter * 2;
				}

			}
		}
		else {
			if (counter * 2 < heapnum && myarray[counter] > myarray[counter * 2]) {

				temp = myarray[counter];
				myarray[counter] = myarray[counter * 2];
				myarray[counter * 2] = temp;


			}
			flag = false;

		}
	}
	//traverseheap();
}
int main() {
	int insertNum, del, search;
	string insertData, deleteData, searchData;
	ifstream insertFile, deleteFile, searchFile;
	insertFile.open("insertion.txt");
	deleteFile.open("deletion.txt");
	searchFile.open("searching.txt");
	vector<int> v;
	BTree b(3);

	int choose;
	cout << "Choose : " << endl;
	cout << "1. Heap : \n2. BST : \n3. B-Tree :" << endl;
	cin >> choose;

	if (choose == 1) {
		cout << "Choose : " << endl;
		cout << "1. Insert : \n2. Delete :" << endl;
		cin >> choose;
		if (choose == 1) {
			//Insert
			auto start = std::chrono::high_resolution_clock::now();
			while (getline(insertFile, insertData)) {
				insertNum = stoi(insertData);

				insertHeap(insertNum);

			}

			auto finish = std::chrono::high_resolution_clock::now();
			std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << "ns\n";
		}
		else if (choose == 2) {
			
			int a = 50000;
			while (getline(insertFile, insertData)) {
				insertNum = stoi(insertData);

				insertHeap(insertNum);

			}
			auto start = std::chrono::high_resolution_clock::now();

			for (int i = 0; i < a; i++) {
			
				deleteHeap();
			


			}
			auto finish = std::chrono::high_resolution_clock::now();
			std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << "ns\n";

		}
		else {
			cout << "Please enter a valid number!" << endl;
		}

	}
	else if (choose == 2) {
		cout << "Choose : " << endl;
		cout << "1. Insert : \n2. Delete : \n3. Search :" << endl;
		cin >> choose;
		if (choose == 1) {
			
			auto start = std::chrono::high_resolution_clock::now();
			while (getline(insertFile, insertData)) {
				insertNum = stoi(insertData);

				insertstart(insertNum);


			}

			auto finish = std::chrono::high_resolution_clock::now();
			std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << "ns\n";
		}
		else if (choose == 2) {
			
			while (getline(insertFile, insertData)) {
				insertNum = stoi(insertData);

				insertstart(insertNum);


			}
			auto start = std::chrono::high_resolution_clock::now();

			while (getline(deleteFile, deleteData)) {
				del = stoi(deleteData);
				deletebst(root, NULL, del);

			}

			auto finish = std::chrono::high_resolution_clock::now();
			std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << "ns\n";

		}
		else if (choose == 3) {
			
			while (getline(insertFile, insertData)) {
				insertNum = stoi(insertData);

				insertstart(insertNum);


			}
			auto start = std::chrono::high_resolution_clock::now();
			while (getline(searchFile, searchData)) {
				search = stoi(searchData);

				searchanswer(search);


			}
			auto finish = std::chrono::high_resolution_clock::now();
			std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << "ns\n";


		}
		else {
			cout << "Please enter a valid number! " << endl;
		}
	}

	else if (choose == 3) {
		cout << "Choose : " << endl;
		cout << "1. Insert : \n2. Delete : \n3. Search :" << endl;
		cin >> choose;
		if (choose == 1) {
			// Insert
			auto start = std::chrono::high_resolution_clock::now();
			while (getline(insertFile, insertData)) {
				insertNum = stoi(insertData);

				b.insert(insertNum);


			}

			auto finish = std::chrono::high_resolution_clock::now();
			std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << "ns\n";
		}
		else if (choose == 2) {
			//Delete
			while (getline(insertFile, insertData)) {
				insertNum = stoi(insertData);
				b.insert(insertNum);

			}
			auto start = std::chrono::high_resolution_clock::now();

			while (getline(deleteFile, deleteData)) {
				del = stoi(deleteData);

				b.remove(del);


			}
			auto finish = std::chrono::high_resolution_clock::now();
			std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << "ns\n";


		}
		else if (choose == 3) {
			//Search
			while (getline(insertFile, insertData)) {
				insertNum = stoi(insertData);

				b.insert(insertNum);


			}
			auto start = std::chrono::high_resolution_clock::now();
			while (getline(searchFile, searchData)) {
				search = stoi(searchData);

				b.search(search);


			}
			auto finish = std::chrono::high_resolution_clock::now();
			std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << "ns\n";


		}
		else {
			cout << "Please enter a valid number!" << endl;
		}

	}
	else {
		cout << "Please enter a valid number!" << endl;
	}


	return 0;


}