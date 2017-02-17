all:
	docker run -v $(abspath battleship):/source -it werner/devkitpro make
	docker run -v $(abspath megatroid):/source -it werner/devkitpro make
	docker run -v $(abspath pong):/source -it werner/devkitpro make
	docker run -v $(abspath tetris):/source -it werner/devkitpro make

clean:
	docker run -v $(abspath battleship):/source -it werner/devkitpro make clean
	docker run -v $(abspath megatroid):/source -it werner/devkitpro make clean
	docker run -v $(abspath pong):/source -it werner/devkitpro make clean
	docker run -v $(abspath tetris):/source -it werner/devkitpro make clean
