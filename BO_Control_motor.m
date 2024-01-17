A=[-b/J K/J; -K/L -R/L];
B=[0; 1/L];
C=[1,0];
D=0;

%%
pols=[-25 -25];  
K=acker(A,B,pols); 

%%
Cr=[1 0]; 
Dr=D;
M=[A B;
    Cr Dr];
Mi=M^(-1);

Nx=Mi([1,2],end);
Nu=Mi(3,end);


%% Model discret
h=50e-03;
[Phi, Gamma]=c2d(A,B,h);
pols_discret=[exp(pols*h)]; 
Kd=acker(Phi,Gamma,pols_discret); 

%% Disseny del observador
C_sensor=[1 0];
Pols_ob=pols*10; 
Pols_obs_dicret=[exp(Pols_ob*h)];
L=acker(Phi',C_sensor', Pols_obs_dicret)';


%% Disseny del sistema amb refus de perturbacions

Pol_ampliat=-25; 

%Modelització de la perturbació - Constant
Ad = [0];
Cd = [1];

%Model del sistema ampliat:
A_ampli=[A B
    0 0 0];
B_ampli=[B;0];
Cr_ampli=[Cr 0];


%Discretització del sistema ampliat
[Phi_ampli, Gamma_ampli]=c2d(A_ampli,B_ampli,h);
Pols_ampli=[pols Pol_ampliat];

%Contolador del sistema :
Kd_ampli=[Kd Cd];

%Observador del sistema ampliat:
Pols_ob_ampli=Pols_ampli*10;
Pols_obs_dicret_ampli=[exp(Pols_ob_ampli*h)];
L_ampli=acker(Phi_ampli',Cr_ampli', Pols_obs_dicret_ampli)';Nu

%definició de l'estrucutra NX i Nu per el sistema ampliat
Nx_ampli=[Nx;0];
Nu_ampli=Nu;

