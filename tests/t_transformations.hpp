#include "../magma.hpp"

int run_t_transformations_tests() {
    /* 
    Tested function in "transoform_bijectively",
    but it expects binary string as input 
    and outputs binary string,
    so convertions must be made
    */
    string inputs[4] = {
        "fdb97531",
        "2a196f34",
        "ebd9f03a",
        "b039bb3d",
    };

    string outputs[4] = {
        "2a196f34",
        "ebd9f03a",
        "b039bb3d",
        "68695433",
    };

    int failed_tests = 0;

    cout << endl;
    cout << "T tranformations tests" << endl;

    for (unsigned short i = 0; i < 4; i++) {
        unsigned int input_uint = stoul(inputs[i], 0, 16);
        string input_str = bitset<32>(input_uint).to_string();

        string result = transform_bijectively(input_str);

        unsigned int result_uint = stoul(result, 0, 2);
        string result_str = convert_int_to_hex_string(result_uint, 8);
        
        if (result_str == outputs[i]) {
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
