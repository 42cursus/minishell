#!/bin/bash

BLACK='\033[0;30m'
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
WHITE='\033[0;37m'
NC='\033[0m' # No Color

# Bold text colors
BOLD_BLACK='\033[1;30m'
BOLD_RED='\033[1;31m'
BOLD_GREEN='\033[1;32m'
BOLD_YELLOW='\033[1;33m'
BOLD_BLUE='\033[1;34m'
BOLD_PURPLE='\033[1;35m'
BOLD_CYAN='\033[1;36m'
BOLD_WHITE='\033[1;37m'

# Background colors
BG_BLACK='\033[40m'
BG_RED='\033[41m'
BG_GREEN='\033[42m'
BG_YELLOW='\033[43m'
BG_BLUE='\033[44m'
BG_PURPLE='\033[45m'
BG_CYAN='\033[46m'
BG_WHITE='\033[47m'
BG_GREY='\033[100m'

if [[ $# -gt 0 ]]; then
	string2=( "$@" )
	printf "${BOLD_YELLOW}input:\n${YELLOW}$string2\n"
	output=$(./minishell "$string2" 2>&1)
	printf "${BOLD_GREEN}tree:\n${NC}${output}\n"

	valgrind --leak-check=full ./minishell "$string2" 1>/dev/null 2> valgrind_output.txt
	if grep -q "ERROR SUMMARY: [^0][0-9]* errors" valgrind_output.txt; then
		printf "${BOLD_RED}Memory errors detected:${NC}\n\n"
		grep "ERROR SUMMARY: [^0][0-9]* errors" valgrind_output.txt
	else
		printf "${BOLD_GREEN}Memory [OK]${NC}\n"
	fi
	exit 0
else
	test_strings=(
		"basic"
		"ls -l | <<i1 wc -l >o1 | <i2 ls >> o2"
		"several ios with/without space and different order"
		"ls -l | >o1<i1<i2<i3 wc -l | ls >>     o2 >o3"
		"starting with pipe"
		"| <i wc -l >o | ls >> o2"
		"starting with pipe with space"
		"  | <i wc -l >o | ls >> o2"
		"finishing with pipe"
		"<i wc -l >o | ls >> o2 | "
		"starting with pipe with space"
		"<i wc -l >o | ls >> o2 |"
		"no command given"
		"<<EOF cat1 | <i1 >o1 | <i2 cat2 >o2 >>o3"
		"pipe and quotes in quotes"
		"echo -n 't|es\"|\"\"t' >o1 | cat -e >>o2"
		"several quotes without space"
		"echo -n 't|es\"|\"\"t'\"test\" >o1 | cat -e >>o2"
		"several quotes with space"
		"echo -n 't|es\"|\"\"t' 'test' >o1 | cat -e >>o2"
		"quote in file name"
		"<'input' cat | echo -n >>'output'"
		"incomplete quote"
		"<'input' cat | echo -n \"hi 'hi' >>'output'"
	)
fi


# add_line_breaks() {
#     local input="$1"
#     local output=""
#     while IFS= read -r line; do
#         output+="$line\$"
#         output+="\n"
#     done <<< "$input"
#     echo -e "$output"
# }

for ((i = 0; i<${#test_strings[@]}; i +=2)); do
	if ((i+1 < ${#test_strings[@]})); then

	string1=${test_strings[$i]}
	string2=${test_strings[$((i+1))]}
	printf "${BOLD_PURPLE}case $((i/2 + 1)) [$string1]${NC}"
	printf "\n${BOLD_YELLOW}input:\n${YELLOW}$string2\n"

	output=$(./minishell "$string2" 2>&1)
	printf "${BOLD_GREEN}tree:\n${NC}${output}\n"
	# formatted_output=$(add_line_breaks "$output")
	# printf "${BOLD_GREEN}tree:\n${NC}${formatted_output}\n"

	valgrind --leak-check=full ./minishell "$string2" 1>/dev/null 2> valgrind_output.txt
	if grep -q "ERROR SUMMARY: [^0][0-9]* errors" valgrind_output.txt; then
		printf "${BOLD_RED}Memory errors detected:${NC}\n\n"
		grep "ERROR SUMMARY: [^0][0-9]* errors" valgrind_output.txt
	else
		printf "${BOLD_GREEN}Memory [OK]${NC}\n\n"
	fi

	sleep 1
	fi
done