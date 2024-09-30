section .data
    userMsg db 'Enter a name for a directory: '
    lenUserMsg equ $-userMsg

section .bss
    buffer resb 16

section .text
    global _start

_start:
    xor rax, rax
    ;; output msg in var userMsg
    mov rax, 1
    mov rdi, 1
    mov rsi, userMsg
    mov rdx, lenUserMsg
    ;; read from stdin (fd 0)
    syscall
    mov rax, 0
    mov rdi, 0
    mov rsi, buffer
    mov rdx, 16
    ;; output directory name
    syscall
    mov rax, 1
    mov rdi, 1
    mov rsi, buffer
    mov rdx, 16
    ;; if the length of the input string is, just exit
    cmp rsi, 0
    syscall
    mov rax, 83
    mov rdi, buffer
    mov rsi, 0777
    syscall
    mov rax, 3
    mov rdi, 0
    mov rax, 60
    mov rdi, 0
    syscall

strlen:
    xor rcx, rcx
    .loop:
