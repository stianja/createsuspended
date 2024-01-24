#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [arguments]\n", argv[0]);
        return 1;
    }

    // If we want to EBFE the program before creating suspended process
    // Execute custom assembly instructions (0x90 0x90 0xeb 0xfe 0x90 0x90)
    //
    // NOP
    // NOP
    // JMP-2 # Infinate loop
    // NOP
    // NOP
    // RETN
    //char assemblyCode[] = "\x90\x90\xeb\xfe\x90\x90\xc3";
    //LPVOID codeAddress = VirtualAlloc(NULL, sizeof(assemblyCode), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    //if (codeAddress == NULL) {
    //    perror("Error allocating memory for assembly code");
    //    return 1;
    //}
    //memcpy(codeAddress, assemblyCode, sizeof(assemblyCode));
    //((void (*)())codeAddress)();

    // Construct the command line string including arguments
    char commandLine[MAX_PATH * 2];
    snprintf(commandLine, sizeof(commandLine), "%s %s", argv[1], argc > 2 ? argv[2] : "");

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    si.dwFlags |= STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOW;

    if (!CreateProcess(NULL, commandLine, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi)) {
        perror("Error creating process");
        return 1;
    }

    // Waiting for Enter
    printf("Press Enter to resume the process...\n");
    getchar();
    printf("Resuming process\n");

    ResumeThread(pi.hThread);

    WaitForSingleObject(pi.hProcess, INFINITE);

    // Clean up
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    //VirtualFree(codeAddress, 0, MEM_RELEASE);

    return 0;
}

