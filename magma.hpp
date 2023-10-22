#include <algorithm>
#include <iostream>
#include <fstream>
#include <functional>
#include <filesystem>
#include <iomanip>
#include <bit>
#include <bitset>

using namespace std;

#ifndef magma_h
#define magma_h


char PADDING_CHARACTER = '0';
bool HEX_OUTPUT = true;
bool HEX_INPUT = true;

unsigned short bijective_transformations[8][16] = {
    {12, 4, 6, 2, 10, 5, 11, 9, 14, 8, 13, 7, 0, 3, 15, 1},
    {6, 8, 2, 3, 9, 10, 5, 12, 1, 14, 4, 7, 11, 13, 0, 15},
    {11, 3, 5, 8, 2, 15, 10, 13, 14, 1, 7, 4, 12, 9, 6, 0},
    {12, 8, 2, 1, 13, 4, 15, 6, 7, 0, 10, 5, 3, 14, 9, 11},
    {7, 15, 5, 10, 8, 1, 6, 13, 0, 9, 3, 14, 11, 4, 2, 12},
    {5, 13, 15, 6, 9, 2, 12, 10, 11, 7, 8, 1, 4, 3, 14, 0},
    {8, 14, 2, 5, 6, 9, 1, 12, 15, 4, 11, 0, 13, 10, 3, 7},
    {1, 7, 14, 13, 0, 5, 8, 3, 4, 15, 10, 6, 9, 12, 11, 2},
};

string transform_bijectively(string input) {
    string output = "";
    for (short i = 0; i < 8; i++) {
        string ai = input.substr(28 - (4 * i), 4);
        unsigned short ai_int = stoul(ai, 0, 2);
        unsigned short result = bijective_transformations[i][ai_int];
        output = bitset<4>(result).to_string() + output;
    }

    return output;
}

unsigned int get_round_key(unsigned short round, string key) {
    unsigned short round_key_start_index = ((round - 1) % 8) * 8;
    if (round > 24) {
        round_key_start_index = 56 - round_key_start_index;
    }
    return stoul(key.substr(round_key_start_index, 8), 0, 16);
}

string convert_int_to_hex_string(unsigned int input, unsigned short max_characters) {
    stringstream sstream;
    sstream << hex << input;
    string cast_input = sstream.str();
    while (cast_input.length() < max_characters) {
        cast_input = "0" + cast_input;
    }
    return cast_input;
}

string cipher_round(unsigned short round, string block, string key, bool is_last_round, bool display_round_blocks = false) {
    string a1 = block.substr(0, 8);
    string a0 = block.substr(8, 16);

    unsigned int ring_sum_result = stoul(a0, 0, 16) + get_round_key(round, key);
    string ring_sum = bitset<32>(ring_sum_result).to_string();
    string transform_result = transform_bijectively(ring_sum);
    unsigned int uint_transform_result = stoul(transform_result, 0, 2);
    unsigned int bit_shift_result = _rotl(uint_transform_result, 11);
    unsigned int xor_result = bit_shift_result ^ stoul(a1, 0, 16);

    string xor_result_str = convert_int_to_hex_string(xor_result, 8);

    string round_result = a0 + xor_result_str;
    if (is_last_round) {
        round_result = xor_result_str + a0;
    }
    if (display_round_blocks) {
        cout << "ROUND: " << round << " " << round_result << endl;
    }
    return round_result;
}

string decipher_round(unsigned short round, string block, string key, bool is_last_round, bool display_round_blocks = false) {
    return cipher_round(33 - round, block, key, is_last_round, display_round_blocks);
}

string encrypt_block(string plain_block, string key, bool display_round_blocks = false) {
    string current_block = plain_block;
    for (unsigned short round = 1; round < 32; round++) {
        current_block = cipher_round(round, current_block, key, false, display_round_blocks);
    }
    current_block = cipher_round(32, current_block, key, true, display_round_blocks);
    return current_block;
}

string decrypt_block(string encrypted_block, string key, bool display_round_blocks = false) {
    string current_block = encrypted_block;
    for (unsigned short round = 1; round < 32; round++) {
        current_block = decipher_round(round, current_block, key, false, display_round_blocks);
    }
    current_block = decipher_round(32, current_block, key, true, display_round_blocks);
    return current_block;
}

string get_text_block(ifstream& fin, char buffer[]) {
    unsigned short CHARS_COUNT = 8;

    if (HEX_INPUT) {
        CHARS_COUNT = 16;
    }

    for (unsigned short i = 0; i < CHARS_COUNT; i++) {
        buffer[i] = '\0';
    }
    fin.read(buffer, CHARS_COUNT);
    string text_block(buffer, CHARS_COUNT);
    text_block.erase(find(text_block.begin(), text_block.end(), '\0'), text_block.end());

    if (text_block.length() > 0) {
        while (text_block.length() < CHARS_COUNT) {
            text_block += PADDING_CHARACTER;
        }
    }

    return text_block;
}

string xor_two_hex_block_strings(string hex1, string hex2) {
    string hex1_left_half = hex1.substr(0, 8);
    string hex1_right_half = hex1.substr(8, 8);
    string hex2_left_half = hex2.substr(0, 8);
    string hex2_right_half = hex2.substr(8, 8);

    unsigned int uint_1 = stoul(hex1_left_half, 0, 16);
    unsigned int uint_2 = stoul(hex2_left_half, 0, 16);

    string result_left_half = convert_int_to_hex_string(uint_1 ^ uint_2, 8);

    uint_1 = stoul(hex1_right_half, 0, 16);
    uint_2 = stoul(hex2_right_half, 0, 16);

    string result_right_half = convert_int_to_hex_string(uint_1 ^ uint_2, 8);

    return result_left_half + result_right_half;
}

string convert_hex_string_block_to_text(string hex, unsigned short str_len = 16) {
    string output = "";
    for (unsigned short ptr = 0; ptr < str_len; ptr = ptr + 2) {
        output += static_cast<char>(stoul(hex.substr(ptr, 2), 0, 16));
    }
    return output;
}

string convert_text_to_hex_string_block(string text) {
    string hex_string_block = "";
    for (unsigned short index = 0; index < text.length(); index++) {
        hex_string_block += convert_int_to_hex_string(int(text[index]), 2);
    }
    return hex_string_block;
}

void reverse_file_in_blocks(string input_file_path, string output_file_path) {
    // input_file_path MUST NOT be equal to output_file_path
    ifstream fin(input_file_path);
    char input_buffer[16];

    if (filesystem::exists(output_file_path)) {
        filesystem::remove(output_file_path);
    }

    while (!fin.eof()) {
        string text_block = get_text_block(fin, input_buffer);

        if (text_block.length() == 0) {
            break;
        }

        // reverse transition buffer population
        if (filesystem::exists(output_file_path)) {
            uintmax_t filesize = filesystem::file_size(output_file_path);
            char* buffer_between_blocks = new char[filesize];
            ifstream rb_fin(output_file_path);
            rb_fin.read(buffer_between_blocks, filesize);
            buffer_between_blocks[filesize] = '\0';
            rb_fin.close();

            ofstream fout(output_file_path);
            fout << text_block;
            fout << buffer_between_blocks;
            fout.close();
        } else {
            ofstream fout(output_file_path);
            fout << text_block;
            fout.close();
        }
    }

    fin.close();
}

void encrypt_file(string input_file_path, string output_file_path, string key, bool with_chaining = true, bool display_round_blocks = false, function<string(string, string, bool)> func = encrypt_block) {
    ifstream fin(input_file_path);
    char input_buffer[16];
    ofstream fout(output_file_path);
    char output_buffer[16];

    string last_encrypted_block = "0000000000000000";


    while(!fin.eof()) {
        string text_block = get_text_block(fin, input_buffer);

        if (text_block.length() == 0) {
            break;
        } 

        string hex_string_block = text_block;

        if (HEX_INPUT == false) {
            hex_string_block = convert_text_to_hex_string_block(text_block);
        }

        string input = xor_two_hex_block_strings(hex_string_block, last_encrypted_block); // if chaining is off, xor with 0 doesnt change anything

        string encrypted_block = func(input, key, display_round_blocks);

        string output = encrypted_block;

        if (HEX_OUTPUT == false) {
            output = convert_hex_string_block_to_text(output);
        }

        for (unsigned short index = 0; index < output.length(); index++) {
            output_buffer[index] = output[index];
        }

        fout.write(output_buffer, output.length());

        if (with_chaining) {
            last_encrypted_block = encrypted_block;
        }
    }

    fin.close();
    fout.close();
}

void decrypt_file(string input_file_path, string output_file_path, string key, bool with_chaining = true, bool display_round_blocks = false) {
    reverse_file_in_blocks(input_file_path, "./.rb.txt");
    encrypt_file("./.rb.txt", "./.cb.txt", key, with_chaining, display_round_blocks, decrypt_block);
    reverse_file_in_blocks("./.cb.txt", output_file_path);

    filesystem::remove("./.rb.txt");
    filesystem::remove("./.cb.txt");
}

string read_key_from_file(string file_path) {
    ifstream fin(file_path);
    char buffer[64];

    fin.read(buffer, 64);
    string key(buffer, 64);
    fin.close();
    return key;
}

#endif
