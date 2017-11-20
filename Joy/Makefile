#Robot laser tag game
#James Guo
#Autmn 2016
#Makefile for joystick modules
all: joy_stick_lcd_main joy_stick_sender joy_stick_reciever

joy_stick_lcd_main: joy_stick_lcd_main.c joy_stick_lcd.*
	gcc -o joy_stick_lcd_main joy_stick_lcd_main.c joy_stick_lcd.*

joy_stick_sender: joy_stick_sender.c joy_stick_lcd.*
	gcc -o joy_stick_sender joy_stick_sender.c joy_stick_lcd.*

joy_stick_reciever: joy_stick_reciever.c joy_stick_lcd.h joy_stick_lcd.*
	gcc -o joy_stick_reciever joy_stick_reciever.c joy_stick_lcd.*

clean:
	rm joy_stick_sender joy_stick_lcd_main *.o joy_stick_reciever