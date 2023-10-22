#include "magma.hpp"
#include "helpers.hpp"

int main(int argc, char* argv[]) {
    const string ENCRYPTION = "encryption";
    const string DECRYPTION = "decryption";
    string key_file_path = "./key.txt";
    string input_file_path = "./input.txt";
    string output_file_path = "./output.txt";
    bool CHAIN_BLOCKS = false;
    bool PRINT_ROUND_RESULTS = false;
    string type = "";

    unsigned int arg_seeker = 0;

    arg_seeker = find_index(argv, argc, "-i");
    if (arg_seeker != -1) {
        input_file_path = argv[arg_seeker + 1];
    }

    arg_seeker = find_index(argv, argc, "-o");
    if (arg_seeker != -1) {
        output_file_path = argv[arg_seeker + 1];
    }

    arg_seeker = find_index(argv, argc, "-k");
    if (arg_seeker != -1) {
        key_file_path = argv[arg_seeker + 1];
    }

    arg_seeker = find_index(argv, argc, "--chain");
    if (arg_seeker != -1) {
        CHAIN_BLOCKS = true;
    }

    arg_seeker = find_index(argv, argc, "--verbose");
    if (arg_seeker != -1) {
        PRINT_ROUND_RESULTS = true;
    }

    arg_seeker = find_index(argv, argc, "-e");
    if (arg_seeker != -1) {
        type = ENCRYPTION;
    }

    arg_seeker = find_index(argv, argc, "-d");
    if (arg_seeker != -1) {
        type = DECRYPTION;
    }

    string key = read_key_from_file(key_file_path);

    if (type == ENCRYPTION) {
        encrypt_file(input_file_path, output_file_path, key, CHAIN_BLOCKS, PRINT_ROUND_RESULTS);
    } else if (type == DECRYPTION) {
        decrypt_file(input_file_path, output_file_path, key, CHAIN_BLOCKS, PRINT_ROUND_RESULTS);
    } else {
        cout << "Error: no mode selected!" << endl;
        cout << "Specify encryption/decryption mode with flags -e or -d" << endl;
    }
 
    return 0;
}