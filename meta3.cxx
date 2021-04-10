#include <iostream>

template<typename... types>
struct tuple {
  @meta for(int i : sizeof...(types))
    types...[i] @(i);
};

int main() {
  typedef tuple<long, float, const char*> my_tuple;
  my_tuple obj { 10001, 3.14f, "Hello tuple" };

  std::cout<< "tuple:\n";
  std::cout<< "  "<< @member_decl_strings(my_tuple)<< ": "
    << obj.@member_values()<< "\n" ...;
}
