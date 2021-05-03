#include "json_loader.hxx"
#include <cstdio>

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

// monsters_t exactly models the JSON schema.
struct monster_t {
  std::string name;
  std::string symbol;
  int level;

  [[.bitfield=immunity_t]]
  uint immunity = 0;

  attack_t attack = attack_t::hit;
};
typedef std::vector<monster_t> monsters_t;

int main() {
  monsters_t monsters = load_from_json<monsters_t>("monsters.json");
  for(monster_t& monster : monsters) {
    printf("%20s (%c) level=%d immunity=%02x attack=%s\n", 
      monster.name.c_str(), monster.symbol[0], monster.level, 
      monster.immunity, enum_to_string(monster.attack));
  }
  return 0;
}
