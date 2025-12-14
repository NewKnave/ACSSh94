#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void err(const char *string) {
	printf("Error: %s\n", string);
	return;
}

void manual(void) {
	printf("ACSSh94 Manual\n");

	printf("\tValid key\n");
	printf("\t\t number 1 to 94 (example: 1, 10, or 11)\n");

	printf("\tOption\n");
	printf("\t\t-e\tEncrypt\n");
	printf("\t\t-d\tDecrypt\n");

	printf("\tUsage\n");
	printf("\t\t./ACSSh key --option file\n");

	printf("\tExample\n");
	printf("\t\t./ACSSh 11 -e sr.c\n");
	printf("\t\tExplanation: With a key number of 11, encrypt the \"sr.c\" file\n");
	printf("\n");
	printf("\t\t./ACSSh 11 -d sr.c\n");
	printf("\t\tExplanation: With a key number of 11, decrypt the \"sr.c\" file\n");

	return;
}

void encryption(FILE *File, const int Key, FILE *NewEncryptedFile) {
	int ASCII = 0;

	fseek(File, 0, SEEK_END);
	long size = ftell(File) - 1;
	fseek(File, 0, SEEK_SET);

	for(int i = 0; i < size; i++) {
		ASCII = (int)fgetc(File);
		ASCII = ASCII + Key;
		if(ASCII > 126) {
			ASCII = (ASCII % 126) + 31;
		}
		fputc((char)ASCII, NewEncryptedFile);
	}

	return;
}

void decryption(FILE *File, const int Key, FILE *NewDecryptedFile) {
	int ASCII = 0;

	fseek(File, 0, SEEK_END);
	long size = ftell(File);
	fseek(File, 0, SEEK_SET);

	for(int i = 0; i < size; i++) {
		ASCII = (int)fgetc(File);
		ASCII = ASCII - Key;
		if(ASCII < 32) {
			ASCII = ASCII + 95;
		}
		fputc((char)ASCII, NewDecryptedFile);
	}

	return;
}

int main(int argc, char *argv[]) {
	if(argc != 4) {
		manual();
		return 1;
	}

	const char *argFile = argv[3];
	FILE *FilePointer = fopen(argFile, "rb");
	if(FilePointer == NULL) {
		err("File not found");
		return 2;
	}

	int argKey = atoi(argv[1]);
	if(argKey < 1 || argKey > 94) {
		err("Invalid key");
		return 3;
	}

	const char *argOption = argv[2];
	if(strcmp(argOption, "-e") == 0) {
		char NewEncryptedFileName[256];
		snprintf(NewEncryptedFileName, sizeof(NewEncryptedFileName), "E-%s", argv[3]);
	
		FILE *EncryptFile = fopen(NewEncryptedFileName, "wb");
		if(EncryptFile == NULL) {
			err("Unable to create and/or open new file for encryption");
			return 4;
		}
		else {
			encryption(FilePointer, argKey, EncryptFile);
			fclose(EncryptFile);
			printf("Encrypted file \"%s\" created\n", NewEncryptedFileName);
		}
	}

	else if(strcmp(argOption, "-d") == 0) {
		char NewDecryptedFileName[256];
		snprintf(NewDecryptedFileName, sizeof(NewDecryptedFileName), "D-%s", argv[3]);

		FILE *DecryptFile = fopen(NewDecryptedFileName, "wb");
		if(DecryptFile == NULL) {
			err("Unable to create and/or open new file for decryption");
			return 5;
		}
		else {
			decryption(FilePointer, argKey, DecryptFile);
			fclose(DecryptFile);
			printf("Decrypted file \"%s\" created\n", NewDecryptedFileName);
		}
	}

	else {
		err("Invalid option");
		return 6;
	}

	fclose(FilePointer);
	return 0;
}
