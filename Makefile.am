bin_PROGRAMS = matasano
matasano_CFLAGS = -I$(srcdir)/include -Wall -Wextra -Werror $(SSL_CFLAGS)
matasano_LDADD = $(SSL_LIBS)
matasano_SOURCES = main.c \
	include/base64.h \
	include/hex.h \
	include/score.h \
	include/xor.h \
	set1/challenge1.c \
	set1/challenge2.c \
	set1/challenge3.c \
	set1/challenge4.c \
	set1/challenge5.c \
	set1/challenge6.c \
	set1/challenge7.c \
	set1/challenge8.c \
	set1/set1.c \
	set2/challenge9.c \
	set2/challenge10.c \
	set2/challenge11.c \
	set2/set2.c \
	util/base64.c \
	util/hex.c \
	util/score.c \
	util/xor.c

