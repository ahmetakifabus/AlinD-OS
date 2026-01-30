[bits 16]
[org 0x7c00]

KERNEL_OFFSET equ 0x1000

mov [BOOT_DRIVE], dl

mov bp, 0x9000
mov sp, bp

call load_kernel
call switch_to_pm

jmp $

%include "gdt.asm"

[bits 16]
load_kernel:
    mov bx, KERNEL_OFFSET
    mov dh, 15
    mov dl, [BOOT_DRIVE]
    call disk_load
    ret

[bits 16]
disk_load:
    push dx
    mov ah, 0x02
    mov al, dh
    mov ch, 0x00
    mov dh, 0x00
    mov cl, 0x02
    int 0x13
    jc disk_error
    pop dx
    cmp dh, al
    jne disk_error
    ret

disk_error:
    jmp $

[bits 16]
switch_to_pm:
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:init_pm

[bits 32]
init_pm:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ebp, 0x90000
    mov esp, ebp
    call BEGIN_PM

[bits 32]
BEGIN_PM:
    call KERNEL_OFFSET
    jmp $

BOOT_DRIVE db 0

times 510-($-$$) db 0
dw 0xaa55
