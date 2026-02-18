



double P0(double x){
    return 1.;
}

double P1(double x){
    return x;
}

double P2(double x){
    return 0.5*(3*x*x-1);
}

double P4(double x){
    return 1./8. * (35 * x *x *x *x- 30. * x * x +3.0);
}


Double_t myFunc(Double_t *x, Double_t *par) {
    

   

    return par[0] * P0(x[0]) + par[1] * P1(x[0]) + par[2] * P2(x[0]) + par[3] * P4(x[0]); 
}

void plot_theory() {
    //--------------------------------- theory ---------------------------------------
    TF1 *func_theory = new TF1("myFunc_theory", myFunc, -1, 1, 4);
    func_theory->SetLineColor(kRed);
    func_theory->SetLineStyle(1);
    func_theory->SetLineWidth(5);
    func_theory->SetParameters(1.0, 0.0,0.102,0.0091); 
    //--------------------------------------------------------------------------------
    
    const int N_points = 10;
    double angle[N_points] = {0. , 15.,30, 45, 60,75,90,105,120 ,135 };
    double counts[N_points] = {221,180,167,155,179,146,168,150,133,100}; 
    double counts_error[N_points] ={0};

    double time[N_points] = {300.,300.,300,300,300,300,300,300,300,199.74}; 

    double cos_angle[N_points];


    for(int i=0 ; i < N_points; i++){
        cos_angle[i] = TMath::Cos(angle[i]/180 * TMath::Pi());
        counts_error[i] = TMath::Sqrt( counts[i] );
        counts [i] = counts[i]/time[i];
        counts_error[i] = counts_error[i]/time[i]; 
    }

    TF1 *func_fit = new TF1("myFunc_fit", myFunc, -1, 1, 4);
    func_fit->SetParameters(1.0, 0.0,0.102,0.0091); 

    TGraphErrors *gr = new TGraphErrors(N_points,cos_angle,counts,0,counts_error);
    gr->Fit(func_fit);
   
    double counts_normalized[N_points] ={0.};
    double counts_normalized_error[N_points] ={0.};
    for(int i = 0; i < N_points ;i++){
        counts_normalized[i] = counts[i]/func_fit->GetParameter(0); 
        counts_normalized_error[i] = TMath::Sqrt( TMath::Power(counts_error[i]/counts[i],2 ) +  TMath::Power( func_fit->GetParError(0)/func_fit->GetParameter(0),2)      );
        counts_normalized_error[i] = counts_normalized_error[i] * counts_normalized[i];
    }

    //------------------------------------ normalized data ------------------------------------ 
    TGraphErrors *gr_normalized = new TGraphErrors(N_points,cos_angle,counts_normalized,0,counts_normalized_error );
   
    gr_normalized->SetTitle("");
    gr_normalized->GetXaxis()->SetTitle("cos#theta");
    gr_normalized->GetXaxis()->CenterTitle();
    gr_normalized->GetXaxis()->SetTitleSize(0.05);
    gr_normalized->GetXaxis()->SetTitleOffset(0.85);
    gr_normalized->GetYaxis()->SetTitle("Normalized Counting Rate");
    gr_normalized->GetYaxis()->CenterTitle();
    gr_normalized->GetYaxis()->SetTitleSize(0.05);
    gr_normalized->GetYaxis()->SetTitleOffset(0.78);
    //--------------------------------------------------------------------------------



    //------------------------------------ normalized fit ------------------------------------ 
    TF1 *func_fit_normalized = new TF1("myFunc_fit_normalized", myFunc, -1, 1, 4);
    func_fit_normalized->SetParameter(0,func_fit->GetParameter(0)/func_fit->GetParameter(0)  ); 
    func_fit_normalized->SetParameter(1,func_fit->GetParameter(1)/func_fit->GetParameter(0)  ); 
    func_fit_normalized->SetParameter(2,func_fit->GetParameter(2)/func_fit->GetParameter(0)  ); 
    func_fit_normalized->SetParameter(3,func_fit->GetParameter(3)/func_fit->GetParameter(0)  ); 
    func_fit_normalized->SetLineColor(kBlue);
    func_fit_normalized->SetLineStyle(2);
    func_fit_normalized->SetLineWidth(5);


    double a1 = func_fit->GetParameter(1)/func_fit->GetParameter(0); 
    double a2 = func_fit->GetParameter(2)/func_fit->GetParameter(0);
    double a3 = func_fit->GetParameter(3)/func_fit->GetParameter(0);

    double a1_error = TMath::Sqrt( TMath::Power( func_fit->GetParError(1)/func_fit->GetParameter(1) ,2) + TMath::Power( func_fit->GetParError(0)/func_fit->GetParameter(0) ,2)   )  * a1;
    double a2_error = TMath::Sqrt( TMath::Power( func_fit->GetParError(2)/func_fit->GetParameter(2) ,2) + TMath::Power( func_fit->GetParError(0)/func_fit->GetParameter(0) ,2)   )  * a2 ;
    double a3_error = TMath::Sqrt( TMath::Power( func_fit->GetParError(3)/func_fit->GetParameter(3) ,2) + TMath::Power( func_fit->GetParError(0)/func_fit->GetParameter(0) ,2)   )  * a3 ;


    //--------------------------------------------------------------------------------



    TCanvas *c1 = new TCanvas("c1","c1",1200,800);
    c1->SetRightMargin(0.01);
    c1->SetBottomMargin(0.12);
    c1->SetTickx(1);
    c1->SetTicky(1);
    c1->SetTopMargin(0.03);
    c1->SetFrameLineWidth(3);
    gr_normalized->Draw("AP");
    gr_normalized->SetLineWidth(2);
    gr_normalized->SetMarkerStyle(20);
    gr_normalized->SetMarkerSize(1.7);
    func_theory->Draw("same");

   
    func_fit_normalized->Draw("same");




    //add some text 

    TLegend *leg = new TLegend(0.16,0.6,0.45,0.85);
    leg->SetBorderSize(0);
    
    leg->AddEntry(gr_normalized, "Normalized Data");
    leg->AddEntry(func_fit_normalized, "Normalized Fit");
    leg->AddEntry(func_theory, "Theory");
    
    leg->Draw();


    TLatex *text = new TLatex();
    text->SetTextColor(kBlack);
    text->SetTextSize(0.05);
    text->DrawLatexNDC(0.57,0.87,Form("#chi^{2}/NDF = %.3f/%d", func_fit->GetChisquare() , func_fit->GetNDF() ));
    text->SetTextFont(42);
    text->SetTextSize(0.04);


    text->DrawLatexNDC(0.58,0.80,Form("a_{1} = %.3f #pm %.3f", a1 , a1_error ));
    text->DrawLatexNDC(0.58,0.73,Form("a_{2} = %.3f #pm %.3f", a2 , a2_error ));
    text->DrawLatexNDC(0.58,0.66,Form("a_{4} = %.3f #pm %.3f", a3 , a3_error ));
    text->SetTextSize(0.05);
    text->SetTextFont(52);
    text->DrawLatexNDC(0.19,0.87,"Uncorrected");
    c1->SaveAs("Co60/Co60.png");
    c1->SaveAs("Co60/Co60.pdf");


}







