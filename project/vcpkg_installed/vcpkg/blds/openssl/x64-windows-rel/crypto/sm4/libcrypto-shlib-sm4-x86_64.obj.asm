
default	rel
%define XMMWORD
%define YMMWORD
%define ZMMWORD
section	.text code align=64


global	hw_x86_64_sm4_set_key

hw_x86_64_sm4_set_key:
DB	0x0f,0x0b
	DB	0F3h,0C3h		;repret


global	hw_x86_64_sm4_encrypt

hw_x86_64_sm4_encrypt:
DB	0x0f,0x0b
	DB	0F3h,0C3h		;repret


global	hw_x86_64_sm4_decrypt

hw_x86_64_sm4_decrypt:
DB	0x0f,0x0b
	DB	0F3h,0C3h		;repret

