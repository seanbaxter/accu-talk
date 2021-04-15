// https://quuxplusone.github.io/blog/2019/12/28/metaprogramming-n-ways

#include <type_traits>
#include <algorithm>
#include <iostream>

template<typename...> struct typelist_t;

namespace detail {
  template<typename list_t> 
  struct filter_and_sort_t = delete;

  // Sort by decreasing type size, and eliminate elements that are empty.
  // Push the *negative size* of non-empty types. This will effect 
  // a sort from largest to smallest. Stability is guaranteed because
  // the .second pair member will break ties and prefer the type that
  // originally came first.
  template<typename... list_t>
  struct filter_and_sort_t<typelist_t<list_t...> > {
    @meta std::pair<size_t, size_t> types[sizeof...(list_t)];
    @meta size_t count = 0;
    @meta+ for(size_t i : sizeof...(list_t)) {
      if(!std::is_empty_v<list_t...[i]>)
        types[count++] = std::make_pair(-sizeof(list_t...[i]), i);
    }
    @meta std::sort(types, types + count);

    typedef typelist_t<list_t...[types...[:count].second]...> result;
  };
}

template<typename list_t> 
using filter_and_sort_t = typename detail::filter_and_sort_t<list_t>::result;

struct Z { };

int main( ) {
  typedef filter_and_sort_t<
    // Filter and sort these types!
    typelist_t<int[2], Z, int[1], int[4], int[3]>
  > sorted;

  static_assert(std::is_same_v<
    sorted,
    typelist_t<int[4], int[3], int[2], int[1]>
  >);

  @meta std::cout<< @type_string(sorted, true)<< "\n";
}