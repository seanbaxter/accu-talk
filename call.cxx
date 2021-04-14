#include <iostream>
#include <string>
#include <tuple>
#include <variant>

template<typename arg_t>
static constexpr int call_count_v = 1;

template<typename arg_t>
static constexpr bool has_access_v = false;

template<int I, typename arg_t>
decltype(auto) extract_arg(arg_t&& x) {
  if constexpr(has_access_v<arg_t>) {
    return x.template access<I>();
  } else {
    static_assert(0 == I);
    return std::forward<arg_t>(x);
  }
}

// Runtime tuple access.
template<typename tuple_t>
struct access_tuple_t {
  tuple_t&& tuple;
  int index;

  int get_index() const noexcept { return index; }

  template<int I>
  decltype(auto) access() noexcept {
    return std::get<I>(std::forward<tuple_t>(tuple));
  }
};

template<typename tuple_t>
access_tuple_t<tuple_t> access_tuple(tuple_t&& tuple, int index) {
  return { std::forward<tuple_t>(tuple), index };
}

template<typename tuple_t>
static constexpr int call_count_v<access_tuple_t<tuple_t>> = 
  std::tuple_size_v<std::remove_reference_t<tuple_t>>;

template<typename tuple_t>
static constexpr bool has_access_v<access_tuple_t<tuple_t>> = true;

// Runtime variant access.
template<typename variant_t>
struct access_variant_t {
  variant_t&& variant;

  int get_index() const noexcept { return variant.index(); }

  template<int I>
  decltype(auto) access() noexcept {
    return std::get<I>(std::forward<variant_t>(variant));
  }
};

template<typename variant_t>
access_variant_t<variant_t> access_variant(variant_t&& variant) {
  return { std::forward<variant_t>(variant) };
}

template<typename variant_t>
static constexpr int call_count_v<access_variant_t<variant_t>> = 
  std::variant_size_v<std::remove_reference_t<variant_t>>;

template<typename variant_t>
static constexpr bool has_access_v<access_variant_t<variant_t>> = true;


// Dispatch call
template<typename func_t, typename... args_t>
void call(func_t func, args_t&&... args) {
  // Get an outer product of calls.
  constexpr int counts[] { call_count_v<decltype(args)>... };
  constexpr int outer_product = (... * counts);

  // Get the current index.
  int current = 0;
  @meta int adjustment = 1;
  @meta for(int i : sizeof...args) {
    if constexpr(has_access_v<args_t...[i]>) {
      current += args...[i].get_index() * adjustment;
      @meta adjustment *= counts[i];
    }
  }

  switch(current) {
    @meta for(int i : outer_product) {
      case i: {
        // Get the current index by chipping away at i.
        @meta int elements[sizeof...args];
        @meta int counter = i;
        @meta+ for(int i : sizeof...args) {
          elements[i] = counter % counts[i];
          counter /= counts[i];
        }

        func(extract_arg<elements...[:]>(std::forward<args_t>(args))...);
        break;
      }
    }
  }
}

int main() {

  auto output_values = []<typename... args_t>(const args_t&... args) {
    std::cout<< int...<< ": "<< args<< " ("<< @type_string(args_t)<< ")\n"...;
  };

  auto t1 = std::make_tuple('a', 2, "A string");
  auto t2 = std::make_tuple(3.14, std::string("Another string"), 100ll);

  std::variant<char, int, const char*> v1 = 'Q';
  std::variant<float, bool, std::string> v2 = "A variant";

  // The imperative visitor.
  call(output_values, 
    32,
    "Yo there",
    access_tuple(t1, 1),
    access_tuple(t2, 2),
    access_variant(v1),
    access_variant(v2)
  );
}