/*---------------------HIKAS------------------------//
--------------O Jogo Musical Programável--------------

        - Projeto de Introdução Á Robótica -
  		Universidade Católica de Pernambuco      
#Grupo:
- Hemilly
- João
- Karolayne
- Lucas
- Samara

#O que é? Um jogo musical que junta diversas formas divertidas de
tocar musicas com intuito de ensinar e ser algo dinamico e não
monotono.

#Para que serve? Estimular memória, criatividade e precisão;

#Como Funciona?
O Hikas possui quatro modos Base,são eles:

1- Modo Livre: Onde se pode tocar livrimente as notas e modificar
as suas frequencias assim que desejam (Atravez do potenciometro);

2 - Modo Genius: Inspirado no classico jogo Genius dos anos 70,
o modo genius tem diferenças importantes e que dão nome ao jogo.
Nele, ao inves de 4, tem-se 7 teclas que simulam um mini teclado.
É possível selecionar a musica desejada (no momento tem-se duas
músicas configuradas), e o jogo repetirá a musica estimulando a
memoria do jogador atravez de uma musica e não sons atribuidos
aleatoriamento (como é o caso do Genius original);

3 - Modo Programação: Nesse modo tem-se duas opções
	3.1 - Edição: Nele pode-se liberar as notas do jogo genius e Scroll
    para serem livres, ou seja, não mais se prenderem as notas
    padroes (com frequencias predefinidas);
    3.2 - Gravar: Aqui é possível gravar sua musica na memoria
    do arduino para que posteriormente possa ser jogada no modo
    Geniusou Scroll. Na hora da gravação tambem é possivel modificar as
    frequencias atravez do potenciometro.
    
4 - Modo Scroll: Esse modo exige precisão e velocidade para acertar
as notas que correm pela tela. É possivel selecionar a velocidade
desejada e comecar a tocar.

Além dos modos Base, Hikas tambem é composto por outros elementos,
de igual importância, são eles:

- LCD: que controla os Menus e é a interface maquina-Jogador;
- Sensor: que coloca o jogo no modo Sleep, quando não é mais
  utilizado por determinado tempo;
- Potenciômetro Principal: Muda a frequencia das notas, simulando
  um mini sintetizador básico. Além de, no modo Scroll e Genius, 
  mudar a velocidade do jogo;
- Botão Principal: A "cabeça" do jogo, nele os modos são controlados
  e trocados, dependendo da vontade do jogador;
  
Entre diversos outros elementos, que possuem suas importancias
dentro do circuito geral.

---------------------------------------------------//
*/
class PushButton {
  public:
    PushButton(byte pinBotao);
    void button_loop(); 
    bool pressed();
  private:
    unsigned long debounceBotao;
    bool estadoBotaoAnt = HIGH;
    bool apertado = false;
    byte pino;
};

PushButton::PushButton(byte pinBotao) {
  pinMode(pinBotao, INPUT_PULLUP);
  pino = pinBotao;
}

void PushButton::button_loop() {
  bool estadoBotao = digitalRead(pino);
  apertado = false;
  if (!estadoBotao && estadoBotaoAnt) {
         apertado = true;      
  }
  estadoBotaoAnt = estadoBotao;
}

bool PushButton::pressed() {
  return apertado;
}
//------------------------GERAL------------------------//
//Bibliotecas
#include <LiquidCrystal.h>
//#include <avr/sleep.h>

//ShiftRegister
#define pinSH_CP 12   //Pino Clock
#define pinST_CP 10  //Pino Latch
#define pinDS    11  //Pino Data
#define qtdeCI 2  

// DEFINIÇÕES DE PINOS
#define pinBot1 8
#define pinBot2 0
#define pinBot3 15
#define pinBot4 16
#define pinBot5 17
#define pinBot6 18
#define pinBot7 19

#define pinBuzzer 13
#define pinPot 14
//Potenciometro
int pot;

//Usado no Millis
unsigned long agora = 0;

//Botão Multifunções
#define botaoPrincipal 9

//LCD
String mensagem = "HIKAS";
LiquidCrystal lcd(7,6,5,4,3,2);


//Sensor
/*#define sig 9
unsigned long distancia = 0;
unsigned long cm = 0;
*/
//------------------------LIVRE------------------------//
//Notas Piano 
int notasPadrao[] = {262,294,330,349,392,440,494};
int notas[7][7] = {
{33,37,41,44,49,55,62}, //Escala 01
{65,73,82,87,98,110,123}, //Escala 02
{131,147,165,175,196,220,247}, //Escala 03
{262,294,330,349,392,440,494}, //Escala 04
{523,587,659,698,784,880,988}, //Escala 05
{1047,1175,1319,1397,1568,1760,1976}, //Escala 06
{2093,2349,2637,2794,3136,3520,3951}}; //Escala 07

int escalaAtual = 4;
String nomeNotas[] = {"DO","RE","MI","FA","SOL","LA","SI"};
//------------------------GENIUS------------------------//
// Musica da Vitoria
int vitoria[] = {880,988,523,988,523,587,523,587,659,587,659,659};
#define duracaoVitoria 75

// Musica da Falha
int errado[] = {392,247};
int duracaoErrado[] = {500,750};

// DEFINIÇÕES
int nivelMax = 0;
// INSTANCIANDO OBJETOS
PushButton bot1(pinBot1);
PushButton bot2(pinBot2);
PushButton bot3(pinBot3);
PushButton bot4(pinBot4);
PushButton bot5(pinBot5);
PushButton bot6(pinBot6);
PushButton bot7(pinBot7);

// DECLARAÇÃO DE VARIÁVEIS
byte contador = 0;
unsigned int velocidade = 1000;
unsigned int tempoLimite;
bool timeOut = false;
String musicaEscolhida;
bool escolhaConcluida=false;
bool flagTrocaVez=true;
bool perdeu = false;
bool ganhou = false;
byte escala1,escala2;
bool escolhaConcluidaMenu;
byte escalas[20];

//MUSICA 01 -> MARIO THEME
//PADRÃO
/*int marioMelodiaLivre[] = {
  2637,2637,2637,2093,2637,3136,1568,2093,1568,
  1319,1760,1977,1760,1760,1568,2637,3136,
  3520,2794,3136,2637,2093,2349,1976,
  2093,1568,1319,1760,1976,1760,1760,
  1568,2637,3136,3520, 2794,3136,2637,2093,
  2349,1976
};

int marioMelodiaPadrao[] = {
 330,330,330,262,330,392,392,262,392,330,440,494,440,
 440,392,330,392,440,349,392,330,262,294,494,262,392, 
 330,440,494,440,440,392,330,392,440,349,392,330,262,294,494
};
byte tempoMario[] = {
  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
  9,9,9,9,9,9,9,9,7,7,7,9,9,9,9,9,9,9,9,
  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
  7,7,7,9,9,9,9,9,9,9,9,9,9,9,9,
};*/

byte sequenciaMario[41];
int noteDuration;
int pauseEntreNotas;
byte sequenciaUsuario[41];
//MUSICA 02 -> Zelda(Por motivos de armazenamento não é possivel ter
//ambas as musicas simultaneamente)
int zeldaMelodiaLivre[] = {
  440,349,349,440,392,349,392,440,349,349,440,440,392, 
  440,440,349,440,440,440,523,587,587,698,
  698,698,698,698,784,880, 880, 880, 880, 784, 698,784,
  698,698,698,587,587
};
//                      C    D   E   F   G   A  B
//int notasPadrao[] = {262,294,330,349,392,440,494};
int zeldaMelodiaPadrao[] = {
  440,349,349,440,392,349,392,440,349,349,440,440,392,440,
  440,349,440,440,440,262,294,294,349,349,349,349,349,392,
  440,440,440,440,392,349,392,349,349,349,294,294,349,349,
  349,294,262,262,294,349,294
};
byte tempoZelda[] = {
  1,4,4,4,8,8,2,8,1,4,4,4,8,8,2,2,2,8,8,8,8,8,8,2,2,
  8,8,8,8,8,2,2,8,8,8,8,8,8,8,2,4,4,4,4,4,4,4,4,4,4,4,4,
  8,8,4,4,4,4,4,2,8,8,4,8,8,4,8,8,4,4,2,2,8,8,8,8,8,8,2,
  2,8,8,8,8,8,2,4,4,1,4,1,4,4,4,1,4,1,4,4,4,1,4,1,4,2,1,
  4,4,8,8,4,4,8,8,8,4,8,8,4,8,8,4,4,4
};
byte sequenciaZelda[40];

//-----------------------SCROLL------------------------//
bool proxima;

//--------------------PROGRAMAÇÃO------------------------//
int melodiaMus01[25];
int musicasGravadas = 0;
int contNotas = 0;
String opcaoEscolhida;
bool escolhaOpcaoConcluida = false;
bool melodiaPadrao = true;
bool musicaFinalizada = false;
byte sequenciaMus01[25];
bool gravado;

void setup() {
  Serial.begin(9600);
  //LCD
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(1, 0);
   lcd.print(mensagem);
  //Botoes
   pinMode(botaoPrincipal, INPUT_PULLUP);
   pinMode(pinBot1, INPUT);
   digitalWrite(pinBot1,HIGH);
   pinMode(pinBot2, INPUT);
   digitalWrite(pinBot2,HIGH);
   pinMode(pinBot3, INPUT);
   digitalWrite(pinBot3,HIGH);
   pinMode(pinBot4, INPUT);
   digitalWrite(pinBot4,HIGH);
   pinMode(pinBot5, INPUT);
   digitalWrite(pinBot5,HIGH);
   pinMode(pinBot6, INPUT);
   digitalWrite(pinBot6,HIGH);
   pinMode(pinBot7, INPUT);
   digitalWrite(pinBot7,HIGH);
  //ShiftRegister
   pinMode(pinSH_CP, OUTPUT);
   pinMode(pinST_CP, OUTPUT);
   pinMode(pinDS, OUTPUT);
   //Buzzer
   pinMode(pinBuzzer, OUTPUT);

   pot = map(analogRead(pinPot),0,1023,1,147);
   mapearEscala();
   velocidade = 100;
   tempoLimite = 1000;
   randomSeed(analogRead(0));
}

void loop() {
/*OBS: Por questão de armazenamento do Tinkercad para 
utilizar o modo programação o descomente e comente dois outros
modos (Comente o if)
 */
//if (sensor() == true) { //Sensor ultrassonico
  //  acordar();
int command = getCommand();
  bool flag = false;
  
  if(command==1){
      zerarGenius();
      print("   Modo Livre   ",0,0);
       print("                ",0,1);
      ligarLed(8);
      desligarLed(9);
      desligarLed(10);
      desligarLed(11);
      delayMillis(100);
      modoLivre();
  }
  if(command==2){
   print("   Modo Genius  ",0,0);
      print("                ",0,1);
      zerarGenius();
      ligarLed(9);
      desligarLed(8);
      desligarLed(10);
      desligarLed(11);
      delayMillis(100);
      modoGenius();
  }
  if(command==3){
    zerarGenius();
     print("     Scroll    ",0,0);
     print("                ",0,1);
     ligarLed(10);
     desligarLed(11);
     desligarLed(9);
     desligarLed(8);
     delayMillis(100);
     modoScroll();
  }
 /*if(command==4){
     zerarGenius();
     print("  Programacao   ",0,0);
     ligarLed(11);
     desligarLed(10);
     desligarLed(9);
     desligarLed(8);
     delayMillis(100);
     modoProgramacao();
  }*/
  /*}else{
  dormir();
}*/
}

//------------------------SCROLL------------------------//
void modoScroll(){
  float vel= 200;
  bool velocidadeEscolhida = false;
  escolhaConcluida = false;
  perdeu = false;
  ganhou = false;
  while(velocidadeEscolhida==false){
    print("Escolha        ",0,0);
    print("Velocidade C-OK",0,1);
  if(digitalRead(pinBot1) == LOW){
    vel= map(analogRead(pinPot),0,1023,60,250);
    velocidadeEscolhida=true;
  }
  }
  while(escolhaConcluida==false){
   menu(); 
  }
  
  while(perdeu==false && ganhou==false && digitalRead(botaoPrincipal)==HIGH){ 
   if(musicaEscolhida=="Mario"){
    /*if(melodiaPadrao==true){
      scrollMusic(vel,marioMelodiaPadrao,nivelMax);
     }
     else{
      scrollMusic(vel,marioMelodiaLivre,nivelMax);
     }*/
     }
    else if(musicaEscolhida=="Zelda"){
      if(melodiaPadrao==true){
      scrollMusic(vel,zeldaMelodiaPadrao,nivelMax);
     }
     else{
      scrollMusic(vel,zeldaMelodiaLivre,nivelMax);
     }
    }
    else if(musicaEscolhida=="Mus01"){
     scrollMusic(vel,melodiaMus01,nivelMax);
   }
  }
}
void scroll(float ScrollSpeed,int pinBot, String nota, byte escala,byte pos){
  bool flag = false;
  int count = 1;
  lcd.begin(16, 2);               
  lcd.setCursor(16,pos); 
  
  lcd.print(nota+"_"+(escala+1));
  Serial.print(nota);
  
    while (digitalRead(pinBot)==HIGH && count < 18) {
    delay(ScrollSpeed);
    lcd.scrollDisplayLeft();
    count++;
     if(digitalRead(pinBot)==LOW){
       ligarLed(identificaNota(nota)); 
       tone(pinBuzzer,notas[escala][identificaNota(nota)],300);
       delayMillis(40);
       desligarLed(identificaNota(nota));
       proxima = true;
      }
  }

  if  (digitalRead(pinBot)==HIGH){
    lcd.begin(16, 2);             
    lcd.setCursor(3,0);
    perdeu = true;
    
    lcd.print("GAME OVER!!");
    delay(2000);
    lcd.setCursor(0,1);
    lcd.print("Jogue Novamente");
    delay(2000);
     lcd.print("               ");
  }
  else{ }
  
}
int identificaNota(String nota){
  if(nota=="DO"){ return 0;} 
  else if(nota=="RE"){ return 1;} 
  else if(nota=="MI"){ return 2;} 
  else if(nota=="FA"){ return 3;} 
  else if(nota=="SOL"){ return 4;} 
  else if(nota=="LA"){ return 5;} 
  else if(nota=="SI"){ return 6;} 
}

void scrollMusic(float vel,int melodia[], int tam){
  int nota;
  int i = 0;
  byte escala;
  byte pos;
  for(i=0;i<tam&&perdeu==false&&ganhou==false;i++){
    
    nota = melodia[i];
    proxima = false;
    escala = acharEscala(nota);
    pos = random(2);
    
    if(nota==notas[escala][0]){ 
     while(proxima==false&&perdeu==false){ 
     if(digitalRead(botaoPrincipal)==LOW){
     	perdeu = true;
     	break; 
     }
      scroll(vel,pinBot1,"DO",escala,pos);
     }
    }
    else if(nota==notas[escala][1]){
       while(proxima==false&&perdeu==false){
        if(digitalRead(botaoPrincipal)==LOW){
     	perdeu = true;
     	break; 
     }
       scroll(vel,pinBot2,"RE",escala,pos);
    }
    }
     else if(nota==notas[escala][2]){ 
       while(proxima==false&&perdeu==false){
        if(digitalRead(botaoPrincipal)==LOW){
     	perdeu = true;
     	break; 
        }
        scroll(vel,pinBot3,"MI",escala,pos);
       }
    }
     else if(nota==notas[escala][3]){ 
     while(proxima==false&&perdeu==false){ 
       if(digitalRead(botaoPrincipal)==LOW){
     	perdeu = true;
     	break; 
     }
        scroll(vel,pinBot4,"FA",escala,pos);
       
     }
    }
     else if(nota==notas[escala][4]){ 
      while(proxima==false&&perdeu==false){
        if(digitalRead(botaoPrincipal)==LOW){
     	perdeu = true;
     	break; 
     }
        scroll(vel,pinBot5,"SOL",escala,pos);
      }
    }
     else if(nota==notas[escala][5]){ 
      while(proxima==false&&perdeu==false){
        if(digitalRead(botaoPrincipal)==LOW){
     	perdeu = true;
     	break; 
     }
        scroll(vel,pinBot6,"LA",escala,pos);
      }
    }
     else if(nota==notas[escala][6]){ 
      while(proxima==false&&perdeu==false){
        if(digitalRead(botaoPrincipal)==LOW){
     	perdeu = true;
     	break; 
     }
        scroll(vel,pinBot7,"SI",escala,pos);
      }
      }
    if(i==tam-1){
      ganhou = true;
    }
  }
  if(ganhou==true){
    lcd.begin(16, 2);             
    lcd.setCursor(2,0);
    lcd.print("  VITORIA!!  ");
    delay(2000);
    lcd.setCursor(0,1);
    lcd.print("Jogue Novamente");
    delay(2000);
  }
}
//------------------------LIVRE------------------------//
void modoLivre(){
  while(digitalRead(botaoPrincipal)==HIGH){
  
  mapearEscala();
  int ultimaLed=0;
   while(digitalRead(pinBot1) == LOW)
  {
    tone(pinBuzzer,notas[escalaAtual][0]);
    ligarLed(0);
    ultimaLed = 0;
    print(nomeNotas[0]+"_"+(escalaAtual+1)+"        ",0,1);
  }
  
   while(digitalRead(pinBot2) == LOW)
  {
    tone(pinBuzzer,notas[escalaAtual][1]);
     ligarLed(1);
     ultimaLed=1;
     print(nomeNotas[1]+"_"+(escalaAtual+1)+"        ",0,1);
  }
  while(digitalRead(pinBot3) == LOW)
  {
    tone(pinBuzzer,notas[escalaAtual][2]);
     ligarLed(2);
     ultimaLed=2;
     print(nomeNotas[2]+"_"+(escalaAtual+1)+"        ",0,1);
  }
  while(digitalRead(pinBot4) == LOW)
  {
    tone(pinBuzzer,notas[escalaAtual][3]);
     ligarLed(3);
     ultimaLed=3;
     print(nomeNotas[3]+"_"+(escalaAtual+1)+"        ",0,1);
  }
  while(digitalRead(pinBot5) == LOW)
  {
    tone(pinBuzzer,notas[escalaAtual][4]);
     ligarLed(4);
     ultimaLed=4;
     print(nomeNotas[4]+"_"+(escalaAtual+1)+"        ",0,1);
  }
  while(digitalRead(pinBot6) == LOW)
  {
    tone(pinBuzzer,notas[escalaAtual][5]);
     ligarLed(5);
     ultimaLed=5;
     print(nomeNotas[5]+"_"+(escalaAtual+1)+"        ",0,1);
  }
  while(digitalRead(pinBot7) == LOW)
  {
    tone(pinBuzzer,notas[escalaAtual][6]);
     ligarLed(6);
     ultimaLed=6;
     print(nomeNotas[6]+"_"+(escalaAtual+1)+"        ",0,1);
  }
  
  noTone(pinBuzzer);
  desligarLed(ultimaLed);
  }
}
void mapearEscala(){
  pot = map(analogRead(pinPot),0,1023,1,147);
  if(pot <=21){ escalaAtual = 0;} 
  else if(pot<=42){ escalaAtual = 1;} 
  else if(pot<=63){ escalaAtual = 2;}
  else if(pot<=84){ escalaAtual = 3;}
  else if(pot<=105){ escalaAtual = 4;}
  else if(pot<=126){ escalaAtual = 5;}
  else if(pot<=147){ escalaAtual = 6;}
}
//------------------------GENIUS------------------------//
void zerarGenius(){
  contador = 0;
  timeOut = false;
  escolhaConcluida=false;
  flagTrocaVez=true;
  perdeu = false;
  ganhou = false; 
}
void modoGenius(){
 bool velocidadeEscolhida = false;
   while(velocidadeEscolhida==false){
    print("Escolha        ",0,0);
    print("Velocidade C-OK",0,1);
  if(digitalRead(pinBot1) == LOW){
    velocidade = map(analogRead(pinPot),0,1023,40,100);
    tempoLimite = map(velocidade,400,2000,1500,3000);
    velocidadeEscolhida=true;
  }
  }
  while(escolhaConcluida==false){
   menu(); 
  }
  while(!perdeu && !ganhou && digitalRead(botaoPrincipal)==HIGH){ 
   if(musicaEscolhida=="Mario"){
    /* if(melodiaPadrao==true){
       jogarPadrao(sequenciaMario,marioMelodiaPadrao,tempoMario);
     }
     else{
       jogarLivre(sequenciaMario,marioMelodiaLivre,tempoMario);
     }*/
     }
     else if(musicaEscolhida=="Zelda"){
      if(melodiaPadrao==true){
      jogarPadrao(sequenciaZelda,zeldaMelodiaPadrao,tempoZelda);
     }
     else{
      jogarLivre(sequenciaZelda,zeldaMelodiaLivre,tempoZelda);
     }
    }
    else if(musicaEscolhida=="Mus01"){
       jogarLivre(sequenciaMus01,melodiaMus01,tempoZelda);//vetor de tempo padrao
   }
  }
}
void menu(){
  Serial.print(musicasGravadas);
  //if(musicasGravadas>=1){
  if(gravado==true){
  print("D-Mario     F ->",0,0);
  print("E-Zelda         ",0,1);
  if(digitalRead(pinBot2) == LOW){
    musicaEscolhida = "Mario";
    nivelMax = 41;
    escolhaConcluida=true;
  }
  if(digitalRead(pinBot3) == LOW){
    musicaEscolhida = "Zelda";
    nivelMax = 40;
    escolhaConcluida=true;
  }
  
  if(digitalRead(pinBot4) == LOW){
    while(escolhaConcluida==false){
    telaUser();
    }
  }
  }else{
  print("D-Mario       ",0,0);
  print("E-Zelda         ",0,1);
  if(digitalRead(pinBot2) == LOW){
    musicaEscolhida = "Mario";
    nivelMax = 41; 
    escolhaConcluida=true;
  }
  if(digitalRead(pinBot3) == LOW){
    musicaEscolhida = "Zelda";
    nivelMax = 40;
    escolhaConcluida=true;
  }
  }
}
void telaUser(){
   print("MusicaUser    ",0,0);
   print("G-Mus01       ",0,1);
  if(digitalRead(pinBot5) == LOW){
    musicaEscolhida = "Mus01";
    escolhaConcluida=true;
  }
}

void jogarPadrao(byte sequencia[], int melodia[], byte tempo[]){
  trocaVez();
  jogadaArduino(sequencia,melodia);
  musicaAteAgora(sequencia,melodia,tempo);
  trocaVez();
  vezUsuario(melodia);
  
 if (confere(sequencia) == 0) {
      print("DERROTA",0,0);
      perdeu=true;
      for (byte i = 0 ; i < 7; i++) {
        ligarLed(i);
        delayMillis(10);
        desligarLed(i);
      }
  }
  if (contador == nivelMax) {
    print("VITORIA",0,0);
    ganhou=true;
    for (byte nota = 0; nota < 12 ; nota++) {
      tone(pinBuzzer, vitoria[nota], duracaoVitoria);
      delayMillis(duracaoVitoria);
    }
      for (byte i = 0 ; i < 7; i++) {
        ligarLed(i);
        delayMillis(10);
        desligarLed(i);
      }
  }
}

//FUNÇÕES - MODO PADRÃO
void musicaAteAgora(byte sequencia[], int melodia[], byte tempo[]) {
  for (byte i = 0 ; i < contador; i++) {
    noteDuration = 1000 / tempo[i];
    print(nomeNotas[sequencia[i]]+"_"+(acharEscala(melodia[i])+1)+"        ",0,1);
    ligarLed(sequencia[i]);
    //delayMillis(20);
    tone(pinBuzzer, melodia[i], velocidade);
    delayMillis(velocidade);
    desligarLed(sequencia[i]);
    //pauseEntreNotas = noteDuration * 1.30;
    //delayMillis(pauseEntreNotas);
    delayMillis(velocidade/2);
  }
}
void jogadaArduino(byte sequencia[], int melodia[]){
  sequencia[contador] = identificaBotao(melodia[contador]);
  contador++;
  sequencia[contador] = identificaBotao(melodia[contador]);
  contador++;
}

void vezUsuario(int melodia[]) {
  unsigned long controleTempo = millis();
  byte leitura = 0;
  while (millis() < controleTempo + tempoLimite && leitura < contador) {
    bot1.button_loop(); bot2.button_loop(); bot3.button_loop(); bot4.button_loop();
    bot5.button_loop(); bot6.button_loop(); bot7.button_loop(); 
    if (bot1.pressed()) {
      sequenciaUsuario[leitura] = 0;
  	  controleTempo = millis();
      leitura++;
      ligarLed(0);
      print(nomeNotas[0]+"_"+4+"        ",0,1);
      tone(pinBuzzer, notasPadrao[0], velocidade);
      delayMillis(velocidade);
      desligarLed(0);
    } else if (bot2.pressed()) {
      sequenciaUsuario[leitura] = 1;
  	  controleTempo = millis();
      leitura++;
      ligarLed(1);
      print(nomeNotas[1]+"_"+4+"        ",0,1);
      tone(pinBuzzer, notasPadrao[1], velocidade);
      delayMillis(velocidade);
      desligarLed(1);
    } else if (bot3.pressed()) {
      sequenciaUsuario[leitura] = 2;
  	  controleTempo = millis();
      leitura++;
      ligarLed(2);
      print(nomeNotas[2]+"_"+4+"        ",0,1);
      tone(pinBuzzer, notasPadrao[2], velocidade);
      delayMillis(velocidade);
      desligarLed(2);
    } else if (bot4.pressed()) {
      sequenciaUsuario[leitura] = 3;
  	  controleTempo = millis();
      leitura++;
      ligarLed(3);
      print(nomeNotas[3]+"_"+4+"        ",0,1);
      tone(pinBuzzer, notasPadrao[3], velocidade);
      delayMillis(velocidade);
      desligarLed(3);
    }else if (bot5.pressed()) {
      sequenciaUsuario[leitura] = 4;
  	  controleTempo = millis();
      leitura++;
      ligarLed(4);
      print(nomeNotas[4]+"_"+4+"       ",0,1);
      tone(pinBuzzer, notasPadrao[4], velocidade);
      delayMillis(velocidade);
      desligarLed(4);
    }else if (bot6.pressed()) {
      sequenciaUsuario[leitura] = 5;
  	  controleTempo= millis();
      leitura++;
      ligarLed(5);
      print(nomeNotas[5]+"_"+4+"        ",0,1);
      tone(pinBuzzer, notasPadrao[5], velocidade);
      delayMillis(velocidade);
      desligarLed(5);
    }else if (bot7.pressed()) {
      sequenciaUsuario[leitura] = 6;
  	  controleTempo = millis();
      leitura++;
      ligarLed(6);
      print(nomeNotas[6]+"_"+4+"        ",0,1);
      tone(pinBuzzer, notasPadrao[6], velocidade);
      delayMillis(velocidade);
      desligarLed(6);
    }
    
    if (millis() > controleTempo + tempoLimite){
      timeOut = true;
      break;
    }

  }
  
  delayMillis(velocidade/2);
}

// FUNÇÃO PARA INDICAR A TROCA DE VEZ
void trocaVez() {
  if(flagTrocaVez==true){
    print("VEZ HIKAS   ",0,0);
    flagTrocaVez=false;
  }else{
    print("SUA VEZ     ",0,0);
    flagTrocaVez=true;
  }
}

bool confere(byte sequencia[]) {
  bool resultado = true;
  for (byte i = 0; i < contador; i++) {
    if (sequencia[i] != sequenciaUsuario[i]) {
      resultado = false;
      //som de erro
      for (byte nota = 0; nota < 2 ; nota++) {
        tone(pinBuzzer, errado[nota], duracaoErrado[nota]);
        delayMillis(duracaoErrado[nota]);
      }
      break;
    }
  
  }
  return resultado;
}
//FUNÇÕES - MODO LIVRE
void jogarLivre(byte sequencia[], int melodia[], byte tempo[]){
  trocaVez();
  jogadaArduinoLivre(sequencia,melodia);
  musicaAteAgoraLivre(sequencia,melodia,tempo);
  trocaVez();
  vezUsuarioLivre();
  
 if (confere(sequencia) == 0) {
      print("    DERROTA     ",0,0);
      print("                ",0,1);
      perdeu=true;
      for (byte i = 0 ; i < 7; i++) {
        ligarLed(i);
        delayMillis(10);
        desligarLed(i);
      }
  }
  if (contador == nivelMax) {
    print("     VITORIA    ",0,0);
    print("                ",0,1);
    ganhou=true;
    for (byte nota = 0; nota < 12 ; nota++) {
      tone(pinBuzzer, vitoria[nota], duracaoVitoria);
      delayMillis(duracaoVitoria);
    }
      for (byte i = 0 ; i < 7; i++) {
        ligarLed(i);
        delayMillis(10);
        desligarLed(i);
      }
  }
}
byte identificaBotao(int nota){
  if(nota==notas[escalaAtual][0]){ return 0;}
  else if(nota==notas[escalaAtual][1]){ return 1;}
  else if(nota==notas[escalaAtual][2]){ return 2;}
  else if(nota==notas[escalaAtual][3]){ return 3;}
  else if(nota==notas[escalaAtual][4]){ return 4;}
  else if(nota==notas[escalaAtual][5]){ return 5;}
  else if(nota==notas[escalaAtual][6]){ return 6;}
}
void jogadaArduinoLivre(byte sequencia[], int melodia[]){
 
  escalas[contador] = acharEscala(melodia[contador]);
  escalaAtual = escalas[contador];
  sequencia[contador] = identificaBotao(melodia[contador]);
  contador++;
  
  escalas[contador] = acharEscala(melodia[contador]);
  escalaAtual = escalas[contador];
  sequencia[contador] = identificaBotao(melodia[contador]);
  contador++;
}

byte acharEscala(int nota){
  if(nota<=62){
    return 0;
  }else if(nota<=123){
    return 1;
  }else if(nota<=247){
    return 2;
  }else if(nota<=494){
    return 3;
  }else if(nota<=988){
    return 4;
  }else if(nota<=1976){
    return 5;
  }else{
    return 6;
  } 
}
void vezUsuarioLivre(){
  unsigned long controleTempo = millis();
  byte leitura = 0;
  int i =0;
  
  while (millis() < controleTempo + tempoLimite && leitura < contador) {
    bot1.button_loop(); bot2.button_loop(); bot3.button_loop(); bot4.button_loop();
    bot5.button_loop(); bot6.button_loop(); bot7.button_loop(); 
    if (bot1.pressed()) {
      sequenciaUsuario[leitura] = 0;
  	  controleTempo = millis();
      leitura++;
      ligarLed(0);
      print(nomeNotas[0]+"_"+(escalas[i]+1)+"        ",0,1);
      tone(pinBuzzer, notas[escalas[i]][0], velocidade);
     
      delayMillis(velocidade);
      desligarLed(0);
      i++;
    } else if (bot2.pressed()) {
      sequenciaUsuario[leitura] = 1;
  	  controleTempo = millis();
      leitura++;
      ligarLed(1);
      print(nomeNotas[1]+"_"+(escalas[i]+1)+"        ",0,1);
      tone(pinBuzzer, notas[escalas[i]][1], velocidade);
      i++;
      
      delayMillis(velocidade);
      desligarLed(1);
    } else if (bot3.pressed()) {
      sequenciaUsuario[leitura] = 2;
  	  controleTempo = millis();
      leitura++;
      ligarLed(2);
      print(nomeNotas[2]+"_"+(escalas[i]+1)+"        ",0,1);
      tone(pinBuzzer, notas[escalas[i]][2], velocidade);
      i++;
      
      delayMillis(velocidade);
      desligarLed(2);
    } else if (bot4.pressed()) {
      sequenciaUsuario[leitura] = 3;
  	  controleTempo = millis();
      leitura++;
      ligarLed(3);
      print(nomeNotas[3]+"_"+(escalas[i]+1)+"        ",0,1);
      tone(pinBuzzer, notas[escalas[i]][3], velocidade);
      i++;
      
      delayMillis(velocidade);
      desligarLed(3);
    }else if (bot5.pressed()) {
      sequenciaUsuario[leitura] = 4;
  	  controleTempo = millis();
      leitura++;
      ligarLed(4);
      print(nomeNotas[4]+"_"+(escalas[i]+1)+"        ",0,1);
      tone(pinBuzzer, notas[escalas[i]][4], velocidade);
      i++;
      
      delayMillis(velocidade);
      desligarLed(4);
    }else if (bot6.pressed()) {
      sequenciaUsuario[leitura] = 5;
  	  controleTempo= millis();
      leitura++;
      ligarLed(5);
      print(nomeNotas[5]+"_"+(escalas[i]+1)+"        ",0,1);
      tone(pinBuzzer, notas[escalas[i]][5], velocidade);
      i++;
      
      delayMillis(velocidade);
      desligarLed(5);
    }else if (bot7.pressed()) {
      sequenciaUsuario[leitura] = 6;
  	  controleTempo = millis();
      leitura++;
      ligarLed(6);
      print(nomeNotas[6]+"_"+(escalas[i]+1)+"        ",0,1);
      tone(pinBuzzer, notas[escalas[i]][6], velocidade);
      i++;
      
      delayMillis(velocidade);
      desligarLed(6);
    }
    
    if (millis() > controleTempo + tempoLimite){
      timeOut = true;
      break;
    }

  }
  
  delayMillis(velocidade/2);
}
void musicaAteAgoraLivre(byte sequencia[], int melodia[], byte tempo[]) {
  for (byte i = 0 ; i < contador; i++) {
    noteDuration = 1000 / tempo[i];
    print(nomeNotas[sequencia[i]]+"_"+(acharEscala(melodia[i])+1)+"        ",0,1);
    ligarLed(sequencia[i]);
    delayMillis(20);
    tone(pinBuzzer, melodia[i], noteDuration);
    desligarLed(sequencia[i]);
    pauseEntreNotas = noteDuration * 1.30;
    delayMillis(pauseEntreNotas);
  }
}
//--------------------PROGRAMAÇÃO------------------------//
void modoProgramacao(){
   while(escolhaOpcaoConcluida==false){
   menuProg(); 
  }
  if(opcaoEscolhida=="Gravar"){
    musicasGravadas=1;
    gravado = true;
    print("Gravando...     ",0,0);
    print("                ",0,1);
    while(musicaFinalizada == false){
        gravarMusica(); 
    }
    print("Musica Gravada  ",0,0);
    print("                ",0,1);
    tocarMusicaGravada();
  }
  if(opcaoEscolhida=="Editar"){
     print("Genius Editado",0,0);
     print("                ",0,1);
  }
  
}
  void menuProg(){
  print("C-Editar     ",0,0);
  print("D-Gravar     ",0,1);
 
  if(digitalRead(pinBot1) == LOW){
    ligarLed(0);
    delayMillis(40);
    desligarLed(0);
    while(escolhaOpcaoConcluida==false){//ou retornar
      menuEdicao();
    }
  }
  if(digitalRead(pinBot2) == LOW){
    ligarLed(1);
    delayMillis(40);
    desligarLed(1);
    opcaoEscolhida = "Gravar";
    escolhaOpcaoConcluida=true;
  }
}

void menuEdicao(){
  print("E-Tom Padrao",0,0);
  print("F-Tom Livre",0,1);
  if(digitalRead(pinBot3) == LOW){
    editar("Padrao");
    ligarLed(2);
    delayMillis(40);
    desligarLed(2);
    escolhaOpcaoConcluida=true;
    opcaoEscolhida = "Editar";
  }
  if(digitalRead(pinBot4) == LOW){
    editar("Livre");
    ligarLed(3);
    delayMillis(40);
    desligarLed(3);
    escolhaOpcaoConcluida=true;
    opcaoEscolhida = "Editar";
    
  }
}
void editar(String edicao){
  if(edicao=="Padrao"){
   melodiaPadrao = true; 
  }else{
   melodiaPadrao = false; 
  }
}
void gravarMusica(){
 mapearEscala();
 int ultimaLed=-1;
  
  if(digitalRead(botaoPrincipal)==LOW){
     musicaFinalizada=true; 
   }else{
   while(digitalRead(pinBot1) == LOW)
  {
     if(ultimaLed!=0){
     tone(pinBuzzer,notas[escalaAtual][0]);
     ligarLed(0);
     ultimaLed=0;
     print(nomeNotas[0]+"_"+(escalaAtual+1)+"        ",0,1);
     melodiaMus01[contNotas]=notas[escalaAtual][0];
     //sequenciaLedMus01[contNotas]=0;
     contNotas++;
     }
  }
  
   while(digitalRead(pinBot2) == LOW)
  {
     if(ultimaLed!=1){
     tone(pinBuzzer,notas[escalaAtual][1]);
     ligarLed(1);
     ultimaLed=1;
     print(nomeNotas[1]+"_"+(escalaAtual+1)+"        ",0,1);
     melodiaMus01[contNotas]=notas[escalaAtual][1];
     //sequenciaLedMus01[contNotas]=1;
     contNotas++;
     }
  }
  while(digitalRead(pinBot3) == LOW)
  {
    if(ultimaLed!=2){
    tone(pinBuzzer,notas[escalaAtual][2]);
     ligarLed(2);
     ultimaLed=2;
     print(nomeNotas[2]+"_"+(escalaAtual+1)+"        ",0,1);
     melodiaMus01[contNotas]=notas[escalaAtual][2];
     //sequenciaLedMus01[contNotas]=2;
     contNotas++;
    }
  }
  while(digitalRead(pinBot4) == LOW)
  {
    if(ultimaLed!=3){
    tone(pinBuzzer,notas[escalaAtual][3]);
     ligarLed(3);
     ultimaLed=3;
     print(nomeNotas[3]+"_"+(escalaAtual+1)+"        ",0,1);
     melodiaMus01[contNotas]=notas[escalaAtual][3];
    // sequenciaLedMus01[contNotas]=3;
     contNotas++;
    }
  }
  while(digitalRead(pinBot5) == LOW)
  {
    if(ultimaLed!=4){
    tone(pinBuzzer,notas[escalaAtual][4]);
     ligarLed(4);
     ultimaLed=4;
     print(nomeNotas[4]+"_"+(escalaAtual+1)+"        ",0,1);
     melodiaMus01[contNotas]=notas[escalaAtual][4];
    // sequenciaLedMus01[contNotas]=4;
     contNotas++;
    }
  }
  while(digitalRead(pinBot6) == LOW)
  {
    if(ultimaLed!=5){
    tone(pinBuzzer,notas[escalaAtual][5]);
     ligarLed(5);
     ultimaLed=5;
     print(nomeNotas[5]+"_"+(escalaAtual+1)+"        ",0,1);
     melodiaMus01[contNotas]=notas[escalaAtual][5];
     //sequenciaLedMus01[contNotas]=5;
     contNotas++;
    }
  }
  while(digitalRead(pinBot7) == LOW)
  {
    if(ultimaLed!=6){
     tone(pinBuzzer,notas[escalaAtual][6]);
     ligarLed(6);
     ultimaLed=6;
     print(nomeNotas[6]+"_"+(escalaAtual+1)+"        ",0,1);
     melodiaMus01[contNotas]=notas[escalaAtual][6];
     //sequenciaLedMus01[contNotas]=6;
     contNotas++;
    }
  }
  
  noTone(pinBuzzer);
  desligarLed(ultimaLed); 
  }
}
void tocarMusicaGravada(){
  nivelMax = contNotas;
  
for (byte i = 0 ; i < contNotas; i++) {
    Serial.print(i);
    escalaAtual = acharEscala(melodiaMus01[i]);
    ligarLed(identificaBotao(melodiaMus01[i]));
    tone(pinBuzzer, melodiaMus01[i], 100);
    delayMillis(20);
    desligarLed(identificaBotao(melodiaMus01[i]));
    delayMillis(100);
  }
}

//------------------------GERAL------------------------//
//Delay
void delayMillis(int tempo){
 agora = millis();
 while(millis() < (agora + tempo)) {
    //Pausa
 } 
}
//lCD
void print(String msg,int c,int l){
 lcd.setCursor(c, l);
 lcd.print(msg);
}

//Ligar e Desligar Leds
void ligarLed(int led){
   ci74HC595Write(led, HIGH);
}
void desligarLed(int led){
   ci74HC595Write(led, LOW);
}
//-----------------BOTÃO PRINCIPAL------------------------//
byte digitalReadOnce(byte val){
 static byte lastVal = HIGH;
  static unsigned long m = 0;
  if(lastVal != val && millis() > (m+100)){
   lastVal = val;
    m = millis();
    return lastVal;
  }
  return HIGH;
}

int getCommand(){
  static unsigned long m1 = 0;//millis no momento que inicia de pressionar
  static unsigned long m2 = 0;//millis apos soltar
  static byte count =0;
  
  byte r = digitalRead(botaoPrincipal);
  
  if(digitalReadOnce(r)==LOW){
   m1 = millis();
    count++;
  }
  if(r==LOW){
   m2 = millis(); 
  }else{
    if(!(m2>0 && m2-m1 < 500)){
     count = 0;
      m1 =0;
      m2 = 0;
    }
    if(m2>0 && millis()-m2>700){
      byte c = count;
      count = 0;
      m1 = 0;
      m2 = 0;
      return c;
      
    }
    
  }
  return 0;
}

//74HC595
void ci74HC595Write(byte pino, bool estado) {
static byte ciBuffer[qtdeCI];

bitWrite(ciBuffer[pino / 8], pino % 8, estado);

digitalWrite(pinST_CP, LOW); //Inicia a Transmissão

digitalWrite(pinDS, LOW);    //Apaga Tudo para Preparar Transmissão
digitalWrite(pinSH_CP, LOW);

for (int nC = qtdeCI-1; nC >= 0; nC--) {
    for (int nB = 7; nB >= 0; nB--) {

        digitalWrite(pinSH_CP, LOW);  //Baixa o Clock      
        
        digitalWrite(pinDS,  bitRead(ciBuffer[nC], nB) );     //Escreve o BIT
        
        digitalWrite(pinSH_CP, HIGH); //Eleva o Clock
        digitalWrite(pinDS, LOW);     //Baixa o Data para Previnir Vazamento      
    }  
}

digitalWrite(pinST_CP, HIGH);  //Finaliza a Transmissão

}
//SENSOR
/*
long leituraDoSensor() {
  //o Sensor utilizado no projeto possui o canal Sig,onde emite e recebe o sinal
  pinMode(sig, OUTPUT); //inicia a emição por um tempo
  digitalWrite(sig, HIGH);
  delayMicroseconds(5);
  digitalWrite(sig, LOW); //desliga a emição

  pinMode(sig, INPUT); //recebe o sinal
  distancia = pulseIn(sig, HIGH); //armazena o sinal
  return distancia;
}
bool sensor() {
  leituraDoSensor();
  distancia = 5; //limite maximo
  cm = 0.01723 * leituraDoSensor(); //transforma em cm

  //compara a distancia do objeto com o limite fornecido
  if (cm >= distancia) {
    //caso o sinal exceda o limite vai dar falso
    result = false;

  } else {
    //se for menor ou  igual ao limite,ou seja se estiver proximo ele da true e continua
    result = true;
  }
  return result;
}
void acordar() {
  //se o sensor detectar alguma coisa ,ele acorda

  // Desabilita o sleep
  sleep_disable();

  // Desabilita a interrupção
  detachInterrupt(0);

}
void dormir() {
  //define o modo sleep
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  //habilita o modo
  sleep_enable();

  //interrupções
  attachInterrupt(digitalPinToInterrupt(pinBot1), acordar, LOW);
  attachInterrupt(digitalPinToInterrupt(pinBot2), acordar, LOW);
  attachInterrupt(digitalPinToInterrupt(pinBot3), acordar, LOW);
  attachInterrupt(digitalPinToInterrupt(pinBot4), acordar, LOW);
  attachInterrupt(digitalPinToInterrupt(pinBot5), acordar, LOW);
  attachInterrupt(digitalPinToInterrupt(pinBot6), acordar, LOW);
  attachInterrupt(digitalPinToInterrupt(pinBot7), acordar, LOW);
  attachInterrupt(digitalPinToInterrupt(botaoPrincipal), acordar, LOW);

  //manda ele dormir
  sleep_cpu();
}
*/