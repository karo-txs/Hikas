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
Nele, ao invez de 4, tem-se 7 teclas que simulam um mini teclado.
É possível selecionar a musica desejada (no momento tem-se duas
músicas configuradas), e o jogo repetirá a musica estimulando a
memoria do jogador atravez de uma musica e não sons atribuidos
aleatoriamento (como é o caso do Genius original);

3 - Modo Programação: Nesse modo tem-se duas opções
  3.1 - Edição: Nele pode-se liberar as notas do jogo genius
    para serem livres, ou seja, não mais se prenderem as notas
    padroes (com frequencias predefinidas);
    3.2 - Gravar: Aqui é possível gravar sua musica na memoria
    do arduino para que posteriormente possa ser jogada no modo
    Genius. Na hora da gravação tambem é possivel modificar as
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
  um mini sintetizador básico. Além de, no modo Scroll, mudar a 
  velocidade do jogo;
- Botão Principal: A "cabeça" do jogo, nele os modos são controlados
  e trocados, dependendo da vontade do jogador;
  
Entre diversos outros elementos, que possuem suas importancias
dentro do circuito geral.

---------------------------------------------------//
*/
//------------------------GERAL------------------------//
//Bibliotecas
#include <LiquidCrystal.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <avr/sleep.h>

//bluetooth
SoftwareSerial bluetooth(0, 1); // RX, TX do Arduino
String command = "";
char b1;
bool livre = false;
bool programacao = true;
bool gravar = false;
bool genius = false;
bool scrolle = false;
bool flagBluetooth = false;
String mus="";

//Sensor
unsigned long distancia = 0;
unsigned long cm = 0;
#define sig 19

//ShiftRegister
#define pinSH_CP 12   //Pino Clock
#define pinST_CP 10  //Pino Latch
#define pinDS    11  //Pino Data
#define qtdeCI 2  

#define qtdeCi 2
byte enderecosPCF8574[qtdeCi] = {32,33}; 

// DEFINIÇÕES DE PINOS
#define pinBuzzer 8
#define pinPot 13
//Potenciometro
int pot;

//Usado no Millis
unsigned long agora = 0;

//Botão Multifunções
#define botaoPrincipal 9
#define tempoReset 2000
bool flag = false;
int estadoBotaoControlador = -1;

//LCD
String mensagem = "HIKAS";
LiquidCrystal lcd(7,6,5,4,3,2);

//------------------------LIVRE------------------------//
//Notas Piano 
int notasPadrao[] = {262,294,330,349,392,440,494};
int notas[6][16] = {
  33,34,36,37,39,41,42,44,45,48,50,53,55,57,60,63, //1 Oitava
  65,71,74,78,81,85,89,92,99,101,105,110,115,120,125, //2 Oitava
  130,141,142,149,156,162,170,177,185,193,202,211,220,230,240,251, //3 Oitava
  262,273,285,298,311,325,339,354,370,386,403,421,440,459,480,501, //4 Oitava
  523,546,571,596,622,650,679,709,740,773,807,843,880,919,960,1002, //5 Oitava
  1092,1141,1192,1245,1300,1357,1417,1480,1545,1614,1685,1760,1838,1919,2004 //6 Oitava
};

int escalaAtual = 4;

//                      C     C#    Db   D    D#   E    E#    F    F#     G     G#    Ab    A   A#     Bb     B
//                      8    0     9     1    10   2    11    3    12     4     13     5    14   6      15    7   // botao
String nomeNotas[] = {"DO","DOs","Reb","RE","REs","MI","MIs","FA","FAs","SOL","SOLs","LAb","LA","LAs","SIb","SI"};
//                     0     1     2     3    4     5   6     7     8    9     10     11     12   13   14    15 //sequencia da oitava
//                     0     0     1     1    1     2   2     3    3    4       4     5      5     5    6    6 
//------------------------GENIUS------------------------//
// Musica da Vitoria
int vitoria[] = {880,988,523,988,523,587,523,587,659,587,659,659};
#define duracaoVitoria 75

// Musica da Falha
int errado[] = {392,247};
int duracaoErrado[] = {500,750};

// DEFINIÇÕES
int nivelMax = 0;

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

//MUSICA 01 -> MARIO THEME
//PADRÃO
int marioMelodiaLivre[] = {
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
};

int noteDuration;
int pauseEntreNotas;
int sequenciaMario[41];
int sequenciaUsuario[41];
//MUSICA 02 -> Zelda(Por motivos de armazenamento não é possivel ter
//ambas as musicas simultaneamente)
/*int zeldaMelodiaLivre[] = {
  440,349,349,440,392,349,392,440,349,349,440,440,392, 
  440,440,349,440,440,440,523,587,587,698,
  698,698,698,698,784,880, 880, 880, 880, 784, 698,784,
  698,698,698,587,587
};

int zeldaMelodiaPadrao[] = {
  440,349,349,440,392,349,392,440,349,349,440,440,392,440,
  440,349,440,440,440,262,294,294,349,349,349,349,349,392,
  440,440,440,440,392,349,392,349,349,349,294,294,349,349,
  349,294,262,262,294,349,294
};
byte zeldaSequenciaLed[]={
 5 ,3,3,5 ,4,3,4,5 ,3,3,5 ,5 ,4,5 ,
  5 ,3,5 ,5 ,5 ,0,1,1,3,3,3,3,3,4,
  5 ,5 ,5 ,5 ,4,3,4,3,3,3,1,1
};
byte tempoZelda[] = {
  1,4,4,4,8,8,2,8,1,4,4,4,8,8,2,2,2,8,8,8,8,8,8,2,2,
  8,8,8,8,8,2,2,8,8,8,8,8,8,8,2,4,4,4,4,4,4,4,4,4,4,4,4,
  8,8,4,4,4,4,4,2,8,8,4,8,8,4,8,8,4,4,2,2,8,8,8,8,8,8,2,
  2,8,8,8,8,8,2,4,4,1,4,1,4,4,4,1,4,1,4,4,4,1,4,1,4,2,1,
  4,4,8,8,4,4,8,8,8,4,8,8,4,8,8,4,4,4
};
byte sequenciaZelda[40];
*/
//-----------------------SCROLL------------------------//
bool proxima;

//--------------------PROGRAMAÇÃO------------------------//
int melodiaMus01[20];
int musicasGravadas = 0;
int contNotas = 0;
String opcaoEscolhida;
bool escolhaOpcaoConcluida = false;
bool melodiaPadrao = true;
bool musicaFinalizada = false;
int sequenciaMus01[20];
int sequenciaLedMus01[20];


void setup() {
  Wire.begin();
  Serial.begin(9600);
  bluetooth.begin(9600);
  //LCD
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(1, 0);
   lcd.print(mensagem);
  //Botoes
   for (int nL=0; nL <= 15; nL++) { 
        ciPCF8574PinMode(nL, INPUT_PULLUP);
    }
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
   if (sensor() == true) {
    acordar();
  int commandA = getCommand();
 //Via Arduino
  if (commandA== 1) {
      livre = true;
      programacao = false;
      genius = false;
      scrolle = false;
    
  }
  if (commandA == 2) {
     programacao = false;
     livre = false;
     genius = true;
     scrolle = false;
    
  }
  if (commandA == 3) {
     programacao = false;
     livre = false;
     genius = false;
     scrolle = true;
  }
  if (commandA == 4) {
     programacao = true;
     livre = false;
     genius = false;
     scrolle = false;
  }

  //Via Bluetooth
 if (bluetooth.available()>0) { // Se existem dados para leitura
   
    b1 = bluetooth.read(); //Variável para armazenar o dado atual
    command += b1; //Variável para armazenar todos os dados
   
    if (b1 == '\n'){ //Se o dado atual for um pulador de linha (\n)
      Serial.print(command); //Printa o comando
     
      command.trim(); //Remove o \n para comparar o texto
       flagBluetooth=true;
      if (command == "ModoLivre"){
        livre = true;
        programacao = false;
        genius = false;
        scrolle = false;
      }else if(command == "ModoProgramacao_Gravar"){
        programacao = true;
        gravar = true;
        livre = false;
        genius = false;
        scrolle = false;
      }else if(command == "ModoProgramacao_Editar"){
        programacao = true;
        gravar = false;
        livre = false;
        genius = false;
        scrolle = false;
      }else if(command =="Genius_Mus01"){
        genius = true;
        mus = "Mario";
        livre = false;
        programacao = false;
        scrolle = false;
      }else if(command == "Scroll_Mus01"){
        scrolle = true;
        mus = "Mario";
        livre = false;
        programacao = false;
        genius = false;
      }
      command = ""; //Limpa o comando para futuras leituras
    }
  }

if(livre==true){
      zerarGenius();
      print("   Modo Livre   ",0,0);
      ligarLed(8);
      desligarLed(9);
      desligarLed(10);
      desligarLed(11);
      delayMillis(100);
      modoLivre();
}
if(programacao==true){
    zerarGenius();
     print("  Programacao   ",0,0);
     ligarLed(11);
     desligarLed(10);
     desligarLed(9);
     desligarLed(8);
     delayMillis(100);
     if(flagBluetooth==true){
     if(gravar==true){
      modoProgramacao("Gravar");
     }else{
      modoProgramacao("Editar");
     }
     }
     else{
      modoProgramacao("ViaArduino");
     }
}

if(genius==true){
      print("   Modo Genius  ",0,0);
      print("                ",0,1);
      ligarLed(9);
      desligarLed(8);
      desligarLed(10);
      desligarLed(11);
      delayMillis(100);
  if(mus!=""){
    modoGenius(mus);
  }else{
    modoGenius("ArduinoMenu");
  }
}
if(scroll==true){
   zerarGenius();
     print("     Scroll    ",0,0);
     print("                ",0,1);
     ligarLed(10);
     desligarLed(11);
     desligarLed(9);
     desligarLed(8);
     delayMillis(100);
  if(mus!=""){
    modoScroll(mus);
  }else{
    modoScroll("ArduinoMenu");
  }
}
   }else{
     dormir();
   }

}
//------------------------SCROLL------------------------//
void modoScroll(String mus){
  float vel= 200;
  bool velocidadeEscolhida = false;
  escolhaConcluida = false;
  perdeu = false;
  ganhou = false;
if(mus=="ArduinoMenu"){
  while(escolhaConcluida==false){
   menu(); 
  }
}else{
  musicaEscolhida = mus;
}
  while(velocidadeEscolhida==false){
    print("Escolha        ",0,0);
    print("Velocidade C-OK",0,1);
  if(ciPCF8574Read(8)==LOW){
    vel= map(pinPot,0,1023,60,250);
    velocidadeEscolhida=true;
  }
  }
  
  while(perdeu==false && ganhou==false && digitalRead(botaoPrincipal)==HIGH){ 
   if(musicaEscolhida=="Mario"){
     if(melodiaPadrao==true){
      scrollMusic(vel,marioMelodiaPadrao,41);
     }
     else{
      scrollMusic(vel,marioMelodiaLivre,41);
     }
     }
    else if(musicaEscolhida=="Mus01"){
     scrollMusic(vel,melodiaMus01,41);
   }
  }
}
void scroll(float ScrollSpeed,int pinBot, String nota, byte escala, byte pos){
  bool flag = false;
  int count = 1;
  lcd.begin(16, 2);               
  lcd.setCursor(16,pos); 
  
  lcd.print(nota+"_"+(escala+1));
  Serial.print(nota);
  
    while (ciPCF8574Read(pinBot)==HIGH && count < 18) {
    delay(ScrollSpeed);
    lcd.scrollDisplayLeft();
    count++;
     if(ciPCF8574Read(pinBot)==LOW){
       ligarLed(identificaNota(nota)); 
       tone(pinBuzzer,notas[escala][identificaNota(nota)],300);
       delayMillis(40);
       desligarLed(identificaNota(nota));
       proxima = true;
      }
  }

  if  (ciPCF8574Read(pinBot)==HIGH){
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
  else if(nota=="DOs"){ return 0;} 
  else if(nota=="RE"){ return 1;} 
  else if(nota=="REb"){ return 1;}
  else if(nota=="REs"){ return 1;}  
  else if(nota=="MI"){ return 2;} 
  else if(nota=="MIs"){ return 2;} 
  else if(nota=="FA"){ return 3;} 
  else if(nota=="FAs"){ return 3;} 
  else if(nota=="SOL"){ return 4;} 
  else if(nota=="SOLs"){ return 4;} 
  else if(nota=="LAb"){ return 5;} 
  else if(nota=="LA"){ return 5;} 
  else if(nota=="LAs"){ return 5;} 
  else if(nota=="SIb"){ return 6;} 
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
      scroll(vel,8,"DO",escala,pos);
     }
    }
    else if(nota==notas[escala][1]){
       while(proxima==false&&perdeu==false){
        if(digitalRead(botaoPrincipal)==LOW){
      perdeu = true;
      break; 
     }
       scroll(vel,0,"DOs",escala,pos);
    }
    }
     else if(nota==notas[escala][2]){ 
       while(proxima==false&&perdeu==false){
        if(digitalRead(botaoPrincipal)==LOW){
      perdeu = true;
      break; 
        }
        scroll(vel,9,"REb",escala,pos);
       }
    }
     else if(nota==notas[escala][3]){ 
     while(proxima==false&&perdeu==false){ 
       if(digitalRead(botaoPrincipal)==LOW){
      perdeu = true;
      break; 
     }
        scroll(vel,1,"RE",escala,pos);
       
     }
    }
     else if(nota==notas[escala][4]){ 
      while(proxima==false&&perdeu==false){
        if(digitalRead(botaoPrincipal)==LOW){
      perdeu = true;
      break; 
     }
        scroll(vel,10,"REs",escala,pos);
      }
    }
     else if(nota==notas[escala][5]){ 
      while(proxima==false&&perdeu==false){
        if(digitalRead(botaoPrincipal)==LOW){
      perdeu = true;
      break; 
     }
        scroll(vel,2,"MI",escala,pos);
      }
    }
     else if(nota==notas[escala][6]){ 
      while(proxima==false&&perdeu==false){
        if(digitalRead(botaoPrincipal)==LOW){
      perdeu = true;
      break; 
     }
        scroll(vel,11,"MIs",escala,pos);
      }
      }
      else if(nota==notas[escala][7]){ 
      while(proxima==false&&perdeu==false){
        if(digitalRead(botaoPrincipal)==LOW){
      perdeu = true;
      break; 
     }
        scroll(vel,3,"FA",escala,pos);
      }
      }
     else if(nota==notas[escala][8]){ 
      while(proxima==false&&perdeu==false){
        if(digitalRead(botaoPrincipal)==LOW){
      perdeu = true;
      break; 
     }
        scroll(vel,12,"FAs",escala,pos);
      }
      }
      else if(nota==notas[escala][9]){ 
      while(proxima==false&&perdeu==false){
        if(digitalRead(botaoPrincipal)==LOW){
      perdeu = true;
      break; 
     }
        scroll(vel,4,"SOL",escala,pos);
      }
      }
      else if(nota==notas[escala][10]){ 
      while(proxima==false&&perdeu==false){
        if(digitalRead(botaoPrincipal)==LOW){
      perdeu = true;
      break; 
     }
        scroll(vel,13,"SOLs",escala,pos);
      }
      }
      else if(nota==notas[escala][11]){ 
      while(proxima==false&&perdeu==false){
        if(digitalRead(botaoPrincipal)==LOW){
      perdeu = true;
      break; 
     }
        scroll(vel,5,"LAb",escala,pos);
      }
      }
      else if(nota==notas[escala][12]){ 
      while(proxima==false&&perdeu==false){
        if(digitalRead(botaoPrincipal)==LOW){
      perdeu = true;
      break; 
     }
        scroll(vel,14,"LA",escala,pos);
      }
      }
      else if(nota==notas[escala][13]){ 
      while(proxima==false&&perdeu==false){
        if(digitalRead(botaoPrincipal)==LOW){
      perdeu = true;
      break; 
     }
        scroll(vel,6,"LAs",escala,pos);
      }
      }
      else if(nota==notas[escala][14]){ 
      while(proxima==false&&perdeu==false){
        if(digitalRead(botaoPrincipal)==LOW){
      perdeu = true;
      break; 
     }
        scroll(vel,15,"SIb",escala,pos);
      }
      }
      else if(nota==notas[escala][15]){ 
      while(proxima==false&&perdeu==false){
        if(digitalRead(botaoPrincipal)==LOW){
      perdeu = true;
      break; 
     }
        scroll(vel,7,"SI",escala,pos);
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
  mapearEscala();
  int ultimaLed=0;
   while(ciPCF8574Read(8) == LOW)
  {
    tone(pinBuzzer,notas[escalaAtual][0]);
    ligarLed(0);
    ultimaLed = 0;
    print(nomeNotas[0]+"_"+(escalaAtual+1)+"        ",0,1);
  }
  
   while(ciPCF8574Read(0) == LOW)
  {
    tone(pinBuzzer,notas[escalaAtual][1]);
     ligarLed(0);
     ultimaLed=0;
     print(nomeNotas[1]+"_"+(escalaAtual+1)+"        ",0,1);
  }
  while(ciPCF8574Read(9) == LOW)
  {
    tone(pinBuzzer,notas[escalaAtual][2]);
     ligarLed(1);
     ultimaLed=1;
     print(nomeNotas[2]+"_"+(escalaAtual+1)+"        ",0,1);
  }
  while(ciPCF8574Read(1) == LOW)
  {
    tone(pinBuzzer,notas[escalaAtual][3]);
     ligarLed(1);
     ultimaLed=1;
     print(nomeNotas[3]+"_"+(escalaAtual+1)+"        ",0,1);
  }
  while(ciPCF8574Read(10) == LOW)
  {
    tone(pinBuzzer,notas[escalaAtual][4]);
     ligarLed(1);
     ultimaLed=1;
     print(nomeNotas[4]+"_"+(escalaAtual+1)+"        ",0,1);
  }
  while(ciPCF8574Read(2) == LOW)
  {
    tone(pinBuzzer,notas[escalaAtual][5]);
     ligarLed(2);
     ultimaLed=2;
     print(nomeNotas[5]+"_"+(escalaAtual+1)+"        ",0,1);
  }
  while(ciPCF8574Read(11) == LOW)
  {
    tone(pinBuzzer,notas[escalaAtual][6]);
     ligarLed(2);
     ultimaLed=2;
     print(nomeNotas[6]+"_"+(escalaAtual+1)+"        ",0,1);
  }
   while(ciPCF8574Read(3) == LOW)
  {
    tone(pinBuzzer,notas[escalaAtual][7]);
     ligarLed(3);
     ultimaLed=3;
     print(nomeNotas[7]+"_"+(escalaAtual+1)+"        ",0,1);
  }
  while(ciPCF8574Read(12) == LOW)
  {
    tone(pinBuzzer,notas[escalaAtual][8]);
     ligarLed(3);
     ultimaLed=3;
     print(nomeNotas[8]+"_"+(escalaAtual+1)+"        ",0,1);
  }
  while(ciPCF8574Read(4) == LOW)
  {
    tone(pinBuzzer,notas[escalaAtual][9]);
     ligarLed(4);
     ultimaLed=4;
     print(nomeNotas[9]+"_"+(escalaAtual+1)+"        ",0,1);
  }
  while(ciPCF8574Read(13) == LOW)
  {
    tone(pinBuzzer,notas[escalaAtual][10]);
     ligarLed(4);
     ultimaLed=4;
     print(nomeNotas[10]+"_"+(escalaAtual+1)+"        ",0,1);
  }
  while(ciPCF8574Read(5) == LOW)
  {
    tone(pinBuzzer,notas[escalaAtual][11]);
     ligarLed(5);
     ultimaLed=5;
     print(nomeNotas[11]+"_"+(escalaAtual+1)+"        ",0,1);
  }
  while(ciPCF8574Read(14) == LOW)
  {
    tone(pinBuzzer,notas[escalaAtual][12]);
     ligarLed(5);
     ultimaLed=5;
     print(nomeNotas[12]+"_"+(escalaAtual+1)+"        ",0,1);
  }
  while(ciPCF8574Read(6) == LOW)
  {
    tone(pinBuzzer,notas[escalaAtual][13]);
     ligarLed(5);
     ultimaLed=5;
     print(nomeNotas[13]+"_"+(escalaAtual+1)+"        ",0,1);
  }
  while(ciPCF8574Read(15) == LOW)
  {
    tone(pinBuzzer,notas[escalaAtual][14]);
     ligarLed(6);
     ultimaLed=6;
     print(nomeNotas[14]+"_"+(escalaAtual+1)+"        ",0,1);
  }
  while(ciPCF8574Read(7) == LOW)
  {
    tone(pinBuzzer,notas[escalaAtual][15]);
     ligarLed(6);
     ultimaLed=6;
     print(nomeNotas[15]+"_"+(escalaAtual+1)+"        ",0,1);
  }
  noTone(pinBuzzer);
  desligarLed(ultimaLed);
}
void mapearEscala(){
  pot = map(analogRead(pinPot),0,1023,1,72);
  if(pot <=12){ escalaAtual = 0;} 
  else if(pot<=24){ escalaAtual = 1;} 
  else if(pot<=36){ escalaAtual = 2;}
  else if(pot<=48){ escalaAtual = 3;}
  else if(pot<=60){ escalaAtual = 4;}
  else if(pot<=72){ escalaAtual = 5;}
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
void modoGenius(String mus){
  if(mus=="ArduinoMenu"){
    while(escolhaConcluida==false){
     menu();
  }
  }else{
    musicaEscolhida = mus;
  }
 bool velocidadeEscolhida = false;
   while(velocidadeEscolhida==false){
    print("Escolha        ",0,0);
    print("Velocidade C-OK",0,1);
  if(ciPCF8574Read(8) == LOW){
    velocidade = map(analogRead(pinPot),0,1023,40,100);
    tempoLimite = map(velocidade,400,2000,1500,3000);
    velocidadeEscolhida=true;
  }
  }
  
  while(!perdeu && !ganhou && digitalRead(botaoPrincipal)==HIGH){ 
   if(musicaEscolhida=="Mario"){
     if(melodiaPadrao==true){
       jogarPadrao(sequenciaMario,marioMelodiaPadrao,tempoMario);
     }
     else{
       jogarLivre(sequenciaMario,marioMelodiaLivre,tempoMario);
     }
     }
    else if(musicaEscolhida=="Mus01"){
       jogarLivre(sequenciaMus01,melodiaMus01,tempoMario);//vetor de tempo padrao
   }
  }
}
void menu(){
  if(musicasGravadas>=1){
  print("D-Mario     F ->",0,0);
  print("E-Zelda         ",0,1);
  if(ciPCF8574Read(1) == LOW){
    musicaEscolhida = "Mario";
    nivelMax = 41;
    escolhaConcluida=true;
  }
  if(ciPCF8574Read(2) == LOW){
    musicaEscolhida = "Zelda";
    nivelMax = 190;//verificar
    escolhaConcluida=true;
  }
  
  if(ciPCF8574Read(3) == LOW){
    while(escolhaConcluida==false){
    telaUser();
    }
  }
  }else{
  print("D-Mario       ",0,0);
  print("E-Zelda         ",0,1);
  if(ciPCF8574Read(1) == LOW){
    musicaEscolhida = "Mario";
    nivelMax = 41; 
    escolhaConcluida=true;
  }
  if(ciPCF8574Read(2) == LOW){
    musicaEscolhida = "Zelda";
    nivelMax = 40;
    escolhaConcluida=true;
  }
  }
}
void telaUser(){
   print("MusicaUser    ",0,0);
   print("G-Mus01       ",0,1);
  if(ciPCF8574Read(4) == LOW){
    musicaEscolhida = "Mus01";
    nivelMax = 30;
    escolhaConcluida=true;
  }
}

void jogarPadrao(int sequencia[],int melodia[], byte tempo[]){
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
void musicaAteAgora(int sequencia[], int melodia[], byte tempo[]) {
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
int identificaBotao(int nota){
  if(nota==notas[escalaAtual][0]){ return 8;}
  else if(nota==notas[escalaAtual][1]){return 0;}
  else if(nota==notas[escalaAtual][2]){return 9;}
  else if(nota==notas[escalaAtual][3]){return 1;}
  else if(nota==notas[escalaAtual][4]){return 10;}
  else if(nota==notas[escalaAtual][5]){return 2;}
  else if(nota==notas[escalaAtual][6]){return 11;}
  else if(nota==notas[escalaAtual][7]){return 3;}
  else if(nota==notas[escalaAtual][8]){return 12;}
  else if(nota==notas[escalaAtual][9]){return 4;}
  else if(nota==notas[escalaAtual][10]){return 13;}
  else if(nota==notas[escalaAtual][11]){return 5;}
  else if(nota==notas[escalaAtual][12]){return 14;}
  else if(nota==notas[escalaAtual][13]){return 6;}
  else if(nota==notas[escalaAtual][14]){return 15;}
  else if(nota==notas[escalaAtual][15]){return 7;}
}

void jogadaArduino(int sequencia[],int melodia[]){
  sequencia[contador] = identificaBotao(melodia[contador]);
  contador++;
  sequencia[contador] = identificaBotao(melodia[contador]);
  contador++;
}

void vezUsuario(int melodia[]) {
  unsigned long controleTempo = millis();
  byte leitura = 0;
  while (millis() < controleTempo + tempoLimite && leitura < contador) {
    if (ciPCF8574Read(8)==LOW) {
      sequenciaUsuario[leitura] = 8;
      controleTempo = millis();
      leitura++;
      ligarLed(0);
      print(nomeNotas[0]+"_"+4+"        ",0,1);
      tone(pinBuzzer, notas[escalaAtual][0], velocidade);
      delayMillis(velocidade);
      desligarLed(0);
    } else if (ciPCF8574Read(0)==LOW) {
      sequenciaUsuario[leitura] = 0;
      controleTempo = millis();
      leitura++;
      ligarLed(0);
      print(nomeNotas[1]+"_"+4+"        ",0,1);
      tone(pinBuzzer, notas[escalaAtual][1], velocidade);
      delayMillis(velocidade);
      desligarLed(0);
    } else if (ciPCF8574Read(9)==LOW) {
      sequenciaUsuario[leitura] = 9;
      controleTempo = millis();
      leitura++;
      ligarLed(1);
      print(nomeNotas[2]+"_"+(escalaAtual+1)+"        ",0,1);
      tone(pinBuzzer, notas[escalaAtual][2], velocidade);
      delayMillis(velocidade);
      desligarLed(1);
    } else if (ciPCF8574Read(1)==LOW) {
      sequenciaUsuario[leitura] = 1;
      controleTempo = millis();
      leitura++;
      ligarLed(1);
      print(nomeNotas[3]+"_"+(escalaAtual+1)+"        ",0,1);
      tone(pinBuzzer, notas[escalaAtual][3], velocidade);
      delayMillis(velocidade);
      desligarLed(1);
    }else if (ciPCF8574Read(10)==LOW) {
      sequenciaUsuario[leitura] = 10;
      controleTempo = millis();
      leitura++;
      ligarLed(1);
      print(nomeNotas[4]+"_"+(escalaAtual+1)+"        ",0,1);
      tone(pinBuzzer, notas[escalaAtual][4], velocidade);
      delayMillis(velocidade);
      desligarLed(1);
    }else if (ciPCF8574Read(2)==LOW) {
      sequenciaUsuario[leitura] = 2;
      controleTempo= millis();
      leitura++;
      ligarLed(2);
      print(nomeNotas[5]+"_"+(escalaAtual+1)+"        ",0,1);
      tone(pinBuzzer, notas[escalaAtual][5], velocidade);
      delayMillis(velocidade);
      desligarLed(2);
    }else if (ciPCF8574Read(11)==LOW) {
      sequenciaUsuario[leitura] = 11;
      controleTempo = millis();
      leitura++;
      ligarLed(2);
      print(nomeNotas[6]+"_"+(escalaAtual+1)+"        ",0,1);
      tone(pinBuzzer, notas[escalaAtual][6], velocidade);
      delayMillis(velocidade);
      desligarLed(2);
    }
    else if (ciPCF8574Read(3)==LOW) {
      sequenciaUsuario[leitura] = 3;
      controleTempo = millis();
      leitura++;
      ligarLed(3);
      print(nomeNotas[7]+"_"+(escalaAtual+1)+"        ",0,1);
      tone(pinBuzzer, notas[escalaAtual][7], velocidade);
      delayMillis(velocidade);
      desligarLed(3);
    }
    else if (ciPCF8574Read(12)==LOW) {
      sequenciaUsuario[leitura] = 12;
      controleTempo = millis();
      leitura++;
      ligarLed(3);
      print(nomeNotas[8]+"_"+(escalaAtual+1)+"        ",0,1);
      tone(pinBuzzer, notas[escalaAtual][8], velocidade);
      delayMillis(velocidade);
      desligarLed(3);
    }
    else if (ciPCF8574Read(4)==LOW) {
      sequenciaUsuario[leitura] = 4;
      controleTempo = millis();
      leitura++;
      ligarLed(4);
      print(nomeNotas[9]+"_"+(escalaAtual+1)+"        ",0,1);
      tone(pinBuzzer, notas[escalaAtual][9], velocidade);
      delayMillis(velocidade);
      desligarLed(4);
    }
    else if (ciPCF8574Read(13)==LOW) {
      sequenciaUsuario[leitura] = 13;
      controleTempo = millis();
      leitura++;
      ligarLed(4);
      print(nomeNotas[10]+"_"+(escalaAtual+1)+"        ",0,1);
      tone(pinBuzzer, notas[escalaAtual][10], velocidade);
      delayMillis(velocidade);
      desligarLed(4);
    }
    else if (ciPCF8574Read(5)==LOW) {
      sequenciaUsuario[leitura] = 5;
      controleTempo = millis();
      leitura++;
      ligarLed(5);
      print(nomeNotas[11]+"_"+(escalaAtual+1)+"        ",0,1);
      tone(pinBuzzer, notas[escalaAtual][11], velocidade);
      delayMillis(velocidade);
      desligarLed(5);
    }
    else if (ciPCF8574Read(14)==LOW) {
      sequenciaUsuario[leitura] = 14;
      controleTempo = millis();
      leitura++;
      ligarLed(5);
      print(nomeNotas[12]+"_"+(escalaAtual+1)+"        ",0,1);
      tone(pinBuzzer, notas[escalaAtual][12], velocidade);
      delayMillis(velocidade);
      desligarLed(5);
    }
    else if (ciPCF8574Read(6)==LOW) {
      sequenciaUsuario[leitura] = 6;
      controleTempo = millis();
      leitura++;
      ligarLed(5);
      print(nomeNotas[13]+"_"+(escalaAtual+1)+"        ",0,1);
      tone(pinBuzzer, notas[escalaAtual][13], velocidade);
      delayMillis(velocidade);
      desligarLed(5);
    }
    else if (ciPCF8574Read(15)==LOW) {
      sequenciaUsuario[leitura] = 15;
      controleTempo = millis();
      leitura++;
      ligarLed(6);
      print(nomeNotas[14]+"_"+(escalaAtual+1)+"        ",0,1);
      tone(pinBuzzer, notas[escalaAtual][14], velocidade);
      delayMillis(velocidade);
      desligarLed(6);
    }
    else if (ciPCF8574Read(7)==LOW) {
      sequenciaUsuario[leitura] = 7;
      controleTempo = millis();
      leitura++;
      ligarLed(6);
      print(nomeNotas[15]+"_"+(escalaAtual+1)+"        ",0,1);
      tone(pinBuzzer, notas[escalaAtual][15], velocidade);
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
    print("VEZ ARDUINO",0,0);
    flagTrocaVez=false;
  }else{
    print("SUA VEZ    ",0,0);
    flagTrocaVez=true;
  }
}

bool confere(int sequencia[]) {
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
void jogarLivre(int sequencia[], int melodia[], byte tempo[]){
  trocaVez();
  jogadaArduinoLivre(sequencia,melodia);
  musicaAteAgora(sequencia,melodia,tempo);
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

void jogadaArduinoLivre(int sequencia[], int melodia[]){
  sequencia[contador] = identificaBotao(melodia[contador]);
  escala1 = acharEscala(melodia[contador]);
  contador++;
  sequencia[contador] = identificaBotao(melodia[contador]);
  escala2 = acharEscala(melodia[contador]);
  contador++;
}

byte acharEscala(int nota){
  if(nota<=63){ return 0;}
  else if(nota<=125){return 1;}
  else if(nota<=251){ return 2;}
  else if(nota<=501){return 3;}
  else if(nota<=1002){return 4;}
  else if(nota<=2004){return 5;}
}
void vezUsuarioLivre(){
  unsigned long controleTempo = millis();
  byte leitura = 0;
  int cont = 0;
  
  while (millis() < controleTempo + tempoLimite && leitura < contador) {
    if (ciPCF8574Read(8)==LOW) {
      sequenciaUsuario[leitura] = 8;
      controleTempo = millis();
      leitura++;
      ligarLed(0);
      if(cont==0){
      print(nomeNotas[0]+"_"+(escala1+1)+"        ",0,1);
      tone(pinBuzzer, notas[escala1][0], velocidade);
        cont++;
      }else{
        print(nomeNotas[0]+"_"+(escala2+1)+"        ",0,1);
        tone(pinBuzzer, notas[escala2][0], velocidade);
      }
      delayMillis(velocidade);
      desligarLed(0);
    } else if (ciPCF8574Read(0)==LOW) {
      sequenciaUsuario[leitura] = 0;
      controleTempo = millis();
      leitura++;
      ligarLed(0);
      if(cont==0){
        print(nomeNotas[1]+"_"+(escala1+1)+"        ",0,1);
      tone(pinBuzzer, notas[escala1][1], velocidade);
        cont++;
      }else{
        print(nomeNotas[1]+"_"+(escala2+1)+"        ",0,1);
        tone(pinBuzzer, notas[escala2][1], velocidade);
      }
      delayMillis(velocidade);
      desligarLed(0);
    } else if (ciPCF8574Read(9)==LOW) {
      sequenciaUsuario[leitura] = 9;
      controleTempo = millis();
      leitura++;
      ligarLed(1);
      if(cont==0){
        print(nomeNotas[2]+"_"+(escala1+1)+"        ",0,1);
      tone(pinBuzzer, notas[escala1][2], velocidade);
        cont++;
      }else{
        print(nomeNotas[2]+"_"+(escala2+1)+"        ",0,1);
        tone(pinBuzzer, notas[escala2][2], velocidade);
      }
      delayMillis(velocidade);
      desligarLed(1);
    } else if (ciPCF8574Read(1)==LOW) {
      sequenciaUsuario[leitura] = 1;
      controleTempo = millis();
      leitura++;
      ligarLed(1);
      if(cont==0){
        print(nomeNotas[3]+"_"+(escala1+1)+"        ",0,1);
      tone(pinBuzzer, notas[escala1][3], velocidade);
        cont++;
      }else{
        print(nomeNotas[3]+"_"+(escala2+1)+"        ",0,1);
        tone(pinBuzzer, notas[escala2][3], velocidade);
      }
      delayMillis(velocidade);
      desligarLed(1);
    }else if (ciPCF8574Read(10)==LOW) {
      sequenciaUsuario[leitura] = 10;
      controleTempo = millis();
      leitura++;
      ligarLed(1);
      if(cont==0){
        print(nomeNotas[4]+"_"+(escala1+1)+"        ",0,1);
      tone(pinBuzzer, notas[escala1][4], velocidade);
        cont++;
      }else{
        print(nomeNotas[4]+"_"+(escala2+1)+"        ",0,1);
        tone(pinBuzzer, notas[escala2][4], velocidade);
      }
      delayMillis(velocidade);
      desligarLed(1);
    }else if (ciPCF8574Read(2)==LOW) {
      sequenciaUsuario[leitura] = 2;
      controleTempo= millis();
      leitura++;
      ligarLed(2);
      if(cont==0){
        print(nomeNotas[5]+"_"+(escala1+1)+"        ",0,1);
      tone(pinBuzzer, notas[escala1][5], velocidade);
        cont++;
      }else{
        print(nomeNotas[5]+"_"+(escala2+1)+"        ",0,1);
        tone(pinBuzzer, notas[escala2][5], velocidade);
      }
      delayMillis(velocidade);
      desligarLed(2);
    }else if (ciPCF8574Read(11)==LOW) {
      sequenciaUsuario[leitura] = 11;
      controleTempo = millis();
      leitura++;
      ligarLed(2);
      if(cont==0){
        print(nomeNotas[6]+"_"+(escala1+1)+"        ",0,1);
      tone(pinBuzzer, notas[escala1][6], velocidade);
        cont++;
      }else{
        print(nomeNotas[6]+"_"+(escala2+1)+"        ",0,1);
        tone(pinBuzzer, notas[escala2][6], velocidade);
      }
      delayMillis(velocidade);
      desligarLed(2);
    }
    else if (ciPCF8574Read(3)==LOW) {
      sequenciaUsuario[leitura] = 3;
      controleTempo = millis();
      leitura++;
      ligarLed(3);
      if(cont==0){
        print(nomeNotas[7]+"_"+(escala1+1)+"        ",0,1);
      tone(pinBuzzer, notas[escala1][7], velocidade);
        cont++;
      }else{
        print(nomeNotas[7]+"_"+(escala2+1)+"        ",0,1);
        tone(pinBuzzer, notas[escala2][7], velocidade);
      }
      delayMillis(velocidade);
      desligarLed(3);
    }
    else if (ciPCF8574Read(12)==LOW) {
      sequenciaUsuario[leitura] = 12;
      controleTempo = millis();
      leitura++;
      ligarLed(3);
      if(cont==0){
        print(nomeNotas[8]+"_"+(escala1+1)+"        ",0,1);
      tone(pinBuzzer, notas[escala1][8], velocidade);
        cont++;
      }else{
        print(nomeNotas[8]+"_"+(escala2+1)+"        ",0,1);
        tone(pinBuzzer, notas[escala2][8], velocidade);
      }
      delayMillis(velocidade);
      desligarLed(3);
    }
    else if (ciPCF8574Read(4)==LOW) {
      sequenciaUsuario[leitura] = 4;
      controleTempo = millis();
      leitura++;
      ligarLed(4);
      if(cont==0){
        print(nomeNotas[9]+"_"+(escala1+1)+"        ",0,1);
      tone(pinBuzzer, notas[escala1][9], velocidade);
        cont++;
      }else{
        print(nomeNotas[9]+"_"+(escala2+1)+"        ",0,1);
        tone(pinBuzzer, notas[escala2][9], velocidade);
      }
      delayMillis(velocidade);
      desligarLed(4);
    }
    else if (ciPCF8574Read(13)==LOW) {
      sequenciaUsuario[leitura] = 13;
      controleTempo = millis();
      leitura++;
      ligarLed(4);
      if(cont==0){
        print(nomeNotas[10]+"_"+(escala1+1)+"        ",0,1);
      tone(pinBuzzer, notas[escala1][10], velocidade);
        cont++;
      }else{
        print(nomeNotas[10]+"_"+(escala2+1)+"        ",0,1);
        tone(pinBuzzer, notas[escala2][10], velocidade);
      }
      delayMillis(velocidade);
      desligarLed(4);
    }
     else if (ciPCF8574Read(5)==LOW) {
      sequenciaUsuario[leitura] = 5;
      controleTempo = millis();
      leitura++;
      ligarLed(5);
      if(cont==0){
        print(nomeNotas[11]+"_"+(escala1+1)+"        ",0,1);
      tone(pinBuzzer, notas[escala1][11], velocidade);
        cont++;
      }else{
        print(nomeNotas[11]+"_"+(escala2+1)+"        ",0,1);
        tone(pinBuzzer, notas[escala2][11], velocidade);
      }
      delayMillis(velocidade);
      desligarLed(5);
    }
    else if (ciPCF8574Read(14)==LOW) {
      sequenciaUsuario[leitura] = 14;
      controleTempo = millis();
      leitura++;
      ligarLed(5);
      if(cont==0){
        print(nomeNotas[12]+"_"+(escala1+1)+"        ",0,1);
      tone(pinBuzzer, notas[escala1][12], velocidade);
        cont++;
      }else{
        print(nomeNotas[12]+"_"+(escala2+1)+"        ",0,1);
        tone(pinBuzzer, notas[escala2][12], velocidade);
      }
      delayMillis(velocidade);
      desligarLed(5);
    }
    else if (ciPCF8574Read(6)==LOW) {
      sequenciaUsuario[leitura] = 6;
      controleTempo = millis();
      leitura++;
      ligarLed(5);
      if(cont==0){
        print(nomeNotas[13]+"_"+(escala1+1)+"        ",0,1);
      tone(pinBuzzer, notas[escala1][13], velocidade);
        cont++;
      }else{
        print(nomeNotas[13]+"_"+(escala2+1)+"        ",0,1);
        tone(pinBuzzer, notas[escala2][13], velocidade);
      }
      delayMillis(velocidade);
      desligarLed(5);
    }
    else if (ciPCF8574Read(15)==LOW) {
      sequenciaUsuario[leitura] = 15;
      controleTempo = millis();
      leitura++;
      ligarLed(6);
      if(cont==0){
        print(nomeNotas[14]+"_"+(escala1+1)+"        ",0,1);
      tone(pinBuzzer, notas[escala1][14], velocidade);
        cont++;
      }else{
        print(nomeNotas[14]+"_"+(escala2+1)+"        ",0,1);
        tone(pinBuzzer, notas[escala2][14], velocidade);
      }
      delayMillis(velocidade);
      desligarLed(6);
    }
    else if (ciPCF8574Read(7)==LOW) {
      sequenciaUsuario[leitura] = 7;
      controleTempo = millis();
      leitura++;
      ligarLed(6);
      if(cont==0){
        print(nomeNotas[15]+"_"+(escala1+1)+"        ",0,1);
      tone(pinBuzzer, notas[escala1][15], velocidade);
        cont++;
      }else{
        print(nomeNotas[15]+"_"+(escala2+1)+"        ",0,1);
        tone(pinBuzzer, notas[escala2][15], velocidade);
      }
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
//--------------------PROGRAMAÇÃO------------------------//
void modoProgramacao(String opcao){
  if(opcao=="Gravar"){
    opcaoEscolhida = "Gravar";
  }
  else if(opcao=="Editar"){
    opcaoEscolhida = "Editar";
  }
  else{
   while(escolhaOpcaoConcluida==false){
   menuProg(); 
  }
  }
  if(opcaoEscolhida=="Gravar"){
    musicasGravadas=1;
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
 
  if(ciPCF8574Read(8)==LOW){
    ligarLed(0);
    delayMillis(40);
    desligarLed(0);
    while(escolhaOpcaoConcluida==false){
      menuEdicao();
    }
  }
  if(ciPCF8574Read(1)==LOW){
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
  if(ciPCF8574Read(2)==LOW){
    editar("Padrao");
    ligarLed(2);
    delayMillis(40);
    desligarLed(2);
    escolhaOpcaoConcluida=true;
    opcaoEscolhida = "Editar";
  }
  if(ciPCF8574Read(3)==LOW){
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
   while(ciPCF8574Read(8) == LOW)
  {
   if(ultimaLed!=0){
     tone(pinBuzzer,notas[escalaAtual][0]);
     ligarLed(0);
     ultimaLed=0;
     print(nomeNotas[0]+"_"+(escalaAtual+1)+"        ",0,1);
     melodiaMus01[contNotas]=notas[escalaAtual][0];
     sequenciaLedMus01[contNotas]=8;
     contNotas++;
     }
  }
  
   while(ciPCF8574Read(0) == LOW)
  {
     if(ultimaLed!=0){
     tone(pinBuzzer,notas[escalaAtual][1]);
     ligarLed(1);
     ultimaLed=0;
     print(nomeNotas[1]+"_"+(escalaAtual+1)+"        ",0,1);
     melodiaMus01[contNotas]=notas[escalaAtual][1];
     sequenciaLedMus01[contNotas]=0;
     contNotas++;
     }
  }
  while(ciPCF8574Read(9) == LOW)
  {
    if(ultimaLed!=1){
    tone(pinBuzzer,notas[escalaAtual][2]);
     ligarLed(1);
     ultimaLed=1;
     print(nomeNotas[2]+"_"+(escalaAtual+1)+"        ",0,1);
     melodiaMus01[contNotas]=notas[escalaAtual][2];
     sequenciaLedMus01[contNotas]=9;
     contNotas++;
    }
  }
  while(ciPCF8574Read(1) == LOW)
  {
    if(ultimaLed!=1){
    tone(pinBuzzer,notas[escalaAtual][3]);
     ligarLed(1);
     ultimaLed=1;
     print(nomeNotas[3]+"_"+(escalaAtual+1)+"        ",0,1);
     melodiaMus01[contNotas]=notas[escalaAtual][3];
     sequenciaLedMus01[contNotas]=1;
     contNotas++;
    }
  }
  while(ciPCF8574Read(10) == LOW)
  {
    if(ultimaLed!=1){
    tone(pinBuzzer,notas[escalaAtual][4]);
     ligarLed(1);
     ultimaLed=1;
     print(nomeNotas[4]+"_"+(escalaAtual+1)+"        ",0,1);
     melodiaMus01[contNotas]=notas[escalaAtual][4];
     sequenciaLedMus01[contNotas]=10;
     contNotas++;
    }
  }
  while(ciPCF8574Read(2) == LOW)
  {
    if(ultimaLed!=2){
    tone(pinBuzzer,notas[escalaAtual][5]);
     ligarLed(2);
     ultimaLed=2;
     print(nomeNotas[5]+"_"+(escalaAtual+1)+"        ",0,1);
     melodiaMus01[contNotas]=notas[escalaAtual][5];
     sequenciaLedMus01[contNotas]=2;
     contNotas++;
    }
  }
  while(ciPCF8574Read(11) == LOW)
  {
    if(ultimaLed!=2){
     tone(pinBuzzer,notas[escalaAtual][6]);
     ligarLed(2);
     ultimaLed=2;
     print(nomeNotas[6]+"_"+(escalaAtual+1)+"        ",0,1);
     melodiaMus01[contNotas]=notas[escalaAtual][6];
     sequenciaLedMus01[contNotas]=11;
     contNotas++;
    }
  }
  while(ciPCF8574Read(3) == LOW)
  {
    if(ultimaLed!=3){
     tone(pinBuzzer,notas[escalaAtual][7]);
     ligarLed(3);
     ultimaLed=3;
     print(nomeNotas[7]+"_"+(escalaAtual+1)+"        ",0,1);
     melodiaMus01[contNotas]=notas[escalaAtual][7];
     sequenciaLedMus01[contNotas]=3;
     contNotas++;
    }
  }
  while(ciPCF8574Read(12) == LOW)
  {
    if(ultimaLed!=3){
     tone(pinBuzzer,notas[escalaAtual][12]);
     ligarLed(3);
     ultimaLed=3;
     print(nomeNotas[8]+"_"+(escalaAtual+1)+"        ",0,1);
     melodiaMus01[contNotas]=notas[escalaAtual][8];
     sequenciaLedMus01[contNotas]=12;
     contNotas++;
    }
  }
  while(ciPCF8574Read(4) == LOW)
  {
    if(ultimaLed!=4){
     tone(pinBuzzer,notas[escalaAtual][9]);
     ligarLed(4);
     ultimaLed=4;
     print(nomeNotas[9]+"_"+(escalaAtual+1)+"        ",0,1);
     melodiaMus01[contNotas]=notas[escalaAtual][9];
     sequenciaLedMus01[contNotas]=4;
     contNotas++;
    }
  }
  while(ciPCF8574Read(13) == LOW)
  {
    if(ultimaLed!=4){
     tone(pinBuzzer,notas[escalaAtual][10]);
     ligarLed(4);
     ultimaLed=4;
     print(nomeNotas[10]+"_"+(escalaAtual+1)+"        ",0,1);
     melodiaMus01[contNotas]=notas[escalaAtual][10];
     sequenciaLedMus01[contNotas]=13;
     contNotas++;
    }
  }
  while(ciPCF8574Read(5) == LOW)
  {
    if(ultimaLed!=5){
     tone(pinBuzzer,notas[escalaAtual][11]);
     ligarLed(5);
     ultimaLed=5;
     print(nomeNotas[11]+"_"+(escalaAtual+1)+"        ",0,1);
     melodiaMus01[contNotas]=notas[escalaAtual][11];
     sequenciaLedMus01[contNotas]=5;
     contNotas++;
    }
  }
  while(ciPCF8574Read(14) == LOW)
  {
    if(ultimaLed!=5){
     tone(pinBuzzer,notas[escalaAtual][12]);
     ligarLed(5);
     ultimaLed=5;
     print(nomeNotas[12]+"_"+(escalaAtual+1)+"        ",0,1);
     melodiaMus01[contNotas]=notas[escalaAtual][12];
     sequenciaLedMus01[contNotas]=14;
     contNotas++;
    }
  }
  while(ciPCF8574Read(6) == LOW)
  {
    if(ultimaLed!=5){
     tone(pinBuzzer,notas[escalaAtual][13]);
     ligarLed(5);
     ultimaLed=5;
     print(nomeNotas[13]+"_"+(escalaAtual+1)+"        ",0,1);
     melodiaMus01[contNotas]=notas[escalaAtual][13];
     sequenciaLedMus01[contNotas]=6;
     contNotas++;
    }
  }
  while(ciPCF8574Read(15) == LOW)
  {
    if(ultimaLed!=6){
     tone(pinBuzzer,notas[escalaAtual][14]);
     ligarLed(6);
     ultimaLed=6;
     print(nomeNotas[14]+"_"+(escalaAtual+1)+"        ",0,1);
     melodiaMus01[contNotas]=notas[escalaAtual][14];
     sequenciaLedMus01[contNotas]=15;
     contNotas++;
    }
  }
  while(ciPCF8574Read(7) == LOW)
  {
    if(ultimaLed!=6){
     tone(pinBuzzer,notas[escalaAtual][15]);
     ligarLed(6);
     ultimaLed=6;
     print(nomeNotas[15]+"_"+(escalaAtual+1)+"        ",0,1);
     melodiaMus01[contNotas]=notas[escalaAtual][15];
     sequenciaLedMus01[contNotas]=7;
     contNotas++;
    }
  }
  noTone(pinBuzzer);
  desligarLed(ultimaLed); 
  }
}
void tocarMusicaGravada(){
for (byte i = 0 ; i < contNotas; i++) {
    ligarLed(sequenciaLedMus01[i]);
    tone(pinBuzzer, melodiaMus01[i], 100);
    delayMillis(20);
    desligarLed(sequenciaLedMus01[i]);
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

bool ciPCF8574PinMode(byte pino, int modo) {
static byte modoPinos[qtdeCi];

  if (modo == -1) {
     return bitRead(modoPinos[pino / 8], pino % 8); 
  } else {
     bitWrite(modoPinos[pino / 8], (pino % 8), modo);
     return modo;
  }
}

bool ciPCF8574Read(byte pino) {
byte lido;
bool estado;

   Wire.requestFrom(enderecosPCF8574[pino / 8], 1);
   if(Wire.available()) {   
      lido = Wire.read();        
   }
       
   estado = bitRead(lido, pino % 8);
   return estado;  
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
//------------------------------SENSOR------------------------------
//Sensor
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
  bool result=true;
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
  /*attachInterrupt(digitalPinToInterrupt(pinBot1), acordar, LOW);
  attachInterrupt(digitalPinToInterrupt(pinBot2), acordar, LOW);
  attachInterrupt(digitalPinToInterrupt(pinBot3), acordar, LOW);
  attachInterrupt(digitalPinToInterrupt(pinBot4), acordar, LOW);
  attachInterrupt(digitalPinToInterrupt(pinBot5), acordar, LOW);
  attachInterrupt(digitalPinToInterrupt(pinBot6), acordar, LOW);
  attachInterrupt(digitalPinToInterrupt(pinBot7), acordar, LOW);
  */
  attachInterrupt(digitalPinToInterrupt(botaoPrincipal), acordar, LOW);

  //manda ele dormir
  sleep_cpu();
}
