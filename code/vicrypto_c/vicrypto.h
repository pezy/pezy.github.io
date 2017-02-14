#ifndef VICRYPTO_H
#define VICRYPTO_H

//! -----------------------------
//! System functions
//! -----------------------------

void GetLocalUserName(char *buf, unsigned int bufsize);
void GetLocalNodeName(char *buf, unsigned int bufsize);
void GetLocalMACAddress(char *buf);

//! -----------------------------
//! Crypto functions - (AES-CTR)
//! -----------------------------

#include <stddef.h>
#define AES_BLOCK_SIZE 16

void aes_key_setup(const char key[], int w[], int keysize);
void aes_encrypt(const char in[], char out[], const int key[], int keysize);
void aes_decrypt(const char in[], char out[], const int key[], int keysize);

void increment_iv(char iv[], int counter_size);
void aes_encrypt_ctr(const char in[], size_t in_len, char out[], const int key[], int keysize, const char iv[]);
void aes_decrypt_ctr(const char in[], size_t in_len, char out[], const int key[], int keysize, const char iv[]);

void Encrypt(char *plaintext, size_t len);
void Decrypt(char *ciphertext, size_t len);

//! -----------------------------
//! License and File Structure
//! -----------------------------

struct LicenseInfo
{
    char app_product_number[64];
    char username[16];
    char nodename[16];
    char MAC[18];
    int limit; // 0 - no time limit, 1 - have time limit.
    char start_time[9];
    char stop_time[9];
};

struct FileFormat
{
    short size;
    char lic[128];
};

//! -----------------------------
//! Call Interface
//! -----------------------------

static const char LICENSE_FILENAME[] = "resvip.lic";

void AddNodeToLicenseFile(const char* app_product_number, int nMonths);
int CheckCurrentNode(); // return 1 - success, 0 - failed.

#endif // VICRYPTO_H

