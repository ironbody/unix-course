.section .data
.section .text
	.global fact
	.type fact, @function
fact:
	mov $1, %rax
start:
	cmp 	$1, %rdi
	je 		end
	imul 	%rdi, %rax
	dec 	%rdi
	jmp 	start
end:
	ret
