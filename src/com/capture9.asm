
; IGROUP 	group _text
; DGROUP 	group const, _bss, _data

;	Assume cs:IGROUP, ds:DGROUP, ss:DGROUP
code SEGMENT
	org 100h
	Assume cs:code, ds:code, ss:code
	; public _in_mtl

	; org 100h
	; org 0
debut:	jmp start

poretat equ 3FDh
nodata	equ 60h
pordata	equ 3F8h

sizbuf	equ 100h

; CONST	segment word public 'CONST'
; CONST 	ends

; _BSS    segment word public 'BSS'
; _BSS 	ends

; _DATA	segment word public 'DATA'

stop	dw 0	; indicateur de fin de capture
tete 	dw (?)	; pointe sur le prochain caractere a enregistrer
queue 	dw (?)	; pointe sur le prochain emplacement libre pour le
		; prochain caractere lu
delai	dw 0123h
compt   dw (?)

fh 	dw (?)
n	dw (?)

mess1	db 'Fichier ? $'
mess2   db 13,10,'Erreur creation fichier.',13,10,'$'
mess3   db 13,10,'Erreur fermeture fichier.',13,10,'$'
mess4	db 13,10,'Erreur ecriture fichier.',13,10,'$'
mess5	db 13,10,'Fin de capture.',13,10,'$'
; mess6   db 13,10,'Ecriture',13,10,'$'
mess6	db '.$'
mess7	db 13,10,'Capture en cours $'

s_fname db 100
l_fname db 0
b_fname	db 100 dup (?)

ptrout  dw bufout
bufout  db sizbuf dup (?)
	db 0
buffer 	db 7000h dup (?)
endbuf  db (?)

; _DATA   ends

; _TEXT	segment byte public 'CODE'

start:
	mov stop, 0
	mov tete, offset buffer
	mov queue, offset buffer
	mov ptrout, offset bufout

	mov dx, offset mess1
	mov ah, 9
	int 21h

	mov dx, offset s_fname
	mov ah, 0Ah
	int 21h

	; mov bx, offset b_fname
	; add bx, l_fname
	mov bl, l_fname
	mov bh, 0
	add bx, offset b_fname
	mov byte ptr [bx], 0

	mov ax, 3C00h
	mov dx, offset b_fname
	mov cx, 0
	int 21h
	jnc crok

	mov dx, offset mess2
	mov ah, 9
	int 21h

	mov ah, 4Ch
	int 21h
crok:
	mov fh, ax

	mov dx, offset mess7
	mov ah, 9
	int 21h

	mov bx, tete
read:
	mov tete, bx
	mov bx, queue
loopr:
	mov ax, delai
	mov compt, ax
tkey:
	mov ah, 0Bh
	int 21h
	cmp al, 0ffh
				; caractere tape
	jne nokey
	mov stop, 1
	jmp store
nokey:
	mov dx, poretat
	in al, dx
	cmp al, nodata 		; si = pas de caractere disponible
	jne getcar
	cmp compt, 0
	je store		; on sauvegarde
	dec compt
	; jmp nokey
	jmp tkey
getcar:
	mov dx, pordata 	; on lit
	in al, dx
	mov [bx], al
	inc bx
	cmp bx, offset endbuf
	jne loopr
	mov bx, offset buffer
	cmp bx, tete
	jne loopr
				; buffer plein : on sauvegarde
	mov queue, bx
	jmp outcar

store:
	mov queue, bx
	mov bx, tete
loops:
	cmp stop, 1
	je suite

	mov dx, poretat
	in al, dx
	cmp al, nodata          ; y a-t-il un caractere disponible ?
	je suite

	mov tete, bx		; oui : on lit en priorite
	mov bx, queue
	jmp getcar

suite:
	cmp bx, queue           ; file vide ?
	jne outcar              ; non
	cmp stop, 1             ; oui : fin de capture ?
	jne read		; non : on lit

	mov dx, offset mess5
	mov ah, 9
	int 21h

	call save

	mov ax, 3E00h
	mov bx, fh
	int 21h
	jnc clok

	mov dx, offset mess3
	mov ah, 9
	int 21h

	mov ah, 4Ch
	int 21h
clok:
	mov ax, 4C00h		; oui : on sort
	int 21h
outcar:
	; mov dl, [bx]
				; on envoie sur la sortie
	; mov ah, 2
	; int 21h

	; inc bx
	; cmp bx, offset endbuf
	; jne loops
	; mov bx, offset buffer
	; jmp loops

	mov al, [bx]
	inc bx
	mov tete, bx
	mov bx, ptrout
	mov [bx], al
	inc bx
	mov ptrout, bx
	cmp bx, offset bufout + sizbuf
	jne nsave
	; push bx
	call save
	; pop bx
	; jmp loops
nsave:
	mov bx, tete
	; inc bx
	cmp bx, offset endbuf
	jne loops
	mov bx, offset buffer
	jmp loops
save:
	mov bx, fh
	mov cx, ptrout
	sub cx, offset bufout
	mov n, cx
	mov dx, offset bufout
	mov ax, 4000h
	int 21h
	cmp ax, n
	je wrok

	mov dx, offset mess4
	mov ah, 9
	int 21h
wrok:
	mov dx, offset mess6
	mov ah, 9
	int 21h

	mov ptrout, offset bufout
	ret

	mov dx, bx
	mov tete, bx
	mov bx, fh
	mov cx, sizbuf


	cmp bx, queue
	mov tete, bx
	mov bx, fh
	; ja cut

	mov cx, queue
	sub cx, tete
	mov n, cx
	mov ah, 40h
	int 21h
	cmp ax, n
	jne wrok

	mov dx, offset mess4
	mov ah, 9
	int 21h
; wrok:



	push ax
	call _in_mtl
	pop ax
	mov ax, 4C00h
	int 21h

_in_mtl:;proc near
	push bp
	mov bp,sp

	mov bx, [bp+4]
	call in_car
	cmp al, 13h
	jne suite1
fkey:	call in_car
suite1:	mov al, 0
	mov [bx], al

rtn:	mov sp, bp
	pop bp
	ret

in_car: ; [bx++] = caractere lu
	call waitcar
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
code	ends
	end debut
