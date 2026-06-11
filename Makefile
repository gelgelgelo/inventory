SRC = ./src/
RUN = ./run/

inv: $(SRC)main.cpp
	g++ -Wall -Wextra -o $(RUN)inv $(SRC)main.cpp
