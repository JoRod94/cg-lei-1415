#pragma once

#include <string>
#include "point.h"
using namespace std;

typedef struct file_node *FileNode;

class FileTree
{
public:
	FileNode head;
	FileTree();
	~FileTree();

	FileNode add_or_find(string* name, vector<point>* vec);
	bool empty();
	void print();
};


