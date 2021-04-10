#include <cstdlib>
#include <cstdio>
#include <ctime>

// Use popen to make a system call and capture the output in a file handle.
// Make it inline to prevent it from being output by the backend.
inline int capture_call(const char* cmd, char* text, int len) {
  FILE* f = popen(cmd, "r");
  len = f ? fread(text, 1, len, f) : 0;
  pclose(f);
}

// Every time print_version is compiled, it runs "git rev-parse HEAD" to
// get the current commit hash.
void print_version() {
  // Make a format specifier to print the first 10 digits of the git hash.
  @meta const char* fmt =
    "--version:\n"
    "  Circle compiler\n"
    "  Written by Sean Baxter\n"
    "  Compiled %s"
    "  Build %d\n"
    "  Hash %.10s\n"
    "  Machine %s\n";

  // Get the time of compilation.
  @meta time_t t = time(0);
  @meta const char* time = asctime(gmtime(&t));

  // Retrieve the current commit hash. The hash is 40 digits long, and we
  // include space for null.
  @meta char hash[41];
  @meta capture_call("git rev-parse HEAD", hash, 41);

  @meta char uname[100];
  @meta capture_call("uname -srm", uname, 100);

  // Substitute into the format specifier.
  @meta char text[200];
  @meta sprintf(text, fmt, time, __circle_build__, hash, uname);
  
  // Convert text to a string literal, then print that.
  puts(@string(text));
}

int main() {
  print_version();
  return 0;
}
