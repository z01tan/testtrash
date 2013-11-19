// ---------------------------------------------------------------------------
// Created by A.Galkin 16.04.2012

#include "MakeTeX-L.h"
#include <string.h>
#include <fcntl.h>

// ---------------------------------------------------------------------------

//*******************************************************
RepS Rep_st[200];
int Rep_cnt;
int NumTEX = 0;
char fn[] = "TestKOI.tex";
//******************************************************

char DocBeg[24][62] = {
	{"% LaTeX report document; Erstellt von GAI"},
	{"\\documentclass[10pt]{article}"},
	{"\\usepackage{geometry}"},
	{" \\geometry{a4paper}"},
	{" \\oddsidemargin = 0pt"},
	{" \\topmargin = -1,5cm \\headheight = 0pt \\textwidth = 180mm"},
	{" \\textheight = 14cm"},
	////!!!1mm = 2.65pt!!!!!!!!!!175 mm =~ 496pt!!!14cm = 396pt!!!!!
	{"\\headsep = 10pt"},
	{" \\flushbottom"},
	{"\\usepackage{latexsym}"},
	{"\\usepackage[T2A]{fontenc}"},
	{"\\usepackage{ucs}"},
	{"\\usepackage {longtable}"},
	{"\\usepackage[cp1251]{inputenc} % or utf8x"},
	{"\\usepackage[russian]{babel}"},
	{"\\frenchspacing"},
	{"\\usepackage{fancyhdr}"},
	{"\\pagestyle{fancyplain}"},
	{"\\renewcommand{\\headrulewidth}{0pt}"},
	{"\\usepackage{lastpage}"},
	{"\\usepackage{ifthen}"},
	{"\\usepackage{setspace}"},
    {"\\setstretch{1.1}"},
	{"\\fancyhf{}"}};
//-------------------------------------------------------------
char b_ffoot[] = "\\fancyfoot[C]{";
char e_ffoot[] = "\\center\\rm Лист \\thepage}";
char en_ffoot[] = "\\center\\rm Page \\thepage}";

char b_doc[] = "\\begin{document}";
char e_doc[] = "\\end{document}";
char b_tbl[] = "\\begin{longtable}";
char b_tbl1[]= "\\begin{tabular}{\0";
//% col =160/ summ(Columns); p{x} = col * sym; example:  p{65*(160/100)}
char t_tabl[]= "{p{18pt}p{280pt}p{42pt}p{112pt}}";
char e_tbl[] = "\\end{longtable}";
char e_tbl1[]= "\\end{tabular}\0";

char a_left[] = "\\flushleft ";
char a_centr[] = "\\center ";
char a_right[] = "\\flushright ";

char s_norm[]   = "\\rm ";
char s_ital[]   = "\\it ";
char s_slave[]  = "\\sl ";
char s_bold[]   = "\\bf ";
char s_tt[]     = "\\tt ";

char k_norm[] = "\\normalsize ";
char k_small[] = "\\small ";
char k_ssm[] = "\\footnotesize ";
char k_large[] = "\\large ";
char k_Large[] = "\\Large ";
char k_LARGE[] = "\\LARGE ";

char c_under[] = "\\tt \\underline{";
char c_space[] = "\\hspace{";
char c_quad[] = "\\quad ";
char c_verb[] = "\\verb'";
char c_eol[] = "\\\\";
char c_dotfill[] = "\\dotfill ";
char c_vsps[]   =   "[0mm]";
char comment[] = "%%COMMEMT";

char tabs[4][8] = {
                {"0.8cm}\0"},
                {"1.6cm}\0"},
                {"2.4cm}\0"},
                {"3.2cm}\0"}
        };

std::string s;
char si[400];
char fs1[200],fs2[200],fs3[200],fs4[20];
char a_si[30][400];
int Kegel;	//Текущий размер: 10 / 9 / 8 / 12 / 14
int Algn;    //Текущее выравнивание L - лево-1; C -центр-2; R - право-3
int LaufTab;
int P_cnt;

FILE* ft;
//==============================================================================
void bzero(void* s,int ln)
    {
    memset(s,0,ln);
    }
void save_si(char* s)
{
   int i;
   for(i=0;i<100;i++) if(si[i]!=0){*(s+i) = si[i];}
}
//==============================================================================
int Translate(char* f)
{ int i,k,pr,m;
//  char c[4];
  pr=0;k=0;   m=0;
  for (i = 0; i < 256; i++)
  {
   if (*(f+i)==0)
	{if (pr>0){pr=0;si[i+k]=0x7D;}return(m);}
   if (*(f+i)!='_')
	{if (pr>0){pr=0;si[i+k]=0x7D;k++;}
	 if (*(f+i)!=' ') si[i+k] =*(f+i);
	 else {memmove(&si[i+k],"\\ ",3);k+=1;}
	}
   else
   {
	if (pr>0) {memmove(&si[i+k],"\\ ",3);k+=1;}
	if (pr==0)
		{pr=1;m=1;memmove(&si[i+k],&c_under,17);k+=15;si[i+k]='~';}
   }
  }   return(m);
}
//------------------------------------------------------------------------------

//............. Check attributes of Line ....................................
int FormLine(int i)
{  	int k;
	int c;

    if(Rep_st[i].Txt.length()==0)return 0;
	k = Rep_st[i].Size;
		if (k != Kegel)     // если размер не совпадает с текущим
		 switch(k){
		  case 8 :{fprintf(ft,"%s",k_ssm);Kegel = 8;break;}
		  case 9 :{fprintf(ft,"%s",k_small);Kegel = 9;break;}
		  case 10:{fprintf(ft,"%s",k_norm);Kegel = 10;break;}
		  case 12:{fprintf(ft,"%s",k_large);Kegel = 12;break;}
		  case 14:{fprintf(ft,"%s",k_Large);Kegel = 14;break;}
		  case 16:{fprintf(ft,"%s",k_LARGE);Kegel = 16;break;}
		  default:fprintf(ft,"%s\n",comment);
		 }
		c = Rep_st[i].Alg;   // установим выравнивание
		if (c != Algn)
		switch (c) {
		 case 1:{fprintf(ft,"%s",a_left);Algn = 1;break;}
		 case 2:{fprintf(ft,"%s",a_centr);Algn = 2;break;}
		 case 3:{fprintf(ft,"%s",a_right);Algn = 3;break;}
		 default:fprintf(ft,"%s\n",comment);
		}
		c = Rep_st[i].Bold;  // установим начертание
        switch (c) {
		 case 2:{fprintf(ft,"%s",s_bold);break;}
		 case 3:{fprintf(ft,"%s",s_ital);break;}
		 case 1:{fprintf(ft,"%s",s_norm);break;}
		 case 0:{fprintf(ft,"%s",s_slave);break;}
		 case 4:{fprintf(ft,"%s",s_tt);break;}
		 default:fprintf(ft,"%s\n",comment);
		}
		if(Rep_st[i].Tab==0)return 1;
		if(Rep_st[i].Typ!= 2){
		c = Rep_st[i].Tab;
		if(c<0)c=0;
		if(c>4)c=4;
        if(c>0)fprintf(ft,"%s%s",c_space,tabs[c-1]);}
	return(1);
}

int FormLineF(int i)
{  	int k;
	int c;

    if((Rep_st[i].Txt.length()==0)&&(Rep_st[i].Value==0))return 0;
	k = Rep_st[i].Size;
        fs1[0]=0;
		if (k != Kegel)     // если размер не совпадает с текущим
		 switch(k){
		  case 8 :{sprintf(&fs1[0],"%s",k_ssm);Kegel = 8;break;}
		  case 9 :{sprintf(&fs1[0],"%s",k_small);Kegel = 9;break;}
		  case 10:{sprintf(&fs1[0],"%s",k_norm);Kegel = 10;break;}
		  case 12:{sprintf(&fs1[0],"%s",k_large);Kegel = 12;break;}
		  case 14:{sprintf(&fs1[0],"%s",k_Large);Kegel = 14;break;}
		  case 16:{sprintf(&fs1[0],"%s",k_LARGE);Kegel = 16;break;}
		  default:fs1[0]=0;
		 }
		c = Rep_st[i].Alg;   // установим выравнивание
		fs2[0]=0;
		if (c != Algn)
		switch (c) {
		 case 1:{sprintf(&fs2[0],"%s",a_left);Algn = 1;break;}
		 case 2:{sprintf(&fs2[0],"%s",a_centr);Algn = 2;break;}
		 case 3:{sprintf(&fs2[0],"%s",a_right);Algn = 3;break;}
		 default:fs2[0]=0;
		}
		c = Rep_st[i].Bold;  // установим начертание
		fs3[0]=0;
        switch (c) {
		 case 2:{sprintf(&fs3[0],"%s",s_bold);break;}
		 case 3:{sprintf(&fs3[0],"%s",s_ital);break;}
		 case 1:{sprintf(&fs3[0],"%s",s_norm);break;}
		 case 0:{sprintf(&fs3[0],"%s",s_slave);break;}
		 case 4:{sprintf(&fs3[0],"%s",s_tt);break;}
		 default:fs3[0]=0;
		}
		fs4[0]=0;
		if(Rep_st[i].Tab==0)return 1;
		if(Rep_st[i].Typ!= 2){
		c = Rep_st[i].Tab;
		if(c<0)c=0;
		if(c>4)c=4;
        if(c>0)sprintf(&fs4[0],"%s%s",c_space,tabs[c-1]);}
	return(1);
}
//---------------------------------------------------------------------------
int MakeFile(char* NameF)
{
int i,m, k;
int isTab;
int TabCount;
char s1[100],s2[100],s3[100];


	if ((ft = fopen(NameF, "w")) == NULL) return(-1);
  // 	 Обязательная часть - преамбула документа
	for (i = 0; i < 24; i++)    //  24 LINES Now!!!!!
	{
		fprintf(ft, "%s\n", DocBeg[i]);
	}
//......................................................

	 fprintf(ft,"%s",b_ffoot);
     fprintf(ft,"%s",k_ssm);
     fprintf(ft,"%s\n","\\ifthenelse{\\value{page}=\\pageref{LastPage}}");
	 Kegel = 6;
	 Algn  = 0; // Unknown!!!!!
	 P_cnt = 0;
	 LaufTab =0;
	 TabCount =0;
//.................. PageFooter Type ...........................................
	 isTab = 0;
	 for (i=0; i<Rep_cnt; i++)     // ищем Pagefooter
	 {
	  if ( Rep_st[i].Typ== 4)  //PageFooter Type
	  {
		bzero(&si[0], 400);
		if((isTab==1) && (Rep_st[i].Value==0))
            {sprintf(&a_si[P_cnt][0],"%s \n",e_tbl1);isTab=0;
                                        P_cnt++;TabCount=TabCount+2;}// Таблица кончилась - закрываем!
		if(Rep_st[i].Value>0)
        {
            if(isTab && (Rep_st[i].Value!=LaufTab))   // одна таблица сменяют другую
            {
                // закрываем текущую
                sprintf(&a_si[P_cnt][0],"%s \n",e_tbl1);isTab=0;P_cnt++;TabCount=TabCount+2;
            }
			if ((!isTab))
			{   isTab = 1;
                LaufTab = Rep_st[i].Value;
                sprintf(&a_si[P_cnt][0],"%s %s %i %s %i %s %i %s\n",b_tbl1,"p{",Rep_st[i].Cols[0],"mm}p{",Rep_st[i].Cols[1],"mm}p{",Rep_st[i].Cols[2],"mm}}");
                P_cnt++;
			}
        } //if(Rep_st[i].Value>0)!
        if(isTab)
        {
            bzero(&s1[0], 100);
            bzero(&s2[0], 100);
            bzero(&s3[0], 100);
            FormLineF(i);
            k = Translate(&Rep_st[i].Num[0]);
            if (k) Rep_st[i].Bold = 4; // TT Type must to use!
            save_si(&s1[0]);bzero(&si[0], 400);
            Translate(&Rep_st[i].NCand[0]);
            save_si(&s2[0]);bzero(&si[0], 400);
            Translate(&Rep_st[i].Txt[0]);
            sprintf(&a_si[P_cnt][0],"%s %s %s %s %s %s %s %s %s %s%s%i%s\n",fs1,fs2,fs3,fs4,s1,"&",s2,"&",si,c_eol,"[",Rep_st[i].Vsps,"pt]");
            P_cnt++;
        }
        else{
            k = Translate(&Rep_st[i].Txt[0]);
            if (k) Rep_st[i].Bold = 4; // TT Type must to use!
            if (FormLineF(i)>0)// CHECK for empty Txt inline FormLine()!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            {
                sprintf(&a_si[P_cnt][0],"%s %s%s%s%s %s %s%i%s\n",fs1,fs2,fs3,fs4,si,c_eol,"[",Rep_st[i].Vsps,"pt]");      // закончили строку подвала!
                P_cnt++;
            }
            }
	  } // Type == PageFooter
	 } // все просмотрели - заканчиваем footer
	 if(isTab){sprintf(&a_si[P_cnt][0],"%s \n",e_tbl1);isTab=0;
                                        P_cnt++;TabCount=TabCount+2;}// PageFooter кончилась - закрываем!
//!!!!!!!!!!!!!!!!!!! L A N G !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
         fprintf(ft,"%s%d%s","{\\vspace{",int(Kegel*1.3*2*(P_cnt-TabCount)),"pt}");
         if(Lang==0) fprintf(ft,"%s\n",e_ffoot);
         else fprintf(ft,"%s\n",en_ffoot);
    fprintf(ft,"%s","{");
    for(i=0;i<P_cnt;i++)
    {
        fprintf(ft,"%s",a_si[i]);
    }

    if(Lang==0) fprintf(ft,"%s\n",e_ffoot);
    else fprintf(ft,"%s\n",en_ffoot);
    fprintf(ft,"%s\n","}");
//......................End of PageFooter.......................................

	 fprintf(ft,"%s %i %s\n"," \\textheight = ",int(255-(Kegel*2.4*(P_cnt-TabCount))/2.65+0.5),"mm");

// ..................!!!!!! Begin DOKUMENT  !!!!!!!!!!! ........................
	fprintf(ft,"%s\n",b_doc);
	Kegel = 10;
	isTab = 0;
	 for (i=0; i<Rep_cnt; i++)     // просмотр для Head
	 {
	   if ( Rep_st[i].Typ== 1) { // Header Type
	     bzero(&si[0], 400);
		  k = Translate(&Rep_st[i].Txt[0]);
		  FormLine(i);
		  fprintf(ft,"%s %s\n",si,c_eol);
	   }  // end of Header
	  }  // End cicle for Head
//............................................................................
	   for (i=0; i<Rep_cnt; i++)     // просмотр для  ВСЕГО ТЕКСТА
	   {
	   if(Rep_st[i].Typ == 5)   // Immediate Command!!!
        fprintf(ft,"%s\n",Rep_st[i].Txt.c_str());

	   if ( Rep_st[i].Typ== 2)  // Table Type
	   {  	bzero(&si[0], 400);
            if(isTab && (Rep_st[i].Tab!=LaufTab))   // одна таблица сменяют другую
            {
                // закрываем текущую
                fprintf(ft,"%s \n",e_tbl);isTab=0;
            }
			if ((!isTab))
			{   isTab = 1;
                LaufTab = Rep_st[i].Tab;
			    FormLine(i);
				fprintf(ft,"%s",b_tbl);
			/*	k = Rep_st[i].Cols[0]+Rep_st[i].Cols[1]+Rep_st[i].Cols[2]+
					Rep_st[i].Cols[3]+Rep_st[i].Cols[4]; */
				k = abs(Rep_st[i].Cols[0])+abs(Rep_st[i].Cols[1])+abs(Rep_st[i].Cols[2])+
					abs(Rep_st[i].Cols[3])+abs(Rep_st[i].Cols[4]);
				double d = 160./k;
				fprintf(ft,"%s"," {");
				for	(m=0;m<5;m++)
				 if (Rep_st[i].Cols[m]!=0)
				   {
					  //fprintf(ft,"%s %i %s","p{",(int)(d*Rep_st[i].Cols[m]),"mm}");
					  fprintf(ft,"%s %i %s","p{",(int)(d*abs(Rep_st[i].Cols[m])),"mm}");
				   }
				fprintf(ft,"%s\n"," }");
			}
                for	(m=0;m<5;m++)
				 if (Rep_st[i].Cols[m]!=0)
				   {  switch (m)
					 { case 0:{
                            if(abs(Rep_st[i].Cols[0]==1)) fprintf(ft,"%s ",Rep_st[i].Txt.c_str());
                            else fprintf(ft,"%s ",Rep_st[i].Num.c_str());
                            if(Rep_st[i].Cols[0]<0)fprintf(ft,"%s",c_dotfill); //!!!!!!!!!!!
                            break;}
					   case 1:{fprintf(ft,"%s","&");
                            if (Rep_st[i].NCand.length()>0)fprintf(ft,"%s %s",
                            Rep_st[i].NCand.c_str(),". ");
                            if(Rep_st[i].Cols[1]<0)fprintf(ft,"%s",c_dotfill); //!!!!!!!!!!!
                            break;}
                       case 2:{fprintf(ft,"%s","&");fprintf(ft,"%s",Rep_st[i].Txt.c_str());
                            if(Rep_st[i].Cols[2]<0)fprintf(ft,"%s",c_dotfill); //!!!!!!!!!!!
                            break;}
					   case 3:{fprintf(ft,"%s %i","&",Rep_st[i].Value);
                            if(Rep_st[i].Cols[3]<0)fprintf(ft,"%s",c_dotfill); //!!!!!!!!!!!
                            break;}
					   case 4:{fprintf(ft,"%s %s","&",Rep_st[i].V_prop.c_str());break;}
					   default:;
					 }
					}  fprintf(ft,"%s %s %i %s\n",c_eol,"[",Rep_st[i].Vsps,"pt]");
	   }  // end of Body
	   if ( Rep_st[i].Typ== 3)  // Normal text Type
	   {  if(isTab){fprintf(ft,"%s \n",e_tbl);isTab=0;}
          bzero(&si[0], 400);
		  k = Translate(&Rep_st[i].Txt[0]);
		  FormLine(i);
		  fprintf(ft,"%s %s %s %i %s\n",si,c_eol,"[",Rep_st[i].Vsps,"pt]");
	   }
	  } // End cycle for Text
      if(isTab){fprintf(ft,"%s \n",e_tbl);isTab=0;}

//..............................................................................
    fprintf(ft,"%s \n","\\begin{figure}[ht]");
    for(i=0;i<P_cnt;i++)
        fprintf(ft,"%s",a_si[i]);
    fprintf(ft,"%s \n","\\end{figure}");


//..............................................................................
// !!! End of DOKUMENT  !!!!!!
	fprintf(ft,"%s\n",e_doc);
// Конец ТеХ файла - закрываем!
//	fprintf(ft,"%s \n",c_eol);
	fclose(ft);

return(1);
};
