curdir = $(shell pwd)

define make
	docker run --rm -v $(1):/source -it werner/devkitpro make
endef

define clean
	docker run --rm -v $(1):/source -it werner/devkitpro make clean
endef

all:
	$(call make, $(curdir)/battleship)
	$(call make, $(curdir)/megatroid)
	$(call make, $(curdir)/pong)
	$(call make, $(curdir)/tetris)

clean:
	$(call clean, $(curdir)/battleship)
	$(call clean, $(curdir)/megatroid)
	$(call clean, $(curdir)/pong)
	$(call clean, $(curdir)/tetris)
