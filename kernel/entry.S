        # qemu -kernel loads the kernel at 0x80000000
        # and causes each hart (i.e. CPU) to jump there.
        # kernel.ld causes the following code to
        # be placed at 0x80000000.
.section .text
.global _entry
_entry:
        # set up a stack for C. 设置了栈
        # stack0 is declared in start.c,
        # with a 4096-byte stack per CPU.
        # sp = stack0 + (mhartid * 4096) ; 在start.c中定义了stack0，栈的大小位4096的字节，而在RISC-V中，栈顶在低地址，栈底在高地址，所以sp应该指向stack0的末尾
        ; 这个mhartid是啥不知道，应该是单位长度吧。
        la sp, stack0 ; 将stack0的地址装入x[sp]
        li a0, 1024*4 ; 将立即数装入a0, a0到a7都是函数参数
        csrr a1, mhartid ; 将控制状态寄存器mhartid写入a1
        addi a1, a1, 1 ; a1=a1+1
        mul a0, a0, a1 ; a0=a0*a1
        add sp, sp, a0 ; sp = sp*a0
        # jump to start() in start.c
        call start
spin:
        j spin
