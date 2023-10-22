#include "../magma.hpp"

int run_g_transformations_tests() {
    string inputs[4][2] = {
        {"87654321", "fedcba98"},
        {"fdcbc20c", "87654321"},
        {"7e791a4b", "fdcbc20c"},
        {"c76549ec", "7e791a4b"},
    };

    string outputs[4] = {
        "fdcbc20c",
        "7e791a4b",
        "c76549ec",
        "9791c849",
    };

    int failed_tests = 0;

    cout << endl;
    cout << "G tranformations tests" << endl;

    for (unsigned int i = 0; i < 4; i++) {
        unsigned int ring_sum_result = stoul(inputs[i][0], 0, 16) + stoul(inputs[i][1], 0, 16);
        string ring_sum = bitset<32>(ring_sum_result).to_string();
        string transform_result = transform_bijectively(ring_sum);
        unsigned int uint_transform_result = stoul(transform_result, 0, 2);
        unsigned int bit_shift_result = _rotl(uint_transform_result, 11);
        string result = convert_int_to_hex_string(bit_shift_result, 8);

        if (result == outputs[i]) {
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
