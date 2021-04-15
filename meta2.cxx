#include <iostream>

template<typename... types_t>
struct foo {
  @meta std::cout<< @type_string(foo)<< ":\n";
  @meta std::cout<< "  "<< @type_string(types_t)<< "\n"...;
};

template<typename... types_t>
void bar() {
  @meta std::cout<< "bar():\n";
  @meta std::cout<< "  "<< @type_string(types_t)<< "\n"...;
}

int main() {
  // meta statements in templates are executed during instantiation.
  foo<char, short, int> obj1;
  foo<double[3], void*> obj2;

  bar<long, long long, __int128>();
  bar<std::istream&, float()>();
}
