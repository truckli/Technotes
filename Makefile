

default:
	python genart.py one
	cp TechNotes.pdf T.pdf

all:
	./genart.py all

clean:
	./genart.py clean 

one:
	./genart.py one 


