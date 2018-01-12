
LIBS := -lIrrlicht -lGL -lX11 -lXxf86vm -lBox2D -lfmod -lfmodstudio -lsfml-graphics -lsfml-window -lsfml-system

CC=g++
CFLAGS= -I. -I./inc
PATHS:= -Llib/




# Name of the executable created
EJEC:= AncientRise
# Path for the executable
BINPATH = ./bin
# Path for the .o files
BUILDPATH = ./obj
# Path for the source files
SOURCEPATH = ./sourcefiles

MOTORPATH := ./motorgrafico/sourcefiles

EXECUTABLE = $(BINPATH)/$(EJEC)
SRC := $(wildcard $(SOURCEPATH)/*.cpp)
MOTORSRC := $(wildcard $(MOTORPATH)/*.cpp)
#OBJ := $(subst .cpp,.o,$(SRC))
OBJ = $(patsubst $(SOURCEPATH)/%.cpp, $(BUILDPATH)/%.o, $(SRC))
MOTOR := $(patsubst $(MOTORPATH)/%.cpp, $(BUILDPATH)/%.o, $(MOTORSRC))


#FMODSTUDIO
#WL:= -Wl,-rpath=\$$ORIGIN/$(dir ${LOWLEVEL_FMOD_LIB}),-rpath=\$$ORIGIN/$(dir ${STUDIO_FMOD_LIB}) ${LOWLEVEL_FMOD_LIB} ${STUDIO_FMOD_LIB}

$(EJEC): prepare $(OBJ) $(MOTOR)
	$(warning Creando el ejecutable $@...)
	$(CC) -o $(EXECUTABLE) $(OBJ) $(MOTOR) $(LIBS) $(PATHS) 


$(BUILDPATH)/%.o: $(SOURCEPATH)/%.cpp 
	$(warning Creando el binario $@...)
	#$(CC) -c -o $@ $^ $(CFLAGS)
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILDPATH)/%.o: $(MOTORPATH)/%.cpp
	$(warning Creando el binario $@...)
	#$(CC) -c -o $@ $^ $(CFLAGS)
	$(CC) $(CFLAGS) -c $< -o $@


info:
	$(info $(SRC))
	$(info $(OBJ))


clean: 
	rm -f $(BUILDPATH)/*.o 
	rm -f $(BINPATH)/$(EJEC)

prepare:
	$(warning Creando la estructura de carpetas)
	mkdir -p $(BINPATH)
	mkdir -p $(BUILDPATH)


