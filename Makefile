CXX=g++
CXXFLAGS=-std=c++14 -Wall -g -MMD
EXEC=main
OBJDIR=obj
SRCDIR=src
CCFILES=$(wildcard $(SRCDIR)/*.cc)
OBJECTS=$(patsubst $(SRCDIR)/%,$(OBJDIR)/%,${CCFILES:.cc=.o})
# OBJECTS=${CCFILES:.cc=.o}
DEPENDS=${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

${OBJDIR}/%.o: src/%.cc
	mkdir -p obj/src
	${CXX} ${CXXFLAGS} -c -o $@ $<

-include ${DEPENDS}

# Extra recipe that allows us to delete temporary files by saying
# make clean

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}
