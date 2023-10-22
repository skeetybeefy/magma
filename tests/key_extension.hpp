#include "../magma.hpp"

int run_key_extension_tests() {
    string input = "ffeeddccbbaa99887766554433221100f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff";
    string outputs[32] = {
        "ffeeddcc",
        "bbaa9988",
        "77665544",
        "33221100",
        "f0f1f2f3",
        "f4f5f6f7",
        "f8f9fafb",
        "fcfdfeff",
        "ffeeddcc",
        "bbaa9988",
        "77665544",
        "33221100",
        "f0f1f2f3",
        "f4f5f6f7",
        "f8f9fafb",
        "fcfdfeff",
        "ffeeddcc",
        "bbaa9988",
        "77665544",
        "33221100",
        "f0f1f2f3",
        "f4f5f6f7",
        "f8f9fafb",
        "fcfdfeff",
        "fcfdfeff",
        "f8f9fafb",
        "f4f5f6f7",
        "f0f1f2f3",
        "33221100",
        "77665544",
        "bbaa9988",
        "ffeeddcc",
    };

    unsigned int failed_tests = 0;

    cout << endl;
    cout << "Key extension tests" << endl;

    for (unsigned short i = 0; i < 32; i++) {
        string round_key = convert_int_to_hex_string(get_round_key(i + 1, input), 8);
        if (round_key == outputs[i]) {
            cout << "Test " << i << " passed" << endl;
        } else {
            failed_tests++;
            cout << "Test " << i << " failed" << endl;
        }
    }

    cout << failed_tests << " tests failed" << endl;
    cout << endl;

    return failed_tests;
}