GCC = g++
FLAGS = -std=c++11
OUTDIR = build
INDIR = .
EXECUTABLE = particles2d.x

all: ${EXECUTABLE}
#	${EXECUTABLE}

${EXECUTABLE}: directories $(OUTDIR)/particles.o $(OUTDIR)/particle.o $(OUTDIR)/main.o $(OUTDIR)/xwindows.o
	$(GCC) -Wall $(FLAGS) $(OUTDIR)/particle.o $(OUTDIR)/particlesview.o $(OUTDIR)/particles.o $(OUTDIR)/main.o $(OUTDIR)/xwindows.o -o $(EXECUTABLE) $$(pkg-config --libs x11)


$(OUTDIR)/main.o: main.cpp include/rnd.h $(OUTDIR)/particles.o $(OUTDIR)/particlesview.o $(OUTDIR)/xwindows.o constants.h
	$(GCC) $(FLAGS) -c main.cpp -o $(OUTDIR)/main.o

$(OUTDIR)/particles.o: particles.h particles.cpp $(OUTDIR)/particle.o $(OUTDIR)/particlesview.o constants.h particlesviewinterface.h
	$(GCC) $(FLAGS) -c particles.cpp -o $(OUTDIR)/particles.o


$(OUTDIR)/particle.o: particle.h particle.cpp constants.h
	$(GCC) $(FLAGS) -c particle.cpp -o $(OUTDIR)/particle.o

$(OUTDIR)/particlesview.o: particlesview.h particlesview.cpp particlesviewinterface.h $(OUTDIR)/xwindows.o
	$(GCC) $(FLAGS) -c particlesview.cpp -o $(OUTDIR)/particlesview.o

$(OUTDIR)/xwindows.o: xwindows.h xwindows.cpp
	$(GCC) $(FLAGS) -c xwindows.cpp -o $(OUTDIR)/xwindows.o

directories: $(OUTDIR)

$(OUTDIR):
	mkdir -p $(OUTDIR)
