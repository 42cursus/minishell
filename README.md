### Summary:
This project is about creating a simple shell.

### To build project with run cmake:
  ```bash
  mkdir -pv build; cd build
  cmake ..
  cmake --build . --target all
  ```

### To run valgrind with readline suppressions:
  ```bash
  valgrind --leak-check=full \
    --show-leak-kinds=all \
    --suppressions=supp.supp ./minishell
  ```
- if you find the output is mixed with the application output then redirect valigrind output to separate file descriptor:
  ```bash
  valgrind --leak-check=full \
        --show-leak-kinds=all \
        --suppressions=supp.supp \
        --log-fd=9 ./minishell \
        9>>memcheck.log
  ```

### Useful links:

- [Pratt Parsing](http://www.oilshell.org/blog/2016/11/01.html)
- [Top-Down operator precedence](https://eli.thegreenplace.net/2010/01/02/top-down-operator-precedence-parsing)
- [Precedence Climbing](https://eli.thegreenplace.net/2012/08/02/parsing-expressions-by-precedence-climbing)
- [Recursive Descent](https://www.engr.mun.ca/~theo/Misc/exp_parsing.htm)
- [A recursive descent with infix expression](https://eli.thegreenplace.net/2009/03/20/a-recursive-descent-parser-with-an-infix-expression-evaluator)
- [Shell Implementation of Pipelines](https://www.cs.uleth.ca/~holzmann/C/system/shell_does_pipeline.pdf)
- [Shell Grammar](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html#tag_18_10)
- [Token Recognition](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html#tag_18_03)