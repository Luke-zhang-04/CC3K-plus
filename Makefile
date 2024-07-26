CXX=g++
CXXFLAGS=-std=c++14 -Wall -g -MMD
EXEC=main
OBJDIR=obj
CCFILES=$(wildcard src/*.cc)
OBJECTS=$(addprefix $(OBJDIR)/, ${CCFILES:.cc=.o})
DEPENDS=${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

${OBJECTS}: ${CCFILES}
	mkdir -p obj/src
	${CXX} ${CXXFLAGS} -c -o $@ $<

-include ${DEPENDS}

# Extra recipe that allows us to delete temporary files by saying
# make clean

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}
