

void Read_File(std::string InFile, TH1D *h1D_spectrum){
	std::ifstream infile(InFile.c_str());
	if(!infile.is_open()){
		std::cout<<"Can not open file,exit"<< std::endl;
		return ;
	}

	std::string line; 
	//went through the information that will not used 
	for(int i = 0 ; i < 9 ;i++){
		std::getline(infile,line);
		//std::cout<<line<<std::endl;
	}


	//read the duration of data taking (seconds)
	double duration;
	infile >> duration;
	std::cout<<"The duration of this data taking:"<<duration<<" s ."<<std::endl;

	std::getline(infile,line);
	std::getline(infile,line);
	int channel_start  ;
	int channel_end  ;

	infile >> channel_start;
	infile >> channel_end;

	//std::cout<<"Range of Channel # : "<< channel_start << "~" << channel_end << std::endl;
	//define the histogram 
	for(int i = 1 ; i < (channel_end + 2 ) ; i ++  ){
		std::getline(infile,line);
		int counts = 0 ; 
		infile >> counts; 
		h1D_spectrum->SetBinContent(i,counts);

	}
	h1D_spectrum->Scale(1./duration);

	infile.close();	

}



void plot_spectrum(){
	
	TH1D *h1D_Na22_FIXED = new TH1D("h1D_Na22_FIXED","h1D_Na22_FIXED",2048,0,2047);
	Read_File("02_09/na22_0209_FIXED.Spe",h1D_Na22_FIXED);
	TH1D *h1D_Bkg_FIXED = new TH1D("h1D_Bkg_FIXED","h1D_Bkg_FIXED",2048,0,2047);
	Read_File("02_09/bg_0209_FIXED.Spe",h1D_Bkg_FIXED);



	TCanvas *c1 = new TCanvas("c1","c1",800,600);
	h1D_Na22_FIXED->SetLineColor(kBlue);
	h1D_Bkg_FIXED->SetLineColor(kRed);	

	h1D_Na22_FIXED->Draw("P");
	h1D_Bkg_FIXED->Draw("same P");






}