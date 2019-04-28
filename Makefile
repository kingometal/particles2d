GCC = g++
FLAGS = -std=c++11 -lSDL2 -lSDL2_ttf -fPIC
OUTDIR = build
INDIR = .
LIBDIR = libs
SDL_LIBRARY = SdlPresenter
EXECUTABLE = particles2d.x
INTERFACEFOLDER=interfaces
INTERFACEFILES=$(INTERFACEFOLDER)/IPresenter.h
SOURCEDIR=source

all: ${EXECUTABLE}

run: all
	./${EXECUTABLE}

clean:
	if [ -d ${OUTDIR} ]; then rm -rf ${OUTDIR} ; fi
	if [ -e ./${EXECUTABLE} ]; then rm ./${EXECUTABLE} ; fi

directories: $(OUTDIR)

$(OUTDIR):
	mkdir -p $(OUTDIR)

${EXECUTABLE}: directories $(OUTDIR)/particles.o $(OUTDIR)/particle.o $(OUTDIR)/main.o $(OUTDIR)/particlesviewsdladapter.o $(OUTDIR)/particlemanager.o $(OUTDIR)/$(LIBDIR)/lib$(SDL_LIBRARY).a
	$(GCC) -Wall $(OUTDIR)/particle.o $(OUTDIR)/particles.o $(OUTDIR)/main.o $(OUTDIR)/particlesviewsdladapter.o $(OUTDIR)/particlemanager.o -o $(EXECUTABLE) $$(pkg-config --libs x11) -l$(SDL_LIBRARY) -L$(OUTDIR)/$(LIBDIR) $(FLAGS)

$(OUTDIR)/main.o: $(SOURCEDIR)/main.cpp $(SOURCEDIR)/Constants.h
	$(GCC) $(FLAGS) -c $(SOURCEDIR)/main.cpp -o $(OUTDIR)/main.o

$(OUTDIR)/particles.o: $(SOURCEDIR)/Particles.h $(SOURCEDIR)/Particles.cpp $(SOURCEDIR)/Constants.h $(SOURCEDIR)/$(INTERFACEFOLDER)/ParticlesViewInterface.h $(OUTDIR)/particle.o
	$(GCC) $(FLAGS) -c $(SOURCEDIR)/Particles.cpp -o $(OUTDIR)/particles.o

$(OUTDIR)/particle.o: $(SOURCEDIR)/Particle.h $(SOURCEDIR)/Particle.cpp $(SOURCEDIR)/Constants.h
	$(GCC) $(FLAGS) -c $(SOURCEDIR)/Particle.cpp -o $(OUTDIR)/particle.o

$(OUTDIR)/particlemanager.o: $(SOURCEDIR)/ParticleManager.h $(SOURCEDIR)/ParticleManager.cpp $(SOURCEDIR)/Constants.h
	$(GCC) $(FLAGS) -c $(SOURCEDIR)/ParticleManager.cpp -o $(OUTDIR)/particlemanager.o

$(OUTDIR)/particlesviewsdladapter.o: $(SOURCEDIR)/ParticlesViewSdlAdapter.h $(SOURCEDIR)/ParticlesViewSdlAdapter.cpp $(SOURCEDIR)/$(INTERFACEFOLDER)/ParticlesViewInterface.h $(OUTDIR)/rgbdata.o
	$(GCC) $(FLAGS) -c $(SOURCEDIR)/ParticlesViewSdlAdapter.cpp -o $(OUTDIR)/particlesviewsdladapter.o


#############################################
## SDL Framework
#############################################
$(OUTDIR)/$(LIBDIR)/lib$(SDL_LIBRARY).a: $(OUTDIR)/$(LIBDIR) $(OUTDIR)/rgbdata.o $(OUTDIR)/presenter.o $(OUTDIR)/userinput.o
	ar rcs $(OUTDIR)/$(LIBDIR)/lib$(SDL_LIBRARY).a $(OUTDIR)/rgbdata.o $(OUTDIR)/presenter.o $(OUTDIR)/userinput.o

$(OUTDIR)/$(LIBDIR): directories
	mkdir -p $(OUTDIR)/$(LIBDIR)

$(OUTDIR)/rgbdata.o: $(SOURCEDIR)/RGBData.h $(SOURCEDIR)/RGBData.cpp
	$(GCC) -c $(SOURCEDIR)/RGBData.cpp -o $(OUTDIR)/rgbdata.o

$(OUTDIR)/userinput.o: $(SOURCEDIR)/UserInput.h $(SOURCEDIR)/UserInput.cpp
	$(GCC) -c $(SOURCEDIR)/UserInput.cpp -o $(OUTDIR)/userinput.o

$(OUTDIR)/presenter.o: $(SOURCEDIR)/Presenter.cpp $(SOURCEDIR)/Presenter.h $(SOURCEDIR)/$(INTERFACEFILES)
	$(GCC) -c $(SOURCEDIR)/Presenter.cpp -o $(OUTDIR)/presenter.o
#############################################
