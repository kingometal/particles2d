GCC = g++
FLAGS = -std=c++11 -lSDL2 -lSDL2_ttf -fPIC
FLAGS_COMPILE = -fPIC
OUTDIR = build
INDIR = .
LIBDIR = libs
EXECUTABLE = particles2d.x
INTERFACEFOLDER=interfaces
INTERFACEFILES=$(INTERFACEFOLDER)/IPresenter.h
SOURCEDIR=source

all: ${EXECUTABLE}

${EXECUTABLE}: directories $(OUTDIR)/particles.o $(OUTDIR)/particle.o $(OUTDIR)/main.o $(OUTDIR)/particlesviewsdladapter.o $(OUTDIR)/rgbdata.o $(OUTDIR)/userinput.o $(OUTDIR)/presenter.o
	$(GCC) -Wall $(OUTDIR)/particle.o $(OUTDIR)/particles.o $(OUTDIR)/main.o $(OUTDIR)/particlesviewsdladapter.o $(OUTDIR)/rgbdata.o $(OUTDIR)/userinput.o $(OUTDIR)/presenter.o -o $(EXECUTABLE) $$(pkg-config --libs x11) $(FLAGS)

$(OUTDIR)/main.o: $(SOURCEDIR)/main.cpp $(SOURCEDIR)/Constants.h
	$(GCC) $(FLAGS) -c $(SOURCEDIR)/main.cpp -o $(OUTDIR)/main.o

$(OUTDIR)/particles.o: $(SOURCEDIR)/Particles.h $(SOURCEDIR)/Particles.cpp $(SOURCEDIR)/Constants.h $(SOURCEDIR)/$(INTERFACEFOLDER)/ParticlesViewInterface.h
	$(GCC) $(FLAGS) -c $(SOURCEDIR)/Particles.cpp -o $(OUTDIR)/particles.o

$(OUTDIR)/particle.o: $(SOURCEDIR)/Particle.h $(SOURCEDIR)/Particle.cpp $(SOURCEDIR)/Constants.h
	$(GCC) $(FLAGS) -c $(SOURCEDIR)/Particle.cpp -o $(OUTDIR)/particle.o

$(OUTDIR)/particlesviewsdladapter.o: $(SOURCEDIR)/ParticlesViewSdlAdapter.h $(SOURCEDIR)/ParticlesViewSdlAdapter.cpp $(SOURCEDIR)/$(INTERFACEFOLDER)/ParticlesViewInterface.h $(OUTDIR)/rgbdata.o
	$(GCC) $(FLAGS) -c $(SOURCEDIR)/ParticlesViewSdlAdapter.cpp -o $(OUTDIR)/particlesviewsdladapter.o

$(OUTDIR)/rgbdata.o: $(SOURCEDIR)/RGBData.h $(SOURCEDIR)/RGBData.cpp
	$(GCC) -c $(SOURCEDIR)/RGBData.cpp -o $(OUTDIR)/rgbdata.o

$(OUTDIR)/userinput.o: $(SOURCEDIR)/UserInput.h $(SOURCEDIR)/UserInput.cpp
	$(GCC) -c $(SOURCEDIR)/UserInput.cpp -o $(OUTDIR)/userinput.o $(FLAGS_COMPILE)

$(OUTDIR)/presenter.o: $(SOURCEDIR)/Presenter.cpp $(SOURCEDIR)/Presenter.h $(SOURCEDIR)/$(INTERFACEFILES)
	$(GCC) -c $(SOURCEDIR)/Presenter.cpp -o $(OUTDIR)/presenter.o $(FLAGS_COMPILE)

directories: $(OUTDIR)

$(OUTDIR):
	mkdir -p $(OUTDIR)


run: all
	./${EXECUTABLE}
	
clean:
	if [ -d ${OUTDIR} ]; then rm -rf ${OUTDIR} ; fi
	if [ -e ./${EXECUTABLE} ]; then rm ./${EXECUTABLE} ; fi
