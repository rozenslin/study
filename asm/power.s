# calc 2^3 + 5^2 = 33
    .section .data
msg:
    .ascii    "Hello, world!\n"   # our dear string
	len = . - msg                 # length of our dear string

    .section .text
    .globl _start
_start:
    pushl $3        # push args onto stack
    pushl $2
    call  power
    addl  $8,   %esp # restore stack
    pushl %eax       # save ret1 on stack

    pushl $2
    pushl $5
    call  power
    addl  $8,   %esp # restore stack
    popl  %ebx       # pop ret1 from stack, ret2 in eax
    addl  %eax, %ebx
    pushl %ebx

    movl    $len,   %edx           # third argument: message length
	movl    $msg,   %ecx           # second argument: pointer to message to write
	movl    $1, %ebx             # first argument: file handle (stdout)
	movl    $4, %eax             # system call number (sys_write)
	int     $0x80               # call kernel

    popl  %ebx
    movl  $1,   %eax
    int   $0x80

# do power calc
# input:
#   arg1: base
#   arg2: power
# output:
#   arg1^arg2
    .type power, @function
power:
    pushl %ebp           # save base ptr for old frame
    movl  %esp, %ebp     # set new base ptr for new frame
    subl  $4,   %esp     # alloc space for local var
    movl  8(%ebp), %eax  # get arg1
    movl  12(%ebp), %ecx # get arg2
    movl  %eax, -4(%ebp) # init var
    movl  %eax, %ebx     # init var

loop:
    cmpl $1, %ecx
    je loop_end
    decl %ecx
    movl -4(%ebp), %eax
    imull %ebx, %eax
    movl %eax, -4(%ebp)
    jmp loop

loop_end:
    movl -4(%ebp), %eax # ret val to eax
    # leave
    movl %ebp, %esp     # restore currrent frame start
    popl %ebp           # restore old frame, ebp to old addr, esp to ret addr
    ret
