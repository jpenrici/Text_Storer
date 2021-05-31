# Makefile

TextStorer : main.o controller.o model.o view.o connect_db.o
	g++ -g -Wall -o TextStorer main.o controller.o model.o view.o connect_db.o `pkg-config gtkmm-3.0 --cflags --libs` -l sqlite3

main.o : main.cpp controller.hpp
	g++ -g -Wall -c main.cpp `pkg-config gtkmm-3.0 --cflags --libs`

controller.o : controller.cpp controller.hpp view.hpp model.hpp
	g++ -g -Wall -c controller.cpp `pkg-config gtkmm-3.0 --cflags --libs`

view.o : view.cpp view.hpp
	g++ -g -Wall -c view.cpp `pkg-config gtkmm-3.0 --cflags --libs`

model.o : model.cpp model.hpp connect_db.hpp
	g++ -g -Wall -c model.cpp

connect_db.o : connect_db.cpp connect_db.hpp
	g++ -g -Wall -c connect_db.cpp -l sqlite3	

clean :
	rm -f TextStorer *.o