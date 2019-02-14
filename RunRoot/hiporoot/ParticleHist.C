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
    _mapOfParts["CVT"]="p->trk(CVT)->";

    //Particles
    _mapOfParts["PBANK"]="p->par()->";

    //Particle Region
    _mapOfParts["P"]="p->";

    //EVENT
    _mapOfParts["EVNT"]="c12.head()->";

  }
  ParticleHist::~ParticleHist(){
    }
  void ParticleHist::Loop(){
    while(IsMoreFiles())
      gHipoRootAction->Action(this);
    
  }
  TString ParticleHist::ExpandExpression(TString varExp0,TString seperator){
    varExp0.ReplaceAll(" ","");
    varExp0.ReplaceAll("::","@@");
    varExp0.ReplaceAll("()","{}");
    varExp0=AddParenthesis(varExp0);
    
    auto exps = varExp0.Tokenize(":");
    auto Nexp = exps->GetEntries();
    TString varExp1;
    for(Int_t i=0;i<Nexp;i++){
      if(i>0) varExp1+=seperator;
      TString exp = exps->At(i)->GetName();
      varExp1+=ExpandParenthesis(exp,seperator);
    }
    delete exps;
    varExp1.ReplaceAll("@@","::");
    varExp1.ReplaceAll("{}","()");
    cout<<"DONE TString ParticleHist::ExpandExpersion "<<varExp1<<endl;
    return varExp1;
  }
  
  TString ParticleHist::ExpandVars(TString varExp0,TString seperator){
  
    // cout<<"TString ParticleHist::ExpandVars "<<varExp0<<endl;
  
      
    TString varExp1;
    auto exp=varExp0;
    
    auto symbols=RemoveArithmetic(exp);
    auto plusses = exp.Tokenize("#");//+-/* replaced with |
    Int_t Npl=plusses->GetEntries();
    if(Npl){
      Int_t ns=0;
      if(exp[0]=='#') varExp1+=symbols[ns++];//e.g. negative
      
      for(Int_t ipl=0;ipl<plusses->GetEntries();ipl++){
	if(ipl>0)varExp1+=symbols[ns++];
	varExp1+=ExpandPart(plusses->At(ipl)->GetName());
      }
    }
 
    //  else varExp1+=ExpandPart(exp);
    delete plusses;
    
    return varExp1;
  }
  ///////////////////NEED TO FIX FOR FTOF1B.Path<700 conditions
  TString ParticleHist::ExpandPart(TString exp){
    // cout<<" :ExpandPart "<<exp<<endl;
    if(exp.IsFloat()) return exp;
    exp.ReplaceAll(" ","");
    if(exp.Sizeof()==3&&exp[0]=='F'&&exp[1]=='T') return exp;
    if(exp.Sizeof()==3&&exp[0]=='F'&&exp[1]=='D') return exp;
    if(exp.Sizeof()==3&&exp[0]=='C'&&exp[1]=='D') return exp;
    
    auto parts = exp.Tokenize(".");
    if(parts->GetEntries()!=2) return exp;
    TString part= parts->At(0)->GetName();
    exp=Form("%sget%s()",_mapOfParts[part].Data(),parts->At(1)->GetName());
    delete parts;
    return exp;
  }

  vector<TString> ParticleHist::RemoveArithmetic(TString& expr){
    vector<TString> symbols;
    
    const vector<char> operators1={'+','-','/','*','>','<','!'};
    const vector<TString> operators2={"==","!=",">=","<=","&&","||"};
    for(Int_t i=0;i<expr.Sizeof();i++){
      Bool_t got2=kFALSE;
      for(auto& op: operators2){
	if(expr(i,2)==op){
	  expr(i,2)="# ";
	  symbols.push_back(op);
	  i++;
	  got2=kTRUE;
	  break;
	}
      }
      if(got2) continue;
      for(auto& op: operators1)
	if(expr[i]==op){
	  expr[i]='#';
	  symbols.push_back(op);
	  break;
	}
    }
    return symbols;
  }
  TString ParticleHist::ExpandParenthesis(TString varExp0,TString seperator){
    // cout<<"ParticleHist::ExpandParenthesis "<<varExp0<<endl;
    Int_t first = varExp0.First('(');
    TString varExp1;
    
    if(first==-1){
      varExp1=varExp0; //no brackets
      varExp1=ExpandVars(varExp1,seperator);
      return varExp1;
   }
   else {  //expand parenthesis
      Int_t nb=0;
      Int_t nc=0;
      Int_t second=0;
      first=-1;
      for(Int_t i=0;i<varExp0.Sizeof();i++){

	if(varExp0[i]=='('){
	  nb++;
	  if(first==-1){ first=i;
	    varExp1+=varExp0[i];
	  }
	}
	else if(varExp0[i]==')') nb--;

	else if(varExp0[i]==','){ //function commas
	  if(first==-1){
	    first=i;
	    nc++;
	  }
	  else{
	    second=i;
	    varExp1+=Form("%s",ExpandParenthesis(varExp0(first+1,second-first-1),seperator).Data());
	    first=i;
	    second=0;
	    varExp1+=',';
	    nc--;
	  
	  }
	}
	else if(first==-1) varExp1+=varExp0[i];//just copy text if betweeh parenthesis
	if(nb==0&&first!=-1){
	  second=i;
	  if(nc==0) varExp1+=Form("%s)",ExpandParenthesis(varExp0(first+1,second-first-1),seperator).Data());
	  else  varExp1+=Form("%s)",ExpandParenthesis(varExp0(first+1,second-first-1),seperator).Data());
	  first=-1;
	  second=0;
	}
      }
    }
    varExp1=Form("%s",varExp1.Data());
     
    //   cout<< "EXPANDED EXPRESSION PARENTHESIS"<<varExp1.Data()<<" "<<varExp1.Sizeof()<<endl;
    return varExp1;
  }

  TString ParticleHist::AddParenthesis(TString varExp0){ //surround XXX.YYY with ( )
    if(!varExp0.Contains('.')) return varExp0;
    TString varExp1=varExp0;
    const Int_t Nc=varExp0.Sizeof();
    TString alpha;
    for (Int_t i=0;i<Nc-1;i++){
       if(varExp0[i]=='.'){
	if(i==0) continue;
	Int_t iright=0;
	Int_t ileft=0;
	Int_t ia=i+1;
	alpha=varExp0[ia];
	while(alpha.IsAlnum()&&ia<Nc){
	  ia++;
	  alpha=varExp0[ia];
	}
	if(ia!=i+1){
	  iright=ia;
	  if(varExp0[iright]==')') //check if already got )
	    iright=0;
	}
	ia=i-1;
	alpha=varExp0[ia];
	while(alpha.IsAlnum()&&ia>0){
	  ia--;
	  alpha=varExp0[ia];
	}
	if(ia!=i-1){
	  // if(ia==i-2)ileft=ia+1;
ileft=ia+1;	  // else ileft=ia;
	  if(ileft-1>=0)
	    if(varExp0[ileft-1]=='(') //check if already got )
	      iright=0;
	}
	  
	if(iright!=i+1&&iright!=0){
	  if(ileft==1) ileft=0;
	  TString expr=varExp0(ileft,iright-ileft);
	  varExp1.ReplaceAll(expr.Data(),Form("(%s)",expr.Data()));
	  i++;
	}
      }
    }
    return varExp1;
      
  }
}
