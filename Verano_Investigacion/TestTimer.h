/*
 * TestTimer.h - Libreria para crear 
 */

#ifndef TESTTIMER_H
#define TESTTIMER_H

#include "Arduino.h"

class TestTimer {
  public:
    TestTimer();
    inline void TimerStart() __attribute__((always_inline));
    inline void TimerStop() __attribute__((always_inline));
    inline uint16_t GetCycles(void) __attribute__((always_inline));
  public:
    void TestFunction(int testType);
  private:
    void Suma(int a);
    void Multi(int a);
    void Raiz(int a);
    void Seno(int a);
    void Coseno(int a);
    void MatrixFunc();
    void IntTrap();
    double f(double x);
  private:
    int _testType;
    float testVariable;
    float _var;   //  dummy variable
};

#endif
