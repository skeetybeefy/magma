#include "../magma.hpp"

int run_encryption_tests() {
    string key = "ffeeddccbbaa99887766554433221100f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff";
    string input = "fedcba9876543210";
    string output = "4ee901e5c2d8ca3d";

    unsigned int failed_tests = 0;

    cout << endl;
    cout << "Encryption test" << endl;

    string result = encrypt_block(input, key);

    if (result != output) {
        failed_tests++;
        cout << "Test failed" << endl;
    }

    cout << failed_tests << " tests failed" << endl;
    cout << endl;

    return failed_tests;
}