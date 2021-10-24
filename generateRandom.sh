# echo "${1}.txt"
touch ./Inputs/"${1}.txt"
echo $1 > ./Inputs/"${1}.txt"
g++ generateRandomInput.cpp -o generateRandomInput.out
./generateRandomInput.out < ./Inputs/"${1}.txt" >> ./Inputs/"${1}.txt"