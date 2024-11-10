#include <iostream>
#include "JumpList.h"
using namespace std;

int main() {
	// JumpList list;
	// cout << "Empty list:" << endl;
	// cout << list.size() << endl;
	// cout << list.find("a") << endl;
	// cout << list.get(0) << endl;
	// cout << list.print() << endl;

	// cout << "------------------------" << endl;

	// cout << "insert to empty list:" << endl << endl;
	// list.insert("b");
	// list.insert("a");
	// list.insert("d");
	// list.insert("c");
	// list.insert("e");
	// cout << "size: " << list.size() << endl;
	// cout << "find b: " << list.find("b") << endl;
	// cout << "find f: " << list.find("f") << endl;
	// cout << "get 3: " << list.get(3) << endl;
	// cout << list.prettyPrint() << endl;

	// list.insert("f");
	// cout << list.prettyPrint() << endl;

	// cout << "------------------------" << endl;

	// cout << "insert to running example:" << endl;
	// JumpList example(10);
	// cout << example.prettyPrint() << endl;
	// example.insert("dog");
	// cout << example.prettyPrint() << endl;
	// example.insert("AAAA");
	// cout << example.prettyPrint() << endl;
	// example.insert("AAA");
	// cout << example.prettyPrint() << endl;
	// example.insert("AA");
	// cout << example.prettyPrint() << endl;
	// example.insert("doggy");
	// cout << example.prettyPrint() << endl;
	// example.insert("dogggies");
	// cout << example.prettyPrint() << endl;

	// example.erase("blah");
	// example.erase("x");

	// cout << example.prettyPrint() << endl;

	// string arr[] = {"a", "b"};
	// JumpList example3(2, arr);
	// cout << example3.prettyPrint() << endl;

	// example3.erase("a");

	// cout << example3.prettyPrint() << endl;

	// cout << "erase from running example:" << endl;
	// JumpList example(10);
	// cout << example.prettyPrint() << endl;
	// cout << endl << endl;
	// example.erase("d");
	// cout << example.prettyPrint() << endl;

	// cout << "------------------------" << endl;

	// cout << "my example:" << endl;

	// string arr[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s"};
	// JumpList example2(19, arr);
	// example2.insert("gg");
	// cout << example2.prettyPrint() << endl;

	JumpList example(10);
	cout << example.prettyPrint() << endl;
	example.erase("d");
	cout << example.prettyPrint() << endl;
}