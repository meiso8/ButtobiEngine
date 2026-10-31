default	rel
%define XMMWORD
%define YMMWORD
%define ZMMWORD
section	.text code align=64


global	ossl_hwsm3_block_data_order

ossl_hwsm3_block_data_order:
DB	0x0f,0x0b
	DB	0F3h,0C3h		;repret

