.section .data
.section .text
	.global lntwo
	.type lntwo, @function
lntwo:
	mov 	$0, %rax
start:
	cmp 	$1, %rdi
	je 		end
	shr 	$1, %rdi
	inc 	%rax
	jmp 	start
end:
	ret
