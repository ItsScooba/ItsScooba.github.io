//============================================================================
// Name        : HashTable.cpp
// Author      : Stephen Frueh
// Class       : CS-260
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <algorithm>
#include <climits>
#include <iostream>
#include <string> // atoi
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

const unsigned int DEFAULT_SIZE = 179;

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


//============================================================================
// Hash Table class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a hash table with chaining.
 */
class HashTable {

private:

	//2D vector used to store the data of the hash table.
	std::vector< std::vector< Bid > > m_bids;
	
	//Function used to generate a hash value for each piece of data stored in the hash table.
    unsigned int hash(int key);

public:
    HashTable();
    virtual ~HashTable();
    void Insert(Bid bid);
    void PrintAll();
    void Remove(string bidId);
    Bid Search(string bidId);
};

/**
 * Default constructor
 */
HashTable::HashTable() : m_bids(DEFAULT_SIZE) {
}

/**
 * Destructor
 */
HashTable::~HashTable() {
	m_bids.clear();
}

/**
 * Calculate the hash value of a given key.
 * Note that key is specifically defined as
 * unsigned int to prevent undefined results
 * of a negative list index.
 *
 * @param key The key to hash
 * @return The calculated hash
 */
unsigned int HashTable::hash(int key) {

	//Use the modulus function using the key and the default size of the table.
	return key % DEFAULT_SIZE;
}

/**
 * Insert a bid
 *
 * @param bid The bid to insert
 */
void HashTable::Insert(Bid bid) {
	
	//Get the hash value for this bid.
	unsigned int hash_value = hash(strToDouble(bid.bidId, ' '));
	m_bids[hash_value].push_back(bid);
}

/**
 * Print all bids
 */
void HashTable::PrintAll() {
	for (std::vector< std::vector< Bid > >::iterator containerIter = m_bids.begin(); containerIter != m_bids.end(); containerIter++) {
		for (std::vector< Bid >::iterator bidIter = containerIter->begin(); bidIter != containerIter->end(); containerIter++) {
			displayBid(*bidIter);
		}
	}
	
}

/**
 * Remove a bid
 *
 * @param bidId The bid id to search for
 */
void HashTable::Remove(string bidId) {

	//Generate the hash value to find the appropriate bid.
	unsigned int hashValue = hash(strToDouble(bidId, ' '));

	//Loop through the entries at the hashValue set of values in the hash table.
	for (std::vector< Bid >::iterator bidIter = m_bids[hashValue].begin(); bidIter != m_bids[hashValue].end(); bidIter++) {
		if (bidIter->bidId == bidId) {

			//Found the matching value, erase the bid and break from the loop.
			m_bids[hashValue].erase(bidIter);
			break;
		}
	}
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid HashTable::Search(string bidId) {

	//Default bid value to return if no value is found in the hash table.
    Bid bid;
	bid.amount = -1;
	bid.bidId = "Not Found";
	bid.fund = "Not Found";
	bid.title = "Not Found";

	//Find the hash value for the bid ID.
	unsigned int bidHashValue = hash(strToDouble(bidId, ' '));
	
	//Check the index at the hash value where the bid would have been stored.
	if (!m_bids[bidHashValue].empty()) {
		for (std::vector< Bid >::iterator bidIter = m_bids[bidHashValue].begin(); bidIter != m_bids[bidHashValue].end(); bidIter++ ) {
			if (bidIter->bidId == bidId) {
				bid = *bidIter;
				break;
			}
		}
	}

    return bid;
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
void loadBids(string csvPath, HashTable* hashTable) {
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
            hashTable->Insert(bid);
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
    str.erase(remove(str.begin(), str.end(), ch), str.end());
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

    // Define a hash table to hold all the bids
    HashTable* bidTable;

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


		bidTable = new HashTable();

        switch (choice) {

        case 1:

            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            loadBids(csvPath, bidTable);

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bidTable->PrintAll();
            break;

        case 3:
            ticks = clock();

            bid = bidTable->Search(bidKey);

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
            bidTable->Remove(bidKey);
            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
