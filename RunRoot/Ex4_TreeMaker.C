{
  hiporoot::ParticleTree treemaker("/work/dump/filtered/clas_004152.recon.hipo","test.root");

  treemaker.Branch("P.Time/F");
  treemaker.Branch("PBANK.Px/F");
  treemaker.Branch("PBANK.Py/F");
  treemaker.Branch("PBANK.Pz/F");
  treemaker.Branch("PBANK.Vx/F");
  treemaker.Branch("PBANK.Vy/F");
  treemaker.Branch("PBANK.Vz/F");
  treemaker.Branch("PBANK.Pid/I");

  // treemaker.AddParticleCut("PBANK.Pid==11");
  
  treemaker.Fill();
}
