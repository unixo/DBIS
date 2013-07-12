#######################################################################
#  Ferruccio Vitale (unixo@devzero.it)
#  19/01/2010
#
#  Applied Computer Science - Urbino University "Carlo Bo"
#  Databases and information systems
#  Professor Alessandro Aldini
#  Academic Year 2009 - 2010
#######################################################################
CC = gcc
CFLAGS = `mysql_config --cflags` -O
#CFLAGS_DEBUG = -Wall `mysql_config --cflags` -ggdb -O0 
LIBS = `mysql_config --libs`
SRCS = db_util.c dbis.c
OBJS = db_util.o dbis.o

.PHONY: all
all: dbis

dbis: $(OBJS)
	${CC} ${CFLAGS} ${LIBS} ${OBJS} -o dbis

.c.o:
	${CC} ${CFLAGS} -c $<

.PHONY: clean
clean:
	rm -f *.o core *~ dbis dbis_nodata.sql dbis_dump.sql

package:
	tar cvfz dbis.tgz ${SRCS} *.h Makefile
	
mysqldump:
	mysqldump -R -u root --databases dbis > dbis_dump.sql
	mysqldump -R --no-data -u root --databases dbis > dbis_nodata.sql

indent:
	ls -1 *.[ch] | xargs indent --no-tabs --original
	make clean
