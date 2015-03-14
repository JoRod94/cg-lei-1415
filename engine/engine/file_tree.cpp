#include "stdafx.h"
#include "file_tree.h"
#include <iostream>
#include <vector>

using namespace std;

struct file_node {
	struct file_node* left;
	struct file_node* right;
	string* filename;
	vector<point>* points;
};

FileNode new_file_node(string* filename, vector<point>* points) {
	FileNode n = (FileNode)malloc(sizeof(struct file_node));
	n->left = NULL;
	n->right = NULL;
	n->points = points;
	n->filename = filename;
	return n;	
}

FileTree::FileTree()
{
	head = NULL;
}


FileTree::~FileTree()
{
	// add destructor code here
}

bool FileTree::empty() {
	return this->head == NULL;
}

FileNode FileTree::add_or_find(string* filename, vector<point>* vec) {
	FileNode new_node = NULL;
	if (FileTree::empty())  {
		new_node = new_file_node(filename, vec);
		this->head = new_node;
		return new_node;
	}
	
	string name = *filename;
	FileNode iterator = this->head;

	while (iterator) {
		string curr_name = *(iterator->filename);
		if (curr_name == name)
			return iterator;

		else if (curr_name < name) {
			if (iterator->left == NULL) {
				new_node = new_file_node(filename, vec);
				iterator->left = new_node;
			}
			else
				iterator = iterator->left;
		}

		else {
			if (iterator->right == NULL) {
				new_node = new_file_node(filename, vec);
				iterator->right = new_node;
			}
			else
				iterator = iterator->right;
		}
	}

	return new_node;
}

void print_file_node(FileNode fn) {
	if (fn == NULL) return;
	cout << "FILENAME: " << *(fn->filename) << endl;
	for (vector<point>::iterator p = (*(fn->points)).begin(); p != (*(fn->points)).end(); ++p) {
		cout << "("
			<< (*p).x
			<< ", "
			<< (*p).y
			<< ", "
			<< (*p).z
			<< "); ";
	}
	cout << endl;
	print_file_node(fn->left);
	print_file_node(fn->right);
}

void FileTree::print() {
	print_file_node(this->head);
}
