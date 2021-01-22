#!/bin/bash
GREEN="\033[0;32m"
RED="\033[31m"
RESET="\033[0m"

# Arrays for first name and last name
learningRate_array=(0.1 0.01 0.001 0.0001)
epochs_array=(20 50)
batch_array=(516 1024 2056)
euThreshold_array=(0.1 0.01 0.001 0.0001)
rtThreshold_array=(0.01 0.02)
nt_array=(10 20)
rl_array=(3 5 8 10)
rt_array=(0.01)

# Getting the size of the arrays
learningRate_size=${#learningRate_array[@]}
epochs_size=${#epochs_array[@]}
batch_size=${#batch_array[@]}
euThreshold_size=${#euThreshold_array[@]}
rtThreshold_size=${#rtThreshold_array[@]}
nt_size=${#nt_array[@]}
rl_size=${#rl_array[@]}
rt_size=${#rt_array[@]}


function good() { echo -e "$GREEN [ PASS ]  $RESET"; }
function bad()  { echo -e "$RED [ FAIL ] $RESET"; exit 1; }

# function that checks the file properties
function fileCheck(){
	echo -n "  -Exists " 
	if [ -e $1 ]; 
		then good
	else
		bad
	fi
	
	echo -n "  -Regular file "
	if [ -f $1 ]; 
		then good
	else
		bad
	fi
	
	echo -n "  -Read permission "
	if [ -r $1 ]; 
		then good
	else
		bad
	fi

	echo -n "  -File not empty "
	if [ -s $1 ]; 
		then good
	else
		bad
	fi

}

# function that checks directory properties
function directoryCheck(){
	if [ -e $1 ]; 
		then echo "Directory $1 already exists"
		echo -n "- Want to replace it? [y/n] "
		read answer
		if [ $answer == "y" ];
			then rm -r $1
		else
			echo -n Script ended
			bad
		fi
	fi	
	mkdir $1
	echo -n -e "Directory $INPUT_DIRECTORY created"
	good
}

echo -e "\033[0;34m->$RESET Start";
echo -e "\033[0;34m->$RESET Script information:";
echo  "  - Number of input parameters = $#"
echo  "  - All arguments: $* "

numOfArguments=$#
learningRate_array=(0.1 0.01 0.001 0.0001)
epochs_array=(20 50)
batch_array=(516 1024 2056)
euThreshold_array=(0.1 0.01 0.001 0.0001)
rtThreshold_array=(0.01 0.02)
nt_array=(10 20)
rl_array=(3 5 8 10)
rt_array=(0.01)

# learningRate_array=(0.1)
# epochs_array=(20)
# batch_array=(516)
# euThreshold_array=(0.1)
# rtThreshold_array=(0.01)
# nt_array=(10)
# rl_array=(3)
# rt_array=(0.01)
# Creation loop
	# Reading sorted files from an array
cd ..;
make;
cd ./programs/EntityResolution/;
pwd;
echo -n -e "\n"; 
for lr in ${learningRate_array[@]}; do
	for e in ${epochs_array[@]}; do
		for b in ${batch_array[@]}; do
			echo -n "["
			for euT in ${euThreshold_array[@]}; do
				for rtT in ${rtThreshold_array[@]}; do
					for nT in ${nt_array[@]}; do
						echo -n -e "\033[0;34m#$RESET";
						for rl in ${rl_array[@]}; do
							for rt in ${rt_array[@]}; do
								./entityResolutionPairs -jd ./../../data/camera_specs -csv ./../../data/sigmod_medium_labelled_dataset.csv -lr $lr -et $e -rt $rtT -ep $e -rl $rl -bs $b -nt $nT > execute.txt;
							done
						done
					done
				done
			done
			echo -n -e "] \n" 
		done
	done
done
echo -e "\033[0;34m<-$RESET End";

