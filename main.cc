#include <cstdlib>
#include <vector>
#include <set>
#include <cerrno>
#include <stdexcept>

#include "Tokens.h"
#include "Tokenizer.h"
#include "Parser.h"
#include "truncation_error.h"
#include "syntax_error.h"

size_t maxLength = 256;

using std::runtime_error;
using std::cout;
using std::endl;
using std::vector;
using std::set;

void usage(const char* pname) {
    cout << pname << " <regex>" << endl;
}

/**
 * Options
 * -a alphabet set
 * -l max length
 * -r regex
*/
int main(int argc, char** argv) {
    if (argc == 1) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    int i = 1;
    bool alph = false, len = false;
    while (i < argc && argv[i][0] == '-') {
        // option
        switch (argv[i][1]) {
            case 'a':
                if (alph) {
                    cout << "Redefining alphabet" << endl;
                    return EXIT_FAILURE;
                }
                try {
                    CharsetToken* newAlph;
                    if (argv[i][2] == '\0') {
                        i++;
                    } else {
                        argv[i] += 2;
                    }
                    newAlph = readCharset(const_cast<const char**>(&argv[i]));
                    if (argv[i][1] != '\0') {
                        cout << "Poorly formed alphabet" << endl;
                        return EXIT_FAILURE;
                    }
                    alphabet.swap(newAlph->chars);
                    if (alphabet.empty()) {
                        cout << "Error: empty alphabet" << endl;
                        return EXIT_FAILURE;
                    }
                } catch (runtime_error e) {
                    cout << "Poorly formed alphabet" << endl;
                    return EXIT_FAILURE;
                }
                alph = true;
                break;
            case 'l':
                if (len) {
                    cout << "Redefining max length" << endl;
                    return EXIT_FAILURE;
                }
                if (argv[i][2] == '\0') {
                    i++;
                } else {
                    argv[i] += 2;
                }
                char* endptr;
                errno = 0;
                maxLength = strtoull(argv[i], &endptr, 0);
                if (*endptr != '\0' || errno) {
                    cout << "Poorly formatted length" << endl;
                    return EXIT_FAILURE;
                }
                len = true;
                break;
            case 'r':
                if (argv[i][2] == '\0') {
                    i++;
                } else {
                    argv[i] += 2;
                }
                goto noopt;
            default:
                cout << "Unknown option: " << argv[i] << endl;
                return EXIT_FAILURE;
        }
        i++;
    }

 noopt:
    if (i + 1 != argc) {
        // Wrong number of parameters
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    // parse regex
    try {
        string regex(argv[i]);
        set<unsigned char> backrefs;
        vector<Token*> tokens(tokenize(regex, &backrefs));

        // TODO: reduce unused backrefs

        Node* root = parseTokens(tokens);
        for (Token* t : tokens) {
            delete t;
        }

        set<string> strs(root->evaluate());
        delete root;
        for (string s : strs) {
            cout << s << endl;
        }
    } catch (runtime_error e) {
        cout << "Error: " << e.what() << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}