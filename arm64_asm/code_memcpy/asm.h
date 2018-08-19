#define __bionic_asm_custom_entry(f)
#define __bionic_asm_custom_end(f)
#define __bionic_asm_function_type @function

#ifndef _ALIGN_TEXT
# define _ALIGN_TEXT .align 0
#endif

#define ENTRY(f) \
    .text; \
    .globl f; \
    _ALIGN_TEXT; \
    .type f, __bionic_asm_function_type; \
    f: \
    __bionic_asm_custom_entry(f); \
    .cfi_startproc \

#define END(f) \
    .cfi_endproc; \
    .size f, .-f; \
    __bionic_asm_custom_end(f) \

// 