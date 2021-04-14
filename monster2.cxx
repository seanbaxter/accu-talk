#include "json_loader.hxx"
#include <iostream>
#include <iomanip>

// Generic attribute stuff.
template<typename attrib_t, typename type_t>
auto get_attribute(type_t e) {
  switch(e) {
    @meta for enum(type_t e2 : type_t)
      case e2:
        return @enum_attribute(e2, attrib_t);
  }
}


// Monster types.
enum class immunity_t {
  poison  = 1,
  psychic = 2,
  fire    = 4,
  frost   = 8,
};

enum class attack_t {
  hit,
  grab,
  bite,
  scare,
};

namespace meta {

// monsters_t exactly models the JSON schema.
struct monster_data_t {
  std::string name;
  std::string symbol;
  int level;

  [[.bitfield=immunity_t]]
  uint immunity = 0;

  attack_t attack = attack_t::hit;
};
typedef std::vector<monster_data_t> monsters_t;

// Load the monster.json file at compile time
@meta monsters_t monsters = load_from_json<monsters_t>("monsters.json");
 
}

// Define monster attributes.
using symbol     [[attribute]] = char;
using level      [[attribute]] = int;
using immunities [[attribute]] = uint;
using attack     [[attribute]] = attack_t;

// Define the monsters enum.
enum monster_t {
  @meta for(const auto& m : meta::monsters) {
    // Attach mandatory info as C++ attributes.
    @(m.name) [[
      .symbol=m.symbol[0], 
      .level=m.level,
      .immunities=m.immunity,
      .attack_t=m.attack
    ]];

    @meta std::cout<< "Caught a "<< m.name<< "!\n";
  }
};

const char* get_name(monster_t monster) {
  return enum_to_string(monster);
}

char get_symbol(monster_t monster) {
  return get_attribute<symbol>(monster);
}

int get_level(monster_t monster) {
  return get_attribute<level>(monster);
}

attack_t get_attack(monster_t monster) {
  return get_attribute<attack_t>(monster);
}

bool test_immunity(monster_t monster, immunity_t immunity) {
  return get_attribute<immunities>(monster) & (uint)immunity;
}


int main() {
  // Fill an array with monsters.
  monster_t monsters[] { @enum_values(monster_t)... };

  // Report on each monster.
  for(monster_t m : monsters) {
    // Use get_attribute<attrib-name> to retrieve the symbol.
    // Use get_attack to retrieve the attack.
    std::cout<< std::setw(10)<< get_name(m)<< " ("<<
      get_symbol(m)<< "): level "<< get_level(m)<< ", "<< 
      enum_to_string(get_attack(m));

    // Check for each immunity with test_immunity.
    std::cout<< ", immunity:";
    @meta for enum(immunity_t immunity : immunity_t)
      if(test_immunity(m, immunity))
        std::cout<< " "<< enum_to_string(immunity);

    std::cout<< "\n";
  }

  return 0;
}
