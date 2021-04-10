#include <iostream>

template<typename enum_t>
const char* enum_to_string(enum_t e) {
  switch(e) {
    @meta for enum(enum_t e2 : enum_t) {
      case e2:
        // @enum_name may only be called on constant values, like e2, not
        // runtime values like e.
        return @enum_name(e2);
    }

    default:
      return nullptr;
  }
}

enum class shape_t {
  circle = 100,
  ellipse = 200,
  square,
  rectangle,
  octagon = 300,
  trapezoid,
  rhombus,
};

int main(int argc, char** argv) {
  // Create an array that holds each enumerator in shape_t.
  shape_t shapes[] { @enum_values(shape_t)... };

  // Print the array values and string names.
  std::cout<< (int)shapes[:]<< " - "<< enum_to_string(shapes[:])<< "\n" ...;
}
