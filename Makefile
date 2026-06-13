SRC = ./src/
RUN = ./run/
INV = ./inv/

inv: $(SRC)main.cpp
	g++ -Wall -Wextra -o $(RUN)inv $(SRC)main.cpp

runinv: 
<<<<<<< HEAD
	$(RUN)inv $(INV)inventory.txt
=======
	run/inv 
>>>>>>> f5294d9a36091b7ce3e65cec4c4966ce8462dfd5
