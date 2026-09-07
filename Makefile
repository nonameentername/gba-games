.PHONY: all build clean clean-build publish shell-gba

all:
	$(MAKE) shell-gba SHELL_COMMAND='make build'

shell-gba:
	docker run --rm -v "$(CURDIR):$(CURDIR)" -w "$(CURDIR)" werner/devkitpro $(SHELL_COMMAND)

build:
	$(MAKE) -C battleship
	$(MAKE) -C megatroid
	$(MAKE) -C pong
	$(MAKE) -C tetris

clean:
	$(MAKE) shell-gba SHELL_COMMAND='make clean-build'

clean-build:
	$(MAKE) -C battleship clean
	$(MAKE) -C megatroid clean
	$(MAKE) -C pong clean
	$(MAKE) -C tetris clean

publish:
	cp battleship/battleship.gba public/battleship
	cp megatroid/megatroid.gba public/megatroid
	cp pong/pong.gba public/pong
	cp tetris/tetris.gba public/tetris
