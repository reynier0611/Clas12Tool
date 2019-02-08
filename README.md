# Clas12Tool

Data Analysis Tools based on HIPO data format. Implementations
for CLAS12 data structures and convertor to ROOT.

## To Download

git clone --recurse-submodules https://github.com/dglazier/Clas12Tool.git
git checkout mesonex4

## To setup Run ROOT
cd Clas12Tool
setenv CLAS12TOOL $PWD

cd RunRoot
alias clas12reader root -l $CLAS12TOOL/RunRoot/importToROOT.C

##to run example
clas12reader Ex1_CLAS12Reader.C --in=/path/to/hipo/file.hipo (accepts wildcards)


###########################################################
HipoRoot::Draw

alias hipodraw root -l  $CLAS12TOOL/RunRoot/hiporoot/LoadHipoROOT.C

hipodraw

root [0] 
Processing /work/clas12/Clas12Tool2//RunRoot/hiporoot/LoadHipoROOT.C...
root [1] ParticleHist ph("my/hipo/file.hipo")

Examples :

   ph.Hist1D("FTOF1B.Time-FTOF2.Time",1000,-500,500)->Draw("hist")

   ph.Hist1D("P.Px*P.Px+P.Py*P.Py+P.Pz*P.Pz",1000,-500,500)->Draw()


   ph.Hist1D("CND1.Time",1000,1,500)->Draw()
   ph.Hist1D("CTOF.Time",1000,1,500)->Draw("same")

Or Lazy execution
   
   ph.Hist2D("FTOF1B.Path:FTOF2.Path",100,0,1000,100,0,1000)
   ph.Hist2D("FTOF1B.Time:FTOF2.Time",100,0,1000,100,0,1000)->Draw("col1")


   ph.Save("output.root")	