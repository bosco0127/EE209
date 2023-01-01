	.file	"decomment.c"
	.section	.rodata
.LC0:
	.string	"%c%c"
	.align 8
.LC1:
	.string	"Error: line %d: unterminated comment\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	call	getchar@PLT
	movl	%eax, -16(%rbp)
	movl	$0, -12(%rbp)
	movl	$1, -8(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L2
.L26:
	movl	-4(%rbp), %eax
	cmpl	$1, %eax
	je	.L3
	cmpl	$1, %eax
	jb	.L4
	cmpl	$2, %eax
	je	.L5
	cmpl	$3, %eax
	je	.L6
	jmp	.L2
.L4:
	cmpl	$47, -16(%rbp)
	jne	.L7
	call	getchar@PLT
	movl	%eax, -16(%rbp)
	cmpl	$42, -16(%rbp)
	jne	.L8
	movl	$1, -4(%rbp)
	movl	-8(%rbp), %eax
	movl	%eax, -12(%rbp)
	movl	$32, %edi
	call	putchar@PLT
	call	getchar@PLT
	movl	%eax, -16(%rbp)
	jmp	.L2
.L8:
	movl	$47, %edi
	call	putchar@PLT
	jmp	.L2
.L7:
	cmpl	$39, -16(%rbp)
	jne	.L11
	movl	$39, %edi
	call	putchar@PLT
	movl	$2, -4(%rbp)
	call	getchar@PLT
	movl	%eax, -16(%rbp)
	jmp	.L2
.L11:
	cmpl	$34, -16(%rbp)
	jne	.L12
	movl	$34, %edi
	call	putchar@PLT
	movl	$3, -4(%rbp)
	call	getchar@PLT
	movl	%eax, -16(%rbp)
	jmp	.L2
.L12:
	cmpl	$10, -16(%rbp)
	jne	.L13
	addl	$1, -8(%rbp)
.L13:
	movl	-16(%rbp), %eax
	movl	%eax, %edi
	call	putchar@PLT
	call	getchar@PLT
	movl	%eax, -16(%rbp)
	jmp	.L2
.L5:
	cmpl	$92, -16(%rbp)
	jne	.L14
	call	getchar@PLT
	movl	%eax, -16(%rbp)
	movl	-16(%rbp), %eax
	movl	%eax, %edx
	movl	$92, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	call	getchar@PLT
	movl	%eax, -16(%rbp)
	jmp	.L2
.L14:
	cmpl	$39, -16(%rbp)
	jne	.L16
	movl	$39, %edi
	call	putchar@PLT
	movl	$0, -4(%rbp)
	call	getchar@PLT
	movl	%eax, -16(%rbp)
	jmp	.L2
.L16:
	cmpl	$10, -16(%rbp)
	jne	.L17
	addl	$1, -8(%rbp)
.L17:
	movl	-16(%rbp), %eax
	movl	%eax, %edi
	call	putchar@PLT
	call	getchar@PLT
	movl	%eax, -16(%rbp)
	jmp	.L2
.L6:
	cmpl	$92, -16(%rbp)
	jne	.L18
	call	getchar@PLT
	movl	%eax, -16(%rbp)
	movl	-16(%rbp), %eax
	movl	%eax, %edx
	movl	$92, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	call	getchar@PLT
	movl	%eax, -16(%rbp)
	jmp	.L2
.L18:
	cmpl	$34, -16(%rbp)
	jne	.L20
	movl	$34, %edi
	call	putchar@PLT
	movl	$0, -4(%rbp)
	call	getchar@PLT
	movl	%eax, -16(%rbp)
	jmp	.L2
.L20:
	cmpl	$10, -16(%rbp)
	jne	.L21
	addl	$1, -8(%rbp)
.L21:
	movl	-16(%rbp), %eax
	movl	%eax, %edi
	call	putchar@PLT
	call	getchar@PLT
	movl	%eax, -16(%rbp)
	jmp	.L2
.L3:
	cmpl	$10, -16(%rbp)
	jne	.L22
	addl	$1, -8(%rbp)
	movl	-16(%rbp), %eax
	movl	%eax, %edi
	call	putchar@PLT
	call	getchar@PLT
	movl	%eax, -16(%rbp)
	jmp	.L29
.L22:
	cmpl	$42, -16(%rbp)
	jne	.L24
	call	getchar@PLT
	movl	%eax, -16(%rbp)
	cmpl	$47, -16(%rbp)
	jne	.L2
	movl	$0, -4(%rbp)
	call	getchar@PLT
	movl	%eax, -16(%rbp)
	jmp	.L29
.L24:
	call	getchar@PLT
	movl	%eax, -16(%rbp)
	jmp	.L2
.L29:
	nop
.L2:
	cmpl	$-1, -16(%rbp)
	jne	.L26
	cmpl	$1, -4(%rbp)
	jne	.L27
	movq	stderr(%rip), %rax
	movl	-12(%rbp), %edx
	leaq	.LC1(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %eax
	jmp	.L28
.L27:
	movl	$0, %eax
.L28:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.5.0-1ubuntu2) 5.4.1 20171010"
	.section	.note.GNU-stack,"",@progbits
