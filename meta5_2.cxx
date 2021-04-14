#include <iostream>

using alt_name [[attribute]] = const char*;
using tooltip [[attribute]] = const char*;

// template<typename enum_t>
// const char* enum_to_string(enum_t e) {
//   switch(e) {
//     @meta for enum(enum_t e2 : enum_t) {
//       case e2:
//         if constexpr(@enum_has_attribute(e2, alt_name))
//           return @enum_attribute(e2, alt_name);
//         else
//           return @enum_name(e2);
//     }
// 
//     default:
//       return nullptr;
//   }
// }

template<typename enum_t>
const char* enum_to_string(enum_t e) {
  switch(e) {
    @meta for enum(enum_t e2 : enum_t) {
      case e2:
        return @enum_has_attribute(e2, alt_name) ?? 
          @enum_attribute(e2, alt_name) :
          @enum_name(e2);
    }

    default:
      return nullptr;
  }
}

enum class shape_t {
  circle [[.alt_name="what this compiler is"]] = 100,
  ellipse [[.alt_name="squished circle"]] = 200,
  square,
  rectangle,
  octagon [[.alt_name="8-sided shape"]] = 300,
  trapezoid,
  rhombus,
};

int main() {
  // Create an array that holds each enumerator in shape_t.
  shape_t shapes[] { @enum_values(shape_t)... };

  // Print the array values and string names.
  std::cout<< (int)shapes[:]<< " - "<< enum_to_string(shapes[:])<< "\n" ...;
}
