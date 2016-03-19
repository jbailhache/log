
IGROUP 	group _text
DGROUP 	group const, _bss, _data

	Assume cs:IGROUP, ds:DGROUP, ss:DGROUP
;code SEGMENT
;	Assume cs:code, ds:code, ss:code
	public _in_mtl

	; org 100h
	; org 0
	; jmp start

CONST	segment word public 'CONST'
CONST 	ends

_BSS    segment word public 'BSS'
_BSS 	ends

_DATA	segment word public 'DATA'

;	buffer db 100h dup (?)

_DATA   ends

_TEXT	segment byte public 'CODE'

;start:  mov ax, offset buffer
;	push ax
;	call _in_mtl
;	pop ax
;	mov ax, 4C00h
;	int 21h

_in_mtl proc near
	push bp
	mov bp,sp

	mov bx, [bp+4]
	call in_car
	cmp al, 13h
	jne suite
fkey:	call in_car
suite:	mov al, 0
	mov [bx], al

rtn:	mov sp, bp
	pop bp
	ret

in_car:	call waitcar
	mov dx, 3F8h
	in al, dx
	mov [bx], al
	inc bx
	ret

waitcar:mov dx, 3FDh
wait0:	in al, dx
	cmp al, 60h
	je wait0
	ret

wait1:	in al, dx
	cmp al, 60h
	jne wait1

	endp
;code	ends
_text 	ends
	end
