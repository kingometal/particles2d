GCC = g++
FLAGS = -std=c++11  -lSDL2 -lSDL2_ttf -fPIC
OUTDIR = build
INDIR = .
EXECUTABLE = particles2d.x

all: ${EXECUTABLE}

${EXECUTABLE}: directories $(OUTDIR)/particles.o $(OUTDIR)/particle.o $(OUTDIR)/main.o $(OUTDIR)/xwindows.o $(OUTDIR)/particlesviewsdladapter.o $(OUTDIR)/particlesview.o
	$(GCC) -Wall $(OUTDIR)/particle.o $(OUTDIR)/particlesview.o $(OUTDIR)/particles.o $(OUTDIR)/main.o $(OUTDIR)/xwindows.o $(OUTDIR)/particlesviewsdladapter.o -o $(EXECUTABLE) $$(pkg-config --libs x11) -lDataTypes -lPresenter -L/home/igor/lib/ $(FLAGS)

$(OUTDIR)/main.o: main.cpp include/rnd.h  $(OUTDIR)/xwindows.o constants.h
	$(GCC) $(FLAGS) -c main.cpp -o $(OUTDIR)/main.o

$(OUTDIR)/particles.o: particles.h particles.cpp constants.h particlesviewinterface.h
	$(GCC) $(FLAGS) -c particles.cpp -o $(OUTDIR)/particles.o

$(OUTDIR)/particle.o: particle.h particle.cpp constants.h
	$(GCC) $(FLAGS) -c particle.cpp -o $(OUTDIR)/particle.o

$(OUTDIR)/particlesview.o: particlesview.h particlesview.cpp particlesviewinterface.h
	$(GCC) $(FLAGS) -c particlesview.cpp -o $(OUTDIR)/particlesview.o

$(OUTDIR)/particlesviewsdladapter.o: particlesviewsdladapter.h particlesviewsdladapter.cpp particlesviewinterface.h
	$(GCC) $(FLAGS) -c particlesviewsdladapter.cpp -o $(OUTDIR)/particlesviewsdladapter.o

$(OUTDIR)/xwindows.o: xwindows.h xwindows.cpp
	$(GCC) $(FLAGS) -c xwindows.cpp -o $(OUTDIR)/xwindows.o

directories: $(OUTDIR)

$(OUTDIR):
	mkdir -p $(OUTDIR)


run: all
	./${EXECUTABLE}
	
clean:
	if [ -d ${OUTDIR} ]; then rm -rf ${OUTDIR} ; fi
