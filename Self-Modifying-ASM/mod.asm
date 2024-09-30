section .data
    hello:   db 'Hello world', 10
    helloLen:    equ $-hello

section .text
    global _start

_start:
    xor rax, rax
    call print
    call exit

print:
    mov rax, 1                  ; write syscall
    mov rdi, 1                  ; file descriptor stdout
    mov rsi, hello              ; addr of string
    mov rdx, helloLen           ; len of string
    syscall
    ret

exit:
    mov rax, 60                 ; exit syscall
    mov rdi, 0                  ; EXIT_SUCCESS
    syscall
    ret
