health-center:
	mkdir -p out
	g++ code/main.cpp code/WaistlineData.cpp -o out/health-center

run:
	./out/health-center

clean:
	rm out/*

br:
	mkdir -p out
	g++ code/main.cpp code/WaistlineData.cpp -o out/health-center
	./out/health-center