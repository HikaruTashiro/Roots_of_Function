EXPRESSION = ./exp
SOURCES = main.cpp function.cpp
SOURCES += $(EXPRESSION)/expression.cpp $(EXPRESSION)/lexicon.cpp $(EXPRESSION)/token.cpp
OBJECTS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))


COMPILE : LINK 
	g++ -o main $(OBJECTS) 

LINK:
	g++ -c $(SOURCES)

clean:
	rm $(OBJECTS)
