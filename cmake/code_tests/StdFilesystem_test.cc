#if INCLUDE_EXPERIMENTAL
#include <experimental/filesystem>
namespace filesystem = std::experimental::filesystem;
#else
#include <filesystem>
namespace filesystem = std::filesystem;
#endif

int main() {
    filesystem::path path("/root");
    (void)path;
}
