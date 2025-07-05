#!/usr/bin/python3






# ---------------- IMPORTATIONS ----------------

#system
import os

#delay
import time






# -------- TOOLS --------

#real print
def p(text):
	print(text)
	os.sys.stdout.flush()





# -------- EXECUTION --------

#main
def main(args):

	#send orders to S2DE : create window
	p("i00000438000001e0my Wonderful window`")

	#send orders to S2DE : draw some things
	p("o89f5f5c2") #set background color to light cyan
	p("O0000ffff") #set front color to blue
	p("R00000032000000320000006400000064") #draw filled rectangle at (50,50, 100,100)
	p("Off0000ff") #set font color to red
	p("t000000c8000000c8000000e1000000fa000000af000000fa") #draw empty triangle at (200,200, 225,250, 175,250)
	time.sleep(8)

	#reset display
	p("h")
	time.sleep(1)

	#draw something else
	p("of5c2c2ff") #set background color to light red
	p("O00ff00ff") #set front color to green
	p("R00000032000000320000006400000064") #draw filled rectangle at (50,50, 100,100)
	p("O000000ff") #set font color to black
	p("t000000c8000000c8000000e1000000fa000000af000000fa") #draw empty triangle at (200,200, 225,250, 175,250)

	#main loop
	while True:
		time.sleep(0.1)

main(os.sys.argv)
