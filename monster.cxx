// https://quuxplusone.github.io/blog/2021/02/01/x-macros

#include <iostream>
#include <iomanip>
#include <type_traits>

template<typename type_t>
concept Enum = std::is_enum_v<type_t>;

template<typename type_t>
concept Attribute = __is_attribute(type_t);

template<Enum type_t>
const char* enum_to_name(type_t e) {
  switch(e) {
    @meta for enum(type_t e2 : type_t)
      case e2:
        return @enum_name(e2);
    default:
      return nullptr;
  }
}

template<Attribute attrib_t, Enum type_t>
bool test_attribute(type_t e) {
  switch(e) {
    @meta for enum(type_t e2 : type_t)
      case e2:
        return @enum_has_attribute(e2, attrib_t);
  }
}

template<Attribute attrib_t, Enum type_t>
auto get_attribute(type_t e) {
  switch(e) {
    @meta for enum(type_t e2 : type_t)
      case e2:
        return @enum_attribute(e2, attrib_t);
  }
}

namespace game {

enum class attack_t {
  hit,
  bite,
  grab,
  scare,
};

using imm_poison [[attribute]] = void;
using level      [[attribute]] = int;
using symbol     [[attribute]] = char;
using attack     [[attribute(attack_t::hit)]] = attack_t;

enum class monster_t {
  dwarf     [[.symbol='h', .level=2                            ]],
  kobold    [[.symbol='k', .level=2,                .imm_poison]],
  elf       [[.symbol='@', .level=3                            ]],
  centipede [[.symbol='c', .level=3, .attack=bite              ]],
  ghost     [[.symbol='g', .level=4, .attack=scare, .imm_poison]],
  orc       [[.symbol='o', .level=4, .attack=grab,  .imm_poison]]
};

bool is_immune_to_poison(monster_t m) {
  return test_attribute<imm_poison>(m);
}

attack_t get_attack(monster_t m) {
  return get_attribute<attack>(m);
}

}

int main() {
  using namespace game;

  monster_t monsters[] { @enum_values(monster_t)... };

  for(monster_t m : monsters) {
    // Print info about each monster.
    std::cout<< std::setw(10)<< enum_to_name(m)<< " ("<< 
      get_attribute<symbol>(m)<< "): "<< enum_to_name(get_attack(m));
    if(is_immune_to_poison(m))
      std::cout<< " immune";
    std::cout<< "\n";
  }
}