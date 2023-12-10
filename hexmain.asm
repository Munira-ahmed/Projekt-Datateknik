# hexmain.asm
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
   main:
	li	$a0,17	# change this to test different values

	jal	hexasc		# call hexasc
	nop			# delay slot filler (just in case)	

	move	$v0, $a0		# copy return value to argument register

	li	$v0,11		# syscall with v0 = 11 will print out
	syscall			# one byte from a0 to the Run I/O window
	
  
    stop:	j stop		# stop after one run
	nop			# delay slot filler (just in case)

  # You can write your own code for hexasc here
 

  hexasc:
     
     and $a0, $a0, 0xf #ignorera allt som inte är de fyra första bitarna
      
     ble $a0, 0x9, else
     
     add $a0, $a0, 0x37
     j end  # hoppa till slutet
     
 else:
   add $a0,$a0, 0x30 # om a0 är större eller lika med tio. addera 30
   
 end:
  add $v0, $v0, $a0
  jr $ra
      
