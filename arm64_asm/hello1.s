.text //code section
.globl main
main:
	mov x0, 0     // stdout has file descriptor 0
	adrp x1, msg  // buffer to write
	add x1, x1, :lo12:msg
	mov x2, len   // size of buffer
	mov x8, 64    // sys_write() is at index 64 in kernel functions table
	svc #0        // generate kernel call sys_write(stdout, msg, len);
	
	mov x0, 123 // exit code
	mov x8, 93  // sys_exit() is at index 93 in kernel functions table
	svc #0      // generate kernel call sys_exit(123);

.data //data section
msg:
    .ascii      "Hello, ARM!\n"
len = . - msg
