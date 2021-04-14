#include <iostream>

// template<typename... types>
// struct tuple {
//   @meta for(int i : sizeof...(types))
//     types...[i] @(i);
// };

template<typename... types>
struct tuple {
  types @(int...) ...;
};

int main() {
  typedef tuple<long, float, const char*> my_tuple;
  my_tuple obj { 10001, 3.14f, "Hello tuple" };

  std::cout<< @member_decl_strings(my_tuple)<< ": "<< obj...[:]<< "\n" ...;
}
