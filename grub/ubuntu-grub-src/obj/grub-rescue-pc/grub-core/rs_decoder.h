	.file	"reed_solomon.c"
	.text
	.type	pol_evaluate, @function
pol_evaluate:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	movl	%eax, %esi
	movl	%ecx, %edi
	xorl	%ebx, %ebx
	xorl	%eax, %eax
.L2:
	testl	%edx, %edx
	js	.L11
	movzbl	(%esi,%edx), %ecx
	testb	%cl, %cl
	je	.L3
	movzbl	1049088(%ecx), %ecx
	xorb	1048576(%ebx,%ecx), %al
.L3:
	addl	%edi, %ebx
	cmpl	$254, %ebx
	jle	.L4
	subl	$255, %ebx
.L4:
	decl	%edx
	jmp	.L2
.L11:
	popl	%ebx
	popl	%esi
	popl	%edi
	popl	%ebp
	ret
	.size	pol_evaluate, .-pol_evaluate
	.type	gf_mul, @function
gf_mul:
	testb	%dl, %dl
	je	.L14
	testb	%al, %al
	je	.L14
	movzbl	%al, %eax
	movzbl	1049088(%eax), %ecx
	movzbl	%dl, %edx
	movzbl	1049088(%edx), %eax
	movb	1048576(%ecx,%eax), %al
	ret
.L14:
	xorl	%eax, %eax
	ret
	.size	gf_mul, .-gf_mul
	.type	gauss_solve.constprop.2, @function
gauss_solve.constprop.2:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$60, %esp
	movl	%eax, -40(%ebp)
	movl	%edx, -28(%ebp)
	movl	%ecx, -44(%ebp)
	xorl	%eax, %eax
.L16:
	cmpl	-40(%ebp), %eax
	jge	.L42
	movl	$-1, 1049344(,%eax,4)
	incl	%eax
	jmp	.L16
.L42:
	xorl	%eax, %eax
.L18:
	cmpl	-28(%ebp), %eax
	jge	.L43
	movl	-44(%ebp), %edi
	movb	$0, (%edi,%eax)
	incl	%eax
	jmp	.L18
.L43:
	movl	-28(%ebp), %eax
	incl	%eax
	movl	%eax, -36(%ebp)
	xorl	%ecx, %ecx
	movl	$0, -32(%ebp)
	movl	%eax, -60(%ebp)
.L20:
	movl	-32(%ebp), %edi
	cmpl	%edi, -40(%ebp)
	jle	.L44
	xorl	%ebx, %ebx
.L31:
	cmpl	%ebx, -28(%ebp)
	jle	.L21
	cmpb	$0, 1052672(%ebx,%ecx)
	jne	.L23
	incl	%ebx
	jmp	.L31
.L21:
	jne	.L23
.L30:
	incl	-32(%ebp)
	addl	-36(%ebp), %ecx
	jmp	.L20
.L23:
	movl	-32(%ebp), %eax
	movl	%ebx, 1049344(,%eax,4)
	movzbl	1052672(%ebx,%ecx), %eax
	movzbl	1049088(%eax), %eax
	movl	$255, %edx
	subl	%eax, %edx
	xorl	%esi, %esi
	leal	1052672(%ecx), %edi
	movl	%edi, -48(%ebp)
	movzbl	1048576(%edx), %eax
	movl	%eax, -52(%ebp)
.L24:
	cmpl	%esi, -28(%ebp)
	jl	.L45
	movl	%ecx, -56(%ebp)
	movl	-48(%ebp), %eax
	leal	(%eax,%esi), %edi
	movzbl	(%edi), %eax
	movl	-52(%ebp), %edx
	call	gf_mul
	movb	%al, (%edi)
	incl	%esi
	movl	-56(%ebp), %ecx
	jmp	.L24
.L45:
	movl	-32(%ebp), %eax
	incl	%eax
	movl	%eax, -48(%ebp)
	movl	-36(%ebp), %eax
	leal	(%ecx,%eax), %edi
.L26:
	movl	-48(%ebp), %edx
	cmpl	%edx, -40(%ebp)
	je	.L30
	xorl	%esi, %esi
	movzbl	1052672(%ebx,%edi), %eax
	movl	%eax, -52(%ebp)
	leal	1052672(%edi), %eax
	movl	%eax, -56(%ebp)
.L27:
	cmpl	%esi, -28(%ebp)
	jl	.L46
	movzbl	1052672(%esi,%ecx), %eax
	movl	%ecx, -64(%ebp)
	movl	-52(%ebp), %edx
	call	gf_mul
	movl	-56(%ebp), %edx
	xorb	%al, (%edx,%esi)
	incl	%esi
	movl	-64(%ebp), %ecx
	jmp	.L27
.L46:
	incl	-48(%ebp)
	addl	-60(%ebp), %edi
	jmp	.L26
.L44:
	movl	-40(%ebp), %esi
	decl	%esi
	movl	-28(%ebp), %eax
	notl	%eax
	movl	%eax, -40(%ebp)
	movl	-36(%ebp), %ebx
	imull	%esi, %ebx
	addl	-28(%ebp), %ebx
.L32:
	testl	%esi, %esi
	js	.L47
	movl	1049344(,%esi,4), %eax
	movl	%eax, -32(%ebp)
	incl	%eax
	je	.L33
	movl	%ebx, %eax
	subl	-28(%ebp), %eax
	movl	%eax, -36(%ebp)
	xorl	%edi, %edi
	xorl	%ecx, %ecx
.L34:
	cmpl	%ecx, -28(%ebp)
	jle	.L48
	movl	-44(%ebp), %eax
	movzbl	(%eax,%ecx), %edx
	movl	-36(%ebp), %eax
	movzbl	1052672(%ecx,%eax), %eax
	movl	%ecx, -48(%ebp)
	call	gf_mul
	xorl	%eax, %edi
	movl	-48(%ebp), %ecx
	incl	%ecx
	jmp	.L34
.L48:
	movl	%edi, %eax
	xorb	1052672(%ebx), %al
	movl	-44(%ebp), %edx
	movl	-32(%ebp), %edi
	movb	%al, (%edx,%edi)
.L33:
	decl	%esi
	addl	-40(%ebp), %ebx
	jmp	.L32
.L47:
	addl	$60, %esp
	popl	%ebx
	popl	%esi
	popl	%edi
	popl	%ebp
	ret
	.size	gauss_solve.constprop.2, .-gauss_solve.constprop.2
	.globl	grub_reed_solomon_recover
	.type	grub_reed_solomon_recover, @function
grub_reed_solomon_recover:
	endbr32
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$76, %esp
	movl	%eax, -64(%ebp)
	movl	%ecx, -36(%ebp)
	testl	%ecx, %ecx
	je	.L49
	movl	%edx, %edi
	addl	%edx, %eax
	movl	%eax, -52(%ebp)
	leal	-1(%eax,%ecx), %ecx
	movl	%ecx, %eax
.L51:
	cmpl	-52(%ebp), %eax
	jb	.L52
	cmpb	$0, (%eax)
	jne	.L52
	decl	%eax
	jmp	.L51
.L52:
	subl	%eax, %ecx
	movl	$2, %ebx
	movl	-36(%ebp), %eax
	cltd
	idivl	%ebx
	cmpl	%eax, %ecx
	jg	.L49
	movb	$0, 1049088
	movl	$1048576, %edx
	movb	$1, %al
.L57:
	movb	%al, (%edx)
	movb	%al, 255(%edx)
	movzbl	%al, %ecx
	movb	%dl, 1049088(%ecx)
	testb	%al, %al
	jns	.L55
	leal	(%ecx,%ecx), %eax
	xorl	$29, %eax
	jmp	.L56
.L55:
	sall	%eax
.L56:
	incl	%edx
	cmpl	$1048831, %edx
	jne	.L57
.L58:
	testl	%edi, %edi
	je	.L49
	movl	-36(%ebp), %eax
	leal	(%edi,%eax), %ecx
	cmpl	$102400, %ecx
	jbe	.L88
	imull	$200, %edi, %eax
	xorl	%edx, %edx
	divl	%ecx
	sall	$9, %eax
	movl	%eax, -68(%ebp)
	imull	$200, -36(%ebp), %eax
	xorl	%edx, %edx
	divl	%ecx
	sall	$9, %eax
	movl	%eax, -72(%ebp)
	jmp	.L59
.L88:
	movl	-36(%ebp), %eax
	movl	%eax, -72(%ebp)
	movl	%edi, -68(%ebp)
.L59:
	movl	-64(%ebp), %eax
	movl	%eax, -60(%ebp)
	movl	$511, -44(%ebp)
.L62:
	movl	-68(%ebp), %eax
	addl	-44(%ebp), %eax
	movl	%eax, -56(%ebp)
	movl	-72(%ebp), %eax
	addl	-44(%ebp), %eax
	movl	%eax, -48(%ebp)
	cmpl	$511, %eax
	jbe	.L60
	cmpl	$511, -56(%ebp)
	jbe	.L60
	movl	-56(%ebp), %eax
	shrl	$9, %eax
	movl	%eax, -40(%ebp)
	movl	-60(%ebp), %edx
	xorl	%eax, %eax
	jmp	.L61
.L99:
	movl	-48(%ebp), %eax
	shrl	$10, %eax
	movl	%eax, -32(%ebp)
	xorl	%ebx, %ebx
	movl	-40(%ebp), %eax
	movl	-28(%ebp), %esi
	leal	-1(%eax,%esi), %esi
.L67:
	movl	%ebx, %ecx
	movl	%esi, %edx
	movl	$1052160, %eax
	call	pol_evaluate
	movb	%al, 1051904(%ebx)
	incl	%ebx
	cmpl	%ebx, -28(%ebp)
	jg	.L67
	xorl	%eax, %eax
.L69:
	cmpb	$0, 1051904(%eax)
	jne	.L68
	incl	%eax
	cmpl	%eax, -28(%ebp)
	jg	.L69
	jne	.L68
.L70:
	movl	-60(%ebp), %edx
	xorl	%eax, %eax
.L72:
	movb	1052160(%eax), %cl
	movb	%cl, (%edx)
	incl	%eax
	addl	$512, %edx
	cmpl	%eax, -40(%ebp)
	jg	.L72
.L60:
	decl	-44(%ebp)
	incl	-60(%ebp)
	cmpl	$-1, -44(%ebp)
	jne	.L62
	movl	-56(%ebp), %ebx
	addl	%ebx, -64(%ebp)
	movl	-48(%ebp), %ebx
	addl	%ebx, -52(%ebp)
	subl	-56(%ebp), %edi
	subl	%ebx, -36(%ebp)
	jmp	.L58
.L61:
	cmpl	-40(%ebp), %eax
	jge	.L98
	movb	(%edx), %cl
	movb	%cl, 1052160(%eax)
	incl	%eax
	addl	$512, %edx
	jmp	.L61
.L98:
	movl	-48(%ebp), %eax
	shrl	$9, %eax
	movl	%eax, -28(%ebp)
	movl	-52(%ebp), %eax
	addl	$511, %eax
	subl	-44(%ebp), %eax
	xorl	%edx, %edx
.L65:
	cmpl	-28(%ebp), %edx
	jge	.L99
	movb	(%eax), %cl
	movl	-40(%ebp), %ebx
	movb	%cl, 1052160(%ebx,%edx)
	incl	%edx
	addl	$512, %eax
	jmp	.L65
.L68:
	movl	-32(%ebp), %eax
	leal	1(%eax), %esi
	xorl	%ecx, %ecx
	xorl	%edx, %edx
	jmp	.L71
.L73:
	movb	1051904(%edx,%eax), %bl
	movb	%bl, 1052672(%eax,%ecx)
	incl	%eax
	cmpl	-32(%ebp), %eax
	jle	.L73
	incl	%edx
	addl	%esi, %ecx
.L71:
	xorl	%eax, %eax
	cmpl	-32(%ebp), %edx
	jl	.L73
.L74:
	cmpl	%eax, -32(%ebp)
	jle	.L100
	movb	$0, 1050368(%eax)
	incl	%eax
	jmp	.L74
.L100:
	movl	$1050368, %ecx
	movl	-32(%ebp), %edx
	movl	%edx, %eax
	call	gauss_solve.constprop.2
	xorl	%esi, %esi
	xorl	%ebx, %ebx
	movl	-40(%ebp), %eax
	addl	-28(%ebp), %eax
	movl	%eax, -76(%ebp)
	decl	%eax
	movl	%eax, -84(%ebp)
.L76:
	movl	%ebx, -80(%ebp)
	cmpl	-76(%ebp), %esi
	jge	.L101
	movl	$255, %ecx
	subl	%esi, %ecx
	movl	-32(%ebp), %eax
	leal	-1(%eax), %edx
	movl	$1050368, %eax
	call	pol_evaluate
	cmpb	1048576(%esi), %al
	jne	.L77
	movb	%al, 1050624(%ebx)
	incl	%ebx
	movl	-84(%ebp), %eax
	subl	%esi, %eax
	movl	-80(%ebp), %edx
	movl	%eax, 1050880(,%edx,4)
.L77:
	incl	%esi
	jmp	.L76
.L101:
	xorl	%eax, %eax
.L79:
	cmpl	%eax, %ebx
	je	.L102
	movb	$1, 1052672(%eax)
	incl	%eax
	jmp	.L79
.L102:
	movb	1051904, %al
	movb	%al, 1052672(%ebx)
	leal	1(%ebx), %eax
	movl	%eax, -80(%ebp)
	leal	(%ebx,%ebx), %eax
	movl	%eax, -76(%ebp)
	movl	%ebx, -32(%ebp)
	movl	$1, %ecx
.L81:
	cmpl	%ecx, -28(%ebp)
	jle	.L83
	movl	-32(%ebp), %eax
	subl	%ebx, %eax
	movl	%eax, -84(%ebp)
	xorl	%esi, %esi
.L84:
	cmpl	%esi, %ebx
	je	.L103
	movl	%ecx, -88(%ebp)
	movl	-84(%ebp), %eax
	movzbl	1052672(%eax,%esi), %edx
	movzbl	1050624(%esi), %eax
	call	gf_mul
	movl	-32(%ebp), %ecx
	movb	%al, 1052673(%ecx,%esi)
	incl	%esi
	movl	-88(%ebp), %ecx
	jmp	.L84
.L103:
	movb	1051904(%ecx), %al
	movl	-76(%ebp), %esi
	movb	%al, 1052673(%esi)
	incl	%ecx
	movl	-80(%ebp), %esi
	addl	%esi, -76(%ebp)
	addl	%esi, -32(%ebp)
	jmp	.L81
.L83:
	movl	$1052416, %ecx
	movl	%ebx, %edx
	movl	-28(%ebp), %eax
	call	gauss_solve.constprop.2
	xorl	%eax, %eax
.L85:
	cmpl	%eax, %ebx
	je	.L70
	movl	1050880(,%eax,4), %edx
	movb	1052416(%eax), %cl
	xorb	%cl, 1052160(%edx)
	incl	%eax
	jmp	.L85
.L49:
	addl	$76, %esp
	popl	%ebx
	popl	%esi
	popl	%edi
	popl	%ebp
	ret
	.size	grub_reed_solomon_recover, .-grub_reed_solomon_recover
	.ident	"GCC: (Ubuntu 8.4.0-3ubuntu2) 8.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 4
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 4
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 4
4:
