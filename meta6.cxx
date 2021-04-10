#include <iostream>
#include <iomanip>
#include <fstream>
#include <iterator>
#include <vector>

template<typename type_t>
std::vector<type_t> load_file(const char* name) {
  std::ifstream f(name, std::ios::binary);
  std::vector<type_t> data;
  std::copy(
    std::istreambuf_iterator<type_t>(f), 
    std::istreambuf_iterator<type_t>(),
    std::back_inserter(data)
  );
  return data;
}

@meta std::vector<char> blah = load_file<char>(__FILE__);

const char embed[] { blah[:]... };

// Also, @embed as a first class language feature.
const char embed2[] = @embed(char, __FILE__);

int main() {
  std::cout.write(embed, embed.length);
}
