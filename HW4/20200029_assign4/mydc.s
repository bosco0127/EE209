### --------------------------------------------------------------------
### mydc.s
###
### Desk Calculator (dc)
### --------------------------------------------------------------------

	.equ	ARRAYSIZE, 20
	.equ	EOF, -1
	.equ	POPONE, 4
	.equ	POPTWO, 8
	
.section ".rodata"

scanfFormat:
	.asciz "%s"

printfFormat:
	.asciz "%d\n"

stackempty:
	.asciz "dc: stack empty\n"

invalid:
	.asciz "invalid input\n"
### --------------------------------------------------------------------

        .section ".data"

### --------------------------------------------------------------------

        .section ".bss"
buffer:
        .skip  ARRAYSIZE

### --------------------------------------------------------------------

	.section ".text"

	## -------------------------------------------------------------
	## int main(void)
	## Runs desk calculator program.  Returns 0.
	## -------------------------------------------------------------

	.globl  main
	.type   main,@function

main:
	## dc number stack initialized. %esp = %ebp
	pushl   %ebp
	movl    %esp, %ebp

loop:

	## dc number stack initialized. %esp = %ebp
	
	## scanf("%s", buffer)
	pushl	$buffer
	pushl	$scanfFormat
	call    scanf
	addl    $POPTWO, %esp

	## check if user input EOF
	cmp	$EOF, %eax
	je	quit
	
	## PSEUDO-CODE
	## /*
	##  * In this pseudo-code we assume that you do not use no local variables
	##  * in the _main_ process stack. In case you want to allocate space for local
	##  * variables, please remember to update logic for 'empty dc stack' condition
	##  * (stack.peek() == NULL). 
	##  */
	##
        ##  while (1) {
        ##     /* read the current line into buffer */
        ##     if (scanf("%s", buffer) == EOF)
        ##         return 0;
        ## 
        ##     /* is this line a number? */
ifnumber:	
        ##     if (isdigit(buffer[0]) || buffer[0] == '_') {
	##	isdigit(buffer[0])
	movl $0, %eax
	movl $0, %ecx
	movb buffer(,%eax,1), %cl
	pushl %ecx
	call isdigit
	addl $POPONE, %esp
	## 	if it is digit, goto makenum.
	cmpl $0, %eax
	jne makenum
        ##        int num;
        ##        if (buffer[0] == '_') buffer[0] = '-';
	movl $0, %eax
	movb buffer(,%eax,1), %cl
	cmpb $'_', %cl
	jne endifnumber
	## 	buffer[0] = '-';
	movl $0, %eax
	movb $'-', buffer(,%eax,1)
        ##        num = atoi(buffer);
makenum:
	pushl $buffer
	call atoi
	addl $POPONE, %esp
        ##        stack.push(num);	/* pushl num */
	pushl %eax
        ##        continue;
	jmp loop
        ##     }
endifnumber:	
        ## 
        ##     /* p command */
ifp:	
        ##     if (buffer[0] == 'p') {
	movl $0, %eax
	movb buffer(,%eax,1), %cl
	cmpb $'p', %cl
	jne endifp	
        ##        if (stack.peek() == NULL) { /* is %esp == %ebp? */
ifpempty:
	cmpl %ebp, %esp
	jne ifpelse
        ##           printf("dc: stack empty\n");
	pushl $stackempty
	call printf
	addl $POPONE, %esp
        ##        }
	jmp endifpempty
ifpelse:	
	##	  else {
        ##           /* value is already pushed in the stack */
        ##           printf("%d\n", (int)stack.top());
	movl (%esp), %eax
	pushl %eax
	pushl $printfFormat
	call printf
	addl $POPTWO, %esp
        ##        }
endifpempty:	
        ##        continue;
	jmp loop
        ##     }
endifp:	
        ## 
        ##     /* q command */
ifq:	
        ##     if (buffer[0] == 'q') {
	movl $0, %eax
	movb buffer(,%eax,1), %cl
	cmpb $'q', %cl
	jne endifq
        ##        goto quit;
	jmp quit
        ##     }
endifq:	
        ##
        ##     /* + operation */
ifplus:	
        ##     if (buffer[0] == '+') {
	movl $0, %eax
	movb buffer(,%eax,1), %cl
	cmpb $'+', %cl
	jne endifplus
        ##        int a, b;
	##	a->%ebx, b->%edx
        ##        if (stack.peek() == NULL) {
ifplusempty1:
	cmpl %ebp, %esp
	jne endifplusempty1
        ##           printf("dc: stack empty\n");
	pushl $stackempty
	call printf
	addl $POPONE, %esp
        ##           continue;
	jmp loop
        ##         }
endifplusempty1:	
        ##         a = (int)stack.pop();
	movl (%esp), %ebx
	addl $POPONE, %esp
        ##         if (stack.peek() == NULL) {
ifplusempty2:
	cmpl %ebp, %esp
	jne endifplusempty2
        ##            printf("dc: stack empty\n");
	pushl $stackempty
	call printf
	addl $POPONE, %esp
        ##            stack.push(a); /* pushl some register value */
	pushl %ebx
        ##            continue;
	jmp loop
        ##         }
endifplusempty2:
        ##         b = (int)stack.pop(); /* popl to some register */
	movl (%esp), %edx
	addl $POPONE, %esp
        ##         res = b + a;
	addl %ebx, %edx
        ##         stack.push(res);
	pushl %edx
        ##         continue;
	jmp loop
        ##     }
endifplus:	
        ## 
        ##     /* - operation */
ifminus:
        ##     if (buffer[0] == '-') {
	movl $0, %eax
	movb buffer(,%eax,1), %cl
	cmpb $'-', %cl
	jne endifminus
        ##        int a, b;
	##	a->%ebx, b->%edx
        ##        if (stack.peek() == NULL) {
ifminusempty1:
	cmpl %ebp, %esp
	jne endifminusempty1
        ##           printf("dc: stack empty\n");
	pushl $stackempty
	call printf
	addl $POPONE, %esp
        ##           continue;
	jmp loop
        ##         }
endifminusempty1:
        ##         a = (int)stack.pop();
	movl (%esp), %ebx
	addl $POPONE, %esp
        ##         if (stack.peek() == NULL) {
ifminusempty2:
	cmpl %ebp, %esp
	jne endifminusempty2
        ##            printf("dc: stack empty\n");
	pushl $stackempty
	call printf
	addl $POPONE, %esp
        ##            stack.push(a); /* pushl some register value */
	pushl %ebx
        ##            continue;
	jmp loop
        ##         }
endifminusempty2:
        ##         b = (int)stack.pop(); /* popl to some register */
	movl (%esp), %edx
	addl $POPONE, %esp
        ##         res = b - a;
	subl %ebx, %edx
        ##         stack.push(res);
	pushl %edx
        ##         continue;
	jmp loop
        ##     }
	##
endifminus:
	##     /* * operation */
ifmul:
        ##     if (buffer[0] == '*') {
	movl $0, %eax
	movb buffer(,%eax,1), %cl
	cmpb $'*', %cl
	jne endifmul
        ##        int a, b;
	##	a->%ebx, b->%eax
        ##        if (stack.peek() == NULL) {
ifmulempty1:
	cmpl %ebp, %esp
	jne endifmulempty1
        ##           printf("dc: stack empty\n");
	pushl $stackempty
	call printf
	addl $POPONE, %esp
        ##           continue;
	jmp loop
        ##         }
endifmulempty1:
        ##         a = (int)stack.pop();
	movl (%esp), %ebx
	addl $POPONE, %esp
        ##         if (stack.peek() == NULL) {
ifmulempty2:
	cmpl %ebp, %esp
	jne endifmulempty2
        ##            printf("dc: stack empty\n");
	pushl $stackempty
	call printf
	addl $POPONE, %esp
        ##            stack.push(a); /* pushl some register value */
	pushl %ebx
        ##            continue;
	jmp loop
        ##         }
endifmulempty2:
        ##         b = (int)stack.pop(); /* popl to some register */
	movl (%esp), %eax
	addl $POPONE, %esp
        ##         res = b * a;
	imul %ebx
        ##         stack.push(res);
	pushl %eax
        ##         continue;
	jmp loop
        ##     }
endifmul:
	##
	##     /* / operation */
ifdiv:
        ##     if (buffer[0] == '/') {
	movl $0, %eax
	movb buffer(,%eax,1), %cl
	cmpb $'/', %cl
	jne endifdiv
        ##        int a, b;
	##	a->%ebx, b->%eax
        ##        if (stack.peek() == NULL) {
ifdivempty1:
	cmpl %ebp, %esp
	jne endifdivempty1
        ##           printf("dc: stack empty\n");
	pushl $stackempty
	call printf
	addl $POPONE, %esp
        ##           continue;
	jmp loop
        ##         }
endifdivempty1:	
        ##         a = (int)stack.pop();
	movl (%esp), %ebx
	addl $POPONE, %esp
        ##         if (stack.peek() == NULL) {
ifdivempty2:
	cmpl %ebp, %esp
	jne endifdivempty2
        ##            printf("dc: stack empty\n");
	pushl $stackempty
	call printf
	addl $POPONE, %esp
        ##            stack.push(a); /* pushl some register value */
	pushl %ebx
        ##            continue;
	jmp loop
        ##         }
endifdivempty2:	
        ##         b = (int)stack.pop(); /* popl to some register */
	movl (%esp), %eax
	addl $POPONE, %esp
	##	if %eax has negative value
	cmpl $0, %eax
	jge computediv
	##	change sign of the both value
	movl %eax, %edx
	movl $0, %eax
	subl %edx, %eax
	movl %ebx, %edx
	movl $0, %ebx
	subl %edx, %ebx
	##         res = b / a;
computediv:	
	movl $0, %edx
	idivl %ebx
        ##         stack.push(res);
	pushl %eax
        ##         continue;
	jmp loop
        ##     }
endifdiv:
        ## 
        ##     /* | operation */
ifabssum:
	##     if (buffer[0] == '|') {
	movl $0, %eax
	movb buffer(,%eax,1), %cl
	cmpb $'|', %cl
	jne endifabssum
        ##        int a, b;
        ##        if (stack.peek() == NULL) {
ifabssumdivempty1:
	cmpl %ebp, %esp
	jne endifabssumempty1
        ##           printf("dc: stack empty\n");
	pushl $stackempty
	call printf
	addl $POPONE, %esp
        ##           continue;
	jmp loop
        ##         }
endifabssumempty1:
        ##         a = (int)stack.pop();
	movl (%esp), %esi
	addl $POPONE, %esp
        ##         if (stack.peek() == NULL) {
ifabssumempty2:
	cmpl %ebp, %esp
	jne endifabssumempty2
        ##            printf("dc: stack empty\n");
	pushl $stackempty
	call printf
	addl $POPONE, %esp
        ##            stack.push(a); /* pushl some register value */
	pushl %esi
        ##            continue;
	jmp loop
        ##         }
endifabssumempty2:	
        ##         b = (int)stack.pop(); /* popl to some register */
	movl (%esp), %edx
	addl $POPONE, %esp
        ##         res = abssum(a,b);
	##	int abssum(int a, int b) {
	##	int i, s = 0;
	## goto abssumloop if edx < esi
	cmpl %esi, %edx
	movl $0, %ebx
	jle abssumloop
	## if not switch edx and esi
	movl %edx, %eax
	movl %esi, %edx
	movl %eax, %esi
	movl $0, %eax
	movl $0, %ebx
	##		for (i = min(a, b); i <= max(a,b); i++)
abssumloop:
	## if edx is negative value, change into positive value
	cmpl $0, %edx
	jge assignvalue
	movl $0, %eax
	subl %edx, %eax
	jmp abssumation
	##		s += |i|;
assignvalue:	
	movl %edx, %eax
abssumation:	
        addl %eax, %ebx
	incl %edx
	cmpl %esi, %edx
	jle abssumloop
	##		return s;
	##	}
endabssumloop:	
        ##         stack.push(res);
	pushl %ebx
        ##         continue;
	jmp loop
        ##     }
endifabssum:
        ## 
        ##     /* f operation  */
iff:
        ##     if (buffer[0] == 'f') {
	movl $0, %eax
	movb buffer(,%eax,1), %cl
	cmpb $'f', %cl
	jne endiff
	## initialize %ebx
	movl $0, %ebx
        ##     	while(%esp address == %ebp address) {
loopf:
	leal (%esp,%ebx,4), %ecx
	cmpl %ecx, %ebp
	je endloopf
	##	  printf("%d\n",current %esp value);
	movl (%ecx), %eax
	pushl %eax
	pushl $printfFormat
	call printf
	addl $POPTWO, %esp
	##	  increasse address of %esp;
	addl $1, %ebx
	##	goto loopf
	jmp loopf
	##	}
endloopf:
	##	continue:
	jmp loop
        ##     }
endiff:
        ##
	##     /* c operation  */
ifc:	
        ##     if (buffer[0] == 'c') {
	movl $0, %eax
	movb buffer(,%eax,1), %cl
	cmpb $'c', %cl
	jne endifc
        ##
	##  move %esp to %ebp
	movl %ebp, %esp
	##	continue:
	jmp loop
        ##     }
endifc:	
	##
	##     /* d operation  */
ifd:	
        ##     if (buffer[0] == 'd') {
	movl $0, %eax
	movb buffer(,%eax,1), %cl
	cmpb $'d', %cl
	jne endifd
	##      if (stack.peek() == NULL) { /* is %esp == %ebp? */
ifdempty:
	cmpl %ebp, %esp
	jne ifdelse
        ##           printf("dc: stack empty\n");
	pushl $stackempty
	call printf
	addl $POPONE, %esp
        ##        }
	jmp endifdempty
	##	else {
ifdelse:
	##	     int a;
        ##           a=(int)stack.top();
	movl (%esp), %eax
	pushl %eax 
        ##        }
endifdempty:	
	##	continue:
	jmp loop
        ##     }
endifd:	
	##
	##     /* r operation  */
ifr:	
        ##     if (buffer[0] == 'r') {
	movl $0, %eax
	movb buffer(,%eax,1), %cl
	cmpb $'r', %cl
	jne endifr
	##        int a, b;
	##	a->%ebx, b->%edx
        ##        if (stack.peek() == NULL) {
ifrempty1:
	cmpl %ebp, %esp
	jne endifrempty1
        ##           printf("dc: stack empty\n");
	pushl $stackempty
	call printf
	addl $POPONE, %esp
        ##           continue;
	jmp loop
        ##         }
endifrempty1:
        ##         a = (int)stack.pop();
	movl (%esp), %ebx
	addl $POPONE, %esp
        ##         if (stack.peek() == NULL) {
ifrempty2:
	cmpl %ebp, %esp
	jne endifrempty2
        ##            printf("dc: stack empty\n");
	pushl $stackempty
	call printf
	addl $POPONE, %esp
        ##            stack.push(a); /* pushl some register value */
	pushl %ebx
        ##            continue;
	jmp loop
        ##         }
endifrempty2:
        ##         b = (int)stack.pop(); /* popl to some register */
	movl (%esp), %edx
	addl $POPONE, %esp
        ##      int tmp;
	##	tmp = a;
	movl %ebx, %eax
	##	a = b;
	movl %edx, %ebx
	##	b = tmp;
	movl %eax, %edx
        ##         stack.push(b);
	pushl %edx
	##         stack.push(a);
	pushl %ebx
	##	continue:
	jmp loop
        ##     }
endifr:	
	##	else {
	##		printf("invalid input\n");
	pushl $invalid
	call printf
	addl $POPONE, %esp
	##		continue;
	jmp loop
	##	}
        ##   } /* end of while */
endloop:	

quit:	
	## return 0
	movl    $0, %eax
	movl    %ebp, %esp
	popl    %ebp
	ret
