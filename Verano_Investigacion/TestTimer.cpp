#include "Arduino.h"
#include "TestTimer.h"

TestTimer::TestTimer(){
  DDRB = 0b00100001;    //  configurar pin 8 y 13 como salida
  TIMSK1 = 0b00000001;  //  habilitar overflow interrupts (pag 144)
}

void TestTimer::TimerStart() {
  TCCR1A = 0x00;    //  establecer el modo timer1 como normal
  TCCR1B = 0x00;    //  deshabilitar el reloj del timer1
  TCNT1  = 0x0000;  //  reestablecer el valor del timer1
  TIFR1 = 0x00;     //  reestablecer Timer Overflow Flag
  TCCR1B = 0x01;    //  inicializar timer1 con el reloj de cristal (16 MHz)
  PORTB = 0b00100000;   //  poner el pin 13 en alto
}

void TestTimer::TimerStop() {
  TCCR1B = 0x0000;      //  detener timer1
  PORTB = 0b00000000;   //  poner el pin 13 en bajo
}

uint16_t TestTimer::GetCycles(void) {
  return TCNT1 - 3;      //  obtener los ciclos con correccion
}

void TestTimer::TestFunction(int testType) {

  _var = 0;

  for (int m = 0; m < 1000; m++) {
    switch (testType) {
      case 1:
        Suma(m);
        break;
      case 2:
        Multi(m);
        break;
      case 3:
        Raiz(m);
        break;
      case 4:
        Seno(m);
        break;
      case 5:
        Coseno(m);
        break;
      case 6:
        MatrixFunc();
        break;
      case 7:
        IntTrap();
        break;
    }
  }
}

void TestTimer::Suma(int a) {
  testVariable += a;
}

void TestTimer::Multi(int a) {
  testVariable *= a;
}

void TestTimer::Raiz(int a) {
  testVariable += sqrt(a);
}

void TestTimer::Seno(int a) {
  testVariable += sin(a);
}

void TestTimer::Coseno(int a) {
  testVariable += cos(a);
}

void TestTimer::MatrixFunc() {
  int a[3], b[3][3], mul[3], i, j, k, r = 3, c = 3, count = 0;

  //  1x3 matrix
  for (i = 0; i < r; i++) {
    a[i] = i;
  }

  //  3x3 matrix
  for (i = 0; i < r; i++) {
    for (j = 0; j < c; j++) {
      count++;
      b[i][j] = count;
    }
  }

  //  multiplicacion
  for (i = 0; i < r; i++) {
    mul[i] = 0;
    for (j = 0; j < c; j++) {
      mul[i] += a[j] * b[i][j];
    }
  }
}

//  algoritmo para integración numerica con el método trapezoidal
void TestTimer::IntTrap() {
  int n = 50, i;
  double a = 0, b = M_PI, h, x, sum = 0, integral;

  /*Begin Trapezoidal Method: */
  h = fabs(b - a) / n;

  for (i = 1; i < n; i++) {
    x = a + i * h;
    sum = sum + f(x);
  }
  integral = (h / 2) * (f(a) + f(b) + 2 * sum);
}

//  funcion que será integrada numericamente
double TestTimer::f(double x) {
  return sin(x);
}
