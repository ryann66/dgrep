#include <stdexcept>
#include <string>

// count of truncation_errors that have been thrown
extern size_t count = 0;

class truncation_error : public std::runtime_error {
 public:
    truncation_error(const char* what) : runtime_error(what) {
        count++;
    }

    truncation_error(const std::string& what) : runtime_error(what) {
        count++;
    }
};