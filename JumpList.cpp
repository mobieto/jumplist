#include "JumpList.h"
#include <stdexcept>
#include <iostream>
#include <string>
#include <cmath>

Node::Node(const string& s, Node* next, Node* jump, int gap) {
	data_ = s;
	next_ = next;
	jump_ = jump;
	gap_ = gap;
}

Node::~Node() {
	// IMPLEMENT ME
}


JumpList::JumpList() {
	head_ = nullptr;
}

// DO NOT CHANGE
// You do not need to understand the code here to complete the assignment
JumpList::JumpList(int size, const string* arr) {

	const string s[] = {"a", "b", "blah", "c", "cat", "d", "etc", "ooo...", "x", "zzz"};
	const string* sp = (arr == nullptr) ? s : arr;

	Node** npp = new Node*[size];
	npp[size-1] = new Node(sp[size-1], nullptr, nullptr, 0);
	for(int i = size-2; i >= 0; i--)
		npp[i] = new Node(sp[i], npp[i+1], nullptr, 0);

	if (arr == nullptr) {

		if (size==1) npp[0]->gap_ = 1;
		else if (size==3) npp[0]->gap_ = 3;
 		else if (size==10) {
			npp[0]->jump_ = npp[5];
			npp[5]->jump_ = npp[8];
			npp[0]->gap_ = 5;
			npp[5]->gap_ = 3;
			npp[8]->gap_ = 2;
		}
		else throw std::invalid_argument("size must be 1, 3 or 10");

	}
	else {

		// jump node every MAX_GAP_SIZE
		int i;
		for(i=0; i < size-MAX_GAP_SIZE; i += MAX_GAP_SIZE) {
			npp[i]->jump_ = npp[i+MAX_GAP_SIZE];
			npp[i]->gap_ = MAX_GAP_SIZE;
		}
		npp[i]->gap_ = size % MAX_GAP_SIZE;
		if (npp[i]->gap_ == 0) npp[i]->gap_ = MAX_GAP_SIZE; // 0 need to become 5!

	}

	head_ = npp[0];
	delete [] npp; // note that the local array npp gets destroyed here but the nodes stay alive!
}

JumpList::~JumpList() {
	Node* current = head_;

	while (current != nullptr) { // remove every node from memory
		Node* next = current->next_;
		delete current;
		current = next;
	}
}


int JumpList::size() const {
	if (head_ == nullptr)
		return 0;

	int size = 0;

	Node* current = head_;

	while (current != nullptr) { // loop through each node and increase size by 1
		size++;
		current = current->next_;
	}

	return size;
}

// DONE FOR YOU, DO NOT CHANGE
bool JumpList::find(const string& s) const {
	if (head_ == nullptr) return false;

	// moving along the fast lane
	Node* tmp = head_;
	while(tmp->jump_ != nullptr && tmp->jump_->data_ < s)
		tmp = tmp->jump_;

	// tmp now points to the jump node at the start of the segment where s could be

	// now slow lane
	while(tmp != nullptr) {
		if (tmp->data_ == s) return true; // match
		else if (tmp->data_ > s) return false; // went past without finding s
		else tmp = tmp->next_;
	}

	return false; // end of list
}

string JumpList::get(int i) const {
	if (i < 0 || i >= this->size()) return ""; // out of bounds

	int step = 0;
	Node* current = head_;

	while (step < i) {
		if (current->jump_ != nullptr && step + current->gap_ - 1 < i) { // move through fast lane
			step += current->gap_;
			current = current->jump_;
		} else { // slow lane
			step += 1;
			current = current->next_;
		}
	}

	return current->data_;
}

string JumpList::print() const {
	string out = "";
	Node* current = head_;

	while (current != nullptr) { // line 1, data_ of every node
		out.append(current->data_ + (current->next_ != nullptr ? " " : ""));
		current = current->next_;
	}

	out.append("\n");
	current = head_;

	while (current != nullptr) { // line 2, data_ of every jump node
		out.append(current->data_ + (current->jump_ != nullptr ? " " : ""));
		current = current->jump_;
	}

	out.append("\n");
	current = head_;
	
	while (current != nullptr) { // line 3, gap_ of every jump node
		out.append(std::to_string(current->gap_) + (current->jump_ != nullptr ? " " : ""));
		current = current->jump_;
	}

	return out;
}

string JumpList::prettyPrint() const {
	int numJumpNodes = 0;
	string outLine1 = "";
	string outLine2 = "";
	string outLine3 = "";
	Node* current = head_;

	while (current != nullptr) { // find how many jump nodes there are
		numJumpNodes++;	
		current = current->jump_;
	}

	current = head_;

	int jumpNodePositions[numJumpNodes]; // array to store the position of the first character of data_ inside string
	Node* jumpNodes[numJumpNodes]; // array to match position of jump node to the jump node itself, probably better to use a vector of pairs but not allowed
	int jumpIndex = 0;

	while (current != nullptr) { // line 1, data_ of every node
		if (current->gap_ != 0) { // found a jump node, store data in arrays
			jumpNodePositions[jumpIndex] = outLine1.length();
			jumpNodes[jumpIndex] = current;
			jumpIndex++;
		}

		outLine1.append(current->data_ + (current->next_ != nullptr ? " --> " : ""));
		current = current->next_;
	}

	for (int i = 0; i < numJumpNodes; i++) { // line 2, data_ of every jump node
		outLine2.append( // calculate difference from this jump node's position in string to the next and place that many '-'. Minus 4 for ' ', '> ' and 0 index, minus the extra characters in data_
			jumpNodes[i]->data_ + ((jumpNodes[i]->jump_ != nullptr) ? (" " +
			std::string(jumpNodePositions[i + 1] - jumpNodePositions[i] - 4 - (jumpNodes[i]->data_.length() - 1), '-') +
			"> ") : "")
		);
	}

	for (int i = 0; i < numJumpNodes; i++) { // line 3, gap_ of every jump node
		outLine3.append( // same as line 2 but place ' ', and only minus 1 for 0 index
			std::to_string(jumpNodes[i]->gap_) + ((jumpNodes[i]->jump_ != nullptr) ?
			std::string(jumpNodePositions[i + 1] - jumpNodePositions[i] - 1, ' ') : "")
		);
	}

	return (outLine1 + "\n" + outLine2 + "\n" + outLine3); // join lines with newline
}

bool JumpList::insert(const string& s) {
	Node* newNode = new Node(s, nullptr, nullptr, 0);
	Node* affectedJumpNode = head_;

	if (head_ == nullptr) { // list is empty, just create new head
		head_ = newNode;
		head_->gap_ = 1;
		
		return true;
	}

	if (head_->data_ == s) // head already contains s
		return false;

	if (head_->data_ > s) { // insert before head
 		Node* oldJump = head_->jump_;
		int oldGap = head_->gap_;

		head_->jump_ = nullptr;
		head_->gap_ = 0;

		newNode->next_ = head_;
		newNode->jump_ = oldJump;
		newNode->gap_ = oldGap;

		head_ = newNode;
		affectedJumpNode = newNode;
	} else { // insert after head
		Node* tmp = head_;

		//find position to put new node
		while (tmp->jump_ != nullptr && tmp->jump_->data_ < s) { // fast lane
			affectedJumpNode = tmp->jump_;
			tmp = tmp->jump_;
		}

		while (tmp->next_ != nullptr && tmp->next_->data_ < s) // slow lane
			tmp = tmp->next_;

		if (tmp->next_ != nullptr && tmp->next_->data_ == s) // similar node already exists
			return false;

		Node* oldNext = tmp->next_;

		tmp->next_ = newNode;
		newNode->next_ = oldNext;
	}

	affectedJumpNode->gap_ += 1;

	if (affectedJumpNode->gap_ > MAX_GAP_SIZE) // split segment in half if too big
		splitSegment(affectedJumpNode);

	return true;
}

bool JumpList::erase(const string& s) {
	Node* targetNode = head_;
	Node* prevNode = head_;
	Node* prevJumpNode = head_;

	if (head_ == nullptr) // empty list
		return false;

	if (head_->data_ == s && head_->next_ == nullptr) { // only head
		delete head_;
		head_ = nullptr;

		return true;
	}

	// find node to erase
	while (targetNode->jump_ != nullptr && targetNode->jump_->data_ <= s) { // fast lane
		Node* runner = targetNode;

		while (runner->next_->data_ < s) // use runner to find previous node as fast lane skips over it
			runner = runner->next_;

		prevJumpNode = targetNode;
		prevNode = runner;
		targetNode = targetNode->jump_;
	}

	while (targetNode->next_ != nullptr && targetNode->next_->data_ <= s) { // slow lane
		if (targetNode->gap_ != 0)
			prevJumpNode = targetNode;

		prevNode = targetNode;
		targetNode = targetNode->next_;
	}

	if (targetNode->data_ != s) // not in list
		return false;

	if (targetNode == head_) { // deleting head
		head_ = targetNode->next_;

		if (head_->gap_ == 0) { // if next node was already a jump node, dont change anything
			head_->jump_ = targetNode->jump_;
			head_->gap_ = targetNode->gap_ - 1;
		}
	} else if (targetNode->gap_ != 0) { // deleting jump node, merge previous jump node with next jump node
		Node* nextJumpNode = targetNode->jump_;

		prevJumpNode->jump_ = (nextJumpNode == nullptr) ? nullptr: nextJumpNode; // previous jump node will now jump to next jump node or nullptr if jump node was at end
		prevJumpNode->gap_ += targetNode->gap_ - 1; // merge segments, -1 to take into account removed node
		prevNode->next_ = targetNode->next_;

		if (prevJumpNode->gap_ > MAX_GAP_SIZE) // split if merged segments too big
			splitSegment(prevJumpNode);
	} else { // deleting normal node
		prevNode->next_ = targetNode->next_;
		prevJumpNode->gap_ -= 1;		
	}

	delete targetNode; // finally remove erased node from memory
	targetNode = nullptr;

	return true;
}

void JumpList::splitSegment(Node* targetJumpNode) { // helper method to split a segment in half
	int firstHalf = std::ceil(targetJumpNode->gap_ / 2.0);
	int secHalf = std::floor(targetJumpNode->gap_ / 2.0);

	Node* newJumpNode = targetJumpNode;

	for (int i = 0; i < firstHalf; i++) // find middle of segment, this is new jump node
		newJumpNode = newJumpNode->next_;

	Node* targetNode = newJumpNode;

	for (int i = 0; i < secHalf; i++) // find the node that our new jump node will jump to
		targetNode = targetNode->next_;

	targetJumpNode->jump_ = newJumpNode;
	targetJumpNode->gap_ = firstHalf;
	newJumpNode->jump_ = targetNode;
	newJumpNode->gap_ = secHalf;
}