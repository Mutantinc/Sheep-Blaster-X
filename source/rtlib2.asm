;		RTLIB2.ASM - small math run time library replacement
;		Written by: Ryg/FR, kb/FR - code
;
;		thanks guys i didn't have to do it...

bits        32

section		.data
einhalb		dd 0.5

section		.text

; ---- double sin(double a)

global      _sin@8
_sin@8:
	fld		qword [esp+4]
	fsin
	ret		8

; ---- double cos(double a)

global      _cos@8
_cos@8:
	fld		qword [esp+4]
	fcos
	ret		8

; ---- double asin(double a)

global      _asin@8
_asin@8:
  fld   qword [esp+4]
	fld		st0
	fabs
	fcom	dword [einhalb]
	fstsw	ax
	sahf
	jbe		.kleiner
	fld1
	fsubrp	st1, st0
	fld		st0
	fadd	st0
	fxch	st1
	fmul	st0
	fsubp	st1, st0
	jmp		short .end

.kleiner:
	fstp	st0
	fld		st0
	fmul	st0
	fld1
	fsubrp	st1, st0
	
.end:
	fsqrt
	fpatan
	ret   8


; ---- double acos(double a)

global      _acos@8
_acos@8:
	fld		qword [esp+4]
	fld1
	fchs
	fcomp	st1
	fstsw	ax
	je		.suckt

	fld		st0
	fld1
	fsubrp	st1, st0
	fxch	st1
	fld1
	faddp	st1, st0
	fdivp	st1, st0
	fsqrt
	fld1
	jmp		short .end

.suckt:
	fld1
	fldz

.end:
	fpatan
	fadd	st0, st0
	ret		8

; ---- double atan2(double y, double x)

global      _atan2@16
_atan2@16:
	fld		qword [esp+4]
	fld		qword [esp+12]
	fpatan
	ret		16
    
; ---- double atan(double a)

global		_atan@8
_atan@8:
	fld		qword [esp+4]
    fld1
    fpatan
    ret		8

; ---- double sqrt(double a)

global      _sqrt@8
_sqrt@8:
	fld		qword [esp+4]
	fsqrt
	ret		8

; ---- double pow(double a, double b)

global      _pow@16
_pow@16:
	fld	    qword [esp+12]
	fld		qword [esp+4]
	ftst
	fstsw	ax
	sahf
	jz		.zero

	fyl2x
	fld1
	fld		st1
	fprem
	f2xm1
	faddp	st1, st0
	fscale

.zero:
	fstp	st1
	ret		16
    
; ---- double tan(double a)

global		_tan@8
_tan@8:
	fld		qword [esp+4]
    fptan
    ret		8
    
; ---- float->int
global		__ftol
__ftol:
	sub		esp, byte 8
 	fistp	qword [esp]
 	pop		eax
 	pop		edx
 	ret


global __chkesp
__chkesp:
  ret
