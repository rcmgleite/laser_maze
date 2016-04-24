all:
	g++ -std=c++11 laser_maze.cpp -o laser_maze

run:
	g++ -std=c++11 laser_maze.cpp -o laser_maze
	./laser_maze 

clean:
	rm -rf laser_maze
