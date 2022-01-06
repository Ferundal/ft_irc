# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cjettie <cjettie@21-school.ru>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#                                                      #+#    #+#              #
#                                                     ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SHELL = /bin/sh		#for systems where SHELL variable can be inherited from environment

.SUFFIXES:			#no suffix rules are used

#Path to project libraries *.a files

LIBS=

#External libraries

LIBS_EXT=

SRC_DIR=			src

OBJ_DIR=			obj

#Classes's names should be placed here

CLASSES=			Server

CLASSES_B=

#Independent opp files here

CPP_FILES=			main.cpp

CPP_FILES_B=

LIBS_DIR=			${dir ${LIBS}}

LIBS_INC_F=			-L ${dir ${LIBS}} -l${patsubst lib%.a, %, ${notdir ${LIBS}}}

LIBS_INC=			${foreach LIBS, ${LIBS}, ${LIBS_INC_F}}

LIBS_HEADERS=		${patsubst %.a, %.h, ${LIBS}}

CLASS_HEADERS=		${addprefix ${SRC_DIR}/ , ${addsuffix .hpp, ${CLASSES}}}

CLASS_HEADERS_B=	${addprefix ${SRC_DIR}/ , ${addsuffix .hpp, ${CLASSES_B}}}

HEADERS=			${LIBS_HEADERS} ${CLASS_HEADERS}

ifdef COMPILE_BONUS
HEADERS:=			${HEADERS} ${CLASS_HEADERS_B}
endif

INC_HEADERS_FORMAT=	-I ${dir ${HEADERS}}

INC_HEADERS_DIR=	${foreach HEADERS, ${HEADERS}, ${INC_HEADERS_FORMAT}}

CLASSES_SRC=		${addprefix ${SRC_DIR}/, ${addsuffix .cpp, ${CLASSES}}}

CLASSES_SRC_B=		${addprefix ${SRC_DIR}/, ${addsuffix .cpp, ${CLASSES_B}}}

SRCS_CPP=			${CLASSES_SRC} \
					${addprefix ${SRC_DIR}/, ${CPP_FILES}}

ifdef COMPILE_BONUS
SRCS_CPP:=			${SRCS_CPP} ${CLASSES_SRC_B}
endif

OBJS_CPP=			${SRCS_CPP:${SRC_DIR}/%.cpp=${OBJ_DIR}/%.o}

NAME=				ircserv

CC=					clang++
RM=					rm -f
LD=					ld

ALL_CFLAGS=			-Wall -Wextra -Werror -std=c++98 ${INC_HEADERDIR} ${CFLAGS}
ALL_LDFLAGS=		${LDFLAGS}

NORM=				norminette ${NORMO}

.PHONY:				all clean fclean re bonus libs_make libs_clean obj_dir_make

all:				libs_make obj_dir_make ${NAME}

obj_dir_make:
					- mkdir obj

libs_make:
					${foreach LIBS_DIR, ${LIBS_DIR}, ${MAKE} -C ${LIBS_DIR} bonus}

${OBJ_DIR}/%.o:		${SRC_DIR}/%.cpp ${HEADERS}
					${CC} ${ALL_CFLAGS} ${INC_HEADERS_DIR} -c ${<} -o ${@}

${NAME}:			${OBJS_CPP} ${LIBS}
					${CC} ${ALL_LDFLAGS} ${OBJS_CPP} ${LIBS_INC} ${LIBS_EXT} -o ${NAME}

bonus:
					${MAKE} COMPILE_BONUS=1 all

#$(INSTALL_PROGRAM)=	${TEST_NAME} $(DESTDIR)$(bindir)/foo	#where executeable shoud be placed
#$(INSTALL_DATA)=	${NAME} $(DESTDIR)$(libdir)/libfoo.a		#where libs shoud be placed

clean:
					${foreach LIBS_DIR, ${LIBS_DIR}, ${MAKE} -C ${LIBS_DIR} clean}
					${RM} ${OBJS_CPP} ${OBJS_CPP_BONUS}

fclean:
					${foreach LIBS_DIR, ${LIBS_DIR}, ${MAKE} -C ${LIBS_DIR} fclean}
					${RM} ${OBJS_CPP} ${OBJS_CPP_BONUS}
					${RM} ${NAME}

norm:		
					${NORM} ${LIBS_DIR} *.c *.h

re:					fclean all

# .DELETE_ON_ERROR