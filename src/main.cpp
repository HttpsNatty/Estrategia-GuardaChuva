#include <stdio.h>
#include <Arduino.h>

#define s_frontal A0
#define s_esq A1
#define s_dir A2
#define s_besq 2
#define s_bdir 3
#define microstart 4
#define m_pwmesq 5
#define m_pwmdir 6
#define m_esq_direcaoA 7
#define m_esq_direcaoB 8
#define m_dir_direcaoA 9
#define m_dir_direcaoB 10

void frente(void);
void frente_full(void);
void tras(void);
void esquerda(void);
void direita(void);
void borda_esquerda(void);
void borda_direita(void);
void giro_direita(void);
void giro_esquerda(void);

// motor esquerdo
// direcao b== frente
// direcao a== tras

// motor direito
// direcao a== frente
// direcao b== tras

// os sensores de borda possuem logica invertida

void setup()
{

  pinMode(s_frontal, INPUT); // frontal
  pinMode(s_esq, INPUT);     // esquerda
  pinMode(s_dir, INPUT);     // direita
  pinMode(s_besq, INPUT);    // borda esquerda
  pinMode(s_bdir, INPUT);    // borda direito

  pinMode(microstart, INPUT); // microstart

  pinMode(m_pwmesq, OUTPUT);       // pwm a motor esquerdo
  pinMode(m_pwmdir, OUTPUT);       // pwm b motor direito
  pinMode(m_esq_direcaoA, OUTPUT); // in1a
  pinMode(m_esq_direcaoB, OUTPUT); // in2a
  pinMode(m_dir_direcaoA, OUTPUT); // in1b
  pinMode(m_dir_direcaoB, OUTPUT); // in2b

  pinMode(13, OUTPUT); // led
}

void frente(void)
{
  digitalWrite(m_esq_direcaoA, LOW);
  digitalWrite(m_esq_direcaoB, HIGH);
  digitalWrite(m_dir_direcaoA, HIGH);
  digitalWrite(m_dir_direcaoB, LOW);
  analogWrite(m_pwmesq, 96);
  analogWrite(m_pwmdir, 100);
}

void frente_full(void)
{
  digitalWrite(m_esq_direcaoA, LOW);
  digitalWrite(m_esq_direcaoB, HIGH);
  digitalWrite(m_dir_direcaoA, HIGH);
  digitalWrite(m_dir_direcaoB, LOW);
  analogWrite(m_pwmesq, 174); // diferença dos motores
  analogWrite(m_pwmdir, 220);
}

void tras(void)
{
  digitalWrite(m_esq_direcaoA, HIGH);
  digitalWrite(m_esq_direcaoB, LOW);
  digitalWrite(m_dir_direcaoA, LOW);
  digitalWrite(m_dir_direcaoB, HIGH);
  analogWrite(m_pwmesq, 126);
  analogWrite(m_pwmdir, 130);
}

void esquerda(void)
{
  digitalWrite(m_esq_direcaoA, HIGH);
  digitalWrite(m_esq_direcaoB, LOW);
  digitalWrite(m_dir_direcaoA, HIGH);
  digitalWrite(m_dir_direcaoB, LOW);
  analogWrite(m_pwmesq, 96);
  analogWrite(m_pwmdir, 100);
}

void direita(void)
{
  digitalWrite(m_esq_direcaoA, LOW);
  digitalWrite(m_esq_direcaoB, HIGH);
  digitalWrite(m_dir_direcaoA, LOW);
  digitalWrite(m_dir_direcaoB, HIGH);
  analogWrite(m_pwmesq, 96);
  analogWrite(m_pwmdir, 100);
}

void giro_direita(void)
{
  frente();
  analogWrite(m_pwmesq, 120);
  analogWrite(m_pwmdir, 80);
}

void giro_esquerda(void)
{
  frente();
  analogWrite(m_pwmesq, 80);
  analogWrite(m_pwmdir, 120);
}

void parar(void)
{
  digitalWrite(m_esq_direcaoA, LOW);
  digitalWrite(m_esq_direcaoB, LOW);
  digitalWrite(m_dir_direcaoA, LOW);
  digitalWrite(m_dir_direcaoB, LOW);
  analogWrite(m_pwmesq, 0);
  analogWrite(m_pwmdir, 0);
}

void borda_esquerda(void)
{
  tras();
  delay(200);
  direita();
  delay(400);
  frente();
  delay(200);
}

void borda_direita(void)
{
  tras();
  delay(200);
  esquerda();
  delay(400);
  frente();
  delay(200);
}

void loop()
{

  if (!digitalRead(microstart)) // microstart desligado
    parar();

  else // microstart ligado
  {
    if (digitalRead(s_bdir) && digitalRead(s_besq))
    {
      if (digitalRead(s_frontal))
      {
        frente_full();
      }
      else if (!digitalRead(s_frontal))
      {

        if (digitalRead(s_esq) && !digitalRead(s_dir))
        {
          esquerda();
        }
        else if (!digitalRead(s_esq) && digitalRead(s_dir))
        {
          direita();
        }
        else
        {
          giro_esquerda(); // gira ate chegar no meio do dojo
          delay(1000);
          esquerda();
          delay(100); // tempo para ficar paralelo a linha de centro do robo
        }
        // else if (!digitalRead(s_esq) && !digitalRead(s_dir)){frente();}
      }
    }
    else if (!digitalRead(s_besq))
    {
      borda_esquerda();
    }
    else if (!digitalRead(s_bdir))
    {
      borda_direita();
    }
  }
}