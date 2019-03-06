//currently runs with clas12reader '$CLAS12TOOL/RunRoot/hiporoot/LoadProof.C(Nworkers)' Ex3_ProcessSelector.C

{
  hiporoot::HipoChain chain;
  chain.Add("/work/dump/filtered/*hipo");
  //  chain.Add("my/hipo/files*.hipo");
 
  hiporoot::HipoSelector sel(&chain);
  gBenchmark->Start("proof");
  gProof->Process(&sel,chain.GetNRecords());
  gBenchmark->Stop("proof");
  gBenchmark->Print("proof");
  
}
