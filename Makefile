sdf = ${SDF}

cast:
	@g++ -w raycast.cpp -o ./raycast && ./raycast

run:
	@g++ ${SDF} -o ./${SDF} && ./${SDF}

sdf:
	@g++ -w sdf.cpp -o ./sdf && ./sdf
