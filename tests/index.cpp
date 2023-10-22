#include "t_transformations.hpp"
#include "g_transformations.hpp"
#include "key_extension.hpp"
#include "encryption.hpp"
#include "decryption.hpp"

int main() {
    unsigned int total_failed_tests = 0;
    total_failed_tests += run_t_transformations_tests();
    total_failed_tests += run_g_transformations_tests();
    total_failed_tests += run_key_extension_tests();
    total_failed_tests += run_encryption_tests();
    total_failed_tests += run_decryption_tests();

    cout << endl;
    cout << "In total " << total_failed_tests << " tests failed";
    return 0;
}