#ifndef BARCODE_H_
#define BARCODE_H_

#include <iostream>
#include <string>

struct bar {
  std::string name;
  int id;
    friend std::ostream& operator<< (std::ostream& os, const bar& rhs) {
      return os << rhs.name << ": " << rhs.id;
    }
  bar() {
    name = "";
    id = -1;
  }
  bar(std::string _name, int _id) :name(_name), id(_id) {
    return;
  } /* review:  */
};

bool operator> (const bar &lhs, const bar &rhs) {
  if (lhs.id > rhs.id) return true;
  return false;
}

bool operator>= (const bar &lhs, const bar &rhs) {
  if (lhs.id >= rhs.id) return true;
  return false;
}

bool operator< (const bar &lhs, const bar &rhs) {
  if (lhs.id <= rhs.id) return true;
  return false;
}

bool operator<= (const bar &lhs, const bar &rhs) {
  if (lhs.id <= rhs.id) return true;
  return false;
}





#endif // BARCODE_H_
