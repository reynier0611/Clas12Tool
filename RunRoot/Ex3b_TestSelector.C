//usage :
//clas12root  4 myFirstSelector.C Ex3_ProofLite.C
//Where 4  is however many workers you would like
//myFirstSelector.C is the source dode of the selector
//   this can be created with any name using makeHipoSelector executable
//Note you must also change myFirstSelector in the macro below to your
//selector name
{
  clas12root::HipoChain chain;
  chain.Add("/work/jlab/clas12data/skim4_5038.hipo");
  //chain.Add("/home/dglazier/fastdata/hipo4test/skim1_inclusive.hipo");
  clas12root::testSelector sel(&chain);

  gProof->Process(&sel,chain.GetNRecords());
}
