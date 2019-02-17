;
; da1a_patelp3.asm
;
; Created: 2/16/2019 6:54:07 PM
; Author : patel
;

; PRACHI PATEL CPE 301 Design Assignment 1A

.include<m328pdef.inc>
.cseg
.ORG 0

		LDI R22, 0x10	; Load Immediate 0x10 into R22, this is the 8-bit number. 16 in decimal value.
		LDI R16, 0x2C	; Load Immediate 0x2C into R25, this is the higher 8-bits of 16-bit number
		LDI R17, 0x3A	; Load Immediate 0x3A into R24, this is the lower 8-bits of the 16-bit number
		LDI R24, 0x2C	; Load Immediate 0x2C into R16, this is the lower 8-bit of the 16-bit that won't change
		LDI R25, 0x3A	; Load Immediate 0x3A into R17, this is the lower 8-bit of the 16-bit that won't change
		DEC R22			; Decrementing once since the number is already multiplied with 1.
	
L1:						; Label L1
		LDI R18, 0x00	; Load 0x00 into R18
		LDI R19, 0x00	; Load 0x00 into R19
		LDI R21, 0x00	; Load 0x00 into R21
		LDI R23, 0x00	; Load 0x00 into R23

		ADD R16, R24	; R16 = R16 + R24 (R16 keeps value from previous loop)
		ADC R21, R18	; R21 = R21 + Carry from (R16 + R24) + R18 (0x00)

		ADD R17, R21	; R17 = R17 + R21 (R17 keeps value from previous loop)
		ADC R23, R18	; R23 = R23 + Carry from (R17 + R21) + R18 (0x00)

		ADD R17, R25	; R17 = R17 + R21
		ADC R20, R23	; R20 = 0x00 + Carry from R17 + R21 (R20 keeps value calculate in previous loop)

		ADD R18, R16	; R18 = R18 + R16 = R16 since R18 is 0
		ADD R19, R17	; R19 = R19 + R17 = R17 since R19 is 0

		DEC R22			; Decrement R22
		BRNE L1			; If R22 is not equal to 0, jump to label L1

END: JMP END			; Else stay here forever