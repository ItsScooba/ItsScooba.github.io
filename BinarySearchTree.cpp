//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Stephen Frueh
// Class       : CS-260
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);


// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};


void displayBid(Bid bid);



//Internal structure for tree node
struct Node {
public:

	//Destructor for the node, checks for and deletes any child nodes, which do the same to their children.
	~Node() {
		if (left_child_node)
			delete left_child_node;
		if (right_child_node)
			delete right_child_node;
	}


	//Data held in this node. This can be anything, from a simple int or char, to complicated classes. Whatever is meant to be held in the binary search tree as far as data.
	Bid bid_data;

	//Child nodes of this parent node, which will have their own children, and so on.
	Node* left_child_node;
	Node* right_child_node;

};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:

	//The root node, which is the beginning of the binary search tree. The first entry.
    Node* root;

	//Private helper functions used when calling the public functions, and using recursion.
    void addNode(Node* node, Bid bid);
    void inOrder(Node* node);
    Node* removeNode(Node* node, string bidId);

public:

	//Outward, public facing functions used to perform operations on the binary search tree.
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();									//Display the values of the binary tree in order from least to greatest.
    void Insert(Bid bid);							//Insert a value into the tree.
    void Remove(string bidId);						//Remove and delete a node from the tree.
    Bid Search(string bidId);						//Search for a node in the tree provided an identifier.
	Bid Search(Node* node, string bidId);			//Recursive function used to search down the tree.
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
	root = new Node();
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node

	//Just delete root, and root's destructor handles deleting the children.
	delete root;
}











/**
 * Insert a bid into a node, and add it to the binary search tree.
 */
void BinarySearchTree::Insert(Bid bid) {
    // Implement inserting a bid into the tree

	//First check if the root node has bid data.
	if (root && root->bid_data.amount == 0) {
		root->bid_data = bid;
		return;
	}

	//Otherwise, if the root already has bid data, recursively call addNode to add it to a leaf.
	else
		addNode(root, bid);
}




/**
 * Add a bid to some node (recursive)
 *
 * @param node Current node in tree
 * @param bid Bid to be added
 */
void BinarySearchTree::addNode(Node* node, Bid bid) {
	// FIXME (2b) Implement inserting a bid into the tree

	//Check if the new bid would go to the left or the right of the given node.
	if (node->bid_data.bidId > bid.bidId) {

		//Check if there is a left child node already. If so, recursively call addNode on it. Otherwise, create it with the new bid.
		if (node->left_child_node)
			addNode(node->left_child_node, bid);
		else {
			node->left_child_node = new Node();
			node->left_child_node->bid_data = bid;
		}

	} 

	//Otherwise, it goes on the right if it's equal or greater.
	else {

		//Check if there is a right child node already. If so, recursively call addNode on it. Otherwise, create it with the new bid.
		if (node->right_child_node)
			addNode(node->right_child_node, bid);
		else {
			node->right_child_node = new Node();
			node->right_child_node->bid_data = bid;
		}
	}

}




/**
 * Remove a bid
 */
void BinarySearchTree::Remove(string bidId) {

	//Recursively call removeNode and it will handle finding and removing the proper node.
	root = removeNode(root, bidId);

}





Node* BinarySearchTree::removeNode(Node* root, string bidId) {

	//Passed an empty node. 
	if (root == NULL) return root;

	//First, check if the key is less than or greater than the current node's value. Setting root to the returned value of removeNode rebuilds the tree properly after a 
	//node is removed.

	//Left side
	if (root->bid_data.bidId > bidId)
		root->left_child_node = removeNode(root->left_child_node, bidId);

	//Right node.
	else if (root->bid_data.bidId < bidId)
		root->right_child_node = removeNode(root->right_child_node, bidId);



	//Last, check if the root node is the node to be removed.
	else if (root->bid_data.bidId == bidId) {

		//First, check whether this node has 2, 1 or no children.
		if (root->left_child_node && root->right_child_node) {

			//Find the lowest value in the right sub-tree and set it as the new root.
			Node* temp_node = root->right_child_node;
			while (temp_node != NULL) {
				temp_node = temp_node->left_child_node;
			}

			//Set the root's data to the temp nodes, and delete the old node replacing the root.
			root->bid_data = temp_node->bid_data;
			removeNode(root->right_child_node, temp_node->bid_data.bidId);

		}

		//Otherwise, check for a left child node only.
		else if (root->left_child_node == NULL) {
			//Left child node is returned and set to parents new child.
			Node* temp_node = root->right_child_node;
			delete root;
			return temp_node;
		}

		//Otherwise, check for a right child node only.
		else if (root->right_child_node == NULL) {
			Node* temp_node = root->left_child_node;
			delete root;
			return temp_node;
		}
	}
	
	//Return root node, if not returned anywhere else and no children to replace root.
	return root;
}



/**
 * Search for a bid
 */
Bid BinarySearchTree::Search(string bidId) {

	//Call the recursive search function, using the root node as a starting point.
	return Search(root, bidId);
}




//Overloaded search function used to take advantage of recursion, making searching much simpler.
Bid BinarySearchTree::Search(Node* node, string bidId) {

	//The bid structure to return.
	Bid bid;

	//Check to see if the parent node passed is the matching node.
	if (node->bid_data.bidId == bidId)
		bid = node->bid_data;

	//Otherwise, if the bidId is less than the node's value, check the left side of the tree.
	else if (node->left_child_node && node->left_child_node->bid_data.bidId > bidId)
		bid = Search(node->left_child_node, bidId);

	//Lastly, if neither of the above, check the right of the tree.
	else if (node->right_child_node && node->right_child_node->bid_data.bidId > bidId)
		bid = Search(node->right_child_node, bidId);

	return bid;
}













/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
	//If this is being called on the root node, start calling the private, recursive helper function.
	if (root)
		inOrder(root);
}




void BinarySearchTree::inOrder(Node* node) {

	//Display nodes on the left, then the root, followed by the right.
	if (node != NULL) {
		inOrder(node->left_child_node);
		displayBid(node->bid_data);
		inOrder(node->right_child_node);
	}

}





//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            bst->Insert(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
	//str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98109";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
        bidKey = "98109";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a binary search tree to hold all bids
    BinarySearchTree* bst;

    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            bst = new BinarySearchTree();

            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            loadBids(csvPath, bst);

            //cout << bst->Size() << " bids read" << endl;

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bst->InOrder();
            break;

        case 3:
            ticks = clock();

            bid = bst->Search(bidKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
            	cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 4:
            bst->Remove(bidKey);
            break;
        }
    }

    cout << "Good bye." << endl;

	return 0;
}
