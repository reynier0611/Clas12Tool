void LoadProof(Int_t Nworkers=1){
  
  TString CLAS12TOOL=gSystem->Getenv("CLAS12TOOL");
  TString hipopath=CLAS12TOOL+"/Hipo4";
  TString bankpath=CLAS12TOOL+"/Banks";
  TString hiproopath=CLAS12TOOL+"/RunRoot/hiporoot";
 
  //Now finalsstate classes
  vector<TString > HIPOClasses={"utils","dictionary","event","record","bank","reader"};
  vector<TString > BANKClasses= {"vectors","header","vtp","scaler","particle","mcparticle","particle_detector","scintillator","tracker","traj","forwardtagger","cherenkov","calorimeter","covmatrix","region_particle","region_ft","region_fdet","region_cdet","clas12reader"};

  vector<TString > HIPOROOTClasses={"HipoChain","HipoSelector","HipoFileSelector"};
  
  if(!TString(gInterpreter->GetIncludePath()).Contains(hipopath)){
    gInterpreter->AddIncludePath(hipopath);
    gROOT->SetMacroPath(Form("%s:%s",gROOT->GetMacroPath(),(hipopath).Data()));
  }
  
  if(!TString(gInterpreter->GetIncludePath()).Contains(bankpath)){
    gInterpreter->AddIncludePath(bankpath);
    gROOT->SetMacroPath(Form("%s:%s",gROOT->GetMacroPath(),bankpath.Data()));
  }
  if(!TString(gInterpreter->GetIncludePath()).Contains(hiproopath)){
    gInterpreter->AddIncludePath(hiproopath);
    gROOT->SetMacroPath(Form("%s:%s",gROOT->GetMacroPath(),hiproopath.Data()));
  }
  
  gSystem->Load("libProof.so");
  TProof *proof =nullptr;
  if(!gProof) 
    proof = TProof::Open("://lite");
  else
    proof=gProof;
  
  Int_t NCores=Nworkers;
  proof->SetParallel(NCores);
  if(gProof->Load(TString(gSystem->Getenv("CLAS12TOOL"))+"/Lz4/lib/liblz4.so",kTRUE)==0){}
  gProof->AddIncludePath(TString(gSystem->Getenv("CLAS12TOOL"))+"/Lz4/lib");
   
  proof->AddIncludePath(hipopath);
  proof->AddIncludePath(bankpath);
  proof->AddIncludePath(hiproopath);
   

  for(auto const& name : HIPOClasses){
     std::cout<<"HHHHHHHHHHHHHHHHHH "<<name<<"\n";
    if(!gROOT->GetListOfClasses()->Contains(name))
      proof->Load(hipopath+"/"+name+".cpp+",kTRUE);
  }
  for(auto const& name : BANKClasses){
    std::cout<<"BBBBBBBBBBBBBBBBBBB "<<name<<"\n";
    if(!gROOT->GetListOfClasses()->Contains(name))
      proof->Load(bankpath+"/"+name+".cpp+",kTRUE);
  }
  for(auto const& name : HIPOROOTClasses){
    std::cout<<"HRHRHRHRHRHRHRHRHRHRH "<<name<<"\n";
    if(!gROOT->GetListOfClasses()->Contains(name))
      proof->Load(hiproopath+"/"+name+".C++",kTRUE);
  }
  gROOT->ProcessLine(".L HipoChain.C+");
  gROOT->ProcessLine(".L HipoSelector.C+");
  gROOT->ProcessLine(".L HipoFileSelector.C+");
 
}
