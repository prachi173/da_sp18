;
; DA2B_ASM.asm
;
; Created: 3/9/2019 4:53:11 PM
; Author : patel
;


; Replace with your application code
.org 0
	jmp top
.org 0x02		;interrupt subroutine origin
	JMP EX0_ISR

top:
	LDI R20, HIGH(RAMEND)
	OUT SPH, R20
	LDI R20, LOW(RAMEND)
	OUT SPL, R20	;intialize the stack

	LDI R16, 0x00
	SBI DDRB, 2 ;set output port pb.2
	SBI PORTD, 2 ;set portd2 high
	LDI R20, 0x2 
	STS EICRA, R20 ;set interrupt request at falling edge
	OUT PORTB, R16 ;set all leds off
	LDI R20, 1<<INT0 ;set INT0 to enable and load into R20
	OUT EIMSK, R20 ;mask register INT0
	SEI ;enable interrupt
	RJMP top 

EX0_ISR: ;set interrupt functions
	LDI R16, 0x04 
	OUT PORTB, R16 ;output led3 or pb2
	;call delay subroutine
	rcall delay_1s
	rcall delay_100ms
	rcall delay_100ms
	rcall delay_50ms
	;set leds off again
	LDI R16, 0x00
	OUT PORTB, R16
	RETI ;exit interrupt function


	;DELAY SUBROUTINE
delay_100ms:
	push r17
	ldi r17, 100
l2:
	rcall delay_1ms
	dec r17
	brne l2
	pop r17
	ret

delay_50ms:
	push r22
	ldi r22, 50
l3:
	rcall delay_1ms
	dec r22
	brne l3
	pop r22
	ret

delay_1s:
	push r19
	ldi r19, 10
l4:
	rcall delay_100ms
	dec r19
	brne l4
	pop r19
	ret 

delay_1ms:
	push r16
	push r17
	ldi r16, 200
	ldi r17, 130
l1: 
	dec r16
	brne l1
	
l12:
	dec r17
	brne l12
	pop r16
	pop r17
	ret