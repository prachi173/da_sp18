;
; DA2A_1ctc.asm
;
; Created: 3/2/2019 6:48:27 PM
; Author : patel
;


; Replace with your application code
.org 0

	LDI R16, 0x04		;Loaded for the second bit of the PortB as required
	LDI R18, 0XFF		;Loaded to set all the PORTB to output
	OUT DDRB, R18		;Set all pins of PORTB to Output (connected to LEDs)
	LDI R17, 0			;Loaded 0
	OUT PORTB, R17		;Set all PORTB (led's) to 0
	LDI R20, 5			;Loaded 5 or 101
	STS TCCR1B, R20		;Set prescaler to 1024, which is done by assigning 101 to TCCR1B

begin:
	RCALL delay			;Calling label delay
	EOR R17, R16		;XOR R17 and R16. This is a bitmasking technique used to clear the output pins
	OUT PORTB, R17		;Output PORTB.2 (bitmask to 00000100 since PORTB is[7:0]
	RJMP begin			;Jump to label begin

delay:
	LDS R29, TCNT1H		;Load High bits of counter to R29
	LDS R28, TCNT1L		;Load Low bits of counter to R28
	CPI R28, 0X1E		;Compare the lower bit to 0x1E
	BRSH body			;Branch to label body if same or higher
	RJMP delay			;if lower, RJMP delay

body:
	CPI R29, 0x16		;Compare the higher bits to 0x16
	BRSH done			;Branch to label done if same or higher
	RJMP delay			;if lower, branch to label delay

done:
	LDI R20, 0x08		;Load 0x08 into R20
	STS TCNT1H, R20		;Load 0x08 into higher bits of counter
	LDI R20, 0xD9		;Load 0xD9 into R20
	STS TCNT1L, R20		;Load 0xD9 into lower bits of counter. This sets the counter to 0x08D9 which is 2265.
						;Since we want 60% DC, I divided them cycle into 435ms ON and 290ms OFF.
						;Using the prescaler, I calculate this times to 3397 and 2265 or 0x0D45 and 0x08D9
						;The time starts at this lower bit 0x08D9 and keeps LED ON until timer reaches 0x161E
						;Then the LED stays OFF for 290ms. The total period of this cycle is about 725ms.
	RET					;Returns to label begin

