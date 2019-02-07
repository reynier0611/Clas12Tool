# Clas12Tool

Data Analysis Tools based on HIPO data format. Implementations
for CLAS12 data structures and convertor to ROOT.

## To Download

git clone --recurse-submodules https://github.com/dglazier/Clas12Tool.git

## To setup Run ROOT
cd Clas12Tool
setenv CLAS12TOOL $PWD

cd RunRoot
alias clas12reader root importToROOT.C

##to run example
clas12reader Ex1_CLAS12Reader.C --in=/path/to/hipo/file.hipo (accepts wildcards)