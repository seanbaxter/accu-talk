#include "json.hpp"
#include <optional>
#include <iostream>
#include <iomanip>
#include <fstream>

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

template<Enum type_t>
std::optional<type_t> name_to_enum(const char* name) {
  @meta for enum(type_t e : type_t) {
    if(0 == strcmp(@enum_name(e), name)) 
      return e;
  }
  return { };
}

template<Attribute attrib_t, Enum type_t>
auto get_attribute(type_t e) {
  switch(e) {
    @meta for enum(type_t e2 : type_t)
      case e2:
        return @enum_attribute(e2, attrib_t);
  }
}

using symbol [[attribute]] = char;
using level [[attribute]] = int;

enum class attack_t {
  hit,
  bite,
  grab,
  scare,
};

enum class immunity_t {
  poison,
  frost,
  fire,
  psychic,
};

using nlohmann::json;

@meta json monsters_json;
@meta+ {
  std::ifstream i("monster.json");
  static_assert(i.is_open(), "Cannot open monster.json");

  i>> monsters_json;
  monsters_json = monsters_json["monsters"];
}

enum monster_t {
  @meta for(json& monster : monsters_json) {
    // Attach mandatory info as C++ attributes.
    @(monster["name"]) [[
      .symbol=((std::string)monster["symbol"])[0], 
      .level=(int)monster["level"]
    ]];
  }
};

@meta std::cout<< @enum_names(monster_t)<< "\n"...;

attack_t get_attack(monster_t monster) {
  switch(monster) {
    @meta for enum(monster_t m2 : monster_t) {
      @meta json& j = monsters_json[(int)m2];
      case m2:
        @meta if(j.contains("attack")) {
          @meta auto attack = name_to_enum<attack_t>(
            j["attack"].get<std::string>().c_str());
          static_assert(attack.has_value(), "unrecognized attack name");
          return *attack;

        } else {
          return attack_t::hit;
        }
    }
  }
}

template<immunity_t immunity>
bool test_immunity(monster_t monster) {
  switch(monster) {
    @meta for enum(monster_t m : monster_t) {
      @meta json& j = monsters_json[(int)m];
      case m:
        return (@meta j.contains("immunity") && 
          std::count(j["immunity"].begin(), j["immunity"].end(), @enum_name(immunity)));
    } 
  }
}

bool test_immunity(monster_t monster, immunity_t immunity) {
  switch(immunity) {
    @meta for enum(immunity_t i : immunity_t) {
      case i:
        return test_immunity<i>(monster);
    }
  }
}

int main() {
  monster_t monsters[] { @enum_values(monster_t)... };

  for(monster_t m : monsters) {
    std::cout<< std::setw(10)<< enum_to_name(m)<< " ("<<
      get_attribute<symbol>(m)<< "): "<< enum_to_name(get_attack(m));

    // Check for each immunity.
    std::cout<< ", immunity:";
    @meta for enum(immunity_t immunity : immunity_t)
      if(test_immunity<immunity>(m))
        std::cout<< " "<< enum_to_name(immunity);

    std::cout<< "\n";
  }
}
