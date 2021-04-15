#include <tuple>
#include <iostream>

void print(const auto&... objs) {  
  std::cout<< int...<< ": "<< objs<< "\n"...;
}

int main() {
  void* p = (void*)0xfeedface;
  auto tuple = std::make_tuple('a', 100, 1.618, "A string", p);
  print(0, 1, tuple..., 1000);
}