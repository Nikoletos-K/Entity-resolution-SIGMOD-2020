#!/bin/bash
GREEN="\033[0;32m"
RED="\033[31m"
RESET="\033[0m"

# Arrays for first name and last name
learningRate_array=(0.001)
epochs_array=(50)
batch_array=(2056)
euThreshold_array=(0.0001)
rtThreshold_array=(0.02 0.05)
nt_array=(20)
rl_array=(3 9)
# rt_array=(0.02 0.05)
sv_array=(0.01)

# Getting the size of the arrays
learningRate_size=${#learningRate_array[@]}
epochs_size=${#epochs_array[@]}
batch_size=${#batch_array[@]}
euThreshold_size=${#euThreshold_array[@]}
rtThreshold_size=${#rtThreshold_array[@]}
nt_size=${#nt_array[@]}
rl_size=${#rl_array[@]}
rt_size=${#rt_array[@]}
sv_size=${#sv_array[@]}

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

cd ..;
cd ./programs/EntityResolution/;

echo -n -e "\n";
echo -n -e "Starting executing the system for multiple variables\n";
counter=1;
allLoops=`expr $learningRate_size \* $epochs_size \* $batch_size`;
for lr in ${learningRate_array[@]}; do
	for e in ${epochs_array[@]}; do
		for b in ${batch_array[@]}; do
			echo -n "["
			for euT in ${euThreshold_array[@]}; do
				for rtT in ${rtThreshold_array[@]}; do
					for nT in ${nt_array[@]}; do
						for sv in ${sv_array[@]}; do
							for rl in ${rl_array[@]}; do
								./entityResolutionPairs -jd ./../../data/camera_specs -csv ./../../data/sigmod_large_labelled_dataset.csv -lr $lr -sv $sv -et $euT -rt $rtT -ep $e -rl $rl -bs $b -nt $nT
								echo -n -e "\033[0;34m#$RESET";
							done
						done
					done
				done
			done
			echo -n -e "] $counter / $allLoops \n" 
			((counter=counter+1))
		done
	done
done
echo -e "\033[0;34m<-$RESET End";

