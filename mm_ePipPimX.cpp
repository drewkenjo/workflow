#include<iostream>
#include<TChain.h>
#include<TCanvas.h>
#include<ROOT/RDataFrame.hxx>
#include<Math/VectorUtil.h>
#include<Math/Vector3D.h>
#include<Math/Vector4D.h>


int main(int argc, char** argv) {
  double mele = 0.00051099891, mpip = 0.13957, mpim = 0.13957, mpro = 0.938272;
  auto beam = ROOT::Math::PxPyPzMVector(0,0,10.604,mele);
  auto targ = ROOT::Math::PxPyPzMVector(0,0,0,mpro);

  TChain h22("h22");
  for(int iarg=1;iarg<argc;iarg++)
    h22.Add(argv[iarg]);
  ROOT::RDataFrame df(h22);
  auto rdf = df.Define("mePipPimX", [&](float ex,float ey,float ez,float pipx,float pipy,float pipz,float pimx,float pimy,float pimz) {
    auto ele = ROOT::Math::PxPyPzMVector(ex,ey,ez,mele);
    auto pip = ROOT::Math::PxPyPzMVector(pipx,pipy,pipz,mpip);
    auto pim = ROOT::Math::PxPyPzMVector(pimx,pimy,pimz,mpim);

    auto ePipPimX = beam+targ - ele-pip-pim;
    double mePipPimX = ePipPimX.mass();
    return mePipPimX;
  }, {"ex","ey","ez","pipx","pipy","pipz","pimx","pimy","pimz"});


  auto h1 = rdf.Histo1D({"mePipPimX","missing mass",200,0.6,1.4}, "mePipPimX");

  auto c1 = new TCanvas("c1","c1",1100,800);
  h1->Draw();
  c1->Print("mm.pdf");

  return 0;
}
