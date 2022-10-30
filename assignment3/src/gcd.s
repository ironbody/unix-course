.section .data
.section .text
	.global gcd
	.type gcd, @function
gcd:
start:
	# rdi = a, rsi = b
	cmp 	%rsi, %rdi
	je		end
	jl		b_larger
	sub 	%rsi, %rdi
	jmp 	start
b_larger:
	sub 	%rdi, %rsi
	jmp 	start
end:
	mov 	%rdi, %rax
	ret
