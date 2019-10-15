/*  MOTORI NUOVI - NUOVO PROGRAMMA

  FUNZIONAMENTO IR VECCHIO
  Gli infrarossi ritornano valori tra 0 e 900, 900 è nero, -1 è solo un valore di debug
  per indicare che il sensore non è _iState in grado di leggere nulla, se il pin digitale del sensore
  è zero allora il sensore è collegato, altrimenti non è collegato / non è detecato da arduino

  FUNZIONAMENTO IR NUOVO
  Vedere lo sketch su GitHub riguardante es_IR
  
  FUNZIONAMENTO ALGORITMO
  Di seguito indicherò i sensori con sx, top e dx per sinistra, avanti e destra. Con sensori ON intendo la rilevazione del colore nero.
  Robot parte in posizione neutra con i sensori sulla linea nera, di conseguenza tutti attivati. Deve andare avanti proseguendo e avendo prevalentemente il sensore al mezzo sul nero
  e gli altri sul bianco. Per la curvatura verso il sensore opposto ON, modulare l'angolo con l'impiego della cache per verificare da quanto sta uscendo fuori
  per evitare robot a zig-zag.

  Riguardare logica curve a 90 gradi.

    tabella per 
    #define delay_giro_180 1028
    #define delay_giro_90 514
    #define delay_giro_45 257
    #define delay_giro_30 200
*/

//Librerie
#include <Servo.h> //servo motori

#pragma region PIN

//Ruote
#define PIN_RUOTA_SINISTRA 10
#define PIN_RUOTA_DESTRA 9

//Infrarossi
#define PIN_INFRA_DESTR_DIG 3
#define PIN_INFRA_AV_DIG 2
#define PIN_INFRA_SINIST_DIG 4

#pragma endregion

//Direzioni Servo (valori)
#define SERV_STOP 90
#define SERV_SINIST_AV 180
#define SERV_SINIST_DIETR 0
#define SERV_DEST_AV 0
#define SERV_DEST_DIETR 180

//enumeratore di stato indicante la direzione da seguire (dov'è la linea)
enum eState
{
  osx,
  xsx,
  sx,
  top_mid,
  top_all,
  back,
  dx,
  xdx,
  odx
};

//Cache stati precedenti (gioco di tempo/impatto per curve anomale)
eState _eState;
eState _eCache; //verificare necessarietà

//Bool Nero colore
bool _bNeroDestra;
bool _bNeroAvanti;
bool _bNeroSinistra;

//classe componente infrarossi casereccia
class Infrarossi
{
private:
  int _iPinDig;

public:
  //Costruttore parametri pin analog
  Infrarossi(int _digitale)
  {
    _iPinDig = _digitale;
    pinMode(_iPinDig, INPUT);
  }

  //Funzione bool ritorna true se il sensore vede nero
  bool IsBlack()
  {
    return digitalRead(_iPinDig);
  }
};

//I vari servo per i motori
Servo Ruota_Destra;
Servo Ruota_Sinistra;

//dichiarazione oggetti infrarossi
Infrarossi *InfraDestra;
Infrarossi *InfraAvanti;
Infrarossi *InfraSinistra;

//fermo ruota sinistra
void Stop_Sx()
{
  Ruota_Sinistra.write(SERV_STOP);
}

//ferma ruota destra
void Stop_Dx()
{
  Ruota_Destra.write(SERV_STOP);
}

//Procedura ferma entrambe le ruote
void Stop_Both()
{
  Ruota_Sinistra.write(SERV_STOP);
  Ruota_Destra.write(SERV_STOP);
}

//Procedura avanti con entrambe le ruote al max
void Go_Avanti()
{
  Ruota_Sinistra.write(SERV_SINIST_AV);
  Ruota_Destra.write(SERV_DEST_AV);
}

//Procedura retromarcia con entrambe le ruote al max (da curvare leggermente di qualche grado verso un lato?)
void Go_Dietro()
{
  Ruota_Sinistra.write(SERV_SINIST_DIETR);
  Ruota_Destra.write(SERV_DEST_DIETR);
}

//Procedura setta e avvia svolta a sx a seconda della proriotà data (xsx o sx)
void Vai_Sinistra(eState _incisione)
{
  // dx 90stop a 180max, sx da 90stop a 0max
  int _iGradazione_sx, _iGradazione_dx;
  switch (_incisione)
  {
  case osx:
    _iGradazione_dx = SERV_DEST_AV; //cammina-, curva+
    _iGradazione_sx = SERV_STOP;
    break;

  case xsx:
    _iGradazione_dx = 20;  //35  60
    _iGradazione_sx = 115; //80   20
    break;

  case sx:
    _iGradazione_dx = SERV_DEST_AV;  //cammina+, curva- 20   50
    _iGradazione_sx = SERV_STOP; //70    30
    break;
  }

  Ruota_Destra.write(_iGradazione_dx);
  Ruota_Sinistra.write(_iGradazione_sx);
}

//Procedura setta e avvia svolta a dx a seconda della priorità data (xdx o dx)
void Vai_Destra(eState _incisione)
{
  // dx 90stop a 180max, sx da 90stop a 0max
  int _iGradazione_dx, _iGradazione_sx;
  switch (_incisione)
  {
  case odx:
    _iGradazione_dx = SERV_STOP; //cammina-, curva+
    _iGradazione_sx = SERV_SINIST_AV;
    break;

  case xdx:
    _iGradazione_dx = 65;  //120  20
    _iGradazione_sx = 160; //130 60
    break;

  case dx:
    _iGradazione_dx = SERV_STOP;  //cammina+, curva- 135  30
    _iGradazione_sx = SERV_SINIST_AV; //120   50
    break;
  }

  Ruota_Sinistra.write(_iGradazione_sx);
  Ruota_Destra.write(_iGradazione_dx);
}

//Procedura principale in loop
void FollowTheLine()
{
  //get valori sensori
  _bNeroDestra = InfraDestra->IsBlack();
  _bNeroAvanti = InfraAvanti->IsBlack();
  _bNeroSinistra = InfraSinistra->IsBlack();

  //andare dritto se
  if (!_bNeroSinistra && _bNeroAvanti && !_bNeroDestra)
    _eState = top_mid;
  else if (_bNeroSinistra && _bNeroAvanti && _bNeroDestra)
    _eState = top_all;
  //se tutto bianco (robot uscito), controllare in cache da dove e assegnarlo
  else if (!_bNeroSinistra && !_bNeroAvanti && !_bNeroDestra)
  {
    switch (_eCache)
    {
    case osx:
    case xsx:
    case sx:
      _eState = osx;
      break;

    case odx:
    case xdx:
    case dx:
      _eState = odx;
      break;

    case top_all:
      _eState = back;
      break;
    }
  }
  else
  {
    //linea a sinistra
    if (_bNeroSinistra && !_bNeroAvanti && !_bNeroDestra)
      _eState = xsx; //andare a sinistra (mod+)
    //linea a sinistra e centro
    else if (_bNeroSinistra && _bNeroAvanti && !_bNeroDestra)
      _eState = sx; //andare a sinistra (mod)
    //linea a destra e centro
    else if (!_bNeroSinistra && _bNeroAvanti && _bNeroDestra)
      _eState = dx; //andare a destra (mod)
    //linea solo a destra
    else if (!_bNeroSinistra && !_bNeroAvanti && _bNeroDestra)
      _eState = xdx; //andare a destra (mod+)
  }

  //switch gestore del moto del robot
  switch (_eState)
  {
  case osx:
  case xsx:
  case sx:
    Vai_Sinistra(_eState);
    break;

  case top_mid:
  case top_all:
    Go_Avanti(); //vai avanti con sensori captanti nero ovunque o solo davanti
    break;

  case back:
    Go_Dietro();
    break;

  case dx:
  case xdx:
  case odx:
    Vai_Destra(_eState); //mod
    break;
  }

  _eCache = _eState;
}

void setup()
{
  Ruota_Sinistra.attach(PIN_RUOTA_SINISTRA);
  Ruota_Destra.attach(PIN_RUOTA_DESTRA);
  InfraDestra = new Infrarossi(PIN_INFRA_DESTR_DIG);
  InfraAvanti = new Infrarossi(PIN_INFRA_AV_DIG);
  InfraSinistra = new Infrarossi(PIN_INFRA_SINIST_DIG);
  delay(1000); //ritardo iniziale
}

void loop()
{
  FollowTheLine();
}