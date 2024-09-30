#include <iostream>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

static int fire = 0;

static void
fireman()
{
    fire = 1;
}

std::string readFile(std::string const& file) 
{
    /* placeholder */
    std::ifstream is(file);
    if( !is.good() ){
        throw std::runtime_error("error: bad ifstream");
    }
    std::stringstream ss;
    ss << is.rdbuf();
	std::string m;
    // Remove ending line character '\n' or '\r\n'.
    std::getline(ss, m);
    return m;
} 

inline void*
align_back(void* ptr, std::size_t alignment) noexcept
{
    std::uintptr_t ptr_int = reinterpret_cast<std::uintptr_t>(ptr);
    std::size_t remainder = ptr_int % alignment;
    return static_cast<char*>(ptr) - remainder;
}

int main() {
    std::size_t page_size = sysconf(_SC_PAGESIZE);
    std::cout << "Page Size:\t" << page_size << " bytes" << std::endl;
    void* addr = (void*) main;
    std::cout << "Address:\t" << &addr << " " << std::endl;
    void* page_start = align_back(addr, page_size);
    std::cout << "Page Start:\t" << &page_start << std::endl;
    auto va_space = readFile("/proc/sys/kernel/randomize_va_space");
    std::cout << "status /proc/sys/kernel/randomize_va_space: \t" << va_space << std::endl;
    if (mprotect(page_start, page_size, PROT_READ | PROT_WRITE | PROT_EXEC) != 0) {
        std::cout << "mprotect failed" << std::endl;
    }
    else {
        std::cout << "set mem location to be writable." << std::endl;
    }
    /* Wacky stuff goes here. */
    void* mem = mmap(NULL, page_size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (mem == MAP_FAILED) {
        std::cerr << "mmap failed!" << std::endl;
        exit(1);
    }
    std::cout << "Modifying memory directly..." << std::endl;
    unsigned char code[] = {
        0xB8, 0x02, 0x00, 0x00, 0x00,  // mov eax, 5
        0x05, 0x07, 0x00, 0x00, 0x00,  // add eax, 3
        0xC3                           // ret
    };
    // unsigned char code[] = "\x31\xc0\x31\xdb\x31\xc9\x31\xd2\x50\x68\x6e\x2f\x73\x68\x68\x2f\x2f\x62\x69\x89\xe3\xb0\x0b\xcd\x80";
    memcpy(mem, code, sizeof(code));
    typedef int (*func_ptr_t)();
    func_ptr_t func_ptr = (func_ptr_t)mem;
    int result = func_ptr();
    std::cout << "Result of dynamic code execution: " << result << std::endl;
    munmap(mem, page_size);
    exit(0);
}
