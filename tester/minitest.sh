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

current_dir=$(pwd)
cd ../
make
cd "$current_dir" 

#create set input and output file


directory="t"
if [ ! -d "$directory" ]; then
    mkdir "$directory"
    echo "Directory $directory created."
fi

in=(
	"$directory/i0"
	"$directory/i1"
	"$directory/i2"
	"$directory/i3"
)

for input_file in "${in[@]}"; do
	if [ ! -f "$input_file" ]; then
    touch "$input_file"
    echo "Test input file created: $input_file"
	fi
done

out=(
	"$directory/o0"
	"$directory/o1"
	"$directory/o2"
	"$directory/o3"
)

filename="args"

# for user input
if [[ $# -gt 0 ]]; then
	string=( "$@" )
	printf "${BOLD_YELLOW}input:\n${YELLOW}$string\n"
	printf "${BOLD_GREEN}tree:\n${NC}"

	valgrind --leak-check=full --show-leak-kinds=all -s --log-file="$directory"/valgrind_output.txt ../minishell "$string"
	# valgrind --leak-check=full --show-leak-kinds=all -s  ../minishell "$string"

	if [ -f "$filename" ]; then
		printf "${BOLD_GREEN}args:${NC}\n"
		cat "$filename"
	else
		echo "$filename does not exist."
	fi

	if grep -q "ERROR SUMMARY: [^0][0-9]* errors" "$directory"/valgrind_output.txt || grep -q "LEAK SUMMARY" "$directory"/valgrind_output.txt; then
		printf "${BOLD_RED}Memory errors detected${NC}\n\n"
		# grep "ERROR SUMMARY: [^0][0-9]* errors" "$directory"/valgrind_output.txt
	else
		printf "${BOLD_GREEN}Memory [OK]${NC}\n\n"
	fi
	exit 0
else
	test_strings=(
		"basic"
		"ls -l | <<EOF wc -l >${out[0]} | <${in[1]} ls >> ${out[1]}"
		"several ios with/without space and different order"
		"ls -l | >${out[0]}<${in[0]}<${in[1]}<${in[2]} wc -l | <<EOF ls >>     ${out[1]} >${out[2]}"
		"starting with pipe"
		"| <${in[0]} wc -l >${out[0]} | ls >> ${out[1]}"
		"starting with pipe with space"
		"  | <${in[0]} wc -l >${out[0]} | ls >> ${out[1]}"
		"finishing with pipe"
		"<${in[0]} wc -l >${out[0]} | ls >> ${out[1]} | "
		"finishing with pipe with space"
		"<${in[0]} wc -l >${out[0]} | ls >> ${out[1]} |"
		"no command given"
		"<<EOF cat | <${in[0]} >${out[0]} | <${in[1]} cat >${out[1]} >>${out[2]}"
		"pipe and quotes in quotes"
		"echo -n 't|es\"|\"\"t' >${out[0]} | cat -e >>${out[1]}"
		"several quotes without space in between"
		"echo -n 't|es\"|\"\"t'\"test\" >${out[0]} | cat -e >>${out[1]}"
		"several quotes with space in between"
		"echo -n '    t|es\"   |\"\"t    ' 'test' >${out[0]} | cat -e >>${out[1]}"
		"quote in file name"
		"<'input' cat | echo -n >t1/'output' >>${out[0]}"
		"quote in here doc"
		"<<'input' cat | echo -n >>${out[0]}"
		"incomplete quote"
		"<'input' cat | echo -n \"hi 'hi' >>${out[0]}"
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




#test starts
for ((i = 0; i<${#test_strings[@]}; i +=2)); do
	if ((i+1 < ${#test_strings[@]})); then

	string1=${test_strings[$i]}
	string2=${test_strings[$((i+1))]}
	printf "${BOLD_PURPLE}case $((i/2 + 1)) [$string1]${NC}"
	printf "\n${BOLD_YELLOW}input:\n${YELLOW}$string2\n"

	# output=$(./minishell "$string2" 2>&1)
	printf "${BOLD_GREEN}tree:\n${NC}"
	# formatted_output=$(add_line_breaks "$output")
	# printf "${BOLD_GREEN}tree:\n${NC}${formatted_output}\n"

	valgrind --leak-check=full --show-leak-kinds=all -s --log-file="$directory"/valgrind_output$((i/2 + 1)).txt ../minishell "$string2"

	if [ -f "$filename" ]; then
		printf "${BOLD_GREEN}args:${NC}\n"
		cat "$filename"
	else
		echo "$filename does not exist."
	fi

	if grep -q "ERROR SUMMARY: [^0][0-9]* errors" "$directory"/valgrind_output$((i/2 + 1)).txt || grep -q "LEAK SUMMARY" "$directory"/valgrind_output$((i/2 + 1)).txt; then
		printf "${BOLD_RED}Memory errors detected${NC}\n\n"
	else
		printf "${BOLD_GREEN}Memory [OK]${NC}\n\n"
	fi
	# sleep 1
	fi

done

rm args