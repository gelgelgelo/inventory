SRC = ./src/
RUN = ./run/
INV = ./inv/

inv: $(SRC)main.cpp
	g++ -Wall -Wextra -o $(RUN)inv $(SRC)*.cpp

runinv: 
	run/inv
