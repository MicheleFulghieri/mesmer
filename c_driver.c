#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char* argv[])
{
  if (argc != 2) {
    printf("Usage %s input_mesmer_card \n",argv[0]);
    exit(100);
  }

  double mm,me;
  int i,j;

  /* in-out for InMuMom_mesmer */
  double pmu[4];
  /* */
  
  /* in-out for mesmer_setup*/
  int sampletag;
  char mesmerversion[21];
  /* char mesmerversion; */
  char hostname[21];
  char datetime[21];
  char RCorder[5];
  int idproc;
  long int nev;
  int includedfs;
  int radmu;
  int rade;
  int iseed1;
  int iseed2;
  double emulab;
  double spread;
  double Qmu;
  double mumass;
  double elmass;
  double invalpha;
  double wnorm;
  double wmax;
  double eemin;
  double themin;
  double themax;
  double eemax;
  double thmumin;
  double thmumax;
  double ethr;
  double ththr;
  double acopl;
  double ela;
  int iacopl;
  int ihadon;
  int iela;
  int ivpwgts;
  int ivpfl;
  int nwarmup;
  int ndistrw;
  int isync;
  double eps;
  double phmass;
  int areweighted;
  int extmubeam;
  /* */
  /* in-out for generate_event_mesmer*/
  int nfs;
  int mcids[20];
  double pmat[20][4];
  double weight;
  int itag;
  long int ievtnr;
  double wnovp;
  double wnohad;
  double wLO;
  double wNLO;
  double cwvp[11];
  int ierr;
  /* */
  /* in-out for finalize_mesmer*/
  double xsw;
  double exsw;
  long int foohpm;
  long int fooh;
  double truemax;
  long int nabove;
  long int nlt0;
  double xsbias;
  double exsbias;
  double xsbiasn;
  double exsbiasn;
  double sumow;
  double sum2ow2;
  double sumnow;
  double sum2now2;
  /* */
  /* in-out for init_mesmer*/
  /* */
  
  extern void generate_event_mesmer(double *pmu, int *nfs, int *mcids, double (*pmat)[4], double *weight,
				    int *itag, long int *ievtnr, double *wnovp, double *wnohad, double *wLO,
				    double *wNLO, double *cwvp, int *ierr);
  extern void get_masses_mesmer(double* mm, double* me);
  extern void mesmer_setup
    (int* sampletag,
     char* mesmerversion,
     char* hostname,
     char* datetime,
     int* idproc,
     long int* nev,
     int* areweighted,
     char* RCorder,
     int* includedfs,
     int* radmu,
     int* rade,
     int* iseed1,
     int* iseed2,
     double* emulab,
     double* spread,
     int* extmubeam,
     double* Qmu,
     double* mumass,
     double* elmass,
     double* invalpha,
     double* wnorm,
     double* wmax,
     double* eemin,
     double* themin,
     double* themax,
     double* eemax,
     double* thmumin,
     double* thmumax,
     double* ethr,
     double* ththr,
     int* iacopl,
     double* acopl,
     int* iela,
     double* ela,
     int* ivpwgts,
     int* ihadon,
     int* ivpfl,
     int* nwarmup,
     int* ndistrw,
     int* isync,
     double* eps,
     double* phmass
     );
  extern void finalize_mesmer(double *xsw, double *exsw,long int *foohpm, long int *fooh, double *truemax,long int *nabove,
			       long int *nlt0, double *xsbias, double *exsbias, double *xsbiasn,
			       double *exsbiasn, double *sumow, double *sum2ow2,
			       double *sumnow, double *sum2now2);
  extern int init_mesmer(char* indatacard);

  extern void IncomingMuonMomentum_mesmer(double* pmu);
  
  int errinit = init_mesmer(argv[1]);  // pass the string with the data-card path to the init_mesmer function in userinterface.F

  if (errinit == 1) {  //init_mesmer function in userinterface.F returns 0 if all ok
    printf("Something wrong in initialization! Stopping!\n");
    return 1;
  };
  
  mesmer_setup
    (&sampletag,mesmerversion,hostname,datetime,&idproc,&nev,&areweighted,RCorder,
     &includedfs,&radmu,&rade,&iseed1,&iseed2,&emulab,&spread,&extmubeam,&Qmu,
     &mumass,&elmass,&invalpha,&wnorm,&wmax,&eemin,&themin,&themax,&eemax,&thmumin,
     &thmumax,&ethr,&ththr,&iacopl,&acopl,&iela,&ela,&ivpwgts,&ihadon,&ivpfl,&nwarmup,
     &ndistrw,&isync,&eps,&phmass); 

  printf("-----> %s\n",mesmerversion);
  printf("-----> %s\n",datetime);
  printf("-----> %.40f\n",wmax);
  printf("-----> %.40f\n",wnorm);
  
  double wsum = 0., wsum2 = 0.;
  double avg,error;
  int nadd = 1;

  int long n_photons = 0; // photon counter

  /* do loop over events */
  for (i=0; i<nev; i = i + nadd){
    nadd = 1;

    IncomingMuonMomentum_mesmer(pmu);
    generate_event_mesmer(pmu, &nfs, mcids, pmat, &weight, &itag, &ievtnr, &wnovp, &wnohad, &wLO, &wNLO, cwvp, &ierr);
    
    if (ierr == 0) {  // accepted event
      wsum  = wsum  + weight;        // cumulate the weights of the events satisfying the cuts
      wsum2 = wsum2 + weight*weight; // squared weights (for the MC variance)
      avg   = wsum/(ievtnr);         // partial mean of the weights (wsum / Ntrials)

    if (nfs == 3) {  // 3 fsp -> photon
      n_photons ++;  // update photon counter

      // C row-major pmat extraction pmat[row][col] = pmat[part][4mom idx]
      double E  = pmat[2][0];
      double px = pmat[2][1];   
      double py = pmat[2][2];   // third particle (photon), px
      double pz = pmat[2][3];

      double m_squared = E*E - (px*px + py*py + pz*pz);  // invariant mass

      if (n_photons <= 5) {     // only first 5 events
        printf("Real photon!\n");
        printf("  -> Energy: %.4f GeV\n", E);
        printf("  -> px: %.4f, py: %.4f, pz: %.4f\n", px, py, pz);
        printf("  -> Invariant mass: %.6f GeV^2\n", m_squared);
      }
    }
      

    } else {
      nadd = 0;
    }
  }
  /* end do loop over events */
  printf("Total number of real photons generated: %ld\n", n_photons);

  avg   = wsum/(ievtnr);
  error = fabs((wsum2/(ievtnr) - avg*avg))/(ievtnr);
  error = sqrt(error);  // final error on the mean of the weights
  
  printf("%.16f %.16f\n",avg,error); 

  finalize_mesmer(&xsw, &exsw, &foohpm, &fooh, &truemax, &nabove, &nlt0, &xsbias, &exsbias, &xsbiasn,
		  &exsbiasn, &sumow, &sum2ow2, &sumnow, &sum2now2);

  printf("%.16f %.16f\n",xsw,exsw);
  
  return 0;
}
