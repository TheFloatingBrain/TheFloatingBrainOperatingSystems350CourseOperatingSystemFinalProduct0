.global _GetKey

_GetKey:
;;;;;;;;;;;;;;;;;;;;;;;;;;;

mov ah, 0x0E
mov al, 0x52

int 0x10

;;;;;;;;;;;;;;;;;;;;;;;;;;;
ret
