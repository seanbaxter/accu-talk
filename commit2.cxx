#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <string>

// Use popen to make a system call and capture the output in a file handle.
// Make it inline to prevent it from being output by the backend.
inline int capture_call(const char* cmd, char* text, int len) {
  FILE* f = popen(cmd, "r");
  len = f ? fread(text, 1, len, f) : 0;
  pclose(f);
}

inline std::string get_version_string() {
  // Make a format specifier to print the first 10 digits of the git hash.
  const char* fmt =
    "--version:\n"
    "  Circle compiler\n"
    "  Written by Sean Baxter\n"
    "  Compiled %s"
    "  Build %d\n"
    "  Hash %.10s\n"
    "  Machine %s\n";

  // Get the time of compilation.
  time_t t = time(0);
  const char* time = asctime(gmtime(&t));

  // Retrieve the current commit hash. The hash is 40 digits long, and we
  // include space for null.
  char hash[41];
  capture_call("git rev-parse HEAD", hash, 41);

  char uname[100];
  capture_call("uname -srm", uname, 100);

  // Substitute into the format specifier.
  char text[200];
  sprintf(text, fmt, time, __circle_build__, hash, uname);
  
  return text;
}

void print_version() {
  puts(@string(get_version_string()));
}

int main() {
  print_version();
  return 0;
}
