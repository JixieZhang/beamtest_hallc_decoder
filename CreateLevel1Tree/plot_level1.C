
#include <TROOT.h>
#include <TSystem.h>
#include <TChain.h>
#include <TFile.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TPaveText.h>
#include <TCut.h>

#include <vector>
#include <iostream>
#include <string>
#include <stdlib.h>

const char *Slot7Name[16]= { 
  "CerA0", "CerA1", "CerA2", "CerA3", "CerB0", "CerB1", "CerB2", "CerB3", 
  "CerC0", "CerC1", "CerC2", "CerC3", "CerD0", "CerD1", "CerD2", "CerD3" };
  
const char *Slot8Name[16]= { 
  "SC0_t", "SC0_b", "SC1_t", "SC1_b", "LASPD_t", "LASPD_b", "SPD", "PreSh_l", "PreSh_r", "PreSh_t",
  "Shower_l", "Shower_r", "Shower_t", "notuse", "ShowerSum", "Trig" };

const int kCerPadIndex[16] = {16, 15, 12, 11,  14, 13, 10, 9,  8, 7, 4, 3,  6, 5, 2, 1};


TCut TrigCut="SC0_t>100 && SC1_t>100 && SC0_b>100 && SC1_b>100 && GEM00_n>0 && GEM10_n>0";
TCut ElCut="CerSum>100 && PreShSum>200 && ShowerSum>200";
TCut PiCut="CerSum<25 && PreShSum>200 && ShowerSum>200";
  

void plotEcal(int run)
{
  TTree* T = (TTree*)gROOT->FindObject("T");
  TCanvas *c1= new TCanvas("c1","",900,700);
  c1->Divide(2,2);
  c1->cd(1);
  T->Draw("Shower_l>>h1(250,0,2500)",TrigCut && "Shower_l>20");
  TH1F *h1 = (TH1F*) gROOT->FindObject("h1");
  gPad->SetLogy(1);
  h1->SetTitle("Shower_l;Integrated ADC");
  h1->Draw();

  c1->cd(2);
  T->Draw("Shower_r>>h2(250,0,2500)",TrigCut && "Shower_r>20");
  TH1F *h2 = (TH1F*) gROOT->FindObject("h2");
  gPad->SetLogy(1);
  h2->SetTitle("Shower_r;Integrated ADC");
  h2->Draw();

  c1->cd(3);
  T->Draw("Shower_t>>h3(250,0,2500)",TrigCut && "Shower_t>20");
  TH1F *h3 = (TH1F*) gROOT->FindObject("h3");
  gPad->SetLogy(1);
  h3->SetTitle("Shower_t;Integrated ADC");
  h3->Draw();

  c1->cd(4);
  T->Draw("ShowerSum>>h4(250,0,2500)",TrigCut && "ShowerSum>20");
  TH1F *h4 = (TH1F*) gROOT->FindObject("h4");
  gPad->SetLogy(1);
  h4->SetTitle("ShowerSum;Integrated ADC");
  h4->Draw();
  
  c1->cd(0);
  c1->SaveAs(Form("gr/EcalADC_Run%d.png",run));
}

void plotEcal2D(int run,int logz=0)
{
  gStyle->SetPadRightMargin(0.12);
  TTree* T = (TTree*)gROOT->FindObject("T");
  
  if(!(T->FindBranch("PreShSum"))) {
    T->SetAlias("PreShSum","PreSh_r+PreSh_l+PreSh_t");
  }

  TCanvas *c1= new TCanvas("c1","",900,700);
  c1->Divide(2,2);
  c1->cd(1);
  T->Draw("PreSh_l:Shower_l>>h1(120,100,2500,120,100,2500)",TrigCut && "Shower_l>120","colz");
  TH2F *h1 = (TH2F*) gROOT->FindObject("h1");
  h1->SetTitle("bottom left block;Shower_l;PreSh_l");
  gPad->SetLogz(logz);
  h1->Draw("colz");

  c1->cd(2);
  T->Draw("PreSh_r:Shower_r>>h2(120,100,2500,120,100,2500)",TrigCut && "Shower_r>120","colz");
  TH2F *h2 = (TH2F*) gROOT->FindObject("h2");
  h2->SetTitle("bottom right block;Shower_r;PreSh_r");
  gPad->SetLogz(logz);
  h2->Draw("colz");

  c1->cd(3);
  T->Draw("PreSh_t:Shower_t>>h3(120,100,2500,120,100,2500)",TrigCut && "Shower_t>120","colz");
  TH2F *h3 = (TH2F*) gROOT->FindObject("h3");
  h3->SetTitle("top block;Shower_t;PreSh_t");
  gPad->SetLogz(logz);
  h3->Draw("colz");

  c1->cd(4);
  T->Draw("PreShSum:ShowerSum>>h4(120,100,2500,120,100,2500)",TrigCut && "ShowerSum>120","colz");
  TH2F *h4 = (TH2F*) gROOT->FindObject("h4");
  h4->SetTitle("Sum of 3 blocks;ShowerSum;PreShSum");
  gPad->SetLogz(logz);
  h4->Draw("colz");
  
  c1->cd(0);
  c1->SaveAs(Form("gr/EcalADC2D_Run%d%s.png",run,(logz?"_logz":"")));
  c1->SaveAs(Form("gr/EcalADC2D_Run%d%s.pdf",run,(logz?"_logz":"")));
  gStyle->SetPadRightMargin(0.06);
}

void plotEcal2D_large(int run,int logz=0)
{
  gStyle->SetPadRightMargin(0.12);
  TTree* T = (TTree*)gROOT->FindObject("T");
  
  if(!(T->FindBranch("PreShSum"))) {
    T->SetAlias("PreShSum","PreSh_r+PreSh_l+PreSh_t");
  }

  TCanvas *c1= new TCanvas("c1","",900,700);
  c1->Divide(2,2);
  c1->cd(1);
  T->Draw("PreSh_l:Shower_l>>h1(250,100,5100,250,100,5100)",TrigCut && "Shower_l>120","colz");
  TH2F *h1 = (TH2F*) gROOT->FindObject("h1");
  h1->SetTitle("bottom left block;Shower_l;PreSh_l");
  gPad->SetLogz(logz);
  h1->Draw("colz");

  c1->cd(2);
  T->Draw("PreSh_r:Shower_r>>h2(250,100,5100,250,100,5100)",TrigCut && "Shower_r>120","colz");
  TH2F *h2 = (TH2F*) gROOT->FindObject("h2");
  h2->SetTitle("bottom right block;Shower_r;PreSh_r");
  gPad->SetLogz(logz);
  h2->Draw("colz");

  c1->cd(3);
  T->Draw("PreSh_t:Shower_t>>h3(250,100,5100,250,100,5100)",TrigCut && "Shower_t>120","colz");
  TH2F *h3 = (TH2F*) gROOT->FindObject("h3");
  h3->SetTitle("top block;Shower_t;PreSh_t");
  gPad->SetLogz(logz);
  h3->Draw("colz");

  c1->cd(4);
  T->Draw("PreShSum:ShowerSum>>h4(250,100,5100,250,100,5100)",TrigCut && "ShowerSum>120","colz");
  TH2F *h4 = (TH2F*) gROOT->FindObject("h4");
  h4->SetTitle("Sum of 3 blocks;ShowerSum;PreShSum");
  gPad->SetLogz(logz);
  h4->Draw("colz");
  
  c1->cd(0);
  c1->SaveAs(Form("gr/EcalADC2D_Run%d_largeRange%s.png",run,(logz?"_logz":"")));
  c1->SaveAs(Form("gr/EcalADC2D_Run%d_largeRange%s.pdf",run,(logz?"_logz":"")));
  gStyle->SetPadRightMargin(0.06);
}


void plotSC(int run)
{
  TTree* T = (TTree*)gROOT->FindObject("T");
  TCanvas *c1= new TCanvas("c1","",900,700);
  c1->Divide(2,2);
  
  c1->cd(1);
  T->Draw("SC0_t>>h1(250,0,2500)",TrigCut);
  TH1F *h1 = (TH1F*) gROOT->FindObject("h1");
  h1->SetTitle("SC0_t;Integrated ADC");
  h1->Draw();

  c1->cd(2);
  T->Draw("SC0_b>>h2(250,0,2500)",TrigCut);
  TH1F *h2 = (TH1F*) gROOT->FindObject("h2");
  h2->SetTitle("SC0_b;Integrated ADC");
  h2->Draw();

  c1->cd(3);
  T->Draw("SC1_t>>h3(250,0,2500)",TrigCut);
  TH1F *h3 = (TH1F*) gROOT->FindObject("h3");
  h3->SetTitle("SC1_t;Integrated ADC");
  h3->Draw();

  c1->cd(4);
  T->Draw("SC1_b>>h4(250,0,2500)",TrigCut);
  TH1F *h4 = (TH1F*) gROOT->FindObject("h4");
  h4->SetTitle("SC1_b;Integrated ADC");
  
  c1->cd(0);
  c1->SaveAs(Form("gr/SC_Run%d.png",run));
}


void plotSPD(int run)
{
  TTree* T = (TTree*)gROOT->FindObject("T");
  TCanvas *c1= new TCanvas("c1","",500,900);
  c1->Divide(1,3);
  
  c1->cd(1);
  T->Draw("LASPD_t>>h1(250,0,2500)",TrigCut && "LASPD_t>20");
  TH1F *h1 = (TH1F*) gROOT->FindObject("h1");
  gPad->SetLogy(1);
  h1->SetTitle("LASPD_t;Integrated ADC");
  h1->Draw();

  c1->cd(2);
  T->Draw("LASPD_b>>h2(250,0,2500)",TrigCut && "LASPD_b>20");
  TH1F *h2 = (TH1F*) gROOT->FindObject("h2");
  gPad->SetLogy(1);
  h2->SetTitle("LASPD_b;Integrated ADC");
  h2->Draw();

  c1->cd(3);
  T->Draw("SPD>>h3(250,0,2500)",TrigCut && "SPD>20");
  TH1F *h3 = (TH1F*) gROOT->FindObject("h3");
  gPad->SetLogy(1);
  h3->SetTitle("SPD;Integrated ADC");
  h3->Draw();

  
  c1->cd(0);
  c1->SaveAs(Form("gr/SPD_Run%d.png",run));
}

void plotPreSh(int run)
{
  TTree* T = (TTree*)gROOT->FindObject("T");
  TCanvas *c1= new TCanvas("c1","",900,700);
  c1->Divide(2,2);
  
  c1->cd(1);
  T->Draw("PreSh_l>>h1(250,0,2500)",TrigCut && "PreSh_l>20");
  TH1F *h1 = (TH1F*) gROOT->FindObject("h1");
  gPad->SetLogy(1);
  h1->SetTitle("PreSh_l;Integrated ADC");
  h1->Draw();

  c1->cd(2);
  T->Draw("PreSh_r>>h2(250,0,2500)",TrigCut && "PreSh_r>20");
  TH1F *h2 = (TH1F*) gROOT->FindObject("h2");
  gPad->SetLogy(1);
  h2->SetTitle("PreSh_r;Integrated ADC");
  h2->Draw();

  c1->cd(3);
  T->Draw("PreSh_t>>h3(250,0,2500)",TrigCut && "PreSh_t>20");
  TH1F *h3 = (TH1F*) gROOT->FindObject("h3");
  gPad->SetLogy(1);
  h3->SetTitle("PreSh_t;Integrated ADC");
  h3->Draw();

  c1->cd(4);
  T->Draw("PreSh_l+PreSh_r+PreSh_t>>h4(250,0,2500)",TrigCut && "PreSh_l+PreSh_r+PreSh_t>20");
  TH1F *h4 = (TH1F*) gROOT->FindObject("h4");
  gPad->SetLogy(1);
  h4->SetTitle("PreShSum;Integrated ADC");
  h4->Draw();
  
  c1->cd(0);
  c1->SaveAs(Form("gr/PreShADC_Run%d.png",run));
}


void plotCer(int run)
{
  TTree* T = (TTree*)gROOT->FindObject("T");
  TCanvas *c4= new TCanvas("c4","",1100,800);
  c4->Divide(4,4,0.01,0.01);

  char strTg[255],strName[255],strCut[255];
  TH1F *h1=0;
  for(int ch=0;ch<16;ch++) {
    c4->cd(kCerPadIndex[ch]);
    sprintf(strName,"h1_Cer_%02d",ch);
    sprintf(strTg,"Cer[%d]>>%s(200,0,1000)",ch,strName);
    sprintf(strCut,"Cer[%d]>20",ch);
    T->Draw(strTg,TrigCut && strCut);
    h1 = (TH1F*) gROOT->FindObject(strName);
    h1->SetTitle(Form("%s;Integrated ADC",Slot7Name[ch]));
    gPad->SetLogy(1);
  }

  c4->cd(0);
  c4->SaveAs(Form("gr/Cer_run%04d.png",run));
}


void plotSlot8Raw(int run, int nevent=10, int start=0, int save=0)
{
  TTree* T = (TTree*)gROOT->FindObject("T");
  TCanvas *c4= new TCanvas("c4","",1100,800);
  c4->Divide(4,4,0.01,0.01);

  cout<<"nevent="<<nevent<<"  start="<<start<<endl;
  char strTg[255],strCut[255],strName[255];
  for(int ch=0;ch<16;ch++) {
    c4->cd(ch+1);
    sprintf(strName,"h2_Slot8_%02d",ch);
    if(ch!=6 && ch<=9) sprintf(strTg,"Slot8Raw[%d]:Iteration$>>%s(100,0,100,256,0,512)",ch,strName);
    else sprintf(strTg,"Slot8Raw[%d]:Iteration$>>%s(100,0,100,256,0,256)",ch,strName);
    sprintf(strCut,"1");
    T->Draw(strTg,TrigCut && strCut,"l",nevent,start);
    TH2F* h2=(TH2F*)gROOT->FindObject(strName);
    h2->SetTitle(Form("%s;Time(4ns);ADC",Slot8Name[ch]));
    h2->Draw("l");
  }

  c4->cd(0);
  if(save) c4->SaveAs(Form("gr/Slot8Raw_run%04d_%devent%03d.png",run,nevent,start));
}

void plotSlot8Pulse(int run, int ch, int nevent=1, int start=0, int save=0)
{
  TTree* T = (TTree*)gROOT->FindObject("T");
  TCanvas *c5= new TCanvas("c5","c5",800,600);

  cout<<"nevent="<<nevent<<"  start="<<start<<endl;
  char strTg[255],strName[255],strHistoBins[255],cut1[255],cut2[255];
  TCut myCut = "TrigType==1";
  
  
  c5->cd();  
  sprintf(strHistoBins,"(100,0,100,1024,0,1024)");
  
  sprintf(strName,"h2_Slot8_%02d_0",ch);
  sprintf(strTg,"Slot8Raw[%d]+0:Iteration$>>%s%s",ch,strName,strHistoBins);    
  T->Draw(strTg,myCut,"l",nevent,start);
  TH2D* h20=(TH2D*)gROOT->FindObject(strName);
  TH1D* h10=(TH1D*) h20->ProfileX(Form("h1_Slot8_%02d_0",ch));
  h10->SetTitle(Form("%s;Time(4ns);ADC",Slot8Name[ch]));
  h10->SetLineColor(3);
  
  sprintf(cut1,"Iteration$>=Slot8Left0[%d] && Iteration$<=Slot8Right0[%d]",ch,ch);
  sprintf(strName,"h2_Slot8_%02d_1",ch);
  sprintf(strTg,"Slot8Raw[%d]+50:Iteration$>>%s%s",ch,strName,strHistoBins);    
  T->Draw(strTg,myCut && cut1,"l",nevent,start);
  TH2D* h21=(TH2D*)gROOT->FindObject(strName);
  TH1D* h11=(TH1D*) h21->ProfileX(Form("h1_Slot8_%02d_1",ch));
  h11->SetTitle(Form("%s;Time(4ns);ADC",Slot8Name[ch]));
  h11->SetLineColor(6);
  
  sprintf(cut1,"Iteration$>=Slot8Left[%d] && Iteration$<=Slot8Right[%d]",ch,ch);
  sprintf(strName,"h2_Slot8_%02d_2",ch);
  sprintf(strTg,"Slot8Raw[%d]+100:Iteration$>>%s%s",ch,strName,strHistoBins);    
  T->Draw(strTg,myCut && cut1,"l",nevent,start);
  TH2D* h22=(TH2D*)gROOT->FindObject(strName);
  TH1D* h12=(TH1D*) h22->ProfileX(Form("h1_Slot8_%02d_2",ch));
  h12->SetTitle(Form("%s;Time(4ns);ADC",Slot8Name[ch]));
  h12->SetLineColor(4);
  
  c5->SetGridx(1);
  c5->SetGridy(1);
  h10->SetTitle(Form("%s: green(raw+0), pink(level0+50), blue(level1+100);Time(4ns);ADC",Slot8Name[ch]));
  double max = h10->GetMaximum()+120.0;
  if(max>512) max=512;
  h10->GetYaxis()->SetRangeUser(-10,max);
  h10->Draw("lc");
  h11->Draw("lsame");
  h12->Draw("lsame");  
  
  if(save) c5->SaveAs(Form("gr/Slot8Ch%02dRaw_run%04d_%devent%03d.png",ch,run,nevent,start));
}


void plot2Peak(int run, const char *Var="LASPD_t")
{
  TTree* T = (TTree*)gROOT->FindObject("T");
  if(!(T->FindBranch("PreShSum"))) {
    T->SetAlias("PreShSum","PreSh_r+PreSh_l+PreSh_t");
  }
  
  char strCut[255];
  TCanvas *c1= new TCanvas("c1","",900,700);
  c1->Divide(2,2);
  c1->cd(1);
  sprintf(strCut,"(%s > 20)",Var);
  T->Draw(Form("%s>>h1(250,0,2500)",Var),TrigCut && strCut);
  TH1F *h1 = (TH1F*) gROOT->FindObject("h1");
  //gPad->SetLogy(1);
  h1->SetTitle(Form("%s;Integrated ADC",Var));
  h1->Draw();

  c1->cd(2);
  sprintf(strCut,"(%s > 20 && CerSum>200)",Var);
  T->Draw(Form("%s>>h2(250,0,2500)",Var),TrigCut && strCut);
  TH1F *h2 = (TH1F*) gROOT->FindObject("h2");
  //gPad->SetLogy(1);
  h2->SetTitle(Form("%s: CerSum>200 ;Integrated ADC",Var));
  h2->Draw();

  c1->cd(3);
  sprintf(strCut,"(%s > 20 && CerSum>200 && ShowerSum>1000 )",Var);
  T->Draw(Form("%s>>h3(250,0,2500)",Var),TrigCut && strCut);
  TH1F *h3 = (TH1F*) gROOT->FindObject("h3");
  //gPad->SetLogy(1);
  h3->SetTitle(Form("%s: CerSum>200 && ShowerSum>1000;Integrated ADC",Var));
  h3->Draw();

  c1->cd(4);
  sprintf(strCut,"(%s > 20 && PreSh_t>100 && Shower_t>200 )",Var);
  T->Draw(Form("%s>>h4(250,0,2500)",Var),TrigCut && strCut);
  TH1F *h4 = (TH1F*) gROOT->FindObject("h4");
  //gPad->SetLogy(1);
  h4->SetTitle(Form("%s: PreSh_t>100 && Shower_t>200;Integrated ADC",Var));
  h4->Draw();
  
  c1->cd(0);
  c1->SaveAs(Form("gr/%s_Run%d.png",Var,run));
  c1->SaveAs(Form("gr/%s_Run%d.pdf",Var,run));
}

TF1* FitGaus(TH1* h1, double range_in_sigma=1.0)
{
  return 0;
  //cout<<"histo name="<<h1->GetName()<<" entries="<<h1->GetEntries()<<endl;
  if(h1->GetEntries()<500) return NULL;

  double xmin=h1->GetMean()-1.0*h1->GetRMS();
  double xmax=h1->GetMean()+1.0*h1->GetRMS();
  h1->Fit("gaus","RQ","",xmin,xmax);
  TF1 *f=(TF1*)h1->GetListOfFunctions()->FindObject("gaus");
  if(!f) return NULL;
  double mean=f->GetParameter(1);
  double sigma=f->GetParameter(2);
  xmin=mean-range_in_sigma*sigma;
  xmax=mean+range_in_sigma*sigma;

  h1->Fit("gaus","RQ","",xmin,xmax);
  f=(TF1*)h1->GetListOfFunctions()->FindObject("gaus");
  if(!f) return NULL;
  mean=f->GetParameter(1);
  sigma=f->GetParameter(2);
  f->SetLineColor(2);

  if(gStyle->GetOptFit()==0)
  {
    char str[100];
    TText *text=0;

    double xx=gStyle->GetPadLeftMargin()+0.03; 
    TPaveText *pt = new TPaveText(xx,0.20,xx+0.45,0.45,"brNDC");
    pt->SetBorderSize(0);
    pt->SetFillColor(0);
    sprintf(str,"Mean = %.3G",mean);
    text=pt->AddText(str);
    text->SetTextColor(2);
    sprintf(str,"Sigma = %.3G",sigma);
    text=pt->AddText(str);
    text->SetTextColor(2);
    pt->Draw("same");
  }

  return f;
}


void plotPreShDShower(int run)
{
  TTree* T = (TTree*)gROOT->FindObject("T");
  if(!(T->FindBranch("PreShSum"))) {
    T->SetAlias("PreShSum","PreSh_r+PreSh_l+PreSh_t");
  }
  
  char Var_l[255],Var_r[255],Var_t[255];
  char strCut_l[255],strCut_r[255],strCut_t[255];
  
  sprintf(Var_l,"PreSh_l/Shower_l");
  sprintf(strCut_l,"PreSh_l>120 && Shower_l>250");
  
  sprintf(Var_r,"PreSh_r/Shower_r");
  sprintf(strCut_r,"PreSh_r>120 && Shower_r>250");
  
  sprintf(Var_t,"PreSh_t/Shower_t");
  sprintf(strCut_t,"PreSh_t>120 && Shower_t>250");
  
  TCanvas *c1= new TCanvas("c1","",900,700);
  c1->Divide(2,2);
  
  c1->cd(1);
  T->Draw(Form("%s>>h10_l(100,0,5)",Var_l),TrigCut && strCut_l);
  TH1F *h10_l= (TH1F*) gROOT->FindObject("h10_l");
  h10_l->SetTitle(Form("%s:left(red),right(green),top(blue);PreSh/Shower",strCut_l));
  h10_l->SetLineColor(2);
  TH1F *h10_l_copy = (TH1F*)h10_l->Clone();
  FitGaus(h10_l);
  
  T->Draw(Form("%s>>h10_r(100,0,5)",Var_r),TrigCut && strCut_l,"same");
  TH1F *h10_r = (TH1F*) gROOT->FindObject("h10_r");
  h10_r->SetLineColor(3);
  
  T->Draw(Form("%s>>h10_t(100,0,5)",Var_t),TrigCut && strCut_l,"same");
  TH1F *h10_t = (TH1F*) gROOT->FindObject("h10_t");
  h10_t->SetLineColor(4);

  c1->cd(2);
  T->Draw(Form("%s>>h11_r(100,0,5)",Var_r),TrigCut && strCut_r);
  TH1F *h11_r = (TH1F*) gROOT->FindObject("h11_r");
  h11_r->SetLineColor(3);
  h11_r->SetTitle(Form("%s:left(red),right(green),top(blue);PreSh/Shower",strCut_r));
  TH1F *h11_r_copy = (TH1F*)h11_r->Clone();
  FitGaus(h11_r);
  
  T->Draw(Form("%s>>h11_l(100,0,5)",Var_l),TrigCut && strCut_r,"same");
  TH1F *h11_l= (TH1F*) gROOT->FindObject("h11_l");
  h11_l->SetLineColor(2);
  
  T->Draw(Form("%s>>h11_t(100,0,5)",Var_t),TrigCut && strCut_r,"same");
  TH1F *h11_t = (TH1F*) gROOT->FindObject("h11_t");
  h11_t->SetLineColor(4);

  c1->cd(3);
  T->Draw(Form("%s>>h12_t(100,0,5)",Var_t),TrigCut && strCut_t);
  TH1F *h12_t = (TH1F*) gROOT->FindObject("h12_t");
  h12_t->SetLineColor(4);
  h12_t->SetTitle(Form("%s:left(red),right(green),top(blue);PreSh/Shower",strCut_t));
  TH1F *h12_t_copy = (TH1F*)h12_t->Clone();
  FitGaus(h12_t);

  T->Draw(Form("%s>>h12_l(100,0,5)",Var_l),TrigCut && strCut_t,"same");
  TH1F *h12_l= (TH1F*) gROOT->FindObject("h12_l");
  h12_l->SetLineColor(2);
  
  T->Draw(Form("%s>>h12_r(100,0,5)",Var_r),TrigCut && strCut_t,"same");
  TH1F *h12_r = (TH1F*) gROOT->FindObject("h12_r");
  h12_r->SetLineColor(3);
  
  c1->cd(4);
  h12_t_copy->SetTitle("single-block-cut:left(red),right(green),top(blue);PreSh/Shower");
  h12_t_copy->Draw();
  h11_r_copy->Draw("same");
  h10_l_copy->Draw("same");
  
  c1->cd(0);
  c1->SaveAs(Form("gr/%s_Run%d.png","PreShDShower",run));
  c1->SaveAs(Form("gr/%s_Run%d.pdf","PreShDShower",run));
}

void plotPreShDShower_pi(int run)
{
  TTree* T = (TTree*)gROOT->FindObject("T");
  if(!(T->FindBranch("PreShSum"))) {
    T->SetAlias("PreShSum","PreSh_r+PreSh_l+PreSh_t");
  }
  
  char Var_l[255],Var_r[255],Var_t[255];
  char strCut_l[255],strCut_r[255],strCut_t[255];
  
  sprintf(Var_l,"PreSh_l/Shower_l");
  sprintf(strCut_l,"PreSh_l>120 && Shower_l>250");
  
  sprintf(Var_r,"PreSh_r/Shower_r");
  sprintf(strCut_r,"PreSh_r>120 && Shower_r>250");
  
  sprintf(Var_t,"PreSh_t/Shower_t");
  sprintf(strCut_t,"PreSh_t>120 && Shower_t>250");
  
  TCanvas *c1= new TCanvas("c1","",900,700);
  c1->Divide(2,2);
  
  c1->cd(1);
  T->Draw(Form("%s>>h10_l(100,0,5)",Var_l),TrigCut && PiCut && strCut_l);
  TH1F *h10_l= (TH1F*) gROOT->FindObject("h10_l");
  h10_l->SetTitle(Form("%s:left(red),right(green),top(blue);PreSh/Shower",strCut_l));
  h10_l->SetLineColor(2);
  TH1F *h10_l_copy = (TH1F*)h10_l->Clone();
  FitGaus(h10_l);
  
  T->Draw(Form("%s>>h10_r(100,0,5)",Var_r),TrigCut && PiCut && strCut_l,"same");
  TH1F *h10_r = (TH1F*) gROOT->FindObject("h10_r");
  h10_r->SetLineColor(3);
  
  T->Draw(Form("%s>>h10_t(100,0,5)",Var_t),TrigCut && PiCut && strCut_l,"same");
  TH1F *h10_t = (TH1F*) gROOT->FindObject("h10_t");
  h10_t->SetLineColor(4);

  c1->cd(2);
  T->Draw(Form("%s>>h11_r(100,0,5)",Var_r),TrigCut && PiCut && strCut_r);
  TH1F *h11_r = (TH1F*) gROOT->FindObject("h11_r");
  h11_r->SetLineColor(3);
  h11_r->SetTitle(Form("%s:left(red),right(green),top(blue);PreSh/Shower",strCut_r));
  TH1F *h11_r_copy = (TH1F*)h11_r->Clone();
  FitGaus(h11_r);
  
  T->Draw(Form("%s>>h11_l(100,0,5)",Var_l),TrigCut && PiCut && strCut_r,"same");
  TH1F *h11_l= (TH1F*) gROOT->FindObject("h11_l");
  h11_l->SetLineColor(2);
  
  T->Draw(Form("%s>>h11_t(100,0,5)",Var_t),TrigCut && PiCut && strCut_r,"same");
  TH1F *h11_t = (TH1F*) gROOT->FindObject("h11_t");
  h11_t->SetLineColor(4);

  c1->cd(3);
  T->Draw(Form("%s>>h12_t(100,0,5)",Var_t),TrigCut && PiCut && strCut_t);
  TH1F *h12_t = (TH1F*) gROOT->FindObject("h12_t");
  h12_t->SetLineColor(4);
  h12_t->SetTitle(Form("%s:left(red),right(green),top(blue);PreSh/Shower",strCut_t));
  TH1F *h12_t_copy = (TH1F*)h12_t->Clone();
  FitGaus(h12_t);

  T->Draw(Form("%s>>h12_l(100,0,5)",Var_l),TrigCut && PiCut && strCut_t,"same");
  TH1F *h12_l= (TH1F*) gROOT->FindObject("h12_l");
  h12_l->SetLineColor(2);
  
  T->Draw(Form("%s>>h12_r(100,0,5)",Var_r),TrigCut && PiCut && strCut_t,"same");
  TH1F *h12_r = (TH1F*) gROOT->FindObject("h12_r");
  h12_r->SetLineColor(3);
  
  c1->cd(4);
  h12_t_copy->SetTitle("single-block-cut:left(red),right(green),top(blue);PreSh/Shower");
  h12_t_copy->Draw();
  h11_r_copy->Draw("same");
  h10_l_copy->Draw("same");
  
  c1->cd(0);
  c1->SaveAs(Form("gr/%s_Run%d.png","PreShDShower_pi",run));
  c1->SaveAs(Form("gr/%s_Run%d.pdf","PreShDShower_pi",run));
}

void plotPreShDShower_el(int run)
{
  TTree* T = (TTree*)gROOT->FindObject("T");
  if(!(T->FindBranch("PreShSum"))) {
    T->SetAlias("PreShSum","PreSh_r+PreSh_l+PreSh_t");
  }
  
  char Var_l[255],Var_r[255],Var_t[255];
  char strCut_l[255],strCut_r[255],strCut_t[255];
  
  sprintf(Var_l,"PreSh_l/Shower_l");
  sprintf(strCut_l,"PreSh_l>120 && Shower_l>250");
  
  sprintf(Var_r,"PreSh_r/Shower_r");
  sprintf(strCut_r,"PreSh_r>120 && Shower_r>250");
  
  sprintf(Var_t,"PreSh_t/Shower_t");
  sprintf(strCut_t,"PreSh_t>120 && Shower_t>250");
  
  TCanvas *c1= new TCanvas("c1","",900,700);
  c1->Divide(2,2);
  
  c1->cd(1);
  T->Draw(Form("%s>>h10_l(100,0,5)",Var_l),TrigCut && ElCut && strCut_l);
  TH1F *h10_l= (TH1F*) gROOT->FindObject("h10_l");
  h10_l->SetTitle(Form("%s:left(red),right(green),top(blue);PreSh/Shower",strCut_l));
  h10_l->SetLineColor(2);
  TH1F *h10_l_copy = (TH1F*)h10_l->Clone();
  FitGaus(h10_l);
  
  T->Draw(Form("%s>>h10_r(100,0,5)",Var_r),TrigCut && ElCut && strCut_l,"same");
  TH1F *h10_r = (TH1F*) gROOT->FindObject("h10_r");
  h10_r->SetLineColor(3);
  
  T->Draw(Form("%s>>h10_t(100,0,5)",Var_t),TrigCut && ElCut && strCut_l,"same");
  TH1F *h10_t = (TH1F*) gROOT->FindObject("h10_t");
  h10_t->SetLineColor(4);

  c1->cd(2);
  T->Draw(Form("%s>>h11_r(100,0,5)",Var_r),TrigCut && ElCut && strCut_r);
  TH1F *h11_r = (TH1F*) gROOT->FindObject("h11_r");
  h11_r->SetLineColor(3);
  h11_r->SetTitle(Form("%s:left(red),right(green),top(blue);PreSh/Shower",strCut_r));
  TH1F *h11_r_copy = (TH1F*)h11_r->Clone();
  FitGaus(h11_r);
  
  T->Draw(Form("%s>>h11_l(100,0,5)",Var_l),TrigCut && ElCut && strCut_r,"same");
  TH1F *h11_l= (TH1F*) gROOT->FindObject("h11_l");
  h11_l->SetLineColor(2);
  
  T->Draw(Form("%s>>h11_t(100,0,5)",Var_t),TrigCut && ElCut && strCut_r,"same");
  TH1F *h11_t = (TH1F*) gROOT->FindObject("h11_t");
  h11_t->SetLineColor(4);

  c1->cd(3);
  T->Draw(Form("%s>>h12_t(100,0,5)",Var_t),TrigCut && ElCut && strCut_t);
  TH1F *h12_t = (TH1F*) gROOT->FindObject("h12_t");
  h12_t->SetLineColor(4);
  h12_t->SetTitle(Form("%s:left(red),right(green),top(blue);PreSh/Shower",strCut_t));
  TH1F *h12_t_copy = (TH1F*)h12_t->Clone();
  FitGaus(h12_t);

  T->Draw(Form("%s>>h12_l(100,0,5)",Var_l),TrigCut && ElCut && strCut_t,"same");
  TH1F *h12_l= (TH1F*) gROOT->FindObject("h12_l");
  h12_l->SetLineColor(2);
  
  T->Draw(Form("%s>>h12_r(100,0,5)",Var_r),TrigCut && ElCut && strCut_t,"same");
  TH1F *h12_r = (TH1F*) gROOT->FindObject("h12_r");
  h12_r->SetLineColor(3);
  
  c1->cd(4);
  h12_t_copy->SetTitle("single-block-cut:left(red),right(green),top(blue);PreSh/Shower");
  h12_t_copy->Draw();
  h11_r_copy->Draw("same");
  h10_l_copy->Draw("same");
  
  c1->cd(0);
  c1->SaveAs(Form("gr/%s_Run%d.png","PreShDShower_el",run));
  c1->SaveAs(Form("gr/%s_Run%d.pdf","PreShDShower_el",run));
}


void plot_level1(int run)
{
  plotCer(run);
  plotEcal(run);
  plotPreSh(run);
  plotSC(run);
  plotSPD(run);
  plot2Peak(run,"PreSh_l");
  plot2Peak(run,"PreSh_r");
  plot2Peak(run,"PreSh_t");
  plot2Peak(run,"PreShSum");
  plot2Peak(run,"CerSum");
  plot2Peak(run,"SC0_b");
  plot2Peak(run,"SC1_b");
  plot2Peak(run,"SC0_t");
  plot2Peak(run,"SC1_t");
  plot2Peak(run,"LASPD_t");
  plot2Peak(run,"LASPD_b");
  plot2Peak(run,"SPD");
  plot2Peak(run,"Shower_l");
  plot2Peak(run,"Shower_r");
  plot2Peak(run,"Shower_t");
  plot2Peak(run,"ShowerSum");
  plotEcal2D(run,0);
  plotEcal2D(run,1);
  plotEcal2D_large(run,0);
  plotEcal2D_large(run,1);
}
