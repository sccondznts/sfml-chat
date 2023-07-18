build:
	 g++ ./src/*.cpp -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -o game

run:
	./game

clean:
	rm game
