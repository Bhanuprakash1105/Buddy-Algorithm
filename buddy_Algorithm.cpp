#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
using namespace std;

typedef size_t li;

li MEMORY = 0;
struct node {
	li space, value; bool allocated;
	li startAddress, endAddress;
	node(li space): space(space), value(0), allocated(false) {}
	node(li space, li value): space(space), value(value), allocated(true) {}
};
vector<node*> m;

li give_fitter(li value) {
	if(value == 1) { return 2; }
	return pow(2, ceil( log(value) / log(2) ));
}

vector<node*> give_slots(li space, li fitter) {
	li number_of_slots = 2;
	if(space != fitter) { 
		number_of_slots = 1 + ( log(space / fitter) / log(2) );
	}
	vector<node*> v(number_of_slots, NULL);
	v[0] = new node(fitter);
	for(li i = 1, s = fitter; i < number_of_slots; ++i, s *= 2) { v[i] = new node(s); }
	return v;
}

bool allocate(li value) {
	if(m.size() == 0) { return false; }
	li fitter = give_fitter(value);
	for(li i = 0; i < m.size(); ++i) { if(m[i]->space == fitter && !m[i]->allocated) {
		m[i]->value = value; 
		m[i]->allocated = true;
		return true;
	} }
	for(auto itr = m.begin(); itr != m.end(); ++itr) {
		node *temp = *itr;
		if(!temp->allocated && temp->space > fitter) {
			auto slots = give_slots(temp->space, fitter);
			slots[0]->value = value;
			slots[0]->allocated = true;
			slots[0]->startAddress = temp->startAddress;
			slots[0]->endAddress = temp->startAddress + slots[0]->space;
			for(li i = 1; i < slots.size(); ++i) {
				slots[i]->startAddress = slots[i-1]->endAddress;
				slots[i]->endAddress = slots[i]->startAddress + slots[i]->space;
			}
			auto prev_itr = itr; --prev_itr;
			m.erase(itr);
			for(li i = 0; i < slots.size(); ++i) {
				prev_itr = m.insert(++prev_itr, slots[i]); 
			}
			slots.clear();
			return true;
		}
	}
	return false;
}

bool buddies(node *a, node *b) {
	li add = MEMORY*2;
	if(floor( (add + a->startAddress)/(2*a->space) ) == floor( (add + b->startAddress)/(2*b->space) )) {
		return true;
	}
	return false;
}

void check_merge() {
	auto itr = m.begin(), prev_itr = itr; ++itr;
	while(itr != m.end()) {
		auto prevNode = *prev_itr, currNode = *itr;
		if(prevNode->allocated || currNode->allocated || (prevNode->space != currNode->space) || !buddies(prevNode, currNode)) { 
			prev_itr = itr; ++itr; continue;
		}
		prevNode->space *= 2;
		prevNode->endAddress = prevNode->startAddress + prevNode->space;
		m.erase(itr);
		prev_itr = m.begin();
		itr = prev_itr; ++itr;
	}
}

bool deallocate(li address) {
	if(address >= MEMORY) { return false; }
	for(auto i: m) {
		if(address == i->startAddress && !i->allocated) { return false; }
		if(address == i->startAddress) {
			i->allocated = false;
			i->value = 0;
			check_merge();
			return true;
		}
	}
	return false;
}

void display() {
	for(auto i: m) {
		li if_ = 0; if(i->allocated) { if_ = i->space - i->value; }
		printf("> Start: %4zu  | SIZE: %4zu |  Value: %4zu  IF: %4zu  End: %4zu\n", 
			i->startAddress, i->space, i->value, if_, i->endAddress);
	}
}

int main(int argc, char const *argv[])
{
	cout <<"\n Amount of memory 2^x, if x = 10 then memory is 2^10 = 1024\n\n Enter the value of x = ";
	cin >> MEMORY;
	if(MEMORY >= 32) { cout <<"\n Memory value is huge (x >= 32), program terminated\n"<< endl; return 0; }
	
	MEMORY = (li)pow(2, MEMORY);
	m.emplace_back(new node(MEMORY));
	m[0]->startAddress = 0;
	m[0]->endAddress = m[0]->startAddress + m[0]->space;

	while(true) {
		int c; cout <<"\n Press: [1] Allocate  [2] Deallocate  [3] Exit -> "; cin >> c;	
		li value = 0;
		switch (c) {	
			case 1: cout <<"\n Enter the amount of memory to allocate (= Block Value): "; cin >> value;
					if(value == 0) { cout <<"\n Error: Zero(0) cannot be allocated"<< endl; break; }
					if(!allocate(value)) {
						cout <<"\n Error: Memory insufficient or space is not availabel"<< endl;
						break;
					}
					cout <<"\n THE BLOCKS IN MEMORY\n" << endl; display(); break;
			case 2: cout <<"\n Enter the start address of block to deallocate (Start): "; cin >> value;
					if(!deallocate(value)) {
						cout <<"\n Error: Block is not allocated at given address"<< endl;
						break;
					}
					cout <<"\n THE BLOCKS IN MEMORY\n" << endl; display(); break;
			case 3: cout <<"\n Program terminated\n"<< endl;
					return 0;
			default: cout <<"\n Error: Invalid input"<< endl; 
		}
	}
	return 0;
}