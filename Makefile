KEYBOARD := silakka54
KEYMAP   := silakka54-qmk-keymap
QMK_HOME := $(shell cd $(CURDIR)/../../.. && pwd)

.PHONY: build clean

build:
	nix develop --command bash -c \
	  "QMK_HOME=$(QMK_HOME) qmk compile -c -kb $(KEYBOARD) -km $(KEYMAP)"

clean:
	nix develop --command bash -c \
	  "QMK_HOME=$(QMK_HOME) qmk compile -kb $(KEYBOARD) -km $(KEYMAP) -- clean" 2>/dev/null; \
	rm -rf $(QMK_HOME)/.build/obj_$(KEYBOARD)_$(KEYMAP) \
	       $(QMK_HOME)/.build/$(KEYBOARD)_$(KEYMAP).elf \
	       $(QMK_HOME)/.build/$(KEYBOARD)_$(KEYMAP).uf2 \
	       $(QMK_HOME)/.build/$(KEYBOARD)_$(KEYMAP).map
