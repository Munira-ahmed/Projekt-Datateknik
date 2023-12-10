 # timetemplate.asm
  # Written 2015 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

.macro	PUSH (%reg)
	addi	$sp,$sp,-4
	sw	%reg,0($sp)
.end_macro

.macro	POP (%reg)
	lw	%reg,0($sp)
	addi	$sp,$sp,4
.end_macro
        .glo
	.data
	
	.align 2
mytime:	.word 0x5957
timstr:	.ascii "text more text lots of text\0"
	.text
main:
	# print timstr
	la	$a0,timstr
	li	$v0,4
	syscall
	nop
	# wait a little
	li	$a0,2
	jal	delay
	nop
	# call tick
	la	$a0,mytime
	jal	tick
	nop
	# call your function time2string
	la	$a0,timstr
	la	$t0,mytime
	lw	$a1,0($t0)
	jal	time2string
	nop
	# print a newline
	li	$a0,10
	li	$v0,11
	syscall
	nop
	# go back and do it all again
	j	main
	nop
# tick: update time pointed to by $a0
tick:	lw	$t0,0($a0)	# get time
	addiu	$t0,$t0,1	# increase
	andi	$t1,$t0,0xf	# check lowest digit
	sltiu	$t2,$t1,0xa	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x6	# adjust lowest digit
	andi	$t1,$t0,0xf0	# check next digit
	sltiu	$t2,$t1,0x60	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa0	# adjust digit
	andi	$t1,$t0,0xf00	# check minute digit
	sltiu	$t2,$t1,0xa00	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x600	# adjust digit
	andi	$t1,$t0,0xf000	# check last digit
	sltiu	$t2,$t1,0x6000	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa000	# adjust last digit
tiend:	sw	$t0,0($a0)	# save updated result
	jr	$ra		# return
	nop

       # you can write your code for subroutine "hexasc" below this line
time2string:
       # When a subroutine returns, registers $s0–$s7, $gp, $sp,
       # $fp, and $ra must have the same contents as when the subroutine was called.
       #PUSH ($a0) och a1 för att jag ändå använder dom 
       PUSH ($s1)
       PUSH ($s0)
       PUSH ($ra)
        move $s0,$a0 # flytta a0 och a1 till s0 respektive s1.
        move $s1,$a1 
        # Använd temporära register till att omvandla en siffra i taget
        andi $t0,$s1,0xf000  # börja med timmar. ignorera allt anna t
        srl $a0,$t0,12 # lägg första minut siffran i a0
        jal hexasc                 # omvandla till ascii
        nop 
        sb $v0, 0($s0) #siffran i ascii från haxasc finns i return värdet vo. s0 är adressen a0 pekade på tidigare. spara med store byte
       
  
  # Använd ny temporär register 
       andi $t1,$s1,0x0f00 #ignorera allt förutom andra tim-siffran
       srl $a0,$t1,8 #shift höger tills siffran ligger i LSB
       jal hexasc #omvandla till ascii
       nop
       sb $v0,1($s0) #spara byte i a0(s0 nu) adressen. offset 1 (Byte 1 i minnet)
       

       li $t3,0x3A #Lägg till colon till adressen.
       sb $t3,2($s0)


       andi $t2,$s1,0x00f0      # Fortsätt med minuterna
       srl $a0,$t2,4
       jal hexasc
       nop
       sb $v0,3($s0)
       move $t5, $v0  #spara ascii värdet på första sekunden

      move $a0,$s1
      jal hexasc
      nop
      sb $v0, 4($s0)
      move $t6, $v0  #spara ascii värdet på andra sekunden
      
      beq $t5, $t6, zero
      nop
      
      j ending
      nop
 zero:
    beq $t5, 0x30, ding # om de är noll gå till ending
    nop
    
    j ending # annars gå till slutet
    nop
 ding:
   li $t0, 0x44
   li $t1, 0x49
   li $t2, 0x4E
   li $t3, 0x47
   sb $t0, 0($s0)
   sb $t1, 1($s0)
   sb $t2, 2($s0)
   sb $t3, 3($s0)
   li $t4,0x00 # a null \0 in ASCII table
   sb $t4,4($s0)

ending:
      li $t4,0x00 # a null \0 in ASCII table
      sb $t4,5($s0)

      POP ($ra)
      POP ($s0)
      POP ($s1)
 
       jr $ra
       nop
       lw
hexasc:
     and $a0, $a0, 0xf #ignorera allt som inte är de fyra första bitarna
      
     ble $a0, 0x9, else
     nop
     
     add $a0, $a0, 0x37
     j end  # hoppa till slutet
     nop
 else:
   add $a0,$a0, 0x30 # om a0 är större eller lika med tio. addera 30
   
 end:
  move $v0, $a0
  jr $ra
  nop    
  
delay:

   #addi $a0, $0, 1000 # ms = 1000
   addi $t1, $0, 4711 # length = 4711 . vidare till while loopen

while:
    addi $t0, $0, 0 # i = 0
    bgt $0, $a0, done # om den är 0?
    nop 
    
    beq $a0, 0, done
    nop
    
    addi $a0, $a0, -1 # minska ms med 1
    
    j for
    nop
   
for:
    beq $t0,$t1, while
    nop
    addi $t0, $t0, 1
    
    j for
    nop 
done:
     jr $ra
     nop
