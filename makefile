health-center:
	mkdir -p out
	g++ -o out/health-center main.cpp

run:
	./out/health-center

clean:
	rm out/*