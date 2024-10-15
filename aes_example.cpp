#include <iostream>
#include <fstream>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <iomanip>
#include <cstring>
// to compile : g++ -o aes_example aes_example.cpp -I/usr/lib/ssl -lssl -lcrypto
// run 
//  function to print data in hexadecimal
// after encrypt or decrypt data, the result is typically raw binary data,
// converting each byte into a two-digit hexadecimal representation.
void print_hex(const unsigned char *data, int length) {
    for (int i = 0; i < length; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)data[i];
    }
    std::cout << std::endl;
}

// Function to encrypt a file using AES in CBC mode
bool encrypt_file(const char *input_file, const char *output_file, const unsigned char *key) {
    // Generate a random IV
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        std::cerr << "Error generating random IV." << std::endl;
        return false;
    }

    // Initialize AES key structure
    AES_KEY encryptKey;
    AES_set_encrypt_key(key, 128, &encryptKey);

    std::ifstream in(input_file, std::ios::binary);
    std::ofstream out(output_file, std::ios::binary);

    if (!in || !out) {
        std::cerr << "Error opening files." << std::endl;
        return false;
    }

    // Write the IV to the beginning of the output file
    //The same IV is needed during decryption to get the original plaintext.
    out.write(reinterpret_cast<const char *>(iv), AES_BLOCK_SIZE);

    unsigned char buffer[AES_BLOCK_SIZE];
    unsigned char encrypted_buffer[AES_BLOCK_SIZE];
    int bytes_read;

    while ((bytes_read = in.readsome(reinterpret_cast<char *>(buffer), AES_BLOCK_SIZE)) > 0) {
        // Apply padding if this is the last block and it's not full
        if (bytes_read < AES_BLOCK_SIZE) {
            int padding = AES_BLOCK_SIZE - bytes_read;
            memset(buffer + bytes_read, padding, padding);
            bytes_read = AES_BLOCK_SIZE;
        }

        AES_cbc_encrypt(buffer, encrypted_buffer, bytes_read, &encryptKey, iv, AES_ENCRYPT);
        out.write(reinterpret_cast<const char *>(encrypted_buffer), bytes_read);
    }

    in.close();
    out.close();
    return true;
}

// Function to decrypt a file using AES in CBC mode
bool decrypt_file(const char *input_file, const char *output_file, const unsigned char *key) {
    std::ifstream in(input_file, std::ios::binary);
    std::ofstream out(output_file, std::ios::binary);

    if (!in || !out) {
        std::cerr << "Error opening files." << std::endl;
        return false;
    }

    // Read the IV from the beginning of the input file
    unsigned char iv[AES_BLOCK_SIZE];
    in.read(reinterpret_cast<char *>(iv), AES_BLOCK_SIZE);

    // Initialize AES key structure
    AES_KEY decryptKey;
    AES_set_decrypt_key(key, 128, &decryptKey);

    unsigned char buffer[AES_BLOCK_SIZE];
    unsigned char decrypted_buffer[AES_BLOCK_SIZE];
    int bytes_read;

    while ((bytes_read = in.readsome(reinterpret_cast<char *>(buffer), AES_BLOCK_SIZE)) > 0) {
        AES_cbc_encrypt(buffer, decrypted_buffer, bytes_read, &decryptKey, iv, AES_DECRYPT);

        // If this is the last block, remove the padding
        if (in.peek() == EOF) {
            int padding = decrypted_buffer[AES_BLOCK_SIZE - 1];
            bytes_read -= padding;
        }

        out.write(reinterpret_cast<const char *>(decrypted_buffer), bytes_read);
    }

    in.close();
    out.close();
    return true;
}

int main() {
    // Generate a random AES key
    unsigned char key[AES_BLOCK_SIZE];
    if (!RAND_bytes(key, sizeof(key))) {
        std::cerr << "Error generating random key." << std::endl;
        return 1;
    }

    const char *input_file = "input.txt";
    const char *encrypted_file = "encrypted.bin";
    const char *decrypted_file = "decrypted.txt";

    // Encrypt the file
    if (encrypt_file(input_file, encrypted_file, key)) {
        std::cout << "File encrypted successfully!" << std::endl;
    } else {
        std::cerr << "File encryption failed." << std::endl;
        return 1;
    }

    // Decrypt the file
    if (decrypt_file(encrypted_file, decrypted_file, key)) {
        std::cout << "File decrypted successfully!" << std::endl;
    } else {
        std::cerr << "File decryption failed." << std::endl;
        return 1;
    }

    return 0;
}
