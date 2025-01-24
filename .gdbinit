set height unlimited
set pagination off
set confirm off

#set trace-commands on
#set print inferior-events on

set sysroot /

directory ~/staging/glibc-2.31
directory ~/staging/glibc-2.31/elf
directory ~/staging/glibc-2.31/nptl
directory ~/staging/glibc-2.31/dlfcn
directory ~/staging/glibc-2.31/misc
directory ~/staging/glibc-2.31/csu
directory ~/staging/glibc-2.31/ctype
directory ~/staging/glibc-2.31/stdlib
directory ~/staging/glibc-2.31/sysdeps
directory ~/staging/glibc-2.31/support
directory ~/staging/readline-8.0
directory ~/staging/gcc-9-9.4.0/src/libsanitizer/sanitizer_common
directory ~/staging/gcc-9-9.4.0/src/libsanitizer/asan
directory ~/staging/gcc-9-9.4.0/src/libsanitizer/ubsan

br main

set breakpoint pending on
set mem inaccessible-by-default off
#set follow-fork-mode parent
set follow-fork-mode child
set follow-exec-mode new
#set follow-exec-mode same
set detach-on-fork off

#catch fork

handle SIGSEGV

#Links:
#  - https://sourceware.org/gdb/onlinedocs/gdb/Forks.html
#  - https://sourceware.org/gdb/onlinedocs/gdb/Inferiors-and-Programs.html
