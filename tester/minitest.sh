#!/bin/bash

# ============================================================
# Section: Initialization

# Basic text colors
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

# compile the program
current_dir=$(pwd)
cd ../
make
cd "$current_dir" 

# create input directory and files
dir="t" #test
if [ ! -d "$dir" ]; then
    mkdir "$dir"
    echo "Directory $dir created."
else
	rm "${dir}"/*
fi

in=(
	"$dir/i0"
	"$dir/i1"
	"$dir/i2"
	"$dir/i3"
)

for input_file in "${in[@]}"; do
	if [ ! -f "$input_file" ]; then
 	   touch "$input_file"
	fi
    # echo "Test input file created: $input_file"
done

# set file names

out=(
	"$dir/o0"
	"$dir/o1"
	"$dir/o2"
	"$dir/o3"
)

args_file="args.txt"

#preset test strings
#guide: use ${in[0~3]} for input file name
#       and ${out[0~3]} for output file name

preset_strings=(
#case 1
		"<< & >>"
		"ls -l | <<EOF wc -l >${out[0]} | <${in[1]} ls >${out[1]} >> ${out[1]}"
#case 2
		"several ios with/without space and different order"
		"ls -l | >${out[0]}<${in[0]}<${in[1]}<${in[2]} wc -l | <<EOF ls >>     ${out[1]} >${out[2]}"
#case 3
		"starting with pipe" #3
		"| <${in[0]} wc -l >${out[0]} | ls >> ${out[1]}"
#case 4
		"starting with pipe with space"
		"  | <${in[0]} wc -l >${out[0]} | ls >> ${out[1]}"
#case 5
		"finishing with pipe"
		"<${in[0]} wc -l >${out[0]} | ls >> ${out[1]} | "
#case 6
		"finishing with pipe with space"
		"<${in[0]} wc -l >${out[0]} | ls >> ${out[1]} |"
#case 7
		"no command given"
		"<<EOF cat | <${in[0]} >${out[0]} | <${in[1]} cat >${out[1]} >>${out[2]}"
#case 8
		"pipe and quotes in quotes"
		"echo -n 't|es\"|\"\"t' >${out[0]} | cat -e >>${out[1]}"
#case 9
		"several quotes without space in between"
		"echo -n 't|es\"|\"\"t'\"test\" >${out[0]} | cat -e >>${out[1]}"
#case 10
		"several quotes with space in between"
		"echo -n '    t|es\"   |\"\"t    ' 'test' >${out[0]} | cat -e >>${out[1]}"
#case 11
		"quote in file name"
		"<'input' cat | echo -n >non-existing/'output' >>${out[0]}"
#case 12
		"quote in here doc"
		"<<'input' cat | echo -n >>${out[0]}"
#case 13
		"incomplete quote"
		"<'input' cat | echo -n \"hi 'hi' >>${out[0]}"
#case 14
		"nonexisting command"
		"ls -l | nonexist >${out[0]}"
#case 15
		"existing command with a wrong directory"
		"ls -l | /cat -e >${out[0]}"
	)

#ser variables

starting_num=0
user=false

# ============================================================
# ============================================================
# Section: Test Category


# exit if too many argument
if [[ $# -gt 1 ]]; then
    echo "$0 only takes one or 0 argument"
    exit 1
fi

#in case of no input, ask for a user input
if [[ $# -eq 0 ]]; then
	echo "Please enter a command:"
	read -r user_input
	string=("user input" "$user_input")
	size=2
	user=true
fi

#in case of one input

if [[ $# -eq 1 ]]; then
	cases=$1
	string=("${preset_strings[@]}")

	if [[ "$cases" = "all" ]]; then
		size=${#string[@]}
	else
		if ! [[ "$cases" =~ ^[0-9]+$ ]]; then
			echo "Invalid input. Please enter 'all' or valid number."
			exit 1
		else
			starting_num=$((cases*2 - 2))
			size=$((starting_num + 2))
		fi
	fi
fi

# ============================================================
# ============================================================
# Section: Test

for ((i = starting_num; i<size; i +=2)); do
	if ((i+1 < size)); then

#clean outputs from a previous case
	for output_file in "${out[@]}"; do
		if [ -f "$dir/$output_file" ]; then
			rm "$dir/$output_file"
		fi
	done

	string1=${string[$i]}
	printf "${BOLD_PURPLE}case $((i/2 + 1)) [$string1]${NC}"

	string2=${string[$((i+1))]}
	printf "\n${BOLD_YELLOW}input:\n${YELLOW}$string2\n"

	# to print out tree
	printf "${BOLD_GREEN}output:\n${NC}"

	valgrind --leak-check=full --show-leak-kinds=all -s --log-file="$dir"/val_case$((i/2 + 1)) ../minishell "$string2"

	if [ -f "$args_file" ]; then
		printf "${BOLD_GREEN}args:${NC}\n"
		cat "$args_file"
	else
		echo "$args_file does not exist."
	fi

	myoutput=false
	for output_file in "${out[@]}"; do
		if [ -f "$output_file" ]; then
			myoutput=true
			cp "$output_file" "${output_file}_case$((i/2 + 1))"
			rm "$output_file"
		fi
	done

	if grep -q "ERROR SUMMARY: [^0][0-9]* errors" "$dir"/val_case$((i/2 + 1)) || grep -q "LEAK SUMMARY" "$dir"/val_case$((i/2 + 1)); then
		printf "${BOLD_RED}Memory errors detected${NC}\n\n"
	else
		printf "${BOLD_GREEN}Memory: [OK]${NC}\n"
	fi

	if [[ user=false ]]; then

		eval "$string2" 1>"/dev/null" 2>"/dev/null"
		printf "${BOLD_GREEN}Bash: ${NC}"
		bashoutput=false
		diff=NA
		for output_file in "${out[@]}"; do
			if [ -f "$output_file" ]; then
				bashoutput=true
				diff=true
				diff "$output_file" "${output_file}_case$((i/2 + 1))"  1>"$dir/bash_diff_case$((i/2 + 1))"
				if [[ $? -eq 0 ]]; then
					:
				elif [[ $? -eq 1 ]]; then
					diff=false
					printf "${BOLD_RED}[KO] ${NC}\n"
					cat "$dir/bash_diff_case$((i/2 + 1))"
					break
				else
					diff=NA
					break
				fi
			fi
		done

		if [ "$diff" = true ]; then
			printf "${BOLD_GREEN}[OK]${NC}\n"
		elif [ "$diff" = false ]; then
			:
		elif [ "$diff" = "NA" ] && [ "$bashoutput" = false ] && [ "$myoutput" = false ]; then
			printf "${BOLD_WHITE}[BOTH NO OUTPUT]\n"
		else
			printf "${BOLD_RED}[KO]\n"
		fi

		for output_file in "${out[@]}"; do
			if [ -f "$output_file" ]; then
				cp "$output_file" "${output_file}_case$((i/2 + 1))_bash"
				rm "$output_file"
			fi
		done
	fi
	printf "\n"
	# sleep 1
	fi
done

# add_line_breaks() {
#     local input="$1"
#     local output=""
#     while IFS= read -r line; do
#         output+="$line\$"
#         output+="\n"
#     done <<< "$input"
#     echo -e "$output"
# }
# formatted_output=$(add_line_breaks "$output")
# printf "${BOLD_GREEN}tree:\n${NC}${formatted_output}\n"

# ============================================================
# ============================================================
# Section: Cleaning up

if [ -f "$args_file" ]; then
	rm "./$args_file"
fi