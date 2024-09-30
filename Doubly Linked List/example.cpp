#include <iostream>
#include "DLinkedList.hpp"
#include "barcode.hpp"
#include <vector>

using namespace std;

int main() {
  DLinkedList<bar> v;
  v.AddTail(bar("First", 1));
  v.AddTail(bar("Seventh", 7));
  v.AddTail(bar("Fifth", 5));
  v.AddTail(bar("Third", 3));
  v.AddTail(bar("Fourth", 4));
  v.AddTail(bar("Second", 2));
  v.AddTail(bar("Sixth", 6));
  cout << "Before sorting: " << endl;
  for (size_t i = 0; i < v.size(); i++) {
    cout << v[i] << ", ";
  }
  cout << endl;
  cout << "After sorting: " << endl;
  v.RecurBubbleSort();
  for (size_t i = 0; i < v.size(); i++) {
    cout << v[i] << ", ";
  }
}
