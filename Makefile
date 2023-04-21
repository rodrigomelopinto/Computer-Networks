CC = gcc
CFLAGS = -g -Wall
TARGET = user
TARGET2 = DS

all: $(TARGET) Server ServerMake

$(TARGET): $(TARGET).c
	@$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

ServerMake: $(TARGET2)

$(TARGET2): $(TARGET2).c
	@$(CC) $(CFLAGS) -o $(TARGET2) $(TARGET2).c

Server:
	@echo User and DS compiled!
	@echo 
	@echo Useful commands:
	@echo "> To run the user: "./user""
	@echo "> To run the server: "./DS""
	@echo
	@echo "File organization:"
	@echo "> user"
	@echo "> DS"
	@echo "> GROUPS/"
	@echo "> USERS/"

clean:
	@echo Cleaning...
	@rm -f user
	@rm -f DS
	@rm -rf GROUPS
	@rm -rf USERS
	@rm -f *.jpg
	@rm -f *.txt
	@rm -f *.jpeg