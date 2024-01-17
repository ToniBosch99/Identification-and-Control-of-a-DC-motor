// CONFIGURACIO PINS ENTRADA I SORTIDA
int pin_intensitat = A0;
int pin_encoder = 2;
int pin_pwm = 3;

// INICIALITZACIO ESTATS
int sortida_arduino;
float x1 = 0.0; float x2 = 0.0;
float u = 0.0; float u_pwm = 0.0;
float y = 0.0;

// MATRIUS DE L'ESPAI D'ESTATS

float Nu = 1.05720;
float Nx[3] = {1.0, -2.6747, 0.0};
float Kd[3] = {0.7647, 0.0671, 1.0}; //{2.0001, 0.1268}; //{-1.2702, -0.1387};
float L[3] = {1.9135, 3.6608, 3.2260}; //{0.7493, -5.5762};//{0.9, -5.7195}; //{-0.2995, 3.025}; 
float Gamma[3] = {0.1913, 2.8926, 0.0};
float Phi[9] = {0.90840, 0.0414, 0.1913, -5.7195, 0.0051, 2.8929, 0.0, 0.0, 1.0};

// VARIABLES AUXILIARS
float r = 0.0;
float u_eq = 0.0;
float x_eq1 = 0.0; float x_eq2 = 0.0; float x_eq3 = 0.0;
float x_h1 = 0.0; float x_h2 = 0.0; float x_h3 = 0.0;
float e_c1 = 0.0; float e_c2 = 0.0;  float e_c3 = 0.0;
float y_h = 0.0; // estat predit
float x_h[3] = {0.0, 0.0, 0.0}; //estats de l'observador
float x_h_s[3] = {0.0, 0.0, 0.0};  
float eo = 0.0;

float t = 0.0;
long m1, m2; /*Store time in interrupts*/
long t_tot=0; /*Mean value of interrupts*/
long up_times[3]={9999999, 9999999, 9999999};

float speed_d; /*Reference of speed*/
float speed_r; /*Real speed of the motor*/
int index=0; /*Index for interrupts timing*/
unsigned long  t_consigna=0; /*Counter of reference*/
long t_min = 0; 
int pwm = 40; /*Initial value of PWM*/
float pol[5] = {3.11797305e-06, -1.29306215e-03,  1.99152156e-01, -1.27542524e+01, 3.39990601e+02}; /*Polinomi grau 4 */
float lectura_i = 0.0;
float intensitat = 0.0;

// FUNCIONS AUXILIARS
void int0()
{
  m1=micros();
  up_times[index]=m1-m2;
  index=(index+1)%3;
  m2=m1;
}

float rpm_to_pwm(float rpm)
{
  float pwm;
  pwm = pol[0]*pow(rpm, 4) + pol[1]*pow(rpm, 3) + pol[2]*pow(rpm, 2) + pol[3]*rpm + pol[4];
  if (pwm > 255){pwm = 255;}
  if (pwm < 0){pwm = 0;}
  return pwm;
}

void setup() {
  pinMode(pin_encoder, INPUT);
  digitalWrite(pin_encoder, HIGH);
  attachInterrupt(digitalPinToInterrupt(pin_encoder), int0, RISING);
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
  ; // wait for serial port to connect. Needed for native USB
  }
  pinMode(pin_intensitat, INPUT);//inputs
}


// LOOP
void loop() {
  // ------------------------------- 1. DEFINIR REFERENCIA
if ((micros() - t_consigna) > 1E6){
    // Set speed reference value
    if (r == 100.0) r = 100.0; else r = 100.0;
    // Update
    t_consigna = micros();
 }

  // ------------------------------- 2. CAPTURA I CONVERSIO 
  // Estat 1 -> Velocitat encoder
  t_min = up_times[0] + up_times[1] + up_times[2];
  y = 60E6/t_min/30; 

  // ------------------------------- 3. CALCUL CONTROL
  u_eq = r*Nu;
  
  x_eq1 = r*Nx[0];
  x_eq2 = r*Nx[1];
  x_eq3 = r*Nx[2];
  
  e_c1 = x_eq1 - x_h[0];
  e_c2 = x_eq2 - x_h[1];
  e_c3 = x_eq3 - x_h[2];
  
  u = u_eq + (Kd[0]*e_c1 + Kd[1]*e_c2 + Kd[2]*e_c3); /*RPM*/

  // Saturació observador estat 1
  if(u > 180.0){u = 180.0;} 
  if(u < 60.0){u = 60.0;}  
  
  // ------------------------------- 4. PREDICCIO ESTAT K+1 
  y_h = x_h[0];
  
  eo = y - y_h; // Comparacio y_mesurada amb y_estimada per l'observador
  
  // Calcul x_h instant k+1
  x_h[0] = L[0]*eo + Gamma[0]*u + Phi[0]*x_h[0] + Phi[1]*x_h[1] + Phi[2]*x_h[2];
  x_h[1] = L[1]*eo + Gamma[1]*u + Phi[3]*x_h[0] + Phi[4]*x_h[1] + Phi[5]*x_h[2];
  x_h[2] = L[2]*eo + Gamma[2]*u + Phi[6]*x_h[0] + Phi[7]*x_h[1] + Phi[8]*x_h[2];
    
  // ------------------------------- 5. RESULTATS
  lectura_i = analogRead(pin_intensitat);
  intensitat = lectura_i*(20.0/1023.0);
  
  u_pwm = rpm_to_pwm(u);
  analogWrite(pin_pwm, u_pwm); 
  
  Serial.print(y);
  Serial.print(" , ");
  Serial.print(u);
  Serial.print(" ; ");
  //Serial.print(u_pwm);
  //Serial.print(" ; ");
  Serial.println(r);
 
  
  // ------------------------------- 6. PAUSA
  delay(50); 
}
