health-center:
	mkdir -p out
	g++ main.cpp -o out/health-center

run:
	./out/health-center

clean:
	rm out/*