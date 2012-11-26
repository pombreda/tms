#include <cstdlib>
#include <config.hpp>
int main() {
  for (;;) {
    if (kSystem == "windows32") {
      system(".\\server_app --db main.db --port 3030");
    } else {
      system("./server_app --db main.db --port 3030");
    }
  }
}
