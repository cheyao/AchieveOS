;*************************  strcpy64.asm  ************************************
; Author:           Agner Fog
; Date created:     2008-07-19
; Last modified:    2011-07-01
; Description:
; Faster version of the standard strcpy function:
; char * A_strcpy(char * dest, const char * src);
; Copies zero-terminated string from src to dest, including terminating zero.
;
; Overriding standard function memcpy:
; The alias ?OVR_strcpy is changed to _strcpy in the object file if
; it is desired to override the standard library function strcpy.
;
; Optimization:
; Uses optimized functions A_strlen and A_memcpy. These functions allow
; calling without proper stack alignment.
;
; Copyright (c) 2011 GNU General Public License www.gnu.org/licenses
;******************************************************************************

default rel

global strcpy                 ; Function A_strcpy

; Imported from strlen64.asm
extern strlen

; Imported from memcpy64.asm
extern memcpy


SECTION .text  align=16

; extern "C" char * A_strcpy(char * dest, const char * src) {
;    return memcpy(dest, src, strlen(src)+1);
; }

; Function entry:
strcpy:
%define Rpar1   rdi                    ; function parameter 1
%define Rpar2   rsi                    ; function parameter 2
%define Rpar3   rdx                    ; function parameter 3

        push    Rpar1                  ; dest
        push    Rpar2                  ; src
        mov     Rpar1, Rpar2
        ; (A_strlen does not require stack alignment)
        call    strlen               ; length of src
        lea     Rpar3,[rax+1]          ; include terminating zero in length
        pop     Rpar2                  ; src
        pop     Rpar1                  ; dest
        jmp     memcpy               ; copy and return
;A_strcpy ENDP
