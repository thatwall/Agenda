FLAG=-c -g -std=c++0x
OBJ=User.o Date.o Meeting.o Storage.o AgendaService.o AgendaUI.o
Agenda:$(OBJ)
	g++ $(OBJ) Agenda.cpp -o Agenda.out -g -std=c++0x
AgendaUI.o:AgendaUI.h
	g++ AgendaUI.cpp $(FLAG)
AgendaService.o:AgendaService.h
	g++ AgendaService.cpp $(FLAG)
Storage.o:Storage.h
	g++ Storage.cpp $(FLAG)
User.o:User.h
	g++ User.cpp $(FLAG)
Date.o:Date.h
	g++ Date.cpp $(FLAG)
Meeting.o:Meeting.h
	g++ Meeting.cpp $(FLAG)
clean:
	rm *.o Agenda.out
