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
