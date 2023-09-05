CC = gcc
FLAGS = -Wall -Wextra -Werror -std=c11
TST_LIBS = -lcheck
OPEN := open
LIB_NAME = s21_decimal
SRCS=s21_decimal.c s21_decimal.h
OBJS=$(SRCS:.c=.o)

ifeq ($(shell uname), Linux)
	TST_LIBS += -lpthread -lrt -lm
	OPEN := xdg-open
endif

all: gcov_report

clean:
	rm -rf *.o test *.a *.gcno *.gcda *.gcov *.html *.css *.info ./html_report *.out 

cli:
	clang-format -style=Google -i *.c *.h

cln:
	clang-format -style=Google -n *.c *.h

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

$(LIB_NAME).a: $(OBJS)
	ar rc $(LIB_NAME).a $^
	ranlib $(LIB_NAME).a

test: $(LIB_NAME).a add_coverage_flag
	$(CC) $(FLAGS) tests.c $(LIB_NAME).a $(TST_LIBS) -o tests
	./tests

add_coverage_flag:
	$(eval FLAGS += --coverage)

gcov_report: test
	lcov -t "report" -o report.info -c -d .
	genhtml -o "html_report" report.info
	$(OPEN) ./html_report/index.html

valgrind:
	valgrind --tool=memcheck --leak-check=yes ./tests
