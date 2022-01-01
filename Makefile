CXXFLAGS = -g -Wall -pedantic -std=c++17

OBJ = email.o users.o auth.o mail.o mailbox.o

OUT = email

$(OUT): $(OBJ)
	gcc $(CXXFLAGS) $(OBJ) -lstdc++ -o $(OUT)

clean:
	rm -f *~ $(OUT) *.o
