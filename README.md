### Summary:
This project is about creating a simple shell.

### To build project with run cmake:
  ```bash
  mkdir -pv build; cd build
  cmake ..
  cmake --build . --target all
  ```

generating suppressions for memory leaks
https://wiki.wxwidgets.org/Valgrind_Suppression_File_Howto
https://stackoverflow.com/questions/17159578/17178559#17178559
### To  generate valgrind readline suppressions:
  ```bash
  valgrind --leak-check=full \
    --show-reachable=yes \
    --error-limit=no \
    --gen-suppressions=all \
    --log-file=memcheck.log ./minishell
  ```
- if you find the output is mixed with the application output then redirect valigrind output to separate file descriptor:
  ```bash
valgrind --leak-check=full \
    --show-reachable=yes \
    --error-limit=no \
    --gen-suppressions=all \
   --log-fd=9 ./minishell 9>>memcheck.log
  ```
- You now have a file containing the raw output, with the suppressions mingled with the errors and other stuff. Also, as errors are usually multiple, there'll usually be multiple instances of each suppression. So the next step is to pass minimalraw.log through this gawk script which removes the cruft.
  ```bash
  cat ./memcheck.log | ./parse_valgrind_suppressions.sh > readline.supp
  ```
### To run valgrind with readline suppressions:
  ```bash
valgrind --leak-check=full \
    --show-leak-kinds=all \
    --suppressions=readline.supp ./minishell
  ```
- if you find the output is mixed with the application output then redirect valigrind output to separate file descriptor:
  ```bash
  valgrind --leak-check=full \
        --show-leak-kinds=all \
        --suppressions=supp.supp \
        --log-fd=9 ./minishell \
        9>>memcheck.log
  ```

### Lexer-parser

Traditionally, syntax analysis is divided into a lexical scanner and a (context-free) parser.
A scanner divides an input string consisting of characters into a string of tokens.
A sequence of input characters t h a t comprises a single token is called a lexeme.
Thus, lexical analyzer insulates a parser from the lexeme representation of tokens.

Tokenizing is breaking up a string in chunks. The lexical properties of the chunks are defined by the lexer rules.
Such a chunk is like a word in a sentence. This tokenization is usually based on regular expression matching.
To choose between overlapping matches a number of standard lexical disambiguation rules are used.
(Advanced feature: A lexer can be put in a certain state, after seeing a certain input.
These states can determine which rules are enabled/disabled.)
Each lexer rule gets an identifier called a token, typically an integer.
So the output of the lexer is a stream of tokens (integers) 
that represent the regular expressions it has seen in the input string.
This output also allows seeing what part of the input string was recognized for each token.
Lexer rules can overlap. Simple precedence logic applies to decide which rules will be matched. 
Internally a lexer (like lex or flex) has a generated state-machine that keeps track of all this.
- [Token Recognition](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html#tag_18_03)
- [vsinha CS-252 Fall 2013](https://github.com/vsinha/shell/blob/master/shell.l)
- [offirgolan lexical analyzer for shell](https://github.com/offirgolan/Shell/blob/master/shell.l)


A next step is feeding these tokens to a parser which has grammatical rules that define
what order of tokens form valid input.
- [Shell Grammar](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html#tag_18_10)
- [Scannerless Generalized LR Parsers](https://www.researchgate.net/publication/221302593_Disambiguation_Filters_for_Scannerless_Generalized_LR_Parsers)

### Bison:
```bash
bison --report=state shell.y
```
- [Parsing - wiki](https://en.wikipedia.org/wiki/Parsing)
- [Pratt Parsing](http://www.oilshell.org/blog/2016/11/01.html)
- [Top-Down operator precedence](https://eli.thegreenplace.net/2010/01/02/top-down-operator-precedence-parsing)
- [Precedence Climbing](https://eli.thegreenplace.net/2012/08/02/parsing-expressions-by-precedence-climbing)
- [Recursive Descent](https://www.engr.mun.ca/~theo/Misc/exp_parsing.htm)
- [A recursive descent with infix expression](https://eli.thegreenplace.net/2009/03/20/a-recursive-descent-parser-with-an-infix-expression-evaluator)
- [Let’s Build a Linux Shell](https://medium.com/swlh/lets-build-a-linux-shell-part-iii-a472c0102849)
- [Scannerless parsing](https://en.wikipedia.org/wiki/Scannerless_parsing)
- [BNF for parsing AND lexing](https://stackoverflow.com/a/66533353/6501644)

###  Shell variable expansion:
- [3.5 Shell Expansions - gnu.org](https://www.gnu.org/software/bash/manual/html_node/Shell-Expansions.html)

### Syntax trees:
A concrete syntax tree matches what the grammar rules say is the syntax.
The purpose of the abstract syntax tree is have a "simple" representation of what's essential in "the syntax tree".

- [AST vs CST](https://stackoverflow.com/questions/1888854/#1916687)

### Executing:
- [Shell Implementation of Pipelines](https://www.cs.uleth.ca/~holzmann/C/system/shell_does_pipeline.pdf)

### Useful links:
- [LALR(1) grammars](https://stackoverflow.com/questions/49493005/is-every-ll1-grammar-also-a-lalr1-grammar)
- [DMSToolkit](https://www.semanticdesigns.com/Products/DMS/DMSToolkit.html)

### Other implementations:
- [Thomas Wagner's minishell](https://github.com/twagger/minishell)
- [Toni Schmitt's minishell](https://github.com/toni-schmitt/minishell)
- [Open Edu Hub - OSdev - minishell](https://github.com/open-education-hub/operating-systems/tree/main/content/assignments/minishell)
- [Vincent Sanders's GNU dbm](https://github.com/pexip/os-gdbm/blob/upstream/1.23/tools/lex.l)
