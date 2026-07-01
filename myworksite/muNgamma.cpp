#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <fstream>
#include <iomanip>

// =========================================================================
// COSTANTI FISICHE GLOBALI (GeV, fm)
// =========================================================================
const double M_MU   = 0.10565837;    // Massa del muone
const double M_E    = 0.000510998;   // Massa dell'elettrone
const double ALPHA  = 1.0 / 137.036; // Costante di struttura fine
const double HBARC  = 0.197327;     // Conversione GeV * fm -> 1
const double PI     = 3.14159265358979323846;

// Struttura dati per un Quadrivettore (E, px, py, pz)
struct Quadrivector {
    double E, px, py, pz;
    
    // Prodotto scalare relativistico (metrica di Minkowski: + - - -)
    double dot(const Quadrivector& v) const {
        return E * v.E - (px * v.px + py * v.py + pz * v.pz);
    }
    
    double m2() const { return this->dot(*this); }
};

// =========================================================================
// FISICA: FORM FACTOR E ELEMENTO DI MATRICE
// =========================================================================
double nuclear_form_factor(double q2, double A) {
    if (q2 == 0.0) return 1.0;
    
    double q_val = std::sqrt(std::abs(q2)) / HBARC; // fm^-1
    double R1 = 1.2 * std::pow(A, 1.0 / 3.0);       // Raggio nucleare (fm)
    double s = 0.9;                                 // Spessore nucleare (fm)
    
    double qr = q_val * R1;
    // Sviluppo analitico della funzione sferica di Bessel j1(qr)
    double j1 = (std::sin(qr) - qr * std::cos(qr)) / (qr * qr);
    
    double F = 3.0 * j1 / qr * std::exp(- (q_val * q_val * s * s) / 2.0);
    return F;
}

double compute_matrix_element(const Quadrivector& p1, const Quadrivector& p2, 
                              const Quadrivector& p3, const Quadrivector& p4, 
                              const Quadrivector& k, double Z, double A) {
    // q = p2 - p4
    Quadrivector q = {p2.E - p4.E, p2.px - p4.px, p2.py - p4.py, p2.pz - p4.pz};
    double q2 = q.m2();
    
    double F_nuc = nuclear_form_factor(q2, A);
    double p3k = p3.dot(k);
    if (std::abs(p3k) < 1e-9) p3k = 1e-9;
    
    // Formula approssimata di Bethe-Heitler per il muone
    double base_amplitude = (Z * Z) * std::pow(ALPHA, 3) / (std::abs(q2) * p3k);
    
    return base_amplitude * (F_nuc * F_nuc);
}

// =========================================================================
// CINEMATICA: SPAZIO DELLE FASI A 3 CORPI
// =========================================================================
bool generate_event(double E_beam, double Z, double A, 
                    std::vector<Quadrivector>& kinematics, double& weight_PS,
                    std::mt19937& gen, std::uniform_real_distribution<double>& dis) {
    
    double M_N = A * 0.9315;          // Massa del nucleo target (GeV)
    double E_gamma_min = 2.0 * M_E;   // Taglio IR fisico (produzione di coppie)
    
    // 1. Stato iniziale nel Laboratorio
    Quadrivector p1 = {E_beam, 0.0, 0.0, std::sqrt(E_beam * E_beam - M_MU * M_MU)};
    Quadrivector p2 = {M_N, 0.0, 0.0, 0.0};
    
    Quadrivector P_tot = {p1.E + p2.E, p1.px + p2.px, p1.py + p2.py, p1.pz + p2.pz};
    double s = P_tot.m2();
    double sqrt_s = std::sqrt(s);
    
    // Estrazione variabili casuali
    double r1 = dis(gen), r2 = dis(gen), r3 = dis(gen), r4 = dis(gen), r5 = dis(gen);
    
    double M_X2_min = std::pow(M_MU + E_gamma_min, 2);
    double M_X2_max = std::pow(sqrt_s - M_N, 2);
    if (M_X2_max <= M_X2_min) return false;
    
    double M_X2 = M_X2_min + r1 * (M_X2_max - M_X2_min);
    double M_X = std::sqrt(M_X2);
    double w_jac = (M_X2_max - M_X2_min);
    
    // Decadimento 2 corpi: CM -> Nucleo (p4) + Sistema X
    double P_cms = std::sqrt(std::max(0.0, (s - std::pow(M_X + M_N, 2)) * (s - std::pow(M_X - M_N, 2)))) / (2.0 * sqrt_s);
    double costheta_N = -1.0 + 2.0 * r2;
    double sintheta_N = std::sqrt(std::max(0.0, 1.0 - costheta_N * costheta_N));
    double phi_N = 2.0 * PI * r3;
    w_jac *= 4.0 * PI;
    
    double E4_cms = std::sqrt(P_cms * P_cms + M_N * M_N);
    Quadrivector p4 = {E4_cms, P_cms * sintheta_N * std::cos(phi_N), P_cms * sintheta_N * std::sin(phi_N), P_cms * costheta_N};
    
    // Decadimento interno di X -> Muone (p3) + Fotone (k) nel Rest Frame di X
    double P_X_rf = (M_X2 - M_MU * M_MU) / (2.0 * M_X);
    double costheta_g = -1.0 + 2.0 * r4;
    double sintheta_g = std::sqrt(std::max(0.0, 1.0 - costheta_g * costheta_g));
    double phi_g = 2.0 * PI * r5;
    w_jac *= 4.0 * PI;
    
    Quadrivector k = {P_X_rf, P_X_rf * sintheta_g * std::cos(phi_g), P_X_rf * sintheta_g * std::sin(phi_g), P_X_rf * costheta_g};
    double E3_rf = std::sqrt(P_X_rf * P_X_rf + M_MU * M_MU);
    Quadrivector p3 = {E3_rf, -k.px, -k.py, -k.pz}; // Semplificato per l'esempio
    
    // Calcolo del peso dello spazio delle fasi
    double fact = (1.0 / std::pow(2.0 * PI, 5)) * (P_cms / (4.0 * sqrt_s)) * (P_X_rf / (4.0 * M_X));
    weight_PS = w_jac * fact;
    
    kinematics = {p1, p2, p3, p4, k};
    return true;
}

// =========================================================================
// MAIN PROGRAM
// =========================================================================
int main() {
    double E_beam = 150.0; // GeV
    double Z = 6.0;        // Carbonio
    double A = 12.0;
    int n_target_events = 50000;
    
    // Inizializzazione generatori casuali nativi C++11 (molto veloci su HPC)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    
    std::cout << "Avvio Generatore C++ per MESMER..." << std::endl;
    
    // Fase 1: Warm-up per W_max
    double W_max = 0.0;
    std::vector<Quadrivector> kin;
    double w_ps;
    
    for (int i = 0; i < 10000; ++i) {
        if (generate_event(E_beam, Z, A, kin, w_ps, gen, dis)) {
            double M2 = compute_matrix_element(kin[0], kin[1], kin[2], kin[3], kin[4], Z, A);
            double W = M2 * w_ps;
            if (W > W_max) W_max = W;
        }
    }
    W_max *= 1.3; // Fattore di sicurezza per picchi d'ampiezza
    std::cout << "W_max stimato: " << std::scientific << W_max << std::endl;
    
    // Fase 2: Unweighting e scrittura file
    std::ofstream outfile("mesmer_output.dat");
    outfile << "# File eventi Muone-Nucleo + Gamma (Stile MESMER)\n";
    
    int accepted = 0;
    long long trials = 0;
    
    while (accepted < n_target_events) {
        trials++;
        if (!generate_event(E_beam, Z, A, kin, w_ps, gen, dis)) continue;
        
        double M2 = compute_matrix_element(kin[0], kin[1], kin[2], kin[3], kin[4], Z, A);
        double W = M2 * w_ps;
        
        // Rejection Sampling
        if (dis(gen) * W_max < W) {
            accepted++;
            outfile << "<event>\n" << std::scientific << std::setprecision(8)
                    << "  13   " << kin[2].px << " " << kin[2].py << " " << kin[2].pz << "\n"  // Muone
                    << "  22   " << kin[4].px << " " << kin[4].py << " " << kin[4].pz << "\n"  // Fotone
                    << "  99   " << kin[3].px << " " << kin[3].py << " " << kin[3].pz << "\n"  // Nucleo
                    << "</event>\n";
            
            if (accepted % (n_target_events / 5) == 0) {
                std::cout << "Generati: " << accepted << " / " << n_target_events << " eventi." << std::endl;
            }
        }
    }
    
    std::cout << "Completato! Efficienza globale: " << (double)accepted / trials * 100.0 << "%" << std::endl;
    return 0;
}

