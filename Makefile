VIMPL := MC
OUTPUT := OUTPUT
BCHECK := BARGINGCHECK

CXX = u++
CXXFLAGS = -g -multi -Wall -Wextra -MMD

MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}

OBJECTS = main.o Printer.o BottlingPlant.o Truck.o NameServer.o VendingMachine.o WATCardOffice.o WATCard.o Groupoff.o Bank.o Parent.o Student.o

EXEC = soda

DEPENDS = ${OBJECTS:.o=.d}

#############################################################

.PHONY : all clean
.ONESHELL :

all : ${EXEC}

-include VoteImpl

ifeq (${shell if [ "${LOCKVIMPL}" = "${VIMPL}" -a "${OUTPUTTYPE}" = "${OUTPUT}" -a \
			"${BCHECKIMPL}" = "${BCHECK}" ] ; then echo true ; fi },true)
${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} $^ -o $@
else						# implementation type has changed => rebuilt
.PHONY : ${EXEC}
${EXEC} :
	rm -f VoteImpl
	touch q3tallyVotes.h
	${MAKE} ${EXEC} VIMPL="${VIMPL}" OUTPUT="${OUTPUT}" BCHECK="${BCHECK}"
endif

VoteImpl :
	echo "LOCKVIMPL=${VIMPL}\nOUTPUTTYPE=${OUTPUT}\nBCHECKIMPL=${BCHECK}" > VoteImpl
	sleep 1

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}

-include ${DEPENDS}

clean :
	rm -f *.d ${OBJECTS} ${EXEC} VoteImpl