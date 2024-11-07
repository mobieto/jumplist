#include "JumpList.h"
#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
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

	while (current->next_ != nullptr) {
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

	while (current != nullptr) {
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
	if (i < 0 || i >= this->size()) return "";

	int step = 0;
	Node* current = head_;

	while (step < i) {
		if (current->jump_ != nullptr && step + current->gap_ - 1 < i) {
			step += current->gap_;
			current = current->jump_;
		} else {
			step += 1;
			current = current->next_;
		}
	}

	return current->data_;
}

int JumpList::index(const string& s) const {
	if (head_ == nullptr) return -1;

	// moving along the fast lane
	Node* tmp = head_;
	int i = 0;

	while(tmp->jump_ != nullptr && tmp->jump_->data_ < s) {
		tmp = tmp->jump_;
		i += tmp->gap_;
	}

	// tmp now points to the jump node at the start of the segment where s could be

	// now slow lane
	while(tmp != nullptr) {
		if (tmp->data_ == s) return i; // match
		else if (tmp->data_ > s) return -1; // went past without finding s
		else tmp = tmp->next_;

		i++;
	}

	return -1; // end of list
}

string JumpList::print() const {
	string out = "";
	Node* current = head_;

	while (current != nullptr) {
		out.append(current->data_ + (current->next_ != nullptr ? " " : ""));
		current = current->next_;
	}

	out.append("\n");
	current = head_;

	while (current != nullptr) {
		out.append(current->data_ + (current->jump_ != nullptr ? " " : ""));
		current = current->jump_;
	}

	out.append("\n");
	current = head_;
	
	while (current != nullptr) {
		out.append(std::to_string(current->gap_) + (current->jump_ != nullptr ? " " : ""));
		current = current->jump_;
	}

	return out;
}

string JumpList::prettyPrint() const {
	string out_l1 = "";
	string out_l2 = "";
	string out_l3 = "";
	Node* current = head_;
	std::vector<std::pair<int, Node*>> nodePositions;

	while (current != nullptr) {
		if (current->gap_ != 0) nodePositions.push_back({out_l1.length(), current});

		out_l1.append(current->data_ + (current->next_ != nullptr ? " --> " : ""));
		current = current->next_;
	}

	for (int i = 0; i < nodePositions.size(); i++) {
		out_l2.append(
			nodePositions[i].second->data_ + ((nodePositions[i].second->jump_ != nullptr) ? (" " +
			std::string(nodePositions[i + 1].first - nodePositions[i].first - 4 - (nodePositions[i].second->data_.length() - 1), '-') +
			"> ") : "")
		);
	}

	for (int i = 0; i < nodePositions.size(); i++) {
		out_l3.append(
			std::to_string(nodePositions[i].second->gap_) + ((nodePositions[i].second->jump_ != nullptr) ?
			std::string(nodePositions[i + 1].first - nodePositions[i].first - 1, ' ') : "")
		);
	}

	return (out_l1 + "\n" + out_l2 + "\n" + out_l3);
}

bool JumpList::insert(const string& s) {
	Node* newNode = new Node(s, nullptr, nullptr, 0);
	Node* affectedJumpNode = this->head_;

	if (this->head_ == nullptr) {
		this->head_ = newNode;
		return true;
	}

	if (this->head_->data_ > s) { // insert before head
 		Node* oldJump = this->head_->jump_;
		int oldGap = this->head_->gap_;

		this->head_->jump_ = nullptr;
		this->head_->gap_ = 0;

		newNode->next_ = this->head_;
		newNode->jump_ = oldJump;
		newNode->gap_ = oldGap;

		this->head_ = newNode;
		affectedJumpNode = newNode;
	} else { // insert after head
		Node* tmp = head_;

		while (tmp->jump_ != nullptr && tmp->jump_->data_ < s) {
			affectedJumpNode = tmp->jump_;
			tmp = tmp->jump_;
		}

		while (tmp->next_ != nullptr && tmp->next_->data_ < s)
			tmp = tmp->next_;

		if (tmp->next_ != nullptr && tmp->next_->data_ == s) 
			return false;

		//tmp.next is new node, node.next is old tmp.next

		Node* oldNext = tmp->next_;

		tmp->next_ = newNode;
		newNode->next_ = oldNext;
	}

	affectedJumpNode->gap_ += 1;

	if (affectedJumpNode->gap_ > MAX_GAP_SIZE) {
		int firstHalf = std::ceil(affectedJumpNode->gap_ / 2.0);
		int secHalf = std::floor(affectedJumpNode->gap_ / 2.0);

		Node* newJumpNode = affectedJumpNode;

		for (int i = 0; i < firstHalf; i++)
			newJumpNode = newJumpNode->next_;

		Node* targetNode = newJumpNode;

		for (int i = 0; i < secHalf; i++)
			targetNode = targetNode->next_;

		affectedJumpNode->jump_ = newJumpNode;
		affectedJumpNode->gap_ = firstHalf;
		newJumpNode->jump_ = targetNode;
		newJumpNode->gap_ = secHalf;
	}

	return true;
}

bool JumpList::erase(const string& s) {
	// IMPLEMENT ME

	return false; // dummy
}