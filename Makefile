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

SRC_DIR=			src

OBJ_DIR=			obj

#Independent .hpp file here

IND_HEADERS=		error_answers.hpp

IND_HEADERS_BONUS=

#Independent .cpp files here

CPP_FILES=			main.cpp

CPP_FILES_BONUS=

#Classes's names should be placed here

CLASSES=			Server Socket

CLASSES_BONUS=

#Class headers

CLASS_HEADERS=		${addprefix ${SRC_DIR}/, ${addsuffix .hpp, ${CLASSES}}}

CLASS_HEADERS_BONUS=${addprefix ${SRC_DIR}/, ${addsuffix .hpp, ${CLASSES_BONUS}}}

#Class sources

CLASSES_SRC=		${addprefix ${SRC_DIR}/, ${addsuffix .cpp, ${CLASSES}}}

CLASSES_SRC_BONUS=	${addprefix ${SRC_DIR}/, ${addsuffix .cpp, ${CLASSES_BONUS}}}

#All .hpp and .cpp

HEADERS=			${LIBS_HEADERS} ${CLASS_HEADERS} ${addprefix ${SRC_DIR}/, ${IND_HEADERS}}

ifdef COMPILE_BONUS
HEADERS:=			${HEADERS} ${CLASS_HEADERS_BONUS}
endif

SOURCES=			${CLASSES_SRC} \
					${addprefix ${SRC_DIR}/, ${CPP_FILES}}
ifdef COMPILE_BONUS
SOURCES:=			${SOURCES} ${CLASSES_SRC_BONUS}
endif

OBJS_CPP=			${SOURCES:${SRC_DIR}/%.cpp=${OBJ_DIR}/%.o}

#Making variable with all directory when placed .hpp
INC_HEADERS_DIRS=	${sort ${dir ${HEADERS}}}

INC_HEADERS_FORMAT=	-I ${HEADER_DIR}

INC_HEADERS_DIR=	${foreach HEADER_DIR, ${INC_HEADERS_DIRS}, ${INC_HEADERS_FORMAT}}



NAME=				ircserv

CC=					clang++
RM=					rm -f
LD=					ld

ALL_CFLAGS=			-Wall -Wextra -Werror -std=c++98 ${CFLAGS}
ALL_LDFLAGS=		${LDFLAGS}

NORM=				norminette ${NORMO}

.PHONY:				all clean fclean re bonus libs_make libs_clean obj_dir_make

all:				obj_dir_make libs_make ${NAME}


obj_dir_make:
					mkdir -p obj


${OBJ_DIR}/%.o:		${SRC_DIR}/%.cpp ${HEADERS}
					${CC} ${ALL_CFLAGS} ${INC_HEADERS_DIR} -c ${<} -o ${@}

${NAME}:			${OBJS_CPP}
					${CC} ${ALL_LDFLAGS} ${OBJS_CPP} -o ${NAME}

bonus:
					${MAKE} COMPILE_BONUS=1 all

clean:
					${foreach LIBS_DIR, ${LIBS_DIR}, ${MAKE} -C ${LIBS_DIR} clean}
					${RM} ${OBJS_CPP} ${OBJS_CPP_BONUS}

fclean:
					${foreach LIBS_DIR, ${LIBS_DIR}, ${MAKE} -C ${LIBS_DIR} fclean}
					${RM} ${OBJS_CPP} ${OBJS_CPP_BONUS}
					${RM} ${NAME}

re:					fclean all

# .DELETE_ON_ERROR