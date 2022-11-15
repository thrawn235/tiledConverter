#========================================== variables =========================================
assetsDir       = ./assets/
sourceDir       = ./source/
binDir          = ./bin/
emuFlags		= -keymap de -scale 2 -debug -sdcard $(assetsDir)sdcard.img -run -prg 
compiler		= g++
compilerFlags 	= -Wc++11-compat-deprecated-writable-strings -lncurses -Wno-write-strings --output

sourceFiles		= $(sourceDir)tiledConv.cpp $(sourceDir)Vector2D.h $(sourceDir)TMXStructs.h
#
#==============================================================================================


#============================================ make ============================================
.PHONY: all
all: $(binDir)tiledConv

$(binDir)tiledConv: $(sourceFiles)
	$(compiler) $(compilerFlags) $(binDir)tiledConv $(sourceDir)tiledConv.cpp

#==============================================================================================


#=========================================== testing ==========================================
.PHONY: run
run:
	$(binDir)tiledConv
#==============================================================================================



#============================================ clean ===========================================
.PHONY: clean
clean:
	rm -Rf $(binDir)tiledConv
#==============================================================================================



#============================================== git ===========================================
.PHONY: push
push:
	git add *
	git commit -m "commit"
	git push origin main

.PHONY: pull
pull:
	git pull origin main
#==============================================================================================
