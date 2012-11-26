#include <cstdlib>
#include <config.hpp>
int main() {
  if (kSystem == "windows32") {
    system(".\\server_app --db main.db --init");
  } else {
    system("./server_app --db main.db --init");
  }
}
