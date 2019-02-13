#include "HipoHist.h"
#include <TBenchmark.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TFile.h>

namespace hiporoot {
  
  HipoHist::HipoHist(TString filename): HipoROOTOut(filename){
    _histList->SetName("HipoHists");
    _histList->SetOwner(kFALSE);
    
  }
  HipoHist::~HipoHist(){
 }
  HipoHist* HipoHist::Hist1D(TString varExp,Int_t nbins,Float_t min,Float_t max,TString condExp){

    varExp=ExpandExpression(varExp,",");
    if(condExp==TString(""))condExp="1";
    if(condExp!=TString("1")) condExp=ExpandExpression(condExp,"");
    cout<<"HIST!D "<<varExp<<" "<<varExp.Data()<<" "<<varExp.Sizeof()<<endl;
    AddAction(varExp,condExp);
    auto hist=new TH1F(varExp,varExp,nbins,min,max);
    _histList->Add(hist);
    _curHist.push_back(hist);   
    return this;
  }
  HipoHist* HipoHist::Hist2D(TString varExp,Int_t nbinsX,Float_t minX,Float_t maxX,Int_t nbinsY,Float_t minY,Float_t maxY,TString condExp){
    
    varExp=ExpandExpression(varExp,","); 
    if(condExp==TString(""))condExp="1";
    if(condExp!=TString("1")) condExp=ExpandExpression(condExp,""); 
    AddAction(varExp,condExp);
    auto hist=new TH2F(varExp,varExp,nbinsX,minX,maxX,nbinsY,minY,maxY);
    _histList->Add(hist);
    _curHist.push_back(hist);   
    return this;
  }
  
  void HipoHist::Draw(const TString opt){
    gBenchmark->Reset();
    gBenchmark->Start("compile time");
    CompileAction();
    gBenchmark->Stop("compile time");
    gBenchmark->Print("compile time");
    gBenchmark->Start("loop time");
    Loop();
    gBenchmark->Stop("loop time");
    gBenchmark->Print("loop time");
    OnCanvas(opt);
    CleanAction();
    _curHist.clear();
  }
  
  void HipoHist::OnCanvas(const TString opt){

    if(_curHist.size()==1){
      if(!gPad) new TCanvas();
      _curHist[0]->DrawCopy(opt);
      return;	
    }
      
    for(auto* hist : _curHist){
      if(!opt.Contains("same")){
	new TCanvas();
      }
      hist->DrawCopy(opt);
    }
  }
  void HipoHist::Save(const TString outname){
    auto outfile=TFile::Open(outname,"recreate");
    outfile->WriteTObject(_histList);
    delete outfile;
  }
}
