


void plot_Na(){
	const int N_data = 14;
	double time[N_data] = {60.70,60.60,60.88,60.76,60.70,60.62,60.64,60.36,60.46,60.56,216.96,126.06,146.80,78.6};
	double counts[N_data] = {212,217,212,165,146,99,72,41,16,3,14,3,1,3};
	double degrees[N_data] ={0,2.5,5.0,7.5,10.0,12.5,15.0,17.5,20,25,30,35,45,90};

	const int N_data_add = 5 ;
	double time_add[N_data_add]   ={126.46,94.44,120.96,90.56,91.98};
	double counts_add[N_data_add] ={469,339,417,247,191};
	double degrees_add[N_data_add]={0,2.5,5.0,7.5,10} ;


	double counts_error[N_data] ={};

	for(int i = 0 ; i<N_data_add;i++){
		time[i]= time[i] + time_add[i];
		counts[i] = counts[i] + counts_add[i];
	}

	double degrees_error[N_data] ={0};

	for(int i=0; i<N_data;i++){
		counts_error[i] = TMath::Sqrt(counts[i])/time[i];
		counts[i] = counts[i]/time[i];
		degrees_error[i] = 0.6844;

	}


	TGraphErrors *gr = new TGraphErrors(N_data,degrees,counts,degrees_error,counts_error);
	gr->GetXaxis()->SetLimits(-5,95);
	gr->GetYaxis()->SetRangeUser(-0.3,4.5);
	gr->SetMarkerStyle(20);
	gr->SetMarkerSize(1);

	TCanvas *c1= new TCanvas("c1","c1",800,600);
	c1->SetLeftMargin(0.12);
    c1->SetRightMargin(0.01);
    c1->SetBottomMargin(0.12);
    c1->SetTickx(1);
    c1->SetTicky(1);
    c1->SetTopMargin(0.03);
    c1->SetFrameLineWidth(3);
	gr->SetTitle("");




	gr->Draw("AP");
	gr->GetXaxis()->SetTitle("#theta [deg]");
	gr->GetXaxis()->CenterTitle();
	gr->GetXaxis()->SetTitleSize(0.05);
	gr->GetYaxis()->SetTitle("Counting Rate [s^{-1}]");
	gr->GetYaxis()->CenterTitle();
	gr->GetYaxis()->SetTitleSize(0.05);


	TF1 *f1 = new TF1("gausBg","[0]*exp(-0.5*((x-[1])/[2])^2) + [3]",0,180);
	f1->SetParameters(3, 0, 10,0);
	f1->SetLineWidth(3);
	gr->Fit(f1);



	TLegend *leg = new TLegend(0.49,0.7,0.95,0.88);
    leg->SetBorderSize(0);
    
    leg->AddEntry(gr, "Data");
    leg->AddEntry(f1, "Fit: Gaussian + Bkg");
    
    leg->Draw();

	TLatex *text = new TLatex();

	text->SetTextColor(kBlack);
	text->SetTextSize(0.05);
	text->DrawLatexNDC(0.50,0.63,Form("#chi^{2}/NDF = %.3f/%d", f1->GetChisquare() , f1->GetNDF() ));
	text->SetTextFont(42);
	text->SetTextSize(0.04);
    text->DrawLatexNDC(0.50, 0.52, Form("A = (%.3f #pm %.3f) s^{-1} ",f1->GetParameter(0),f1->GetParError(0) ));
    text->DrawLatexNDC(0.50, 0.45, Form("#mu = (%.3f #pm %.3f)^{#circ}",f1->GetParameter(1), f1->GetParError(1) ));
    text->DrawLatexNDC(0.50, 0.38, Form("#sigma = (%.3f #pm %.3f)^{#circ}",f1->GetParameter(2), f1->GetParError(2) ));
    text->DrawLatexNDC(0.50, 0.31, Form("Bkg = (%.3f #pm %.3f) s^{-1} ",f1->GetParameter(3), f1->GetParError(3) ));

    c1->SaveAs("Na22/Na22_Angular_Resolution.png");
    c1->SaveAs("Na22/Na22_Angular_Resolution.pdf");


}
