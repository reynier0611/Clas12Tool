#include "ParticleHist.h"
#include "HipoRootAction.h"
#include "clas12reader.h"
#include <TObjArray.h>

namespace hiporoot {

  ParticleHist::ParticleHist(TString filename): HipoHist(filename){
    _tempActionName="ParticleHistAction";

    //Calorimters
    _mapOfParts["ECIN"]="p->cal(ECIN)->";
    _mapOfParts["ECOUT"]="p->cal(ECOUT)->";
    _mapOfParts["PCAL"]="p->cal(PCAL)->";

    //Scintillators
    _mapOfParts["FTOF1A"]="p->sci(FTOF1A)->";
    _mapOfParts["FTOF1B"]="p->sci(FTOF1B)->";
    _mapOfParts["FTOF2"]="p->sci(FTOF2)->";
    _mapOfParts["CTOF"]="p->sci(CTOF)->";
    _mapOfParts["CND1"]="p->sci(CND1)->";
    _mapOfParts["CND2"]="p->sci(CND2)->";
    _mapOfParts["CND3"]="p->sci(CND3)->";

    //FT
    _mapOfParts["FTCAL"]="p->ft(FTCAL)->";
    _mapOfParts["FTHODO"]="p->ft(FTHODO)->";

    //Cherenkov
    _mapOfParts["HTCC"]="p->ft(HTCC)->";
    _mapOfParts["LTCC"]="p->ft(LTCC)->";

    //Trackers
    _mapOfParts["DC"]="p->trk(DC)->";

    //Particles
    _mapOfParts["P"]="p->par()->";
 

  }
  ParticleHist::~ParticleHist(){
    }
  void ParticleHist::Loop(){
    //_c12.reset(new clas12reader(HipoFileName().Data()));
    cout<<"LOOP "<<gHipoRootAction.get()<<endl;
    gHipoRootAction->Action(this);
    
  }

  TString ParticleHist::ExpandVars(TString varExp0,TString seperator){
  

    auto exps = varExp0.Tokenize(":");
    auto Nexp = exps->GetEntries();
    TString varExp1;
    for(Int_t i=0;i<Nexp;i++){
      if(i>0) varExp1+=seperator;
      TString exp = exps->At(i)->GetName();
      //look for arithmetic operations
      //+++++++++++++++++++++++++++++++++
      TString symbols=RemoveArithmetic(exp);
      auto plusses = exp.Tokenize("|");//+-/* replaced with |
      Int_t Npl=plusses->GetEntries();
      if(Npl){
	for(Int_t ipl=0;ipl<plusses->GetEntries();ipl++){
	  if(ipl>0) varExp1+=symbols[ipl-1];
	  varExp1+=ExpandPart(plusses->At(ipl)->GetName());
	}
      }
      //look for detector and variable
      if(Npl==0) varExp1+=ExpandPart(exp);
      delete plusses;
    }
    delete exps;
    return varExp1;
  }
  
  TString ParticleHist::ExpandPart(TString exp){
    auto parts = exp.Tokenize(".");
    TString part= parts->At(0)->GetName();
    exp=Form("%sget%s()",_mapOfParts[part].Data(),parts->At(1)->GetName());
      delete parts;
    return exp;
  }

  TString ParticleHist::RemoveArithmetic(TString& expr){
    TString symbols;
    for(Int_t i=0;i<expr.Sizeof();i++){
      if(expr[i]=='+'){
	expr[i]='|';
	symbols.Append('+');
      }
      if(expr[i]=='-'){
	expr[i]='|';
	symbols.Append('-');
      }
      if(expr[i]=='/'){
	expr[i]='|';
	symbols.Append('/');
      }
      if(expr[i]=='*'){
	expr[i]='|';
	symbols.Append('*');
      }
    }
    return symbols;
  }
}
